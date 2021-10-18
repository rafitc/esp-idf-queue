
#ifndef _RDF_GPIO_H_
#define _RDF_GPIO_H_

#include <string>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"

#define GPIO_SET 1
#define GPIO_RESET 0

#define GPIO_LED GPIO_NUM_16
#define GPIO_MAIN_POWER GPIO_NUM_12

namespace helloWorld
{

typedef enum {
	E_MAIN_MSG_NONE = 0,
	E_MAIN_MSG_MAIN_POWER_STATUS,
} e_main_msg_t;

typedef struct {
	e_main_msg_t hdr;
	uint32_t main_payload[2];
	bool bool_payload;
} main_q_payload_t;


class Rgpio {
private:
    /* data */

public:
    void gpio_init(xQueueHandle *main_q);
    void set_gpio(gpio_num_t pin);
    void reset_gpio(gpio_num_t pin);
    bool get_gpio_status(gpio_num_t pin);
    void gpio_evt_handler(void* arg);
};

};

#endif // _RDF_GPIO_H_