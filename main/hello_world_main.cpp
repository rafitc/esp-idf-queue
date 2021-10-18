/* Hello World Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


#include "Rgpio.hpp"
#include "RgpioContext.hpp"


#include "esp_log.h"
#include "esp_system.h"

#define TAG "MAIN-GPIO"

#define MAIN_Q_SZ	200
#define MAIN_Q_DELAY	1000

namespace helloWorld
{

extern "C"
{
	void app_main(void);
}

typedef struct {
	Rgpio* rdf_gpio;
	xQueueHandle main_q;
} rdf_ctx_t;

void process_main_q (rdf_ctx_t *rdf_ctx, main_q_payload_t msg);
static void main_q_handler(void *arg);

/******************************************************************************
 * FunctionName : app_main
 * Description  : entry of application
 * Parameters   : none
 * Returns      : none
 *******************************************************************************/

void app_main(void)
{
	// esp_err_t err;
	rdf_ctx_t rdf_ctx;
	
	ESP_LOGI(TAG, "SDK version:%s", esp_get_idf_version());

	rdf_ctx.rdf_gpio->gpio_init(&rdf_ctx.main_q);

	rdf_ctx.main_q = xQueueCreate(MAIN_Q_SZ, sizeof(main_q_payload_t));
	if (rdf_ctx.main_q == 0) {
		ESP_LOGE(TAG, "Main queue create failed!!");
	}

	xTaskCreate(main_q_handler, "main_q_handler", 2048, &rdf_ctx, 10, NULL);
}

static void main_q_handler(void *arg)
{
	rdf_ctx_t *rdf_ctx = (rdf_ctx_t *)arg;
	ESP_LOGI (TAG, "Starting main q handler...");
	while (true)
	{
		ESP_LOGI (TAG, "Hi I'm here");
		main_q_payload_t msg;
		if (xQueueReceive(rdf_ctx->main_q, &msg, 200) != pdTRUE) {
			ESP_LOGE(TAG, "Error in receiving from queue!!");
		} else {
			ESP_LOGI(TAG, "Process main q");
			process_main_q(rdf_ctx, msg);
		}
		// RDF_DBG_I(TAG, "GPIO_MAIN_POWER: %d"); //gpio_get_level(GPIO_MAIN_POWER));
		vTaskDelay(pdMS_TO_TICKS(MAIN_Q_DELAY));
	}
}

void process_main_q (rdf_ctx_t *rdf_ctx, main_q_payload_t msg)
{
	uint32_t *payload = msg.main_payload;
	bool bool_payload = msg.bool_payload;

    switch(msg.hdr) {
        case E_MAIN_MSG_MAIN_POWER_STATUS:
            ESP_LOGI(TAG, "Received gpio event");
            //break;
        default:
            ESP_LOGI(TAG, " deafult case in gpio");
            break;
    }
}

}; //namespace