set pwd [pwd]

# HW-task
set num_slots 1
set prj_name "fir"
set top_name "fir_top"
# for pynq
#set device {xc7z020clg400-1}
# for zcu102
set device {xczu9eg-ffvb1156-2-i}
set period 10
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/fir.cpp
add_files src/fir_top.cpp
add_files -tb tb/tb.cpp -csimflags "-I$pwd/tb -Wno-unknown-pragmas"

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
