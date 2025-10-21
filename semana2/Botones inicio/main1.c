/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"

#define PINPULSADOR1 35
#define MASKPINPUL1 1ULL << PINPULSADOR1

static const char* TAG = "MyModule";

void app_main(void)
{
    gpio_config_t configPinPUl1 ={
        .pin_bit_mask = MASKPINPUL1,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&configPinPUl1);
    
    int estado = 0;

    while (1)
    {
        if (gpio_get_level(PINPULSADOR1)==0)
        {
            vTaskDelay(pdMS_TO_TICKS(150));
            if (gpio_get_level(PINPULSADOR1)==1)
            {
                ESP_LOGI(TAG, "deteccion suelta el pulsador");
            }
            
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    

}
