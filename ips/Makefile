# make all IPs
# improved makefile where each directory can run in parallel using 'make -j N'
# by A Amory
SHELL=/bin/bash

# list directory only
#SUBDIRS = $(sort $(dir $(wildcard */)))
#$(info >>>>>>>>>>>>>>>)
#$(info $$SUBDIRS is: ${SUBDIRS})
#$(info <<<<<<<<<<<<<<<)
SUBDIRS = memcpy mul_vec nor_vec sub_vec sum_vec xor_vec
$(info $$SUBDIRS is: ${SUBDIRS})

ifndef XILINX_VIVADO
$(error XILINX_VIVADO is not defined)
endif

ifndef DART_IP_PATH
$(error DART_IP_PATH is not defined)
endif

.PHONY: all $(SUBDIRS)
all: $(SUBDIRS)

$(SUBDIRS):
	@echo "creating IP $@"
	cd $@/hw; vivado_hls build.tcl

clean:
	@for a in $(SUBDIRS); do \
		if [ -d $$a ]; then \
			echo "deleting IP $$a"; \
			rm -rf $$a/hw/$$a; \
		fi; \
		done;
		@echo "Done!"

# TODO: create a rule to create all symbolic links in DART_IP_PATH
