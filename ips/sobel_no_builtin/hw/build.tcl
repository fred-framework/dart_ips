
set pwd [pwd]
#set OPENCV_LIB $::env(XILINX_VIVADO)/lib/lnx64.o
# set OPENCV_INCLUDE /usr/include/opencv
# set OPENCV_LIB /usr/lib/x86_64-linux-gnu/
# set OPENCV_LIB_REF   "-lopencv_imgcodecs -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_flann -lopencv_features2d"

# HW-task
set num_slots 1
set prj_name "sobel"
set top_name "sobel_top"
# for pynq
#set device {xc7z020clg400-1}
# for zcu102
set device {xczu9eg-ffvb1156-2-i}
set period 10
#set sim to 1 to run simulation and synthesis, 0 to run only the synthesis
set sim 0 

open_project -reset $prj_name

# HW-task files
add_files src/sobel_top.cpp
add_files src/hw_mod.cpp
# add_files -tb tb/tb.cpp -csimflags "-I$pwd/tb -I$OPENCV_INCLUDE -D__SDSVHLS__ -std=c++0x -Wno-unknown-pragmas"
add_files -tb tb/tb.cpp -csimflags "-I$pwd/tb -Wno-unknown-pragmas"

# the generic part of the HLS script
source ../../../scripts/ip_hls.tcl
# set_top ${top_name}
# open_solution -reset "solution_0"
# set_part ${device} -tool vivado
# create_clock -period ${period} -name default

# #csim_design -ldflags "-L ${OPENCV_LIB} ${OPENCV_LIB_REF}" -argv "${XF_PROJ_ROOT}/data/128x128.png"
# csim_design

close_project
exit
