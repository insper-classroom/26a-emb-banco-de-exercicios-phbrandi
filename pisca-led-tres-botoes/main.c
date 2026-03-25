#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"


const int BTN_PIN_G = 18;
const int BTN_PIN_Y = 28;
const int BTN_PIN_B = 22;

const int LED_PIN_G = 14;
const int LED_PIN_Y = 6;
const int LED_PIN_B = 10;

volatile int btn_flag_G_up;
volatile int btn_flag_G_d;
volatile int btn_flag_Y;
volatile int btn_flag_B;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (gpio == BTN_PIN_Y) {
            btn_flag_Y = 1;   
        }
        if (gpio == BTN_PIN_G) {
            btn_flag_G_d = 1;   
        }
    }
    if (events == 0x8) {  // rise edge
        if (gpio == BTN_PIN_B) {
            btn_flag_B = 1;   
        }
        if (gpio == BTN_PIN_G) {
            btn_flag_G_up = 1;   
        }
    }
}



int main() {
    stdio_init_all();
    bool piscar_ativo_g = false;
    bool piscar_ativo_y = false;
    bool piscar_ativo_b = false;

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(LED_PIN_Y);
    gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_init(LED_PIN_B);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_RISE, true);
 
  while (true) {
    if (btn_flag_Y){
      piscar_ativo_y = !piscar_ativo_y;
      btn_flag_Y = 0;
    }
    if (btn_flag_B){
      piscar_ativo_b = !piscar_ativo_b;
      btn_flag_B = 0;
    }
    if (btn_flag_G_d){
      piscar_ativo_g = true;
    }
    if (btn_flag_G_up){
      btn_flag_G_d = 0;
      piscar_ativo_g = false;
      btn_flag_G_up = 0;
    }

    if(piscar_ativo_y){
      gpio_put(LED_PIN_Y, 1);
      // printf("ligou");
    }
    if(piscar_ativo_g){
      gpio_put(LED_PIN_G, 1);
    }
    if(piscar_ativo_b){
      gpio_put(LED_PIN_B, 1);
    }

    sleep_ms(200);

    if(piscar_ativo_y){
      gpio_put(LED_PIN_Y, 0);
      // printf("apagou");
    }
    if(piscar_ativo_g){
      gpio_put(LED_PIN_G, 0);
    }
    if(piscar_ativo_b){
      gpio_put(LED_PIN_B, 0);
    }

    sleep_ms(200);

  }
}