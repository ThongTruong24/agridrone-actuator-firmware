#ifndef DRONECAN_CONFIG_H
#define DRONECAN_CONFIG_H

/* Flight controller NodeStatus source selected for this application. */
#define DRONECAN_FC_NODE_ID                    1U
#define DRONECAN_FC_NODE_STATUS_CAN_ID         0x10015501UL
#define DRONECAN_FC_ARRAY_COMMAND_CAN_ID       0x0651A401UL

/* Static libcanard arena: 8 blocks with the current 32-byte block size. */
#define DRONECAN_MEMORY_POOL_SIZE              256U
#define DRONECAN_RX_CLEANUP_PERIOD_MS          1000U

#if ((DRONECAN_FC_NODE_ID < 1U) || (DRONECAN_FC_NODE_ID > 127U))
#error "DRONECAN_FC_NODE_ID must be in the range 1..127"
#endif

#if (DRONECAN_FC_NODE_STATUS_CAN_ID > 0x1FFFFFFFUL)
#error "DRONECAN_FC_NODE_STATUS_CAN_ID must be a 29-bit CAN identifier"
#endif

#if ((DRONECAN_FC_NODE_STATUS_CAN_ID & 0x7FUL) != DRONECAN_FC_NODE_ID)
#error "NodeStatus CAN identifier and DRONECAN_FC_NODE_ID do not match"
#endif

#if (DRONECAN_FC_ARRAY_COMMAND_CAN_ID > 0x1FFFFFFFUL)
#error "DRONECAN_FC_ARRAY_COMMAND_CAN_ID must be a 29-bit CAN identifier"
#endif

#if ((DRONECAN_FC_ARRAY_COMMAND_CAN_ID & 0x7FUL) != DRONECAN_FC_NODE_ID)
#error "ArrayCommand CAN identifier and DRONECAN_FC_NODE_ID do not match"
#endif

#if ((DRONECAN_MEMORY_POOL_SIZE == 0U) || \
     ((DRONECAN_MEMORY_POOL_SIZE % 4U) != 0U))
#error "DRONECAN_MEMORY_POOL_SIZE must be non-zero and 4-byte aligned"
#endif

#endif /* DRONECAN_CONFIG_H */
