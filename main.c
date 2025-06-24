#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_RED_PIN 13

void vTaskBlinkRedLED(void *pvParameters)
{
  (void)pvParameters; // Evita aviso de parâmetro não usado

  gpio_init(LED_RED_PIN);
  gpio_set_dir(LED_RED_PIN, GPIO_OUT);

  while (true)
  {
    gpio_put(LED_RED_PIN, true);    // Liga o LED
    vTaskDelay(pdMS_TO_TICKS(500)); // Espera 500ms
    gpio_put(LED_RED_PIN, false);   // Desliga o LED
    vTaskDelay(pdMS_TO_TICKS(500)); // Espera mais 500ms
  }
}

void setup()
{
  stdio_init_all();

  // Inicializa Task
  xTaskCreate(vTaskBlinkRedLED, "BlinkRedLED", 256, NULL, 1, NULL);

  // Inicia o escalonador do FreeRTOS
  vTaskStartScheduler();
}

int main()
{
  setup();

  while (true)
  {
    // Nada
  }

  // Nunca chega aqui.
  return 0;
}
