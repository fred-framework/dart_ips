
set pwd [pwd]

# HW-task
set num_slots 1
set prj_name "finn"
set top_name "finn_top"
set device {xc7z020clg400-1}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/finn_top.cpp   -cflags "-I$pwd/src -I$pwd/bnn -std=c++11 -Wno-unknown-pragmas"
add_files src/top.cpp -cflags "-I$pwd/src -I$pwd/bnn -std=c++11 -Wno-unknown-pragmas"

# add_files -tb tb/MNIST_DATA.cpp -csimflags "-I$pwd/include -Wno-unknown-pragmas"
# add_files -tb tb/tb.cpp -csimflags "-I$pwd/include -Wno-unknown-pragmas"

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
