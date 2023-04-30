/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

//#include "main_functions.h"
#include "neural_network.h"
#include "dht11.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_sleep.h"

extern "C" void app_main(void) {
  // Inicializa o modelo
  TemperatureClassifier* temp_model = new TemperatureClassifier();

  // Testa alguns valores de temperatura com o modelo
  float temperatures[] = {-1, 0, 5, 10, 15, 20, 25, 30, 35};

  for (int i = 0; i < 9; i++){
    int prediction = temp_model->predict(temperatures[i]);
    printf("Temperature: %f, Prediction: %d\n", temperatures[i], prediction);
  }


  DHT11_init(GPIO_NUM_25);

  while(1) {
      printf("Temperature is %d \n", DHT11_read().temperature);
      printf("Humidity is %d\n", DHT11_read().humidity);
      printf("Status code is %d\n", DHT11_read().status);

      // 5 seconds delay
      vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}


