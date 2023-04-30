
#
# Main component of TF Micro project 'micro_speech'.
#

idf_component_register(
    SRCS main.cc neural_network.cc output_handler.cc model.cc
    PRIV_REQUIRES tflite-lib spi_flash
    REQUIRES Adafruit_DHT
    INCLUDE_DIRS ".")
