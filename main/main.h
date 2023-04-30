#ifdef MAIN_H
#define MAIN_H
#include "neural_network.h"
#include "dht11.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include <esp_log.h>
#include <esp_timer.h>

// Variáveis para medição de tempo
esp_timer_handle_t timer = NULL;
uint64_t init_time, total_time;
// Inicializa o timer
esp_timer_create_args_t timer_cfg = {
    .callback = NULL,
    .name = "meuTimer"
};


void test_temperature_model(TemperatureClassifier* model);
void gpio_test(gpio_num_t pin);
void print_temperature_class (int* temp_class);

#endif // MAIN_H