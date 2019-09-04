This directory contains the minimal code required to interface the SCAMP5 with ROS.

scamp5d_app_one/ contains C++ source code to build an application that reads text messages sent by the SCAMP5 device via usb, and outputs them to STDOUT. It can be build using:
```
make -f scamp5d_app_one.cbp.mak release_rpi
```
The make file itself is generated from the CodeBlocks configuration file, using 
```
cbp2make -in scamp5d_app_one.cbp
```

ROS_control.py reads STDIN and boradcasts steering directions orders to the ROS system.


Finally, interfacing the SCAMP5 to ROS is done by piping the first program to the second:
```
./s5da | python ROS_instructions_dispenser.py
```
(might require to sudo).