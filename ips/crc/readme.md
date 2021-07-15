
# CRC IP

This IP calculates the CRC of a input vector and generates a 64 bit CRC.

Check the software example in the *sw* directory to see how to use this IP.
When running in the FPGA, the expected output is:

```
# code/synthetic 
starting CRC 
fred_lib: connected to fred server!
buff: buffer mapped at addresses: 0x36cc5000, length:1048576 
buff: buffer mapped at addresses: 0x36f14000, length:32768 
Match!
Expected value: [ 28BE905BC3D859EE] 
Output value  : [ 28BE905BC3D859EE] 
Fred finished
```

## Authors

- Alexandre Amory, design and testing.
