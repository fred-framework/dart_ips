
# Sobel filter

The [sobel filter](https://en.wikipedia.org/wiki/Sobel_operator) is used for image edge detection. The default image size is `512x512` pixels, where each pixel is encoded in 32bits (0,R,G,B).

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


## Software Requirements

The linux `convert` command is required for testing this IP. Install it with:

```
sudo apt install imagemagick
```

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
