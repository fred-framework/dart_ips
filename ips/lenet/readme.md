# LeNet CNN

This IP is used with the [AMALTHEA](https://projects.eclipse.org/projects/automotive.app4mc/downloads) design flow to mimic the behavior of a generic hardware accelerator. 

This IP is called PREM because it obbeys the [Pretictable Execution Model](https://www.ideals.illinois.edu/bitstream/handle/2142/16605/PREMtechrep.pdf), i.e., it performs read, execution, write operations in that order.

The user can configure the following constants to change the behavior of the IP:

 - the number of words read by the IP from the main memory (*IN_MEM_SIZE*);
 - the number of clock cycles the IP spend executing some dummy logic (*EXEC_CYCLES*). This time includes the time spent in reading and writing the memory; and
 - the number of words written by the IP into the main memory (*OUT_MEM_SIZE*);


## Resource Usage

The following report includes this IP and it considers the board PYNQ-Z1 (xc7z020clg400-1).

|          Site Type         | Used | Fixed | Available | Util% |
|----------------------------|------|-------|-----------|-------|
| Slice LUTs*                | 1137 |     0 |     53200 |  2.14 |
|   LUT as Logic             | 1070 |     0 |     53200 |  2.01 |
|   LUT as Memory            |   67 |     0 |     17400 |  0.39 |
|     LUT as Distributed RAM |    0 |     0 |           |       |
|     LUT as Shift Register  |   67 |     0 |           |       |
| Slice Registers            | 1900 |     0 |    106400 |  1.79 |
|   Register as Flip Flop    | 1900 |     0 |    106400 |  1.79 |
|   Register as Latch        |    0 |     0 |    106400 |  0.00 |
| F7 Muxes                   |    0 |     0 |     26600 |  0.00 |
| F8 Muxes                   |    0 |     0 |     13300 |  0.00 |


## Authors

- Alexandre Amory (July 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. This HLS design.
- Francesco Restuccia (April 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. Initial design in VHDL.
