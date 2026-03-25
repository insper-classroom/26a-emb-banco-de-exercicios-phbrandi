#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


const int BTN_PIN_G = 28;

const int LED_PIN_Y = 5;
const int LED_PIN_B = 9;

volatile int btn_flag_G_fall;

volatile bool fired = false;
volatile int g_timer_y = 0;
volatile int g_timer_b = 0;


void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (gpio == BTN_PIN_G) {
            btn_flag_G_fall = 1;   
        }
    }
    
}

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    fired = true;
    return 0;
}
bool timer_callback_y(repeating_timer_t *rt) {
    g_timer_y = 1;
    return true; // keep repeating
}
bool timer_callback_b(repeating_timer_t *rt) {
    g_timer_b = 1;
    return true; // keep repeating
}

int main() {

    alarm_id_t alarm;
    repeating_timer_t timer_y;
    repeating_timer_t timer_b;
    bool piscar_ativo = false;
    int LED_y = 0, LED_b = 0;

    stdio_init_all();

    gpio_init(LED_PIN_Y); gpio_set_dir(LED_PIN_Y, GPIO_OUT);

    gpio_init(BTN_PIN_G); gpio_set_dir(BTN_PIN_G, GPIO_IN); gpio_pull_up(BTN_PIN_G);

    gpio_init(LED_PIN_B); gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    add_repeating_timer_ms(500, timer_callback_y, NULL, &timer_y);
    add_repeating_timer_ms(150, timer_callback_b, NULL, &timer_b);


    while (true) {
        if (btn_flag_G_fall) {
            printf("aqui");
            alarm = add_alarm_in_ms(5000, alarm_callback, NULL, false);
            piscar_ativo = true;
            btn_flag_G_fall = 0;
        }
        
        if (fired){
            piscar_ativo = false;
            gpio_put(LED_PIN_Y, 0);
            gpio_put(LED_PIN_B, 0);
        }

        if (g_timer_y) {
            if (piscar_ativo) {
                LED_y = !LED_y;               // alterna o estado do LED
                gpio_put(LED_PIN_Y, LED_y);
            }
            g_timer_y = 0;
        }
        if (g_timer_b) {
            if (piscar_ativo) {
                LED_b = !LED_b;               // alterna o estado do LED
                gpio_put(LED_PIN_B, LED_b);
            }
            g_timer_b = 0;
        }

    }
}