######################################
# OpenOCD Flash
######################################
TARGET_BIN_ADDR = 0x10000000
TOOL = tools/openocd/bin/openocd.exe
INFACE_CFG = tools/openocd/share/openocd/scripts/interface/$(INTERFACE).cfg
CHIP_CFG = tools/openocd/share/openocd/scripts/target/rp2040.cfg

all:
	$(TOOL) -f $(INFACE_CFG) -f $(CHIP_CFG) -c "program $(BIN) verify $(TARGET_BIN_ADDR) reset exit"