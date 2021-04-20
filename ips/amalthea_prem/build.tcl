# Set these variables accordingly before executing the script
set VIVADO_DESIGN_NAME amaltea_prem
set VIVADO_TOP_NAME ${VIVADO_DESIGN_NAME}
# The valid values for DART_BOARD are: pynq or zynq
set DART_BOARD pynq

################################################
# These variables rarely need to be changed
################################################

# The valid values for VIVADO_SYN_STEP are:
#  - opt_design, power_opt_design, place_design, route_design, phys_opt_design, and write_bitstream
set VIVADO_SYN_STEP opt_design

################################################
# dont change from this point
################################################

source ../../scripts/build.tcl
