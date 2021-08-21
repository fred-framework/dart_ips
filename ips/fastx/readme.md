
# Gradient map

This IP applies the FastX filter into an image of size `32x32` pixels, where each pixel is encoded in 32bits (0,R,G,B).

This IP was not tested in the FPGA and the testbench is not working. When running in the FPGA, the errorneous output is:

```
# code/synthetic 
 starting FastX for images[32][32]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36bb8000, length:1048576 
buff: buffer mapped at addresses: 0x36fb3000, length:32768 
Mismatch - Empty output image!
Fred finished
```

## Resource utilization

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 6119 |     0 |     53200 | 11.50 |
|   LUT as Logic             | 5597 |     0 |     53200 | 10.52 |
|   LUT as Memory            |  522 |     0 |     17400 |  3.00 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |  522 |     0 |           |       |
| Slice Registers            | 5583 |     0 |    106400 |  5.25 |
|   Register as Flip Flop    | 5583 |     0 |    106400 |  5.25 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |


|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    | 13.5 |     0 |       140 |  9.64 |
|   RAMB36/FIFO*    |    7 |     0 |       140 |  5.00 |
|     RAMB36E1 only |    7 |       |           |       |
|   RAMB18          |   13 |     0 |       280 |  4.64 |
|     RAMB18E1 only |   13 |       |           |       |


|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |    3 |     0 |       220 |  1.36 |
|   DSP48E1 only |    3 |       |           |       |


## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
