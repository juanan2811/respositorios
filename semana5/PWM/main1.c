/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

 //librerias

#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "driver/ledc.h"


//constantes
#define PINSENSORTEMP 36
#define MASKSENSORTEMP 1ULL << PINSENSORTEMP

#define PWMFREC 4000 
#define PWMDUTYRES 10

#define PINACTUADORLED 22
#define MASKACTUADORLED 1ULL << PINACTUADORLED
#define canalSensorTemp 0
static const char* TAG = "MyModule";

//declaración funciones task
void tareaSensorTemperatura( void *);
void tareaActuadorLed( void * ); 

//handlesç
adc_oneshot_unit_handle_t h_CAD1;
TaskHandle_t h_tareaSensorTemperatura;
TaskHandle_t h_tareaActuadorLed;

void app_main(void)//main principal
{
    
    //creación tareas

    xTaskCreate( tareaSensorTemperatura, "tareaSensorTemperatura", 2048, NULL, 1, & h_tareaSensorTemperatura );
     xTaskCreate( tareaActuadorLed, "tareaActuadorLed", 2048, NULL, 1, & h_tareaActuadorLed );

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(10000));
        ESP_LOGI(TAG, "SOY EL MAIN");
    }
}

//definición funciones tareas
void vATaskFunction( void * parametro)//main de la tarea sensor temperatura led
{
    //Configurar pin GPIO 36 como entrada
    gpio_config_t configGPIO36 = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pin_bit_mask = MASKSENSORTEMP,
    };

    gpio_config(&configGPIO36);

    adc_oneshot_unit_init_cfg_t configCAD1= {
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
        .unit_id = ADC_UNIT_1,
    };


    adc_oneshot_chan_cfg_t confinCanalCadSensorTemp = {
        .atten = ADC_ATTEN_DB_6,
        .bitwidth = ADC_BITWIDTH_10,
    };

     adc_oneshot_config_channel (h_CAD1 , canalSensorTemp , & confinCanalCadSensorTemp);
    
     int valorCad;
    while(1)
    {
        adc_oneshot_read (h_CAD1 , canalSensorTemp , & valorCad);
        ESP_LOGI(TAG, "valor CAD: %d", valorCad);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void tareaActuadorLed( void * parametros ) //main tarea
{
     
    gpio_config_t configGPIO22 = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pin_bit_mask = MASKSENSORTEMP,
    };
    
    ledc_timer_config_t configPWMLed = {
        .clk_cfg =  LEDC_USE_REF_TICK,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .freq_hz = PWMFREC,
        .duty_resolution = PWMDUTYRES,
        .timer_num = LEDC_TIMER_0,
     };
    
    ledc_timer_config( &configPWMLed);

    ledc_channel_config_t configPWMChannelLed = {
        .channel = LEDC_CHANNEL_0,
        .gpio_num = PINACTUADORLED,
        .duty = 0,
        .hpoint = 0,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .sleep_mode = LEDC_SLEEP_MODE_INVALID,
        
    };

    ledc_channel_config (&configPWMChannelLed)


    int dut = 0
    int salto = pow(2, PWMDUTYRES) /4;




    while (1)
    {
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, dut   );
    }
    
}
