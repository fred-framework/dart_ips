# Set these variables accordingly before executing the script
export VIVADO_DESIGN_NAME=fred_static
export VIVADO_TOP_NAME=${VIVADO_DESIGN_NAME}_wrapper
# The valid values for VIVADO_SYN_STEP are: pynq or zynq
export DART_BOARD=pynq

################################################
# These variables rarely need to be changed
################################################

# The valid values for VIVADO_SYN_STEP are:
#  - opt_design, power_opt_design, place_design, route_design, phys_opt_design, and write_bitstream
export VIVADO_SYN_STEP=place_design

################################################
# dont change from this point
################################################
error() {
  echo "$@" 1>&2
}

path_to_vivado=$(which vivado)
if [ ! -x "$path_to_vivado" ] ; then
  error "ERROR: vivado not found"
  exit 1
fi

vivado -mode batch -source ../scripts/build.tcl -notrace
