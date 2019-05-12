Chip Floor PLanner and Top-level Router

Final Project

Made by: Habiba Gamal
Engy Raafat
Maha Moussa

The modules are still not integrated together. 

For router.cpp: 

- The application now takes 3 files, the technology lef, a file containing the nets (see file test1.txt in folder "Deliverable 2/router testcases") and a file that containts the die width and length and the pins to be added as obstacles (see file test1.1 in folder "Deliverable 2/router testcases")
- The resulting grid is outputted on the command line. The resulting section of the DEF file is generated in a file (see file out_test1.txt in folder "Deliverable 2/router testcases")
  
FloorPlanner.cpp:
 -  It takes the input as a directory of def files for the hard macros
 -  The soft Macros are entered through a specified file that contains their names and data
 -  Through this stages, DEF files are parsed then Linear equations are created and solved for both soft and hard macros 
 - It outputs the following:
        A grid for the Router
        File with module locations, module sizes, pin data ( name , metal layer , normalized location) for pin assignment
        Final DEF file components without the NETS section which is an output of the router 
