.PHONY: cov
MAKEFLAGS += --no-print-directory

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
	#for file in ./cov/*.info; do \
	#	echo $$file; \
	#	lcov -o cov/main/main_coverage.info -a "$$file"; \
	#done
	lcov -o cov/main/main_coverage.info -a cov/bsw_ecu.info -a cov/bsw_mcal.info ; 
	lcov -o cov/main/main_coverage.info -r cov/main/main_coverage.info /test/
	@genhtml cov/main/main_coverage.info -o cov/out
	@xdg-open cov/out/index.html

clean:
	@rm -fr cov/* 
	@make -C aux clean
	@make -C reb clean

	
