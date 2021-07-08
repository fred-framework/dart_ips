# AMALTHEA PREM IP

This IP is used with the [AMALTHEA](https://projects.eclipse.org/projects/automotive.app4mc/downloads) design flow to mimic the behavior of a generic hardware accelerator. 

This IP is called PREM because it obbeys the [Pretictable Execution Model](https://www.ideals.illinois.edu/bitstream/handle/2142/16605/PREMtechrep.pdf), i.e., it performs read, execution, write operations in that order.

The user can configure the following constants to change the behavior of the IP:

 - the number of words read by the IP from the main memory (*IN_MEM_SIZE*);
 - the number of clock cycles the IP spend executing some dummy logic (*EXEC_CYCLES*). This time includes the time spent in reading and writing the memory; and
 - the number of words written by the IP into the main memory (*OUT_MEM_SIZE*);
## Authors

- Alexandre Amory (July 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. This HLS design.
- Francesco Restuccia (April 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. Initial design.
