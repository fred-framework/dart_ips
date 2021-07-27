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


|       Name         | BRAM_18K| DSP48E|   FF   |  LUT  | URAM|
|--------------------|--------:|------:|-------:|------:|----:|
|DSP                 |        -|      -|       -|      -|    -|
|Expression          |        -|      -|       -|      -|    -|
|FIFO                |        -|      -|       -|      -|    -|
|Instance            |       59|      3|    7900|  34504|    0|
|Memory              |        -|      -|       -|      -|    -|
|Multiplexer         |        -|      -|       -|     60|    -|
|Register            |        -|      -|      61|      -|    -|
|**Total**           |       59|      3|    7961|  34564|    0|
|**Available**       |      280|    220|  106400|  53200|    0|
|**Utilization (%)** |       21|      1|       7|     64|    0|

## Authors

* Alexandre Amory (August 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. Adaptation to DART/FRED.
* Orignal design by FloyedShen;
