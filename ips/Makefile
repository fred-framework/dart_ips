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

ORIG_DIR = $(PWD)
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

link:
	@for a in $(SUBDIRS); do \
		if [ -d $$a ]; then \
			echo "creating symbolic link to IP $$a"; \
			ln -fs $(PWD)/$$a/hw/$$a/solution_0/impl/ip/ $(DART_IP_PATH)/$$a; \
		fi; \
		done;
		@echo "Done!"

help:
	@echo "Makefile to compile all IPs"
	@echo "  make all      : compile all IPs sequentially"
	@echo "  make all -j N : compile all IPs in parallel using N jobs"
	@echo "  make clean    : delete all generated IPs"
	@echo "  make link     : create symbolic links to the IPs in the directory $(DART_IP_PATH)"
