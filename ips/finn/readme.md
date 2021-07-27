
# FINN CNN IP

This is a binary neural network IP for Pynq board. It works with 1 bit weights and 1 bit activation (W1A1) for CNV and LFC networks described in the [FINN paper](https://arxiv.org/abs/1612.07119).


    @inproceedings{finn,
    author = {Umuroglu, Yaman and Fraser, Nicholas J. and Gambardella, Giulio and Blott, Michaela and Leong, Philip and Jahre, Magnus and Vissers, Kees},
    title = {FINN: A Framework for Fast, Scalable Binarized Neural Network Inference},
    booktitle = {Proceedings of the 2017 ACM/SIGDA International Symposium on Field-Programmable Gate Arrays},
    series = {FPGA '17},
    year = {2017},
    pages = {65--74},
    publisher = {ACM}
    }

These are its original [repository](https://github.com/Xilinx/BNN-PYNQ) and [documentation](https://xilinx.github.io/finn/). 
This IP implements the `cnvW1A1` network, described [here](https://github.com/Xilinx/BNN-PYNQ/tree/master/bnn/src/network/cnvW1A1).


## Utilization Estimates


|       Name          | BRAM_18K| DSP48E|   FF   |  LUT  | URAM|
|---------------------|--------:|------:|-------:|------:|----:|
|DSP                  |        -|      -|       -|       -|    -|
|Expression           |        -|      -|       0|       4|    -|
|FIFO                 |        -|      -|       -|       -|    -|
|Instance             |      251|      -|   30371|  108644|    0|
|Memory               |        -|      -|       -|       -|    -|
|Multiplexer          |        -|      -|       -|      87|    -|
|Register             |        -|      -|     123|       -|    -|
|**Total**            |      251|      0|   30494|  108735|    0|
|**Available**        |      280|    220|  106400|   53200|    0|
|**Utilization (%)**  |       89|      0|      28| **204**|    0|


