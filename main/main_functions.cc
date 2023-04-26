/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

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


#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"


#include "main_functions.h"
#include "model.h"
#include "output_handler.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;

// Número de classes na saída do modelo
const int kNumClasses = 3;
const float norm_mean = 20.07228311;
const float norm_std = 4.49116319;


constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];

}  // namespace

// The name of this function is important for Arduino compatibility.
void setup() {
  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(temp_model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    MicroPrintf("Model provided is schema version %d not equal to supported "
                "version %d.", model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    MicroPrintf("AllocateTensors() failed");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

}

float values[4] = {10.0, 21.0, 25.0, 30.0};
// The name of this function is important for Arduino compatibility.
void loop() {
      // Executa a inferência com o modelo
    for (int i = 0; i < 4; i++) {
        float input_value = values[i];  // Define a temperatura de entrada
        // Define os valores de entrada do tensor
        input->data.f[0] = (input_value - norm_mean) / norm_std;

        // Executa a inferência
        TfLiteStatus invoke_status = interpreter->Invoke();
        if (invoke_status != kTfLiteOk) {
            MicroPrintf("Invoke failed");
            return;
        }

        // Obtém os valores de saída do tensor e busca pelo índice do maior valor
        int output_class = 0;
        float max_score = output->data.f[0];
        for (int j = 1; j < kNumClasses; j++) {
            if (output->data.f[j] > max_score) {
                output_class = j;
                max_score = output->data.f[j];
            }
        }

        // Imprime o resultado da inferência
        printf("\nInput: %f, Output class: %d, Score: %f\n",
            input_value, output_class, max_score);
    }


}
