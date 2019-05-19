Chip Floor PLanner and Top-level Router

Final Project

Made by: Habiba Gamal
Engy Raafat
Maha Moussa

The modules are still not integrated together. 

For router.cpp: 

- The application now takes 4 files, the technology lef, a file containing the nets (see file test3.txt in folder "Deliverable 2/router testcases") , a file that containts the die width and length and the pins to be added as obstacles (see file test1.1 in folder "Deliverable 2/router testcases") and a file that contains the starting coordinates of the macros and their lengths and widths to be added as obstacles(see file .....)
- The resulting section (nets section) of the DEF file is outputted in a file
- On the command line, details about the routes are reported. 
    1) "All net routed": Path was found for all pins in the net
    2) "Could not route the net": No route was found between source pin (first pin) and all target pins
    3) "Could not route pin + {pin location}": A path was found for other pins in the net, but for this particular pin, no path was found. In this case, the whole route is cancelled, and not outputted in the DEF of course. 
  
FloorPlanner.cpp:
 -  It takes the input as a directory of def files for the hard macros
 -  The soft Macros are entered through a specified file that contains their names and data
 -  Through this stages, DEF files are parsed then Linear equations are created and solved for both soft and hard macros 
 - It outputs the following:
        A grid for the Router
        File with module locations, module sizes, pin data ( name , metal layer , normalized location) for pin assignment
        Final DEF file components without the NETS section which is an output of the router 
