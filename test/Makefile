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
	@make -C ./reb cov
	for file in $(INFO_FILES); do \
		echo $$file; \
		lcov -o cov/main/main_coverage.info -a "$$file" --rc branch_coverage=1; \
	done
	lcov -o cov/main/main_coverage.info --rc branch_coverage=1 -r cov/main/main_coverage.info /test/
	@genhtml cov/main/main_coverage.info -o cov/out --branch-coverage
	@xdg-open cov/out/index.html

clean:
	@rm -fr cov/* 
	@make -C aux clean
	@make -C reb clean

	
