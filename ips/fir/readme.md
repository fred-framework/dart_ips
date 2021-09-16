
# FIR filter

The [FIR filter](https://en.wikipedia.org/wiki/Finite_impulse_response) is used for data analysis in the frequency domain. Its default configuration consists of buffer size of 4k words of 64bits and window of size 100. Just default configuration was chosen because it is just enough to present a speed up using FPGA offloading.

This IP also includes a self-checking testbench and a software example. The software has been tested successfully in the Pynq board. This is the expected output when running in the FPGA:

```
# ./synthetic
 starting fir 
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36ca9000, length:1048576 
buff: buffer mapped at addresses: 0x36ba9000, length:1048576 
Time taken by the CPU is : 00.054382
Time taken by FRED is : 00.034146
Match!
Expected value[0:9]:
[ 0 13 24 44 75 132 207 377 504 637 ] 
Output value[0:9]:
[ 0 13 24 44 75 132 207 377 504 637 ] 
Fred finished 
```

## Resource Utilization

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 1132 |     0 |     53200 |  2.13 |
|   LUT as Logic             | 1066 |     0 |     53200 |  2.00 |
|   LUT as Memory            |   66 |     0 |     17400 |  0.38 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |   66 |     0 |           |       |
| Slice Registers            | 1872 |     0 |    106400 |  1.76 |
|   Register as Flip Flop    | 1872 |     0 |    106400 |  1.76 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    8 |     0 |     26600 |  0.03 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |

|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    | 11.5 |     0 |       140 |  8.21 |
|   RAMB36/FIFO*    |   11 |     0 |       140 |  7.86 |
|     RAMB36E1 only |   11 |       |           |       |
|   RAMB18          |    1 |     0 |       280 |  0.36 |
|     RAMB18E1 only |    1 |       |           |       |

|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |    2 |     0 |       220 |  0.91 |
|   DSP48E1 only |    2 |       |           |       |

# Performance

The estimated latency is 2486491 clock cycles or 24.865 ms assuming 10ns clock period. 

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
