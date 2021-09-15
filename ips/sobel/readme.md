
# Sobel filter

The [sobel filter](https://en.wikipedia.org/wiki/Sobel_operator) is used for image edge detection. The default image size is `32x32` pixels, where each pixel is encoded in 32bits (0,R,G,B).

This IP was not tested in the FPGA and the testbench is not working. When running in the FPGA, the errorneous output is:

```
# code/synthetic 
 starting Sobel filter for images[32][32]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36bb8000, length:1048576 
buff: buffer mapped at addresses: 0x36fb3000, length:32768 
Mismatch - Empty output image!
Fred finished
```

## Resource utilization

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 1682 |     0 |     53200 |  3.16 |
|   LUT as Logic             | 1505 |     0 |     53200 |  2.83 |
|   LUT as Memory            |  177 |     0 |     17400 |  1.02 |
|     LUT as Distributed RAM |   24 |     0 |           |       |
|     LUT as Shift Register  |  153 |     0 |           |       |
| Slice Registers            | 2319 |     0 |    106400 |  2.18 |
|   Register as Flip Flop    | 2319 |     0 |    106400 |  2.18 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |


|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    |    3 |     0 |       140 |  2.14 |
|   RAMB36/FIFO*    |    3 |     0 |       140 |  2.14 |
|     RAMB36E1 only |    3 |       |           |       |
|   RAMB18          |    0 |     0 |       280 |  0.00 |

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
