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



