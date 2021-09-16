
# Matrix Multiplication

This IP calculates the matrix multiplication. The default matrix size is 64x64 but this is configurable in design time by the *MAT_SIZE* constant. The default matrix size is 64x64 because at this size the FPGA offloading starts to present better performance than the CPU. Additional debugging is necessary to try to optimize the IP performance.

This IP also includes a self-checking testbench and a software example. The software has been tested in the Pynq board, but it is currently presenting a mismatch between FRED and CPU outputs. Further debug is necessary to fix this. This is the obtained output when running in the FPGA:

```
# synthetic 
 starting Matrix Multiplication[64][64]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36d91000, length:131072 
buff: buffer mapped at addresses: 0x36d71000, length:131072 
buff: buffer mapped at addresses: 0x36d51000, length:131072 
Time taken by FRED is : 00.029810
Time taken by the CPU is : 00.085796
Mismatch!
Fred finished
```

## Resource utilization


|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 3192 |     0 |     53200 |  6.00 |
|   LUT as Logic             | 3117 |     0 |     53200 |  5.86 |
|   LUT as Memory            |   75 |     0 |     17400 |  0.43 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |   75 |     0 |           |       |
| Slice Registers            | 4185 |     0 |    106400 |  3.93 |
|   Register as Flip Flop    | 4185 |     0 |    106400 |  3.93 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |


|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    |   36 |     0 |       140 | 25.71 |
|   RAMB36/FIFO*    |    3 |     0 |       140 |  2.14 |
|     RAMB36E1 only |    3 |       |           |       |
|   RAMB18          |   66 |     0 |       280 | 23.57 |
|     RAMB18E1 only |   66 |       |           |       |


|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |   80 |     0 |       220 | 36.36 |
|   DSP48E1 only |   80 |       |           |       |

# Performance

The estimated latency is 45098 clock cycles or 0.451 ms assuming 10ns clock period. 

## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
