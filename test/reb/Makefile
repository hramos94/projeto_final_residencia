.PHONY: clean

SUBDIRS =  bsw_mcal bsw_ecu 
#reb_app reb_ecu 

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

cov:
	@for covname in $(SUBDIRS); do \
		lcov -c -d $$covname/obj -o ../cov/$$covname.info --ignore-errors=mismatch; \
	done


