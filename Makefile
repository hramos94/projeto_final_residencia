.PHONY: all clean reb aux can delcan downcan test cov misra misra-aux misra-bsw misra-reb misra-clean
MAKEFLAGS += --no-print-directory

REB_DIR = reb
AUX_DIR = aux

AUX_REPORT_DIR = misra/aux
BSW_REPORT_DIR = misra/bsw
REB_REPORT_DIR = misra/reb

AUXSRCFOLDER = ./aux/src
AUXINCFOLDER = ./aux/inc

BSWSRCFOLDER = ./bsw/src
BSWINCFOLDER = ./bsw/inc

REBSRCFOLDER = ./reb/src
REBINCFOLDER = ./reb/inc

AUX_SRC_FILES = $(wildcard $(AUXSRCFOLDER)/*.c) 
BSW_SRC_FILES = $(wildcard $(BSWSRCFOLDER)/*.c) 
REB_SRC_FILES = $(wildcard $(REBSRCFOLDER)/*.c) 


all:
	@echo "Compiling REB\n"
	$(MAKE) -C $(REB_DIR)
	@echo "\nCompiling AUX\n"
	$(MAKE) -C $(AUX_DIR)

clean:
	@echo "\nCleaning REB\n"
	@$(MAKE) -C $(REB_DIR) clean
	@echo "\nCleaning AUX\n"
	@$(MAKE) -C $(AUX_DIR) clean
	@echo "\nCleaning TEST/Cov\n"
	@make -C test clean


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


test:
	@make -C test

cov:
	make -C test cov

doc:
	doxygen Doxyfile
	xdg-open docs/html/index.html


misra:
	make misra-clean
	make misra-aux
	make misra-bsw
	make misra-reb
	@echo "Generating Summary Report"
	@python3 Misra_Report.py
	@echo "Done"


misra-aux:
	@mkdir -p misra
	@mkdir -p $(AUX_REPORT_DIR)
	@for file in $(AUX_SRC_FILES); do \
		FILENAME=$$(basename $$file); \
		OUTPUT_FILE=$(AUX_REPORT_DIR)/$$(basename $$FILENAME .c)_misra.txt; \
		cppcheck --enable=style --addon=./misra.json --inline-suppr  --library=posix --force --template="[{file}:{line}]: => [{severity} - {id}]: {message}\t {code}" \
			-I $(AUXINCFOLDER) -I $(BSWINCFOLDER) \
			$$file \
			--output-file=$$OUTPUT_FILE; \
	done
	@echo "Formatting Report"
	@python3 Misra_Formatter.py misra/aux
	@echo "Done"

misra-bsw:
	@mkdir -p $(BSW_REPORT_DIR)
	@for file in $(BSW_SRC_FILES); do \
		FILENAME=$$(basename $$file); \
		OUTPUT_FILE=$(BSW_REPORT_DIR)/$$(basename $$FILENAME .c)_misra.txt; \
		cppcheck --enable=style --addon=./misra.json --inline-suppr --library=posix --force --template="[{file}:{line}]: => [{severity} - {id}]: {message}\t {code}" \
			-I $(BSWINCFOLDER) \
			$$file \
			--output-file=$$OUTPUT_FILE; \
	done
	@echo "Formatting Report"
	@python3 Misra_Formatter.py misra/bsw
	@echo "Done"


misra-reb:
	@mkdir -p $(REB_REPORT_DIR)
	@for file in $(REB_SRC_FILES); do \
		FILENAME=$$(basename $$file); \
		OUTPUT_FILE=$(REB_REPORT_DIR)/$$(basename $$FILENAME .c)_misra.txt; \
		cppcheck --enable=style  --addon=./misra.json --inline-suppr  --library=posix --force --template="[{file}:{line}]: => [{severity} - {id}]: {message}\t {code}" \
			-I $(REBINCFOLDER) -I $(BSWINCFOLDER) \
			$$file \
			--output-file=$$OUTPUT_FILE; \
	done
	@echo "Formatting Report"
	@python3 Misra_Formatter.py misra/reb
	@echo "Done"

misra-clean:
	rm -f $(REB_REPORT_DIR)/*_misra.txt
	rm -f $(BSW_REPORT_DIR)/*_misra.txt
	rm -f $(AUX_REPORT_DIR)/*_misra.txt
