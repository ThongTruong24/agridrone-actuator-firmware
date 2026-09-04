
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <canard.h>




#define THACO_EQUIPMENT_ACTUATOR_COMMAND_MAX_SIZE 4
#define THACO_EQUIPMENT_ACTUATOR_COMMAND_SIGNATURE (0x4F272607EAA3CC78ULL)






struct thaco_equipment_actuator_Command {




    uint8_t actuator_id;



    uint8_t value;



    uint16_t command_id;



};

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t _thaco_equipment_actuator_Command_encode(struct thaco_equipment_actuator_Command* msg, uint8_t* buffer
#if CANARD_ENABLE_TAO_OPTION
    , bool tao
#endif
);
bool _thaco_equipment_actuator_Command_decode(const CanardRxTransfer* transfer, struct thaco_equipment_actuator_Command* msg);

static inline uint32_t thaco_equipment_actuator_Command_encode(struct thaco_equipment_actuator_Command* msg, uint8_t* buffer
#if CANARD_ENABLE_TAO_OPTION
    , bool tao
#endif
) {

    return _thaco_equipment_actuator_Command_encode(msg, buffer
#if CANARD_ENABLE_TAO_OPTION
    , tao
#endif
    );

}

static inline bool thaco_equipment_actuator_Command_decode(const CanardRxTransfer* transfer, struct thaco_equipment_actuator_Command* msg) {

    return _thaco_equipment_actuator_Command_decode(transfer, msg);

}

#if defined(CANARD_DSDLC_INTERNAL)

static inline void __thaco_equipment_actuator_Command_encode(uint8_t* buffer, uint32_t* bit_ofs, struct thaco_equipment_actuator_Command* msg, bool tao);
static inline bool __thaco_equipment_actuator_Command_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct thaco_equipment_actuator_Command* msg, bool tao);
void __thaco_equipment_actuator_Command_encode(uint8_t* buffer, uint32_t* bit_ofs, struct thaco_equipment_actuator_Command* msg, bool tao) {

    (void)buffer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;






    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->actuator_id);

    *bit_ofs += 8;






    canardEncodeScalar(buffer, *bit_ofs, 8, &msg->value);

    *bit_ofs += 8;






    canardEncodeScalar(buffer, *bit_ofs, 16, &msg->command_id);

    *bit_ofs += 16;





}

/*
 decode thaco_equipment_actuator_Command, return true on failure, false on success
*/
bool __thaco_equipment_actuator_Command_decode(const CanardRxTransfer* transfer, uint32_t* bit_ofs, struct thaco_equipment_actuator_Command* msg, bool tao) {

    (void)transfer;
    (void)bit_ofs;
    (void)msg;
    (void)tao;





    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->actuator_id);

    *bit_ofs += 8;







    canardDecodeScalar(transfer, *bit_ofs, 8, false, &msg->value);

    *bit_ofs += 8;







    canardDecodeScalar(transfer, *bit_ofs, 16, false, &msg->command_id);

    *bit_ofs += 16;





    return false; /* success */

}
#endif
#ifdef CANARD_DSDLC_TEST_BUILD
struct thaco_equipment_actuator_Command sample_thaco_equipment_actuator_Command_msg(void);
#endif
#ifdef __cplusplus
} // extern "C"

#ifdef DRONECAN_CXX_WRAPPERS
#include <canard/cxx_wrappers.h>

#endif
#endif
