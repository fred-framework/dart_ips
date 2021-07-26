
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
add_files src/activation.cpp
add_files src/convolution.cpp
add_files src/fullyconnected.cpp
add_files src/pooling.cpp
add_files src/LeNet.cpp
add_files src/lenet_fred.cpp
add_files src/lenet_top.cpp

add_files include/activation.h
add_files include/fullyconnected.h
add_files include/parameters.h
add_files include/tanh_index.h
add_files include/convolution.h
add_files include/pooling.h
add_files include/tanh_value.h
add_files include/LeNet.h
add_files include/lenet_fred.hpp
add_files include/lenet_top.hpp

add_file -tb include/parameters.h
add_file -tb include/MNIST_DATA.h
add_file -tb tb/MNIST_DATA.cpp
add_file -tb tb/tb.cpp

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl

close_project
exit
