#include <stdio.h>

#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PIN_TO_APP_CORE (0)
#define SLEEP_PERIOD_US (1 * 1000 * 1000LLU)

static void sleep_task(void* p)
{
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(SLEEP_PERIOD_US));
    while (1) {
        printf("sleeping...\n");
        vTaskDelay(pdMS_TO_TICKS(100));
        ESP_ERROR_CHECK(esp_light_sleep_start());
        printf("wake up.\n");
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    if (PIN_TO_APP_CORE) {
        xTaskCreatePinnedToCore(sleep_task, "sleep", 4096, NULL, 1, NULL, APP_CPU_NUM);
    } else {
        xTaskCreate(sleep_task, "sleep", 4096, NULL, 1, NULL);
    }
}
