For router.cpp: 
- The application now takes 4 files
    1) The technology lef, 
    2) a file containing the nets (see file "test6_nets.txt" in folder "Router Testcases") , 
    The format of the file is: 
        (macro_name pin_name):(x1 y1 z1)
        (macro2_name pin2_name):(x2 y2 z2)
        (macro3_name pin3_name):(x3 y3 z3)
        ;
        (macro4_name pin4_name):(x4 y4 z4)
        (macro5_name pin5_name):(x5 y6 z5)
        ;

    where (x1 y1 z1) is the pin coordinates and ';' separates 2 nets
      
    3) a file that containts the die width and length and the pins' coordinates to be added as obstacles (see file "test6_pins.txt" in folder "Router Testcases")  
    The format of the file is :
        L: 
        W: 
        (x1 y1 z1)
        (x2 y2 z2)

    where (x1 y1 z1) is the pin coordinates

    4) and a file that contains the starting coordinates of the macros and their lengths and widths to be added as obstacles (see file "test6_macros.txt" in folder "Router Testcases") 
    The format of the file is: 
        x1,y1,l1,w1
        x2,y2,l2,w2

    Where x1 & y1 is the top left coordinate of the macro. l1 and w1 are the lengths and widths respectively.

- The resulting section (nets section) of the DEF file is outputted in a file (see file "out_test6.txt" in folder "Router Testcases") 
- On the command line, details about the routes are reported. 
    1) "All net routed": Path was found for all pins in the net
    2) "Could not route the net": No route was found between source pin (first pin) and all target pins
    3) "Could not route pin + {pin location}": A path was found for other pins in the net, but for this particular pin, no path was found. In this case, the whole route is cancelled, and not outputted in the DEF of course. 