.PHONY: cov
MAKEFLAGS += --no-print-directory

INFO_FILES = $(wildcard cov/*.info)

all:
	@echo "Cleaning ----"
	@make -C ./aux clean
	@make -C ./reb clean
	@echo "\n========== AUX TEST =========="
	@make -C ./aux 
	@echo "=============================="
	@make -C ./reb

$(shell mkdir -p cov)
$(shell mkdir -p cov/main)

cov:
	$(MAKE) clean
	$(MAKE) all
	@lcov -o cov/main/main_coverage.info -c -d . --ignore-errors=mismatch --rc branch_coverage=1 --ignore-errors=inconsistent --rc mcdc_coverage=1 
	@lcov -o cov/main/main_coverage.info --ignore-errors=mismatch --rc branch_coverage=1 --ignore-errors=inconsistent --rc mcdc_coverage=1 -r cov/main/main_coverage.info /test/
	@genhtml cov/main/main_coverage.info -o cov/out --branch-coverage --mcdc-coverage
	@xdg-open cov/out/index.html

clean:
	@rm -fr cov/* 
	@make -C aux clean
	@make -C reb clean

	
