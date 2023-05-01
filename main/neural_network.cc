#include "neural_network.h"

TemperatureClassifier::TemperatureClassifier() {
    // Carrega o modelo na memória (model.h)
    model_ = tflite::GetModel(__model_temp_model_tflite);
    if (model_->version() != TFLITE_SCHEMA_VERSION) {
        printf("Versão do modelo (%d) não é igual à versão suportada \
                (%d).\n", model_->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    /*
    Operações necessárias no modelo. 
    Para utilizar todas as operações disponíveis:
    static tflite::AllOpsResolver resolver;
    */
    tflite::MicroMutableOpResolver<2>*resolver = new tflite::MicroMutableOpResolver<2>();

    resolver->AddFullyConnected();
    resolver->AddLogistic();
     
    // Define o interpretador para execução do modelo
    interpreter_ = new tflite::MicroInterpreter(
        model_, 
        *resolver, 
        tensor_arena_, 
        TENSOR_ARENA_SIZE
    );

    // Alocam memória para os tensores do modelo
    TfLiteStatus allocate_status = interpreter_->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        MicroPrintf("AllocateTensors() falhou\n");
        return;
    }

    // Computa o tamanho da memória utilizada pelo modelo
    size_t used_bytes = interpreter_->arena_used_bytes();
    printf("Tensorflowlite - Total de memória usada: %d bytes\n", used_bytes);

    // Ponteiros para os tensores de entrada e saída
    input_ = interpreter_->input(0);
    output_ = interpreter_->output(0);
}



int TemperatureClassifier::predict(float* temperature){
    // Normalização através da equação Z-Score
    float norm_temp = (*temperature - norm_mean_) / norm_std_;

    // Transfere o valor normalizado para o tensor de entrada
    input_->data.f[0] = norm_temp;

    // Executa a inferência do modelo
    TfLiteStatus invoke_status = interpreter_->Invoke();
    if (invoke_status != kTfLiteOk) {
        printf("Invoke() falhou.\n");
        return -1;
    }

    // Iteração pelos valores de saída para encontrar o maior valor
    int max_index = 0;
    float value = output_->data.f[0];
    for (int i = 1; i < number_of_classes_; i++) {
        if (output_->data.f[i] > value) {
            max_index = i;
        }
    }

    // Retorna o maior index, que no caso é a própria classe
    return max_index;
}

int TemperatureClassifier::predict_quantized(float* temperature){
    // Get the input quantization parameters
    float input_scale = input_->params.scale;
    int input_zero_point = input_->params.zero_point;

    // Get the output quantization parameters
    float output_scale = output_->params.scale;
    int output_zero_point = output_->params.zero_point;

    // Normalização através da equação Z-Score
    float norm_temp = (*temperature - norm_mean_) / norm_std_;

    // Transfere o valor normalizado para o tensor de entrada
    input_->data.int8[0] = norm_temp / input_scale + input_zero_point;

    // Executa a inferência do modelo
    TfLiteStatus invoke_status = interpreter_->Invoke();
    if (invoke_status != kTfLiteOk) {
        printf("Invoke() falhou.\n");
        return -1;
    }

    // Iteração pelos valores de saída para encontrar o maior valor
    int max_index = 0;

    float value = (output_->data.int8[0] - output_zero_point) * output_scale;
    for (int i = 1; i < number_of_classes_; i++) {
        if ((output_->data.int8[i] - output_zero_point) * output_scale > value) {
            max_index = i;
        }
    }

    // Retorna o maior index, que no caso é a própria classe
    return max_index;
}