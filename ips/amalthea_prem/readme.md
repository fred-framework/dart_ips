# AMALTHEA PREM IP

This IP is used with the [AMALTHEA](https://projects.eclipse.org/projects/automotive.app4mc/downloads) design flow to mimic the behavior of a generic hardware accelerator. The user can configure:

 - the number of bytes read by the IP;
 - the number of clock cycles the IP spend executing some dommy logic; and
 - the number of bytes written by the IP to the host CPU;

This IP is called PREM because it obbeys the [Pretictable Execution Model](https://www.ideals.illinois.edu/bitstream/handle/2142/16605/PREMtechrep.pdf), i.e., it performs read, execution, write operations in that order.

## Authors

- Francesco Restuccia (April 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy.
