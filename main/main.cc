/* ********************************************************************************
 * UNIVERSIDADE FEDERAL DE SANTA CATARINA - JOINVILLE
 * ESE410002-41010086ME (20231) - Análise e Concepção de Sistemas Eletrônicos
 * 
 * Aluno: Kelvin César de Andrade
 * Data: 04/2023
 * Descrição: 
 * 
 * - Trabalho 01 para exploração e testes com ferramentas de TinyML
 * 
 * Neste trabalho, foi treinado um modelo de classificação de temperatura com 
 * dados da cidade de Jaraguá do Sul. As temperaturas foram classificadas em:
 * - Frio (abaixo de 20 graus)
 * - Agradável (entre 20 e 25 graus)
 * - Quente (acima de 25 graus)
 * 
 * O modelo foi treinado utilizando a biblioteca Tensorflow em um ambiente Jupyter
 * utilizando linguagem Python.
 * 
 * Este modelo foi convertido em um modelo Tensorflow Lite e então importado neste
 * projeto.
 * 
 *********************************************************************************
 */


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
esp_timer_create_args_t timer_cfg = {
    .callback = NULL,
    .name = "meuTimer"
};

// Configuração do sensor de temperatura
#define SENSOR_TYPE DHT_TYPE_DHT11
#define GPIO_NUM    GPIO_NUM_13



void print_temperature_class (int* temp_class){
  switch (*temp_class) {
        case 0:
            printf("%d (Frio)", *temp_class);
            break;
        case 1:
            printf("%d (Agradável)", *temp_class);
            break;
        case 2:
            printf("%d (Quente)", *temp_class);
            break;
        default:
            printf("classe inválida! (%d)", *temp_class);
            break;
    }
}

// Função para teste de GPIO
void gpio_test(gpio_num_t pin) {
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, 1);
    vTaskDelay(pdMS_TO_TICKS(2000));
    gpio_set_level(pin, 0);
}

// Função para teste do modelo de classificação de temperatura
void test_temperature_model(TemperatureClassifier* model) {
  // Testa todas as temperaturas entre 10 e 30 graus, com incremento de 0.5 graus
  float temperature = 10.0;
  float time_avg = 0;
  int correct_predictions = 0;
  printf("Testando modelo com todas as temperaturas entre 10 e 30 graus:\n");


  while (temperature <= 30.0) {
    init_time = esp_timer_get_time();
    int prediction = model->predict(&temperature);
    total_time = esp_timer_get_time() - init_time;

    printf("- Temperatura: %f, Resultado: %d (executado em %lld us)\n", temperature, prediction, total_time);
    time_avg += total_time;
    if ((temperature < 20 && prediction == 0) || (temperature >= 20 && temperature <= 25 && prediction == 1) || (temperature > 25 && prediction == 2)) {
      correct_predictions++;
    } else {
      ESP_LOGE("Teste", "Erro na predição da temperatura %f", temperature);
    }
    temperature += 0.5;
  }
  printf("- Tempo médio de execução: %f us\n", time_avg/((30.0-10.0)/0.5+1));
  printf("- Porcentagem de acerto: %f %%\n", (float)correct_predictions/((30.0-10.0)/0.5+1)*100);
}


// Função principal
extern "C" void app_main(void) {
  // Inicializa o modelo
  TemperatureClassifier* temp_model = new TemperatureClassifier();
  // Inicializa timer
  esp_timer_create(&timer_cfg, &timer);
  // Realiza o teste do modelo
  test_temperature_model(temp_model);


  // Operação normal do programa

  float temperature, humidity;
  int prediction = -1;
  gpio_reset_pin(GPIO_NUM);

  // GPIO test...
  //gpio_test(GPIO_NUM);

  printf("Iniciando leitura do sensor DHT11\n");

  while (1) {
    // Leitura do sensor DHT11
    if (dht_read_float_data(SENSOR_TYPE, GPIO_NUM, &humidity, &temperature) == ESP_OK) {

      // Realiza a predição da temperatura
      init_time = esp_timer_get_time();
      int prediction = temp_model->predict(&temperature);
      total_time = esp_timer_get_time() - init_time;

      printf("-> Temperatura: %f, Classificação: ", temperature);
      print_temperature_class(&prediction);
      printf(", Tempo de execução: %lld us\n", total_time);

      //printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);


    } else {
      printf("Não foi possível comunicar com o sensor DHT11\n");
    }

    // O sensor deve ser lido a cada 2 segundos no mínimo conforme:
    // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

