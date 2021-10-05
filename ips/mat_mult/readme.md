
# Matrix Multiplication

This IP calculates the matrix multiplication. The default matrix size is 32x32 but this is configurable in design time by the *MAT_SIZE* constant. Other configurations were also tested in the FPGA, such as 8x8 and 16x16. 64x64 seems to be close to the resource limit of the Pynq board and currently does not have a working configuration. Additional debugging is necessary to try to optimize the IP performance and to define additional FPGA tested configurations.

This IP also includes a self-checking testbench and a software example. The software has been tested in the Pynq board, but it is currently presenting a mismatch between FRED and CPU outputs. Further debug is necessary to fix this. This is the obtained output when running in the FPGA:

```
# synthetic 
 starting Matrix Multiplication[32][32]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36d44000, length:1048576 
buff: buffer mapped at addresses: 0x36c44000, length:1048576 
buff: buffer mapped at addresses: 0x36b44000, length:1048576 
Time taken by FRED is : 00.041090
Time taken by the CPU is : 00.013133
Match!
Fred finished
```

In a second execution, when the FPGA is already configured with this IP, the FRED execution time is:

```
# synthetic 
 starting Matrix Multiplication[32][32]
...
Time taken by FRED is : 00.001156
Time taken by the CPU is : 00.032612
...
```

## Resource utilization


|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 4960 |     0 |     53200 |  9.32 |
|   LUT as Logic             | 4877 |     0 |     53200 |  9.17 |
|   LUT as Memory            |   83 |     0 |     17400 |  0.48 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |   83 |     0 |           |       |
| Slice Registers            | 4711 |     0 |    106400 |  4.43 |
|   Register as Flip Flop    | 4711 |     0 |    106400 |  4.43 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |


|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    |   69 |     0 |       140 | 49.29 |
|   RAMB36/FIFO*    |   69 |     0 |       140 | 49.29 |
|     RAMB36E1 only |   69 |       |           |       |
|   RAMB18          |    0 |     0 |       280 |  0.00 |


|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |   80 |     0 |       220 | 36.36 |
|   DSP48E1 only |   80 |       |           |       |

# Performance

The estimated latency is 7200 clock cycles or 0.072 ms assuming 10ns clock period. 

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, IP design optimization, tb, sw design, and testing.
