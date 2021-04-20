
set i 0
while {$i < $num_slots} {
  set_top ${top_name}
  open_solution -reset "solution_$i"
  set_part ${device} -tool vivado
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
