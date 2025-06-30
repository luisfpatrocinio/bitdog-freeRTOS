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
#include "buzzer.h"
#include "microphone.h"
#include "buttons.h"
#include "analog.h"

// Project
#include "happyBirthday.h"
int playedVerses = 0;
int state = 0;
int secret1 = 0;
int secret2 = 0;
int secret3 = 0;
int secret4 = 0;
int secret5 = 0;

#define LED_RED_PIN 13

void vTaskBuzzer(void *pvParameters);

void vTaskBlinkRedLED(void *pvParameters)
{
  (void)pvParameters; // Evita aviso de parâmetro não usado

  while (true)
  {
    // pulseLed(LED_RED_PIN, 0.20);
    // pulseLed(LED_GREEN_PIN, 0.450);
    // pulseLed(LED_BLUE_PIN, 0.350);
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

    switch (state)
    {
    case 0: // apresentacao
      drawTextCentered("Aperte o botao", 26);
      drawTextCentered("para comecar.", 35);
      if (isButtonPressed(BTA) || isButtonPressed(BTB))
      {
        state = 1;                      // Muda para o estado de canto
        playedVerses = 0;               // Reinicia os versos tocados
        vTaskDelay(pdMS_TO_TICKS(500)); // Espera meio segundo
      }
      break;
    case 1:
      if (playedVerses >= 0)
        drawTextCentered("Parabens pra voce", 6);
      if (playedVerses >= 1)
        drawTextCentered("nessa data querida", 16);
      if (playedVerses >= 2)
        drawTextCentered("muitas felicidades", 26);
      if (playedVerses >= 3)
        drawTextCentered("muitos anos de vida!", 36);

      if (isButtonPressed(BTA) && playedVerses == 1)
      {
        secret1 = 1; // Ativa o segredo 1
      }
      break;
    case 2:
      static int ledInt = 0;
      if (ledInt < 255)
      {
        ledInt++;
      }
      setLedBrightness(LED_RED_PIN, ledInt);

      char micValue[16];
      int micValueInt = readMicrophone();
      snprintf(micValue, sizeof(micValue), "%d", micValueInt);

      static int waitTime = 0;

      if (micValueInt > 1200) // Valor de mic alto
      {
        playTone(1000, 200); // Toca um tom alto
        state = 3;           // Muda para o estado de fim
        setLedBrightness(LED_RED_PIN, 0);
      }
      else
      {

        if (waitTime < 200)
        {
          drawTextCentered("Assopra a vela!", SCREEN_HEIGHT / 2);
          waitTime += 1; // Incrementa o tempo de espera
        }
        else
        {
          drawTextCentered("ta bugado kkkk", SCREEN_HEIGHT / 2 + 10);
          drawTextCentered("aperta o botao msm", SCREEN_HEIGHT / 2 + 20);

          if (isButtonPressed(BTA) || isButtonPressed(BTB))
          {
            state = 3;         // Muda para o estado de fim
            playedVerses = 0;  // Reinicia os versos tocados
            playTone(200, 50); // Toca um tom inicial
            secret3 = 1;       // Ativa o segredo 3
          }

          // desenhar micvalue
          // drawTextCentered(micValue, SCREEN_HEIGHT - 10);
        }
      }

      break;
    case 3:

      setLedBrightness(LED_RED_PIN, 0);
      // Estado de fim, onde o bolo é desenhado
      drawTextCentered("Parabens", SCREEN_HEIGHT / 2 - 10);
      drawTextCentered("Jota Danado!", SCREEN_HEIGHT / 2);
      showDisplay();

      static int waitTimeToState4 = 0;
      if (waitTimeToState4 < 80)
      {
        waitTimeToState4 += 1; // Incrementa o tempo de espera
        if (isButtonPressed(BTB))
        {
          secret2 = 1;
        }
      }
      else
      {
        state = 4; // Muda para o estado de mensagem final
      }
      break;
    case 4:
      static float _y = 0.01;
      _y -= 0.25; // Move o texto para cima lentamente
      drawTextCenteredAlt("feliz aniversario jv", _y + 10);
      drawTextCenteredAlt("do turn back", _y + 20);
      drawTextCenteredAlt("the pain", _y + 30);
      drawTextCenteredAlt("(e do vou assistir)", _y + 40);
      drawTextCenteredAlt("para a vida", _y + 50);
      drawTextCenteredAlt("seja muito feliz", _y + 60);
      drawTextCenteredAlt("e vamo comemorar", _y + 70);
      drawTextCenteredAlt("assim que der", _y + 80);
      drawTextCenteredAlt("tamo junto manito", _y + 90);
      drawTextCenteredAlt("abraco do Patro", _y + 100);
      drawTextCenteredAlt("nao consegui entregar", _y + 110);
      drawTextCenteredAlt("ate 00h, mas o que", _y + 120);
      drawTextCenteredAlt("vale eh a intencao.", _y + 130);

      if (readAnalogX() < 0)
      {
        secret4 = 1;
      }

      if (readAnalogX() > 0)
      {
        secret5 = 1;
      }

      if (_y < -120)
      {
        state = 5;
      }

      break;
    case 5:
      int totalSecrets = secret1 + secret2 + secret3 + secret4 + secret5;

      if (totalSecrets > 0)
      {
        char secretMessage[64];
        snprintf(secretMessage, sizeof(secretMessage), "encontrados: %d", totalSecrets);
        drawTextCentered("Segredos", 12);
        drawTextCentered(secretMessage, 18);

        switch (totalSecrets)
        {
        case 1:
          drawTextCentered("continue tentando", 32);
          break;
        case 2:
          drawTextCentered("ainda falta mto", 32);
          drawTextCentered("para a pasta...", 38);
          break;
        case 3:
          drawTextCentered("ta quase la!", 32);
          drawTextCentered("falta pouco...", 38);
          drawTextCentered("...para a pasta 14", 44);
          break;
        case 4:
          drawTextCentered("so mais um segredo", 32);
          drawTextCentered("e voce desbloqueia", 38);
          drawTextCentered("a pasta 14!", 44);
          break;
        case 5:
          drawTextCentered("parabens! todos os", 32);
          drawTextCentered("segredos encontrados", 38);
          drawTextCentered("pasta 14 liberada!", 44);
          drawTextCentered("senha: patropica", 52);
          break;
        }
      }
      else
      {
        drawTextCentered("nenhum segredo", 30);
        drawTextCentered("encontrado", 36);
      }
      showDisplay();

      static int timeToReset = 0;
      if (timeToReset < 50 + 20 * totalSecrets) // Tempo de espera baseado no número de segredos
      {
        timeToReset += 1; // Incrementa o tempo de espera
      }
      else
      {
        state = 0;        // Reseta o estado para reiniciar o processo
        playedVerses = 0; // Reinicia os versos tocados
        secret1 = 0;      // Reseta o segredo 1
        secret2 = 0;      // Reseta o segredo 2
        secret3 = 0;      // Reseta o segredo 3
        secret4 = 0;
        secret5 = 0;          // Reseta o segredo 5
        waitTimeToState4 = 0; // Reseta o tempo de espera para o estado 4
        waitTime = 0;         // Reseta o tempo de espera para o estado 2
        playTone(200, 50);    // Toca um tom inicial
        _y = 0.01;            // Reseta a posição do texto
        ledInt = 0;           // Reseta a intensidade do LED vermelho
        timeToReset = 0;      // Reseta o tempo de espera para reiniciar
        clearDisplay();       // Limpa a tela para reiniciar o processo
        xTaskCreate(vTaskBuzzer, "Buzzer", 256, NULL, 1, NULL);
      }

      break;
    }

    // drawTextCentered(micValue, 24);

    // Desenhar bolo
    // drawImage(cake, cake_size, 0, 0);

    showDisplay();
    vTaskDelay(pdMS_TO_TICKS(5)); // Atualiza a cada segundo
  }
}

