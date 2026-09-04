#include "dronecan.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "canard.h"
#include "dronecan_config.h"
#include "thaco.equipment.actuator.ArrayCommand.h"
#include "uavcan.protocol.NodeStatus.h"

CANARD_STATIC_ASSERT(
    ((DRONECAN_FC_NODE_STATUS_CAN_ID >> 8U) & 0xFFFFU) ==
        UAVCAN_PROTOCOL_NODESTATUS_ID,
    "Configured CAN identifier is not uavcan.protocol.NodeStatus");
CANARD_STATIC_ASSERT(
    (DRONECAN_FC_NODE_STATUS_CAN_ID & (1UL << 7U)) == 0U,
    "Configured NodeStatus CAN identifier must be a broadcast message");
CANARD_STATIC_ASSERT(
    ((DRONECAN_FC_ARRAY_COMMAND_CAN_ID >> 8U) & 0xFFFFU) ==
        THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID,
    "Configured CAN identifier is not thaco.equipment.actuator.ArrayCommand");
CANARD_STATIC_ASSERT(
    (DRONECAN_FC_ARRAY_COMMAND_CAN_ID & (1UL << 7U)) == 0U,
    "Configured ArrayCommand CAN identifier must be a broadcast message");

volatile uint32_t dronecan_rx_error_count;
volatile uint32_t dronecan_node_status_decode_error_count;
volatile uint32_t dronecan_node_status_received_count;
volatile uint32_t dronecan_array_command_decode_error_count;
volatile uint32_t dronecan_array_command_received_count;

static CanardInstance canard_instance;
static uint32_t canard_memory_pool[DRONECAN_MEMORY_POOL_SIZE / sizeof(uint32_t)];
static DronecanNodeStatusHandler application_node_status_handler;
static DronecanActuatorArrayCommandHandler application_array_command_handler;
static void *application_user_reference;

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

static void handle_node_status(const CanardRxTransfer *transfer)
{
    struct uavcan_protocol_NodeStatus decoded = {0};
    DronecanNodeStatus status;

    /* The generated decoders return true when the payload is invalid. */
    if (uavcan_protocol_NodeStatus_decode(transfer, &decoded))
    {
        dronecan_node_status_decode_error_count++;
        return;
    }

    status.can_id = ((uint32_t) transfer->priority << 24U) |
                    ((uint32_t) transfer->data_type_id << 8U) |
                    transfer->source_node_id;
    status.uptime_sec = decoded.uptime_sec;
    status.vendor_specific_status_code =
        decoded.vendor_specific_status_code;
    status.source_node_id = transfer->source_node_id;
    status.transfer_id = transfer->transfer_id;
    status.priority = transfer->priority;
    status.health = decoded.health;
    status.mode = decoded.mode;
    status.sub_mode = decoded.sub_mode;

    dronecan_node_status_received_count++;

    if (application_node_status_handler != NULL)
    {
        application_node_status_handler(&status,
                                        application_user_reference);
    }
}

static void handle_array_command(const CanardRxTransfer *transfer)
{
    struct thaco_equipment_actuator_ArrayCommand decoded = {0};
    DronecanActuatorArrayCommand array_command = {0};

    if (thaco_equipment_actuator_ArrayCommand_decode(transfer, &decoded))
    {
        dronecan_array_command_decode_error_count++;
        return;
    }

    array_command.can_id =
        ((uint32_t) transfer->priority << 24U) |
        ((uint32_t) transfer->data_type_id << 8U) |
        transfer->source_node_id;
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

    dronecan_array_command_received_count++;

    if (application_array_command_handler != NULL)
    {
        application_array_command_handler(&array_command,
                                          application_user_reference);
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
        /* The acceptance callback rejects all other data types. */
    }
}

void dronecan_init(DronecanNodeStatusHandler node_status_handler,
                   DronecanActuatorArrayCommandHandler array_command_handler,
                   void *user_reference)
{
    application_node_status_handler = node_status_handler;
    application_array_command_handler = array_command_handler;
    application_user_reference = user_reference;

    dronecan_rx_error_count = 0U;
    dronecan_node_status_decode_error_count = 0U;
    dronecan_node_status_received_count = 0U;
    dronecan_array_command_decode_error_count = 0U;
    dronecan_array_command_received_count = 0U;

    canardInit(&canard_instance,
               canard_memory_pool,
               sizeof(canard_memory_pool),
               on_transfer_reception,
               should_accept_transfer,
               NULL);
}

int16_t dronecan_process_rx_frame(uint32_t extended_can_id,
                                  const uint8_t *data,
                                  uint8_t data_length,
                                  uint64_t timestamp_usec)
{
    CanardCANFrame frame = {0};
    const uint16_t data_type_id =
        (uint16_t) ((extended_can_id >> 8U) & 0xFFFFU);
    const uint8_t source_node_id =
        (uint8_t) (extended_can_id & 0x7FU);
    const bool is_service = (extended_can_id & (1UL << 7U)) != 0U;
    const bool supported_data_type =
        (data_type_id == UAVCAN_PROTOCOL_NODESTATUS_ID) ||
        (data_type_id == THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID);

    if ((extended_can_id > CANARD_CAN_EXT_ID_MASK) ||
        is_service ||
        (source_node_id != DRONECAN_FC_NODE_ID) ||
        !supported_data_type ||
        (data == NULL) ||
        (data_length == 0U) ||
        (data_length > CANARD_CAN_FRAME_MAX_DATA_LEN))
    {
        return -CANARD_ERROR_RX_NOT_WANTED;
    }

    frame.id = (extended_can_id & CANARD_CAN_EXT_ID_MASK) |
               CANARD_CAN_FRAME_EFF;
    frame.data_len = data_length;
    frame.iface_id = 0U;
    (void) memcpy(frame.data, data, data_length);

    const int16_t result = canardHandleRxFrame(&canard_instance,
                                               &frame,
                                               timestamp_usec);
    if (result < CANARD_OK)
    {
        dronecan_rx_error_count++;
    }

    return result;
}

void dronecan_cleanup_stale_transfers(uint64_t timestamp_usec)
{
    canardCleanupStaleTransfers(&canard_instance, timestamp_usec);
}
