#include <stdio.h>
#include "pico/stdlib.h"

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

// Patrolibs
#include "led.h"
#include "display.h"
#include "text.h"
#include "draw.h"

#define LED_RED_PIN 13

void vTaskBlinkRedLED(void *pvParameters)
{
  (void)pvParameters; // Evita aviso de parâmetro não usado

  while (true)
  {
    pulseLed(LED_RED_PIN, 0.20);
  }
}

void vTaskUpdateDisplay(void *pvParameters)
{
  (void)pvParameters; // Evita aviso de parâmetro não usado

  while (true)
  {
    clearDisplay();
    int _b = 2;
    drawEmptyRectangle(_b, _b, SCREEN_WIDTH - 2 * _b, SCREEN_HEIGHT - 2 * _b);
    drawTextCentered("Patrocinio", 6);
    drawWave(SCREEN_HEIGHT * 2 / 3, 6.9, 6.9);
    showDisplay();
    vTaskDelay(pdMS_TO_TICKS(5)); // Atualiza a cada segundo
  }
}

void setup()
{
  stdio_init_all();

  initLeds();
  initDisplay();

  // Inicializa Task
  xTaskCreate(vTaskBlinkRedLED, "BlinkRedLED", 256, NULL, 1, NULL);
  xTaskCreate(vTaskUpdateDisplay, "UpdateDisplay", 256, NULL, 1, NULL);

  // Inicia o escalonador do FreeRTOS
  vTaskStartScheduler();
}

int main()
{
  setup();

  while (true)
  {
    tight_loop_contents(); // Loop infinito para manter o programa rodando
  }

  // Nunca chega aqui.
  return 0;
}