void vTaskBuzzer(void *pvParameters)
{
  (void)pvParameters; // Evita aviso de parâmetro não usado

  while (state == 0)
  {
    vTaskDelay(pdMS_TO_TICKS(100)); // Espera 100ms antes de verificar novamente
  }

  int num_notes = sizeof(happy_birthday_tune) / sizeof(note_t);
  for (int i = 0; i < num_notes; i++)
  {
    playTone(happy_birthday_tune[i].freq, happy_birthday_tune[i].duration_ms);
    int _den = 5;
    if (playedVerses > 1)
      _den = 6;
    playedVerses = i / _den;
    vTaskDelay(pdMS_TO_TICKS(PAUSE_BETWEEN_NOTES));
  }

  vTaskDelay(pdMS_TO_TICKS(2000));
  state = 2;         // waiting for blow
  vTaskDelete(NULL); // Encerra a própria task
}

void setup()
{
  stdio_init_all();

  initLeds();
  initDisplay();
  initBuzzerPWM();
  initMicrophone();
  initButtons();
  initAnalog();

  // Inicializa Task
  xTaskCreate(vTaskBlinkRedLED, "BlinkRedLED", 256, NULL, 1, NULL);
  xTaskCreate(vTaskUpdateDisplay, "UpdateDisplay", 256, NULL, 1, NULL);
  xTaskCreate(vTaskBuzzer, "Buzzer", 256, NULL, 1, NULL);

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
