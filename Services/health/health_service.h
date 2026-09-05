#ifndef HEALTH_SERVICE_H
#define HEALTH_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    HEALTH_SERVICE_NODE_HEALTH_OK = 0,
    HEALTH_SERVICE_NODE_HEALTH_WARNING = 1,
    HEALTH_SERVICE_NODE_HEALTH_ERROR = 2,
    HEALTH_SERVICE_NODE_HEALTH_CRITICAL = 3,
} health_service_node_health_t;

typedef enum
{
    HEALTH_SERVICE_NODE_MODE_OPERATIONAL = 0,
    HEALTH_SERVICE_NODE_MODE_INITIALIZATION = 1,
    HEALTH_SERVICE_NODE_MODE_MAINTENANCE = 2,
    HEALTH_SERVICE_NODE_MODE_SOFTWARE_UPDATE = 3,
    HEALTH_SERVICE_NODE_MODE_OFFLINE = 7,
} health_service_node_mode_t;

typedef struct
{
    uint32_t can_id;
    uint32_t uptime_sec;
    uint16_t vendor_specific_status_code;
    uint8_t source_node_id;
    uint8_t transfer_id;
    uint8_t priority;
    uint8_t health;
    uint8_t mode;
    uint8_t sub_mode;
    bool valid;
} health_service_node_status_t;

typedef void (*health_service_observer_t)(
    const health_service_node_status_t *status,
    void *user_reference);

void health_service_init(void);
void health_service_set_observer(health_service_observer_t observer,
                                 void *user_reference);
bool health_service_get_node_status(health_service_node_status_t *out_status);
uint32_t health_service_get_received_count(void);
void health_service_signal_activity(void);

#endif /* HEALTH_SERVICE_H */
