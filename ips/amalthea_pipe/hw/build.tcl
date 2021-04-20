
# HW-task
set num_slots 1
set prj_name "memcpy"
set top_name "memcpy_top"
set device {xc7z020clg400-1}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/memcpy_top.cpp
add_files src/memcpy.cpp
add_file -tb tb/tb.cpp

# the generic part of the HLS script
source ../../scripts/ip_hls.tcl

close_project
exit
