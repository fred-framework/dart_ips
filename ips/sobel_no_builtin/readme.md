
# Sobel filter

The [sobel filter](https://en.wikipedia.org/wiki/Sobel_operator) is used for image edge detection. The default image size is `512x512` pixels, where each pixel is encoded in 32bits (0,R,G,B).

Different from the *sobel* IP, this one does not use builtin Xilinx libraries. The results is that it is less optimized both in terms of area and latency.

This IP also includes a self-checking testbench and a software example. The software has been tested in the Pynq board. However, it still presents mismatch at the ouputs. This is the current output when running in the FPGA:

```
# cd sw
# make all
# make run
convert src/reference/imgs/img.png src/reference/imgs/img.rgb
./synthetic
 starting Sobel filter for images[512][512]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36b49000, length:2097152 
buff: buffer mapped at addresses: 0x36949000, length:2097152 
Time taken by the CPU is : 00.568272
Time taken by FRED is : 00.102232
Mismatch!
Expected value: 
...
Output value: 
...
Fred finished
```

## Resource Utilization

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 1705 |     0 |     53200 |  3.20 |
|   LUT as Logic             | 1552 |     0 |     53200 |  2.92 |
|   LUT as Memory            |  153 |     0 |     17400 |  0.88 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |  153 |     0 |           |       |
| Slice Registers            | 2359 |     0 |    106400 |  2.22 |
|   Register as Flip Flop    | 2359 |     0 |    106400 |  2.22 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |

|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    |  4.5 |     0 |       140 |  3.21 |
|   RAMB36/FIFO*    |    3 |     0 |       140 |  2.14 |
|     RAMB36E1 only |    3 |       |           |       |
|   RAMB18          |    3 |     0 |       280 |  1.07 |
|     RAMB18E1 only |    3 |       |           |       |

|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |    3 |     0 |       220 |  1.36 |
|   DSP48E1 only |    3 |       |           |       |


# Performance

The estimated latency is 266785 clock cycles or 2.668 ms assuming 10ns clock period. 

## Software Requirements

The linux `convert` command is required for testing this IP. Install it with:

```
sudo apt install imagemagick
```

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.

## TO DO

how to model image filters
https://github.com/Xilinx/HLx_Examples/tree/master/Vision/video_edge
https://github.com/Xilinx/HLx_Examples/blob/master/Vision/video_edge/image_filter.cpp
https://github.com/ATaylorCEngFIET/Hackster/blob/master/Sobel/cvt_colour.cpp

sobel
https://github.com/Mr-Anderson/turtlebot_360/blob/master/pre_vis_proc/src/pre_vis_proc.cpp
https://github.com/Nishida-Lab/rosbook_pkgs/blob/master/chapter7/test_opencv/src/test_edge_detection.cpp
https://github.com/vineet0814/Edge-detection-using-Canny-Sobel-and-Laplace/blob/master/Edge_detect/main.cpp

sobel no opencv
https://github.com/hkclki/Sobel-Filter/blob/master/opencvpjt_img/main.cpp
https://github.com/yuryybk/opencv-basic-samples/blob/master/Sobel/Sobel.cpp

A Comparison of the Sobel Filter in C, OpenCV and CUDA
https://danyele.github.io/lecture_notes/SPD_Project_Report_Daniele_Gadler_5_0.pdf
https://github.com/DanyEle/Sobel_Filter/tree/master/Native_Sobel
https://github.com/DanyEle/Sobel_Filter/tree/master/OpenCV_Sobel

sobel in C
https://github.com/fzehracetin/sobel-edge-detection-in-c/blob/main/sobel_edge_detection.c
https://github.com/tahtaciburak/edge-detection/blob/84af3e8a551ef262f94b3be36856b436124f52fe/main.c#L159
https://github.com/erayzeki/SobelFilter/blob/dc3d7ff72bf8e2311e86c7c3c03472a7a997b121/sobel.c#L126
https://github.com/BrianCotter/SobelC/blob/main/sobel.c
https://github.com/BrianCotter/SobelC/blob/main/convolve.c


sobel in C - no multiplication
https://github.com/mjpatter88/SobelEdgeDetection/blob/master/sobel.c


canny
https://github.com/metinkadik/RosOpencv-EdgeDetection/blob/master/opencv_ros/src/image.cpp
https://github.com/tronium/canny_edge_my_face/blob/master/src/cannyedge.cpp


