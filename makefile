.PHONY: all clean reb aux can delcan downcan


REB_DIR = reb
AUX_DIR = aux

all:
	@echo "Compiling REB\n"
	$(MAKE) -C $(REB_DIR)
	@echo "\nCompiling AUX\n"
	$(MAKE) -C $(AUX_DIR)

clean:
	@echo "\nCleaning REB\n"
	$(MAKE) -C $(REB_DIR) clean
	@echo "\nCleaning AUX\n"
	$(MAKE) -C $(AUX_DIR) clean


reb:
	$(REB_DIR)/bin/reb

aux:
	$(AUX_DIR)/bin/aux

can:
	@echo "creating vcan0 interface"
	sudo ip link add dev vcan0 type vcan || true
	@echo "seting up vcan0 interface"
	sudo ip link set up vcan0
	@echo "vcan0 is up."

downcan:
	@echo "Bringing vcan0 interface down"
	sudo ip link set vcan0 down || true
	@echo "vcan0 is down.

delcan:
	@echo "Deleting vcan0 interface"
	sudo ip link delete vcan0 type vcan || true
	@echo "vcan0 has been removed."

doc:
	doxygen Doxyfile
	xdg-open docs/html/index.html
