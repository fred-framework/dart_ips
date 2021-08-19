
# Matrix Multiplication

This IP calculates the matrix multiplication. The default matrix size is 4x4 but this is configurable in design time by the *MAT_SIZE* constant. Check the software example in the *sw* directory to see how to use this IP. 

When running in the FPGA, the expected output is:

```
# synthetic 
 starting Matrix Multiplication[4][4]
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36efe000, length:32768 
buff: buffer mapped at addresses: 0x36ec8000, length:32768 
buff: buffer mapped at addresses: 0x36ec0000, length:32768 
Match!
Expected value: [
0 8 16 24 
2 10 18 26 
4 12 20 28 
6 14 22 30 
]
Output value  : [
0 8 16 24 
2 10 18 26 
4 12 20 28 
6 14 22 30 
]
Fred finished
```

## Resource utilization


|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|-----:|------:|----------:|------:|
| Slice LUTs*                | 1994 |     0 |     53200 |  3.75 |
|   LUT as Logic             | 1410 |     0 |     53200 |  2.65 |
|   LUT as Memory            |  584 |     0 |     17400 |  3.36 |
|     LUT as Distributed RAM |  512 |     0 |           |       |
|     LUT as Shift Register  |   72 |     0 |           |       |
| Slice Registers            | 3366 |     0 |    106400 |  3.16 |
|   Register as Flip Flop    | 3366 |     0 |    106400 |  3.16 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |



|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|-----:|------:|----------:|------:|
| Block RAM Tile    |    4 |     0 |       140 |  2.86 |
|   RAMB36/FIFO*    |    3 |     0 |       140 |  2.14 |
|     RAMB36E1 only |    3 |       |           |       |
|   RAMB18          |    2 |     0 |       280 |  0.71 |
|     RAMB18E1 only |    2 |       |           |       |



|    Site Type   | Used | Fixed | Available | Util% |
|----------------|-----:|------:|----------:|------:|
| DSPs           |   40 |     0 |       220 | 18.18 |
|   DSP48E1 only |   40 |       |           |       |
+----------------+------+-------+-----------+-------+


## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
