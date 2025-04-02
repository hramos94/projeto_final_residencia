.PHONY: clean

SUBDIRS =  bsw_mcal bsw_ecu reb_ecu 
#reb_app 

all:
	@for dir in $(SUBDIRS); do \
		echo "\n\n========== $$dir TEST =========="; \
		make -C ./$$dir; \
		echo "=============================="; \
	done


clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

cov:
	@for covname in $(SUBDIRS); do \
		lcov -c -d $$covname/obj -o ../cov/$$covname.info --ignore-errors=mismatch --rc lcov_branch_coverage=1; \
	done


