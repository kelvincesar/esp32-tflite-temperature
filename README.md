# Rede Neural em ESP32

Trabalho realizado para testar conceitos de TinyML e validar a execução de uma rede neural em um ESP32 através da biblioteca Tensorflow Lite.

Foi treinado um modelo de classificação de temperatura com 
dados da cidade de Jaraguá do Sul. As temperaturas foram classificadas em:
- Frio (abaixo de 20 graus)
- Agradável (entre 20 e 25 graus)
- Quente (acima de 25 graus)

O modelo foi treinado utilizando a biblioteca Tensorflow em um ambiente Jupyter
utilizando linguagem Python.

A construção do modelo é feita através do arquivo `/train/Temperatura_Classificação.ipynb`. Neste o modelo é construido e treinado, e por fim, convertido para o formato `.tflite` que é aceito pela biblioteca do Tensorflow Lite.

Para converter o modelo para C++, utilize o seguinte comando:
```shell
xxd -i ./model/temp_model.tflite > ./main/model.cc
```

Existe uma integração com um sensor de temperatura DHT11, para realizar a classificação em tempo real dos valores de temperatura. Para isso foi conectado o sensor na GPIO 13 do ESP32.

Maiores informações, estão no documento "/docs/KCA_TinyML.pdf".


## Build
Defina o target:
```shell
idf.py set-target esp32
```

Realize o build e monitoramento do dispositivo
```shell
idf.py --port /dev/ttyUSB0 flash monitor
```

Se obtiver sucesso, a seguinte saída será exibida no terminal:

```shell
-> Temperatura: 27.000000, Classificação: 2 (Quente), Tempo de execução: 39 us
-> Temperatura: 27.000000, Classificação: 2 (Quente), Tempo de execução: 41 us
-> Temperatura: 27.000000, Classificação: 2 (Quente), Tempo de execução: 39 us
....
```

## Instalação ESP32 IDF

É necessário instalar a versão 4.4.4 do IDF para ter compatibilidade com os exemplos do Tensor Flow Lite para ESP32. Para isso, execute os seguintes comandos:

```shell
mkdir -p $HOME/.local/opt/esp
cd $HOME/.local/opt/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
git checkout -b v4.4.4 v4.4.4
git submodule update --init --recursive
bash install.sh esp32
. ./export.sh
```

Caso esteja usando ZSH (linux), pode executar o seguinte comando para adicionar a caputura do /export.s através do alias get_idf:

```shell
tee -a $HOME/.zprofile > /dev/null <<'EOF'
alias get_idf='. $HOME/.local/opt/esp/esp-idf/export.sh'
EOF
source ~/.zprofile
```

## Instalação Tensor Flow Lite

A instalação no ESP32 foi feita utilizando o seguinte repositório

https://github.com/espressif/tflite-micro-esp-examples

Os seguintes comandos foram utilizados:

```shell
git clone git@github.com:espressif/tflite-micro-esp-examples.git
cd tflite-micro-esp-examples
git submodule update --init --recursive
```


## Configuração VSCODE C++

Crie um arquivo "c_cpp_properties.json" em ".vscode" para não ter erros visuais no VS Code, com o seguinte conteúdo:

```json
{
  "configurations": [
    {
      "name": "ESP-IDF",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "compileCommands": "${workspaceFolder}/build/compile_commands.json"
    }
  ],
  "version": 4
}
```
