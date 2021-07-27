# LeNet CNN

Lenet for MNIST handwritten digit recognition using Vivado hls tool. Accuracy of 97.5938% (8-bit Mode).

## DART/FRED Compatibility

This HLS design has been adapted to work with DART and FRED tools designed by [Retis lab](retis.sssup.it/) in Scuola Sant'anna, Pisa. 

* The [design](https://github.com/amamory-ml/mnist_hls) adapted for DART/FRED;
* The [original](https://github.com/FloyedShen/mnist_hls) design.

## Requirement
* Vivado hls 2019.2
* Vitis 2019.2
* python 3+
* DART/FRED

## Usage
* Download MNIST dataset from [MNIST](http://yann.lecun.com/exdb/mnist/) and copy the dataset to `./hw/MNIST_DATA/`.

* build hls project
```
# When you are in the project root directory
cd hw
vivado_hls build.tcl
```

* Generate `tanh` table (default: ap_fixed<8,3>)
```
# When you are in the project root directory
cd hw/include/filter
python ./tanh_table.py
```

* copy the `sw` directory into the FPGA for compilation.

* Run and see the results

## Resource Usage

The following report includes this IP and it considers the board PYNQ-Z1 (xc7z020clg400-1).

|          Site Type         |  Used | Fixed | Available | Util% |
|----------------------------|-------|-------|-----------|-------|
| Slice LUTs*                | 16230 |     0 |     53200 | 30.51 |
|   LUT as Logic             | 16088 |     0 |     53200 | 30.24 |
|   LUT as Memory            |   142 |     0 |     17400 |  0.82 |
|     LUT as Distributed RAM |    63 |     0 |           |       |
|     LUT as Shift Register  |    79 |     0 |           |       |
| Slice Registers            |  6401 |     0 |    106400 |  6.02 |
|   Register as Flip Flop    |  6401 |     0 |    106400 |  6.02 |
|   Register as Latch        |     0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |  3052 |     0 |     26600 | 11.47 |
| F8 Muxes                   |  1452 |     0 |     13300 | 10.92 |


|     Site Type     | Used | Fixed | Available | Util% |
|-------------------|------|-------|-----------|-------|
| Block RAM Tile    | 12.5 |     0 |       140 |  8.93 |
|   RAMB36/FIFO*    |    6 |     0 |       140 |  4.29 |
|     RAMB36E1 only |    6 |       |           |       |
|   RAMB18          |   13 |     0 |       280 |  4.64 |
|     RAMB18E1 only |   13 |       |           |       |


|    Site Type   | Used | Fixed | Available | Util% |
|----------------|------|-------|-----------|-------|
| DSPs           |    3 |     0 |       220 |  1.36 |
|   DSP48E1 only |    3 |       |           |       |

## Authors

* Alexandre Amory (August 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. Adaptation to DART/FRED.
* Orignal design by FloyedShen;
