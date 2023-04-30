
#
# Main component of TF Micro project 'micro_speech'.
#

idf_component_register(
    SRCS main.cc neural_network.cc model.cc dht11.cc
    PRIV_REQUIRES tflite-lib spi_flash
    INCLUDE_DIRS ".")
