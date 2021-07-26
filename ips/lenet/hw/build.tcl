
set pwd [pwd]

# HW-task
set num_slots 1
set prj_name "lenet"
set top_name "lenet_top"
set device {xc7z020clg400-1}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/activation.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"
add_files src/convolution.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"
add_files src/fullyconnected.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"
add_files src/pooling.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"
add_files src/LeNet.cpp -cflags "-I$pwd/include -I$pwd/include/filter -Wno-unknown-pragmas"
add_files src/lenet_fred.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"
add_files src/lenet_top.cpp -cflags "-I$pwd/include -Wno-unknown-pragmas"


add_files -tb tb/MNIST_DATA.cpp -csimflags "-I$pwd/include -Wno-unknown-pragmas"
add_files -tb tb/tb.cpp -csimflags "-I$pwd/include -Wno-unknown-pragmas"

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
