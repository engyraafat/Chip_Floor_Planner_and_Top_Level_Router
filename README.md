Chip Floor PLanner and Top-level Router

Final Project

Made by: Habiba Gamal
Engy Raafat
Maha Moussa

The project is composed of 3 programs. They should be run sequentially after the other in the order: 
    1) FloorPlanner.cpp
    2) PinAssignment.cpp
    3) router.cpp
The output files of one program serves as the input of the next one

FloorPlanner.cpp:
-  It takes the input as a directory of def files for the hard macros
-  The soft Macros are entered through a specified file that contains their names and data
-  Through this stages, DEF files are parsed then Linear equations are created and solved for both soft and hard macros 
-  The optimization is according to:
        - total area 
        - modules to modules connections
        - modules to I/Os connections
- It outputs the following:
A grid for the Router
File with module locations, module sizes, pin data ( name , metal layer , normalized location) for pin assignment
Final DEF file components without the NETS section which is an output of the router 

Pin Assignment:
    Inputs:
        the first file is obtained from the hardmacros output from the floor planner. It contains the names of the hardmacros, their coordinates, their pins and their locations.
        the second file is a file that contains the soft macros and the pins and the macros they are connected to. these informaion should be retrieved from parsing the gatelevel netlist.
    outputs:
        the program outputs two files. one file contains the pins on every macro
        the other file contains the coordinates of each macro 
        these two files will be used by the router
    Assumptions:
        one assumption is that there will always be hard macros
        the number of pins connected to a macro will fit on its surface

For router.cpp: 
- The application now takes 4 files
    1) The technology lef, 
    2) a file containing the nets (see file "test6_nets.txt" in folder "Router Testcases") , 
    3) a file that containts the die width and length and the pins' coordinates to be added as obstacles (see file "test6_pins.txt" in folder "Router Testcases")  
    4) and a file that contains the starting coordinates of the macros and their lengths and widths to be added as obstacles (see file "test6_macros.txt" in folder "Router Testcases") 
- The resulting section (nets section) of the DEF file is outputted in a file (see file "out_test6.txt" in folder "Router Testcases") 
- On the command line, details about the routes are reported. 
    1) "All net routed": Path was found for all pins in the net
    2) "Could not route the net": No route was found between source pin (first pin) and all target pins
    3) "Could not route pin + {pin location}": A path was found for other pins in the net, but for this particular pin, no path was found. In this case, the whole route is cancelled, and not outputted in the DEF of course. 

