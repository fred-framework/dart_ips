# Configurable AMALTHEA PREM IP

This IP is used with the [AMALTHEA](https://projects.eclipse.org/projects/automotive.app4mc/downloads) design flow to mimic the behavior of a generic hardware accelerator.

This IP is called PREM because it obbeys the [Pretictable Execution Model](https://www.ideals.illinois.edu/bitstream/handle/2142/16605/PREMtechrep.pdf), i.e., it performs read, execution, write operations in that order.

Different from the [AMALTHEA PREM IP](../prem/readme.md), where the designer needs to change constants in design time to set the behavior of the IP, the *Configurable AMALTHEA PREM IP* allows to change this configuration in runtime, in the FRED software. Check the software example located in *sw/src*.

The first 3 words of the input of this IP represent:

 - the number of words read by the IP from the main memory;
 - the number of clock cycles the IP spend executing some dummy logic. This time includes the time spent in reading and writing the memory; and
 - the number of words written by the IP into the main memory;

This way, the actual input data starts form the 4th position of the input vector.

## Authors

- Alexandre Amory (July 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. This HLS design.
- Francesco Restuccia (April 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy. Initial design in VHDL.
