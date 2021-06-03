# HW-task
set num_slots 1
set prj_name "nor_vec"
set top_name "nor_vec_top"
set device {xc7z020clg400-1}
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
