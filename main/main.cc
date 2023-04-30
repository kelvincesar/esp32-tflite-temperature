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

#define SENSOR_TYPE DHT_TYPE_DHT11
#define GPIO_NUM GPIO_NUM_4

extern "C" void app_main(void) {
  // Inicializa o modelo
  TemperatureClassifier* temp_model = new TemperatureClassifier();

  // Testa alguns valores de temperatura com o modelo
  float temperatures[] = {-1, 0, 5, 10, 15, 20, 25, 30, 35};

  for (int i = 0; i < 9; i++){
    int prediction = temp_model->predict(temperatures[i]);
    printf("Temperature: %f, Prediction: %d\n", temperatures[i], prediction);
  }


 
  float temperature, humidity;
  gpio_set_pull_mode(GPIO_NUM, GPIO_PULLUP_ONLY);


  while (1)
  {
      if (dht_read_float_data(SENSOR_TYPE, GPIO_NUM, &humidity, &temperature) == ESP_OK)
          printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
      else
          printf("Could not read data from sensor\n");

      // If you read the sensor data too often, it will heat up
      // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
      vTaskDelay(pdMS_TO_TICKS(2000));
  }
}




