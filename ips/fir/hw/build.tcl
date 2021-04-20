
# HW-task
set num_slots 1
set prj_name "fir"
set top_name "fir_top"
set device {xc7z020clg400-1}
set period 10
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/fir.cpp
add_files src/fir_top.cpp
add_file -tb src/tb.cpp

# the generic part of the HLS script
source ../../scripts/ip_hls.tcl

close_project
exit
