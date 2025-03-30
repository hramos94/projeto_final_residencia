.PHONY: cov
MAKEFLAGS += --no-print-directory

all:
	@echo "Cleaning ----"
	@make -C ./aux clean
	@make -C ./reb clean
	@echo "\n========== AUX TEST =========="
	@make -C ./aux 
	@echo "=============================="
	@echo "\n\n========== REB TEST =========="
	@make -C ./reb 
	@echo "=============================="

$(shell mkdir -p cov)

cov:
	$(MAKE) clean
	$(MAKE) all
	@lcov -c -d reb/obj -o cov/reb_coverage.info --ignore-errors=mismatch
	@lcov -c -d aux/obj -o cov/aux_coverage.info --ignore-errors=mismatch
	@lcov -o cov/main_coverage.info -a cov/reb_coverage.info  -a cov/aux_coverage.info  
	@lcov -o cov/main_coverage.info -r cov/main_coverage.info /test/ 
	@genhtml cov/main_coverage.info -o cov/out
	@xdg-open cov/out/index.html

clean:
	@rm -fr cov/* 
	@make -C aux clean
	@make -C reb clean

	
