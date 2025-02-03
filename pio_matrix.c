#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"

// Pinos do LED RGB
#define LED_RED 13
#define LED_GREEN 12
#define LED_BLUE 11

// Pino da matriz WS2812B
#define OUT_PIN 7

// Número de LEDs da matriz
#define NUM_PIXELS 25

// Intensidade da luz
#define BRIGHTNESS_FACTOR 0.1

// Delay para a implementação do debounce
#define DEBOUNCE_DELAY_MS 100

// Botões A e B
const uint BTN_A = 5;
const uint BTN_B = 6;

// Mapeamento dos LEDs na matriz
int mapeamento[25] = {
    24, 23, 22, 21, 20,  
    15, 16, 17, 18, 19,  
    14, 13, 12, 11, 10,  
     5,  6,  7,  8,  9,  
     4,  3,  2,  1,  0  
};

// Padrão número 0
double NUM_0[25] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0,
};

// Padrão número 1
double NUM_1[25] = {
    0, 0, 1, 0, 0,
    0, 1, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 1, 1, 1, 0
};

// Padrão número 2
double NUM_2[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0
};

// Padrão número 3
double NUM_3[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
};

// Padrão número 4
double NUM_4[25] = {
    0, 1, 0, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 0, 1, 0
};

// Padrão número 5
double NUM_5[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0 
};

// Padrão número 6
double NUM_6[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 0, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0
};

// Padrão número 7
double NUM_7[25] = {
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 0, 1, 0, 0,
    0, 1, 0, 0, 0,
    0, 1, 0, 0, 0
};

// Padrão número 8
double NUM_8[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0
};

// Padrão número 9
double NUM_9[25] = {
    0, 1, 1, 1, 0,
    0, 1, 0, 1, 0,
    0, 1, 1, 1, 0,
    0, 0, 0, 1, 0,
    0, 1, 1, 1, 0
};

volatile int current_index = 0; // Índice do vetor de desenho

// Lista de figuras
double *NUMBERS[] = {NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9};
const int NUM_COUNT = 10;  // Total de figuras disponíveis

// Converte os valores para inteiros de 0 a 255 e retorna um número uint32_t
uint32_t matrix_rgb(double g, double r, double b) {
    unsigned char R = (unsigned char)(r * 255);
    unsigned char G = (unsigned char)(g * 255);
    unsigned char B = (unsigned char)(b * 255);
    return (G << 24) | (R << 16) | (B << 8);
}

// Desenha o padrão na matriz de LEDs
void draw_pio(PIO pio, uint sm) { 
    double *desenho = NUMBERS[current_index];

    for (int i = 0; i < NUM_PIXELS; i++) {
        int idx_real = mapeamento[i];  // Usa o mapeamento correto
        uint32_t valor_led = matrix_rgb(0.0, 0.0, desenho[idx_real] * BRIGHTNESS_FACTOR);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}

// Parâmetros de tempo para o debounce
volatile uint32_t last_time_a = 0;
volatile uint32_t last_time_b = 0;

// Rotina de interrupção
void button_callback(uint gpio, uint32_t events) {
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if (gpio == BTN_A) {
        if ((now - last_time_a) > DEBOUNCE_DELAY_MS) { // Aplicação do debounce com absolute_time
            current_index = (current_index - 1 + NUM_COUNT) % NUM_COUNT; // Vai para a figura anterior
            last_time_a = now;
        }
    } else if (gpio == BTN_B) {
        if ((now - last_time_b) > DEBOUNCE_DELAY_MS) { // Aplicação do debounce com absolute_time
            current_index = (current_index + 1) % NUM_COUNT; // Vai para a próxima figura
            last_time_b = now;
        }
    }
}

int main()
{
    // Configuração de PIO
    PIO pio = pio0;
    bool ok = set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);

    // Inicialização do stdio
    stdio_init_all();

    // Inicialização do LED RGB
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    // Inicialização dos botões
    gpio_init(BTN_A);
    gpio_init(BTN_B);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_A);
    gpio_pull_up(BTN_B);

    // Chamadas de interrupção de BTN_A e BTN_B
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, 1, & button_callback);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, 1, & button_callback);

    // Loop infinito para desenhar o padrão atual e piscar o LED RGB 5x por segundo em vermelho
    while (true) {
        draw_pio(pio, sm);
        gpio_put(LED_RED, 1);
        sleep_ms(100);
        gpio_put(LED_RED, 0);
        sleep_ms(100);
    }
}

