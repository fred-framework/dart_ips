# HW-task
set num_slots 1
set prj_name "sum_vec"
set top_name "sum_vec_top"
# for pynq
#set device {xc7z020clg400-1}
# for zcu102
set device {xczu9eg-ffvb1156-2-i}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/hw_task.cpp
add_files src/hw_mod.cpp

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
