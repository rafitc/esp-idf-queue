/* Hello World Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <string.h>

char* TAG = "MAC ADD";


void app_main()
{
    
    int i=0;
    char macStr[13];
            //0x5c, 0xcf, 0x7f, 0x17, 0x9d, 0x28
            //5ccf7f179d287f
    unsigned char base_mac_addr[6] = {0};
    esp_efuse_mac_get_default(base_mac_addr);
    printf("\n");
    for (i=0; i<=5; i++){
        //printf("%x", base_mac_addr[i]);
        //s[i]= base_mac_addr[i];
    }
    snprintf(macStr, sizeof(macStr), "%02x%02x%02x%02x%02x%02x",
         base_mac_addr[0], base_mac_addr[1], base_mac_addr[2], base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
    printf("\n");
    printf("%s\n", macStr);
    printf("Hello world!\n");
    printf("Using \"0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x\" as base MAC address",
             base_mac_addr[0], base_mac_addr[1], base_mac_addr[2], base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    
    printf("This is ESP8266 chip with %d CPU cores, WiFi, ",
            chip_info.cores);

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
