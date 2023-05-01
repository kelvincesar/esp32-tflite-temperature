#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#define TENSOR_ARENA_SIZE (2 * 1024) // 2KB

// tensorflow lite library
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Modelo que será carregado
#include "model.h"

// Classe para classificação do modelo de temperatura
class TemperatureClassifier {
   public:
      TemperatureClassifier();
      int predict(float* temperature);

   private:
      const tflite::Model* model_;
      tflite::MicroInterpreter* interpreter_;
      TfLiteTensor* input_;
      TfLiteTensor* output_;

      // Número de classes na saída do modelo
      const int number_of_classes_ = 3;

      // Valores de média e desvio padrão utilizados na normalização
      const float norm_mean_ = 20.07228311;
      const float norm_std_ = 4.49116319;

      uint8_t tensor_arena_[TENSOR_ARENA_SIZE];
};



#endif // NEURAL_NETWORK_H