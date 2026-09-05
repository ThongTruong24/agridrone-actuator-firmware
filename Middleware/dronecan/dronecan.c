#include "dronecan.h"

#include <stddef.h>
#include <string.h>

#include "board_can.h"
#include "board_time.h"
#include "canard.h"
#include "dronecan_config.h"
#include "dronecan_port.h"
#include "thaco.equipment.actuator.ArrayCommand.h"
#include "uavcan.protocol.NodeStatus.h"

CANARD_STATIC_ASSERT(
    ((DRONECAN_FC_NODE_STATUS_CAN_ID >> 8U) & 0xFFFFU) ==
        UAVCAN_PROTOCOL_NODESTATUS_ID,
    "Configured CAN identifier is not uavcan.protocol.NodeStatus");
CANARD_STATIC_ASSERT(
    ((DRONECAN_FC_ARRAY_COMMAND_CAN_ID >> 8U) & 0xFFFFU) ==
        THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID,
    "Configured CAN identifier is not thaco.equipment.actuator.ArrayCommand");

static CanardInstance canard_instance;
static uint32_t canard_memory_pool[DRONECAN_MEMORY_POOL_SIZE / sizeof(uint32_t)];
static DronecanNodeStatusHandler node_status_handler;
static void *node_status_user_reference;
static DronecanActuatorArrayCommandHandler array_command_handler;
static void *array_command_user_reference;
static dronecan_statistics_t statistics;
static uint32_t last_cleanup_ms;

static bool should_accept_transfer(const CanardInstance *instance,
                                   uint64_t *out_data_type_signature,
                                   uint16_t data_type_id,
                                   CanardTransferType transfer_type,
                                   uint8_t source_node_id)
{
    (void) instance;

    if ((out_data_type_signature == NULL) ||
        (transfer_type != CanardTransferTypeBroadcast) ||
        (source_node_id != DRONECAN_FC_NODE_ID))
    {
        return false;
    }

    if (data_type_id == UAVCAN_PROTOCOL_NODESTATUS_ID)
    {
        *out_data_type_signature = UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE;
        return true;
    }

    if (data_type_id == THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID)
    {
        *out_data_type_signature =
            THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_SIGNATURE;
        return true;
    }

    return false;
}

static uint32_t transfer_can_id(const CanardRxTransfer *transfer)
{
    return ((uint32_t) transfer->priority << 24U) |
           ((uint32_t) transfer->data_type_id << 8U) |
           transfer->source_node_id;
}

static void handle_node_status(const CanardRxTransfer *transfer)
{
    struct uavcan_protocol_NodeStatus decoded = {0};
    DronecanNodeStatus status;

    if (uavcan_protocol_NodeStatus_decode(transfer, &decoded))
    {
        statistics.node_status_decode_error_count++;
        return;
    }

    status.can_id = transfer_can_id(transfer);
    status.uptime_sec = decoded.uptime_sec;
    status.vendor_specific_status_code =
        decoded.vendor_specific_status_code;
    status.source_node_id = transfer->source_node_id;
    status.transfer_id = transfer->transfer_id;
    status.priority = transfer->priority;
    status.health = decoded.health;
    status.mode = decoded.mode;
    status.sub_mode = decoded.sub_mode;
    statistics.node_status_received_count++;

    if (node_status_handler != NULL)
    {
        node_status_handler(&status, node_status_user_reference);
    }
}

static void handle_array_command(const CanardRxTransfer *transfer)
{
    struct thaco_equipment_actuator_ArrayCommand decoded = {0};
    DronecanActuatorArrayCommand array_command = {0};

    if (thaco_equipment_actuator_ArrayCommand_decode(transfer, &decoded))
    {
        statistics.array_command_decode_error_count++;
        return;
    }

    array_command.can_id = transfer_can_id(transfer);
    array_command.timestamp_ms = decoded.timestamp_ms;
    array_command.command_count = decoded.commands.len;
    array_command.source_node_id = transfer->source_node_id;
    array_command.transfer_id = transfer->transfer_id;
    array_command.priority = transfer->priority;

    for (uint8_t index = 0U;
         (index < decoded.commands.len) &&
         (index < DRONECAN_ACTUATOR_COMMAND_CAPACITY);
         index++)
    {
        array_command.commands[index].actuator_id =
            decoded.commands.data[index].actuator_id;
        array_command.commands[index].value =
            decoded.commands.data[index].value;
        array_command.commands[index].command_id =
            decoded.commands.data[index].command_id;
    }

    statistics.array_command_received_count++;

    if (array_command_handler != NULL)
    {
        array_command_handler(&array_command,
                              array_command_user_reference);
    }
}

