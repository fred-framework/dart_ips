
# Sobel filter

The [sobel filter](https://en.wikipedia.org/wiki/Sobel_operator) is used for image edge detection. The default image size is `512x512` pixels, where each pixel is encoded in 32bits (0,R,G,B).

This IP also includes a self-checking testbench and a software example. The software has been tested in the Pynq board. However, it still presents mismatch at the ouputs. This is the current output when running in the FPGA:

```
# cd sw
# make all
# make run
convert src/reference/imgs/img.png src/reference/imgs/img.rgb
root@xilinx-zcu102-2020_2:~/repos/rtas22-dart-design/sobel/sw# ./synthetic 
 starting Sobel filter for images 512x512
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0xffff9fa4b000, length:2097152 
buff: buffer mapped at addresses: 0xffff9f84b000, length:2097152 
Time taken by the CPU is : 00.353748
Time taken by FRED is : 00.063896
Mismatch!
Expected value: 
...
Output value: 
...
Fred finished
```

FRED timing sample:

```
root@xilinx-zcu102-2020_2:/opt/fredsys# cat log.txt 
0000000394833298:       fred_sys: hw-task: sobel got slot: 0 of its partition: p0, inserted in fri queue
0000000394833416:       fred_sys: start rcfg of slot: 0 of partition: p0 for hw-task: sobel
0000000394838754:       fred_sys: devcfg, slot: 0 of partition: p0 rcfg completed for hw-task: sobel in 5241 us
0000000394838787:       fred_sys: slot: 0 of partition: p0 started for hw-task: sobel
0000000394841530:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: sobel in 2731 us
0000000676770594:       fred_sys: hw-task: sobel got slot: 0 of its partition: p0, inserted in fri queue
0000000676770678:       fred_sys: skipping rcfg of slot: 0 of partition: p0 for hw-task: sobel
0000000676770704:       fred_sys: slot: 0 of partition: p0 started for hw-task: sobel
0000000676773447:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: sobel in 2730 us
```

## Resource Utilization

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| CLB LUTs*                  | 2200 |     0 |    274080 |  0.80 |
| CLB Registers              | 2743 |     0 |    548160 |  0.50 |
| Block RAM Tile             |   67 |     0 |       912 |  7.35 |
| DSPs                       |    0 |     0 |      2520 |  0.00 |


# Deprecated Libraries

This IP uses he Vivado HLS Video Library (hls_video.h) has been [deprecated in the 2020.1](https://support.xilinx.com/s/article/75345?language=en_US) release and is no longer supported. This library has been replaced by the Vitis Vision Library.

So, it is necessary to migrate the code to the new library. Examples can be found in
[https://github.com/Xilinx/Vitis_Libraries/tree/2020.2/vision](https://github.com/Xilinx/Vitis_Libraries/tree/2020.2/vision).


## Software Requirements

The linux `convert` command is required for testing this IP. Install it with:

```
sudo apt install imagemagick
```

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
