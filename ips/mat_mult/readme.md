
# Matrix Multiplication

This IP calculates the matrix multiplication of 64bits integers. The default matrix size is 32x32 but this is configurable in design time by the *MAT_SIZE* constant. Other configurations were also tested in the FPGA, such as 8x8 and 16x16. 64x64 seems to be close to the resource limit of the Pynq board and currently does not have a working configuration. Additional debugging is necessary to try to optimize the IP performance and to define additional FPGA tested configurations.

This IP also includes a self-checking testbench and a software example. The software has been tested in the Pynq board, but it is currently presenting a mismatch between FRED and CPU outputs. Further debug is necessary to fix this. This is the obtained output when running in the FPGA:

```
# synthetic 
 starting Matrix Multiplication[32][32]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36cb7000, length:1048576 
buff: buffer mapped at addresses: 0x36bb7000, length:1048576 
buff: buffer mapped at addresses: 0x36ab7000, length:1048576 
Time taken by FRED is : 00.037469
Time taken by the CPU is : 00.010942
Match!
...
```

In a second execution, when the FPGA is already configured with this IP, the FRED execution time is:

```
# synthetic 
 starting Matrix Multiplication[32][32]
...
Time taken by FRED is : 00.000431
Time taken by the CPU is : 00.011622
...
```

## Resource utilization


|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 4790 |     0 |     53200 |  9.00 |
|   LUT as Logic             | 4724 |     0 |     53200 |  8.88 |
|   LUT as Memory            |   66 |     0 |     17400 |  0.38 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |   66 |     0 |           |       |
| Slice Registers            | 4568 |     0 |    106400 |  4.29 |
|   Register as Flip Flop    | 4568 |     0 |    106400 |  4.29 |
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

# Limitations and Future Work

See the "TODO" flags in the source code for a list of limitations. As future work, it would be nice to have a configurable version of this IP where the size of the matrix is set in runtime by the programmer.

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, IP design optimization, tb, sw design, and testing.
