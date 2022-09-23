
# FIR filter

The [FIR filter](https://en.wikipedia.org/wiki/Finite_impulse_response) is used for data analysis in the frequency domain. Its default configuration consists of buffer size of 4k words of 64bits and window of size 100. Just default configuration was chosen because it is just enough to present a speed up using FPGA offloading.

This IP also includes a self-checking testbench and a software example. The software has been tested successfully in the Pynq board. This is the expected output when running in the FPGA:

```

# xilinx-zcu102-2020_2:~/repos/rtas22-dart-design/fir/sw# ./run.sh 
power sample PID: 733
 starting fir 
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0xffffa1918000, length:32768 
buff: buffer mapped at addresses: 0xffffa1910000, length:32768 
Time taken by the CPU is : 00.024602
Time taken by FRED is : 00.006673
Match!
Expected value[0:9]:
[ 0 13 24 44 75 132 207 377 504 637 ] 
Output value[0:9]:
[ 0 13 24 44 75 132 207 377 504 637 ] 
Fred finished 
```

FRED timing sample:

```
root@xilinx-zcu102-2020_2:/opt/fredsys# cat log.txt 
0000000006173719:       fred_sys: hw-task: fir got slot: 0 of its partition: p0, inserted in fri queue
0000000006173854:       fred_sys: start rcfg of slot: 0 of partition: p0 for hw-task: fir
0000000006177429:       fred_sys: devcfg, slot: 0 of partition: p0 rcfg completed for hw-task: fir in 3489 us
0000000006177465:       fred_sys: slot: 0 of partition: p0 started for hw-task: fir
0000000006183944:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: fir in 6467 us
0000001077373420:       fred_sys: hw-task: fir got slot: 0 of its partition: p0, inserted in fri queue
0000001077373515:       fred_sys: skipping rcfg of slot: 0 of partition: p0 for hw-task: fir
0000001077373543:       fred_sys: slot: 0 of partition: p0 started for hw-task: fir
0000001077380021:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: fir in 6466 us
0000001449407334:       fred_sys: hw-task: fir got slot: 0 of its partition: p0, inserted in fri queue
0000001449407423:       fred_sys: skipping rcfg of slot: 0 of partition: p0 for hw-task: fir
0000001449407450:       fred_sys: slot: 0 of partition: p0 started for hw-task: fir
0000001449413929:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: fir in 6467 us
0000001583840727:       fred_sys: hw-task: fir got slot: 0 of its partition: p0, inserted in fri queue
0000001583840790:       fred_sys: skipping rcfg of slot: 0 of partition: p0 for hw-task: fir
0000001583840816:       fred_sys: slot: 0 of partition: p0 started for hw-task: fir
0000001583847296:       fred_sys: slot: 0 of partition: p0 completed execution of hw-task: fir in 6467 us
```

## Resource Utilization

 - Device: xczu9egffvb1156-2
 - Board: zcu102


|          Site Type         | Used | Fixed | Available | Util% |
+----------------------------+------+-------+-----------+-------+
| CLB LUTs*                  | 1964 |     0 |    274080 |  0.72 |
| Block RAM Tile             | 12.5 |     0 |       912 |  1.37 |
| DSPs                       |    4 |     0 |      2520 |  0.16 |


## Authors

- Marco Pagani, HLS IP design
- Alexandre Amory, tb and sw design and testing.
