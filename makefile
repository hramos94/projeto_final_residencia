.PHONY: all clean reb aux


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
