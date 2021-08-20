
set pwd [pwd]

# HW-task
set num_slots 1
set prj_name "gmap"
set top_name "gmap_top"
set device {xc7z020clg400-1}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/gmap_top.cpp
add_files src/hw_mod.cpp
# add_files -tb tb/tb.cpp -csimflags "-I$pwd/tb -I$OPENCV_INCLUDE -D__SDSVHLS__ -std=c++0x -Wno-unknown-pragmas"
add_files -tb tb/tb.cpp -csimflags "-I$pwd/tb -Wno-unknown-pragmas"

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
