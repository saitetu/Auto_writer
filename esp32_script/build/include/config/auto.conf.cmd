deps_config := \
	/c/esp-idf/components/bt/Kconfig \
	/c/esp-idf/components/esp32/Kconfig \
	/c/esp-idf/components/ethernet/Kconfig \
	/c/esp-idf/components/freertos/Kconfig \
	/c/esp-idf/components/log/Kconfig \
	/c/esp-idf/components/lwip/Kconfig \
	/c/esp-idf/components/mbedtls/Kconfig \
	/c/esp-idf/components/openssl/Kconfig \
	/c/esp-idf/components/spi_flash/Kconfig \
	/c/esp-idf/components/bootloader/Kconfig.projbuild \
	/c/esp-idf/components/esptool_py/Kconfig.projbuild \
	/c/esp-idf/components/partition_table/Kconfig.projbuild \
	/c/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
