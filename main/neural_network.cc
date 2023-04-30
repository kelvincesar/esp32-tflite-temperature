#include "neural_network.h"

TemperatureClassifier::TemperatureClassifier() {
    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    model_ = tflite::GetModel(__model_temp_model_tflite);
    if (model_->version() != TFLITE_SCHEMA_VERSION) {
        printf("Model provided is schema version %d not equal to supported "
               "version %d.\n", model_->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    // Operações necessárias no modelo
    tflite::MicroMutableOpResolver<6>*resolver = new tflite::MicroMutableOpResolver<6>();

    resolver->AddFullyConnected();
    resolver->AddReshape();
    resolver->AddSoftmax();
    resolver->AddLogistic();
    resolver->AddMul();
    resolver->AddAdd();
    
    /*  
    outras operações:  
        resolver->AddQuantize();
        resolver->AddDequantize();
    
    para coletar todas as operações disponíveis:
        static tflite::AllOpsResolver resolver;
    */
    

    // Build an interpreter to run the model
    interpreter_ = new tflite::MicroInterpreter(
        model_, 
        *resolver, 
        tensor_arena_, 
        TENSOR_ARENA_SIZE
    );

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter_->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        MicroPrintf("AllocateTensors() failed\n");
        return;
    }

    size_t used_bytes = interpreter_->arena_used_bytes();
    printf("Tensorflowlite used bytes %d\n", used_bytes);

    // Obtain pointers to the model's input and output tensors.
    input_ = interpreter_->input(0);
    output_ = interpreter_->output(0);
}



int TemperatureClassifier::predict(float* temperature){
    // Normalização
    
    float norm_temp = (*temperature - norm_mean_) / norm_std_;

    // Input
    input_->data.f[0] = norm_temp;

    // Run inference
    TfLiteStatus invoke_status = interpreter_->Invoke();
    if (invoke_status != kTfLiteOk) {
        printf("Invoke failed\n");
        return -1;
    }

    // Output
    int max_index = 0;
    float value = output_->data.f[0];
    for (int i = 1; i < number_of_classes_; i++) {
        if (output_->data.f[i] > value) {
            max_index = i;
        }
    }

    return max_index;
}