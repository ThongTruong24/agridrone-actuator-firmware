
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <canard.h>


#include <thaco.equipment.actuator.Command.h>



#define THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_SIZE 37
#define THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_SIGNATURE (0x37419265DAF0D012ULL)

#define THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID 20900





#if defined(__cplusplus) && defined(DRONECAN_CXX_WRAPPERS)
class thaco_equipment_actuator_ArrayCommand_cxx_iface;
#endif


struct thaco_equipment_actuator_ArrayCommand {

#if defined(__cplusplus) && defined(DRONECAN_CXX_WRAPPERS)
    using cxx_iface = thaco_equipment_actuator_ArrayCommand_cxx_iface;
#endif




    uint32_t timestamp_ms;



    struct { uint8_t len; struct thaco_equipment_actuator_Command data[8]; }commands;



};

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t _thaco_equipment_actuator_ArrayCommand_encode(struct thaco_equipment_actuator_ArrayCommand* msg, uint8_t* buffer
#if CANARD_ENABLE_TAO_OPTION
    , bool tao
#endif
);
bool _thaco_equipment_actuator_ArrayCommand_decode(const CanardRxTransfer* transfer, struct thaco_equipment_actuator_ArrayCommand* msg);

static inline uint32_t thaco_equipment_actuator_ArrayCommand_encode(struct thaco_equipment_actuator_ArrayCommand* msg, uint8_t* buffer
#if CANARD_ENABLE_TAO_OPTION
    , bool tao
#endif
) {

    return _thaco_equipment_actuator_ArrayCommand_encode(msg, buffer
#if CANARD_ENABLE_TAO_OPTION
    , tao
#endif
    );

}

static inline bool thaco_equipment_actuator_ArrayCommand_decode(const CanardRxTransfer* transfer, struct thaco_equipment_actuator_ArrayCommand* msg) {

    return _thaco_equipment_actuator_ArrayCommand_decode(transfer, msg);

}

#if defined(CANARD_DSDLC_INTERNAL)

static inline void __thaco_equipment_actuator_ArrayCommand_encode(uint8_t* buffer, uint32_t* bit_ofs, struct thaco_equipment_actuator_ArrayCommand* msg, bool tao);
static inline bool __thaco_equipment_actuator_ArrayCommand_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct thaco_equipment_actuator_ArrayCommand* msg, bool tao);
void __thaco_equipment_actuator_ArrayCommand_encode(uint8_t* buffer, uint32_t* bit_ofs, struct thaco_equipment_actuator_ArrayCommand* msg, bool tao) {

    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;






    canardEncodeScalar(buffer, *bit_ofs, 32, &msg->timestamp_ms);

    *bit_ofs += 32;






#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
    const uint8_t commands_len = msg->commands.len > 8 ? 8 : msg->commands.len;
#pragma GCC diagnostic pop

    if (!tao) {


        canardEncodeScalar(buffer, *bit_ofs, 4, &commands_len);
        *bit_ofs += 4;


    }

    for (size_t i=0; i < commands_len; i++) {



        __thaco_equipment_actuator_Command_encode(buffer, bit_ofs, &msg->commands.data[i], false);


    }





}

/*
 decode thaco_equipment_actuator_ArrayCommand, return true on failure, false on success
*/
bool __thaco_equipment_actuator_ArrayCommand_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct thaco_equipment_actuator_ArrayCommand* msg, bool tao) {

    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;





    canardDecodeScalar(transfer, *bit_ofs, 32, false, &msg->timestamp_ms);

    *bit_ofs += 32;








    if (!tao) {


        canardDecodeScalar(transfer, *bit_ofs, 4, false, &msg->commands.len);
        *bit_ofs += 4;



    }





    if (tao) {

        msg->commands.len = 0;
        size_t max_len = 8;
        uint32_t max_bits = (transfer->payload_len*8)-7; // TAO elements must be >= 8 bits
        while (max_bits > *bit_ofs) {

            if (!max_len-- || __thaco_equipment_actuator_Command_decode(transfer, bit_ofs, &msg->commands.data[msg->commands.len], false)) {return true;}
            msg->commands.len++;

        }

    } else {



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
        if (msg->commands.len > 8) {
            return true; /* invalid value */
        }
#pragma GCC diagnostic pop
        for (size_t i=0; i < msg->commands.len; i++) {



            if (__thaco_equipment_actuator_Command_decode(transfer, bit_ofs, &msg->commands.data[i], false)) {return true;}


        }


    }






    return false; /* success */

}
#endif
#ifdef CANARD_DSDLC_TEST_BUILD
struct thaco_equipment_actuator_ArrayCommand sample_thaco_equipment_actuator_ArrayCommand_msg(void);
#endif
#ifdef __cplusplus
} // extern "C"

#ifdef DRONECAN_CXX_WRAPPERS
#include <canard/cxx_wrappers.h>


BROADCAST_MESSAGE_CXX_IFACE(thaco_equipment_actuator_ArrayCommand, THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_ID, THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_SIGNATURE, THACO_EQUIPMENT_ACTUATOR_ARRAYCOMMAND_MAX_SIZE);


#endif
#endif
