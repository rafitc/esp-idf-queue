
#include <string>

#include "Rgpio.hpp"

#define TAG "GPIO"

namespace helloWorld
{
static void main_power_isr_handler(void* arg);

static xQueueHandle local_q = NULL;
static main_q_payload_t msg;
void Rgpio::gpio_init(xQueueHandle* main_q)
{
    
    gpio_config_t io_conf;
    local_q = xQueueCreate(20, sizeof(main_q_payload_t));
    
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = BIT(GPIO_LED);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    set_gpio(GPIO_LED);

    io_conf.intr_type = GPIO_INTR_NEGEDGE;
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
    gpio_isr_handler_add(GPIO_MAIN_POWER, main_power_isr_handler,local_q);
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

bool Rgpio::check_queue(){
    uint32_t *payload = msg.main_payload;
	bool bool_payload = msg.bool_payload;

    if (xQueueReceive(local_q, &msg, 200)) {
        ESP_LOGI(TAG, "Hyyya!!! Here is the queue");

        switch(msg.hdr) {
		    case E_MAIN_MSG_MAIN_POWER_STATUS:
			    ESP_LOGI(TAG, "Received gpio event");
			    break;
            default:
                ESP_LOGI(TAG, "Running default case");
                break;
        }
        return true;
    }
    else{
        ESP_LOGI(TAG, "Queue is not working");
        return false;
    }
    
}

static void main_power_isr_handler(void *arg)
{
    gpio_isr_handler_remove(GPIO_MAIN_POWER);
    
    //uint32_t* gpio_num = (uint32_t*) GPIO_MAIN_POWER;
   // xQueueHandle* local_q = (xQueueHandle*)arg;
    
    msg.hdr = E_MAIN_MSG_MAIN_POWER_STATUS;
    msg.bool_payload = true;//gpio_get_level(GPIO_MAIN_POWER);
    ets_printf("Hey interrupt me | ");
    try{
        ets_printf("Trying.");
        esp_err_t err;
        err = xQueueSendFromISR(local_q, &msg, NULL);
        if(err == pdTRUE){
            ets_printf("Done ");
        }
        else{
            ets_printf("Throwing Exception\n ");
            throw err;
        }
    }
    catch (...){
        ets_printf("GOT ERROR");
    }
    
    //Here is the pronlem. !!!!

    gpio_isr_handler_add(GPIO_MAIN_POWER, main_power_isr_handler, local_q);
}


}; // namespace