static void on_transfer_reception(CanardInstance *instance,
                                  CanardRxTransfer *transfer)
{
    (void) instance;

    if ((transfer->transfer_type != CanardTransferTypeBroadcast) ||
        (transfer->source_node_id != DRONECAN_FC_NODE_ID))
    {
        return;
    }

    if (transfer->data_type_id == UAVCAN_PROTOCOL_NODESTATUS_ID)
    {
        handle_node_status(transfer);
    }
    else if (transfer->data_type_id ==
             THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID)
    {
        handle_array_command(transfer);
    }
    else
    {
        /* The acceptance callback rejects every other data type. */
    }
}

static int16_t process_frame(const board_can_frame_t *frame)
{
    CanardCANFrame canard_frame = {0};

    if ((frame == NULL) ||
        !frame->is_extended ||
        frame->is_remote ||
        (frame->data_length == 0U) ||
        (frame->data_length > CANARD_CAN_FRAME_MAX_DATA_LEN))
    {
        return -CANARD_ERROR_RX_INCOMPATIBLE_PACKET;
    }

    canard_frame.id = (frame->id & CANARD_CAN_EXT_ID_MASK) |
                      CANARD_CAN_FRAME_EFF;
    canard_frame.data_len = frame->data_length;
    canard_frame.iface_id = 0U;
    (void) memcpy(canard_frame.data,
                  frame->data,
                  frame->data_length);

    return canardHandleRxFrame(&canard_instance,
                               &canard_frame,
                               board_time_us());
}

static void cleanup_stale_transfers(void)
{
    const uint32_t now_ms = board_time_ms();

    if ((uint32_t) (now_ms - last_cleanup_ms) >=
        DRONECAN_RX_CLEANUP_PERIOD_MS)
    {
        canardCleanupStaleTransfers(&canard_instance, board_time_us());
        last_cleanup_ms = now_ms;
    }
}

void dronecan_set_node_status_handler(DronecanNodeStatusHandler handler,
                                      void *user_reference)
{
    node_status_handler = handler;
    node_status_user_reference = user_reference;
}

void dronecan_set_array_command_handler(
    DronecanActuatorArrayCommandHandler handler,
    void *user_reference)
{
    array_command_handler = handler;
    array_command_user_reference = user_reference;
}

bool dronecan_init(void)
{
    statistics = (dronecan_statistics_t) {0};
    last_cleanup_ms = board_time_ms();

    canardInit(&canard_instance,
               canard_memory_pool,
               sizeof(canard_memory_pool),
               on_transfer_reception,
               should_accept_transfer,
               NULL);

    return dronecan_port_init();
}

dronecan_process_result_t dronecan_process(uint32_t timeout_ms)
{
    board_can_frame_t frame;

    if (!dronecan_port_receive(&frame, timeout_ms))
    {
        cleanup_stale_transfers();
        return DRONECAN_PROCESS_IDLE;
    }

    const int16_t result = process_frame(&frame);
    if (result < CANARD_OK)
    {
        statistics.rx_error_count++;
        cleanup_stale_transfers();
        return DRONECAN_PROCESS_ERROR;
    }

    cleanup_stale_transfers();
    return DRONECAN_PROCESS_FRAME;
}

dronecan_statistics_t dronecan_get_statistics(void)
{
    const dronecan_port_statistics_t port_statistics =
        dronecan_port_get_statistics();
    const board_can_statistics_t board_statistics =
        board_can_get_statistics();
    dronecan_statistics_t snapshot = statistics;

    snapshot.queue_drop_count = port_statistics.queue_drop_count;
    snapshot.hardware_received_frame_count =
        board_statistics.received_frame_count;
    snapshot.hardware_error_count = board_statistics.hardware_error_count;
    return snapshot;
}
