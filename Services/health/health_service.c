#include "health_service.h"

#include <stddef.h>

#include "dronecan.h"
#include "status_led.h"

static health_service_node_status_t node_status;
static health_service_observer_t status_observer;
static void *status_observer_user_reference;
static uint32_t received_count;

static void on_node_status(const DronecanNodeStatus *status,
                           void *user_reference)
{
    (void) user_reference;

    node_status.can_id = status->can_id;
    node_status.uptime_sec = status->uptime_sec;
    node_status.vendor_specific_status_code =
        status->vendor_specific_status_code;
    node_status.source_node_id = status->source_node_id;
    node_status.transfer_id = status->transfer_id;
    node_status.priority = status->priority;
    node_status.health = status->health;
    node_status.mode = status->mode;
    node_status.sub_mode = status->sub_mode;
    node_status.valid = true;
    received_count++;

    if (status_observer != NULL)
    {
        status_observer(&node_status, status_observer_user_reference);
    }
}

void health_service_init(void)
{
    node_status = (health_service_node_status_t) {0};
    received_count = 0U;
    dronecan_set_node_status_handler(on_node_status, NULL);
}

void health_service_set_observer(health_service_observer_t observer,
                                 void *user_reference)
{
    status_observer = observer;
    status_observer_user_reference = user_reference;
}

bool health_service_get_node_status(health_service_node_status_t *out_status)
{
    if ((out_status == NULL) || !node_status.valid)
    {
        return false;
    }

    *out_status = node_status;
    return true;
}

uint32_t health_service_get_received_count(void)
{
    return received_count;
}

void health_service_signal_activity(void)
{
    status_led_toggle();
}
