######################################
# OpenOCD Flash
######################################
TARGET_BIN_ADDR = 0x08000000
TOOL = tools/openocd/bin/openocd.exe
INFACE_CFG = tools/openocd/share/openocd/scripts/interface/$(INTERFACE).cfg
CHIP_CFG = tools/openocd/share/openocd/scripts/target/stm32f4x.cfg

all:
	@printf "\033[31mERROR: not supported OpenOCD flash for HC32F4 yet\033[0m\n"
#$(TOOL) -f $(INFACE_CFG)  -f $(CHIP_CFG) -c init -c halt -c "flash write_image erase $(BIN) $(TARGET_BIN_ADDR)" -c reset -c shutdown
