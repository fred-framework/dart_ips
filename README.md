
# DART IPs

This repository has a list of ready-to-use hardware IPs and static parts for DART.

## Summary of IPs

[//]: <> (This is a comment.)

The following table summarizes the existing IPs and some of their features.
The columns refer to the existance of software example, the IP hardware description language, if it has a testbench, and if it has been tested in FPGA. Click in IP link to go to their respective documentations.

| IP name                                      | sw ex. | hw lang | tb | FPGA |
|----------------------------------------------|--------|---------|----|------|
| [CRC](ips/crc/readme.md)                     |    Y   |   cpp   | Y  |  Y   |
| [fastx](ips/fastx/readme.md)                 |    Y*  |   cpp   | Y* |  N   |
| [FINN CNN](ips/finn/readme.md)               |    N   |   cpp   | N  |  N   |
| [FIR](ips/fir/readme.md)                     |    Y   |   cpp   | Y  |  Y   |
| [Lenet CNN](ips/lenet/readme.md)             |    Y   |   cpp   | Y  |  N   |
| [mat_mult](ips/mat_mult/readme.md)           |    Y   |   cpp   | Y  |  Y   |
| [memcpy](ips/memcpy/readme.md)               |    Y   |   cpp   | Y  |  Y   |
| [mul_vec](ips/mul_vec/readme.md)             |    Y   |   cpp   | Y  |  Y   |
| [nor_vec](ips/nor_vec/readme.md)             |    Y   |   cpp   | Y  |  Y   |
| [prem](ips/prem/readme.md)                   |    Y   |   cpp   | Y  |  Y   |
| [prem_config](ips/prem_config/readme.md)     |    Y   |   cpp   | Y  |  Y   |
| [sobel](ips/sobel/readme.md)                 |    Y   |   cpp   | Y  |  Y   |
| [sub_vec](ips/sub_vec/readme.md)             |    Y   |   cpp   | Y  |  Y   |
| [sum_vec](ips/sum_vec/readme.md)             |    Y   |   cpp   | Y  |  Y   |
| [xor_vec](ips/xor_vec/readme.md)             |    Y   |   cpp   | Y  |  Y   |
|----------------------------------------------|--------|---------|----|------|
| [amalthea_prem](ips/amalthea_prem/readme.md) |    N   |   vhd   | Y  |  N   |
| [amalthea_pipe](ips/amalthea_pipe/readme.md) |    N   |   vhd   | Y  |  N   |
| [gmap](ips/gmap/readme.md)                   |    N   |   cpp   | N  |  N   |
|                                              |        |         |    |      |

* not self-checking

## Summary of Static Parts

[//]: <> (This is a comment.)

The following table summarizes the existing Static Parts and some of their features.
The columns refer to the number of existing **reconfigurable regions (RR)** in the design, the number of IPs for DPR, and a brief description of its application. Click in the link to go to their respective documentations.
**BE AWARE THAT THESE DESIGNS WERE NOT FULLY TESTED YET.**

| IP name                                      | # RR | # cores | application |
|----------------------------------------------|------|---------|-------------|
| [1partition](static/1partition/readme.md)    |  1   |   1     | generic     |
| [2partition](static/2partition/readme.md)    |  2   |   1     | generic     |
| [3partition](static/3partition/readme.md)    |  3   |   1     | generic     |
| [img_filters](static/img_filters/readme.md)  |  3   |   x     | image proc. |
|                                              |      |         |             |


# Guidelines and structure for IPs

All IPs should follow these guidelines and this structure in order to reuse the scripts and minimize the integration issues with DART and FRED. The rest of this section show the IP guidelines and the structure for the **two kinds of IPs: HLS and RTL**.

## Guidelines

 - the IPs must not have automatically genereted files, like files generated by synthesis tools. Use the scripts instead to recreate the design;

[//]: <> (###################################################.)

## HLS IPs


HLS IPs are those created from *vivado_hls*, based on C source code. These IPs use a different synthesis script and have a different directory structure, presented next.

### HLS IP structure 

All HLS-based IPs must have the following directory structure: 
 
   
```
.
├── LICENSE
├── readme.md
├── hw
│   ├── build.tcl
│   ├── src
│   │   ├── memcpy.cpp
│   │   └── ...
│   └── tb
│       ├── tb.cpp
│       └── ...
└── sw
    ├── Makefile
    └── src
        ├── memcpy.cpp
        └── ...
```   

 - the *readme.md* file documents the IP, including usage/perfomance/power reports, configuration parameters, authors, etc;
 - the file *hw/build.tcl* is a script synthesis for Vivado (for Verilog or VHDL sources) or for Vivado HLS (for cpp sources). This script refers to the common synthesis script located at the *scripts* directory, at same level of the IP directory;
 - the *sw* directory must have an example FRED-enabled application to test the IP;
 - the *sw/Makefile* compiles the example application;

### running synthesis for the HLS IPs

Go to the directory *ips/ip-name/hw* and run:


```
$ vivado_hls build.tcl
```   

The generated IP is located in the directory *ips/ip-name/hw/ip-name/solution_0/impl/ip*.



[//]: <> (###################################################.)

## RTL IPs

RTL IPs are those created from *vivado*, based on VHDL or Verilog source code. Their directory structure is presented next.

### RTL IP structure 

All RTL-based IPs must have the following directory structure: 
 
   
```   
.
├── build.tcl
├── LICENSE
├── readme.md
├── hw
│   ├── bd
│   ├── hdl
│   │   └── my_module.vhd
│   ├── ips
│   ├── sim
│   │   └── tb.vhd
│   └── xdc
│   │   └── constraint.xdc
├── vivado
└── sw
    ├── Makefile
    └── src
        ├── test_app.cpp
        └── ...
```    

 - the *build.tcl* script is set to rerun the synthesis to create a vivado design;
 - use the *readme.md* to document the design including, for example, usage/perfomance/power reports, configuration parameters, authors;
 - in the case of an IP design, the directory *hw/bd* is typically empty since IPs are usually described in RTL or HLS, not with BD;
 - the IP description files (.vhd and/or .v) must be placed in the *hw/hdl* directory;
 - if there is a testbench, this must be placed in the *hw/sim* directory. VHDL, Verilog, and SystemVerilog are accepted as testbench languages;
 - if there are constraint files, these must be placed in the *hw/xdc* directory;
 - a IP might be composed of other sub-IPs. In this case, they must be linked in the *hw/ips* directory. Just place symbolic links to point to other IPs directories;
 - the vivado directory must be empty. This is where the vivado design is saved;
 - the *sw* directory must have an example FRED-enabled application to test the IP;
 - the *sw/Makefile* compiles the example application;

### running synthesis for the RTL IPs

Go to the directory *ips/ip-name/hw* and run:

```
$ vivado -mode batch -source build.tcl
```

The generated IP is located in the directory *ips/ip-name/vivado/ip-name.runs/impl_1/*.


[//]: <> (###################################################.)

# Guidelines and structure for the static part of the design

The static part of the FPGA design is the part that does not suffer DPR.
All static parts must follow these guidelines and this structure in order to reuse the scripts and minimize the integration issues with DART and FRED.

## guidelines

 - the static parts must not have automatically genereted files, like files generated by synthesis tools. Use the scripts instead to recreate the design;

## structure 

All static parts must have the following directory structure: 

```   
.
├── build.tcl
├── LICENSE
├── README.md
├── hw
│   ├── bd
│   │   └── design_1.tcl
│   ├── hdl
│   ├── ips
│   ├── sim
│   └── xdc
└── vivado
```   

 - the *build.tcl* script is set to rerun the synthesis to create a vivado design;
 - use the *readme.md* to document the design including, for example, usage/perfomance/power reports;
 - the static part is typically based on a block design, thus, the BD file must be placed in the *hw/bd* directory;
 - since the static part is based on block design, the *hw/hdl* directory, where hardware description files (.vhd and/or .v) are saved, must be empty;
 - if there is a testbench, this must be placed in the *hw/sim* directory;
 - if there are constraint files, these must be placed in the *hw/xdc* directory;
 - the IPs under DPR must be linked in the *hw/ips* directory. The default IP is the memcpy IP. Just replace the  symbolic links to point to other IPs directories;
 - the vivado directory must be empty. This is where the vivado design is saved.

## running synthesis for the static part

Go to the directory *static/static-name/* and run:

```
$ vivado -mode batch -source build.tcl
```   

The generated design is located in the *vivado* directory.
The generated DCP file is located in the directory *static/static-name/vivado/proj-name/proj-name.runs/synth_1/proj-name_wrapper.dcp*.


# Compiling all IPs at once

There is a makefile to easy the task of compiling the IPs in the first time. To run it, execute the following steps:

```
$ cd dart_ips/ips
$ make all -j 8
$ make link 
```

This sequence compiles all IPs using 8 parallel jobs and it creates the symbolic links in the `DART_IP_PATH` environment variable. This
 step is done only once. You have to repeate it only if the IP hardware descrition is changed.  

# Authors

- Alexandre Amory (April 2021), ReTiS Lab, Scuola Sant'Anna, Pisa, Italy.


# Funding

This tool has been developed in the context of the [AMPERE project](https://ampere-euproject.eu/).
This project has received funding from the European Union’s Horizon 2020 
research and innovation programme under grant agreement No 871669.
