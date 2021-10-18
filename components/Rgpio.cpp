
#include <string>

#include "Rgpio.hpp"

#define TAG "GPIO"

namespace helloWorld
{
static void main_power_isr_handler(void* arg);

void Rgpio::gpio_init(xQueueHandle* main_q)
{
    gpio_config_t io_conf;

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = BIT(GPIO_LED);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    set_gpio(GPIO_LED);

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.pin_bit_mask = BIT(GPIO_MAIN_POWER);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // io_conf.intr_type = GPIO_INTR_ANYEDGE;
    // io_conf.pin_bit_mask = BIT(GPIO_BACKUP_POWER);
    // io_conf.mode = GPIO_MODE_INPUT;
    // io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    // gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_MAIN_POWER, main_power_isr_handler, main_q);
    // gpio_isr_handler_add(GPIO_BACKUP_POWER, gpio_isr_handler, main_q);

    ESP_LOGI(TAG, "GPIO initialised.");
}

void Rgpio::set_gpio(gpio_num_t pin)
{
    ESP_LOGI(TAG, "Set GPIO %d", pin);
    gpio_set_level(pin, GPIO_SET);
}

void Rgpio::reset_gpio(gpio_num_t pin)
{
    ESP_LOGI(TAG, "Reset GPIO %d", pin);
    gpio_set_level(pin, GPIO_RESET);
}

bool Rgpio::get_gpio_status(gpio_num_t pin)
{
    bool state;
    state = gpio_get_level(pin);
    //ESP_LOGI(TAG, "Status of GPIO %d is %d", pin, state);
    return state;
}

static void main_power_isr_handler(void* arg)
{
    xQueueHandle* main_q = (xQueueHandle*)arg;
    static main_q_payload_t msg;
    msg.hdr = E_MAIN_MSG_MAIN_POWER_STATUS;
    msg.bool_payload = gpio_get_level(GPIO_MAIN_POWER);
    //ets_printf("Hey interrupt me | ");
    xQueueSendFromISR(main_q, &msg, NULL);
    //Here is the pronlem. !!!!
}

}; // namespace