#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define NUM_PIXELS 25       // Número total de LEDs (5x5)
#define WS2812_PIN 7        // GPIO usado para controlar os LEDs
#define IS_RGBW false       // Use false para RGB e true para RGBW


const int buzzer = 21;
const int botao = 5;    
//--------------------------------

// Função para gerar um beep intermitente
void beep(); 
// Função de travessia de pedestres
void travessia_pedestre();
// Ativar o LED verde
void sinal_verde();
// Ativar o LED amarelo
void sinal_amarelo();
// Ativar o LED vermelho
void sinal_vermelho();
//Todos os LEDs apagados.
void low_Lends();
// Função para inicializar LEDs, botão e buzzer
void setup_init();
// Função genérica para verificar o botão durante a execução do Semáforo
void verificar_executa(void (*sinal)(), int tempo);
void set_leds(bool red, bool green, bool blue);


// Função para definir a cor de um pixel
void set_pixel_color(uint32_t *pixels, int index, uint8_t r, uint8_t g, uint8_t b) {
    pixels[index] = ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b);
}

// Função para enviar o buffer de LEDs para o WS2812
void send_pixels(PIO pio, int sm, uint32_t *pixels, int num_pixels) {
    for (int i = 0; i < num_pixels; i++) {
        pio_sm_put_blocking(pio, sm, pixels[i] << 8);  // Ajuste necessário para o WS2812
    }
}

int main() {
    stdio_init_all();
    // Inicialização do LED de pedestres
    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_init(12);
    gpio_set_dir(12, GPIO_OUT);
    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);
    gpio_put(11, 0);
 
  // Inicialização do botão
  gpio_init(botao);
  gpio_set_dir(botao, GPIO_IN);
  gpio_pull_up(botao);
 
 
 // Inicialização do buzzer
  gpio_init(buzzer);
  gpio_set_dir(buzzer, GPIO_OUT);
  gpio_set_function(buzzer, 100);
    uint slice_num = pwm_gpio_to_slice_num(buzzer);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (100 * 4096));
    pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(buzzer, 0);

while (true) {
    // Semáforo o sinal verde
    verificar_executa(sinal_verde,8000);
    
    // Semáforo o sinal amarelo
    verificar_executa(sinal_amarelo,2000);
    
    // Semáforo o sinal vermelho
    verificar_executa(sinal_vermelho,10000);
   
  }


}

// Função genérica para verificar o botão durante a execução do Semáforo
void verificar_executa(void (*sinal)(), int tempo) {
   
    // Dividir o tempo em passos de 10ms para checar o botão
    for (int i = 0; i < (tempo / 10); i++) {
       // Acender o sinal correspondente
       sinal();
        if (gpio_get(botao) == 0) {
            // Botão pressionado, executa a travessia
            travessia_pedestre();
            break;
        }
        sleep_ms(10); // Espera 10ms por iteração
    }
}

// Função de travessia de pedestres
void travessia_pedestre() {
    // Ativar o LED amarelo por 5 segundos
     low_Lends();
     sleep_ms(5);
    sinal_amarelo();
    sleep_ms(5000);

    // Ativar o LED vermelho e o LED verde para pedestres
    low_Lends();
    sleep_ms(10);
    sinal_vermelho();
    set_leds(0,1,0);

    // Acionar o buzzer intermitente por 15 segundos
    for (int i = 0; i < 15; i++) {
        beep();
        sleep_ms(1000); // Pausa de 1 segundo entre os beeps
    }

    // Desativar LEDs e buzzer
    low_Lends();
    set_leds(0,0,0);
    pwm_set_gpio_level(buzzer, 0);
    return;
}

// Função para gerar um beep intermitente
void beep() {
    uint slice_num = pwm_gpio_to_slice_num(buzzer);
    pwm_set_gpio_level(buzzer, 2048); // Ativar PWM (50% duty cycle)
    sleep_ms(100); // Duração do beep
    pwm_set_gpio_level(buzzer, 0);   // Desativar PWM
    sleep_ms(10); // Pausa entre os beeps
}


// Ativar o LED verde
void sinal_verde(){
    set_leds(0,0,0);
     PIO pio = pio0;
    int sm = 0;

    uint32_t pixels[NUM_PIXELS] = {0};  // Array para armazenar as cores dos LEDs

    // Inicializa o programa WS2812 no PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    for (int i = 0; i < NUM_PIXELS; i++) {
            set_pixel_color(pixels, i, 0, 255, 0);  // Verde
        }
        send_pixels(pio, sm, pixels, NUM_PIXELS);

} 
// Ativar o LED amarelo
void sinal_amarelo(){
    set_leds(0,0,0);
     PIO pio = pio0;
    int sm = 0;

    uint32_t pixels[NUM_PIXELS] = {0};  // Array para armazenar as cores dos LEDs

    // Inicializa o programa WS2812 no PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    for (int i = 0; i < NUM_PIXELS; i++) {
            set_pixel_color(pixels, i, 255, 255, 0);  // Amarelo
        }
        send_pixels(pio, sm, pixels, NUM_PIXELS);
} 
// Ativar o LED vermelho
void sinal_vermelho( ){
     set_leds(0,0,0);
      PIO pio = pio0;
    int sm = 0;

    uint32_t pixels[NUM_PIXELS] = {0};  // Array para armazenar as cores dos LEDs

    // Inicializa o programa WS2812 no PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

  for (int i = 0; i < NUM_PIXELS; i++) {
            set_pixel_color(pixels, i, 255, 0, 0);  // Vermelho
        }
        send_pixels(pio, sm, pixels, NUM_PIXELS);
  
} 
//Todos os LEDs apagados.
void low_Lends(){
    set_leds(0,0,0);
 PIO pio = pio0;
    int sm = 0;

    uint32_t pixels[NUM_PIXELS] = {0};  // Array para armazenar as cores dos LEDs

    // Inicializa o programa WS2812 no PIO
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    for (int i = 0; i < NUM_PIXELS; i++) {
            set_pixel_color(pixels, i, 0, 0, 0); 
        }
        send_pixels(pio, sm, pixels, NUM_PIXELS);

}
void set_leds(bool red, bool green, bool blue){
    gpio_put(13, red);
    gpio_put(11, green);
    gpio_put(12, blue);
}