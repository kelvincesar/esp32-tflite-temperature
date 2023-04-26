# Tensorflow Lite - Temperature classify with ESP32

Convert model:
```shell
xxd -i ./model/temp_model.tflite > ./main/model.cc
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
