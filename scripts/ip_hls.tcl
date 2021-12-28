
set i 0
while {$i < $num_slots} {
  set_top ${top_name}
  open_solution -reset "solution_$i"
  set_part ${device}
  # 64-bit AXI-M address capability
  # This is supported by FRED, but currently not by DART.
  # Therefore, we stick with 32 bits C_M_AXI_MEM_BUS_ADDR_WIDTH even for ZynqM devices
  # The second implication is that FRED server MUST BE compiled without the define `HW_TASKS_A64`, to make it compatible with 32 bits
  #config_interface -m_axi_addr64  
  create_clock -period ${period} -name default

  if {$sim != 0} {
    csim_design
  }
  csynth_design
  if {$sim != 0} {
    cosim_design
  }
  export_design -format ip_catalog -description "${prj_name}_${i}" -display_name "${prj_name}_${i}"
  incr i
}
