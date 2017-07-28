# Boogie Board Cursor Driver

## Installation/Compilation

To compile this driver you will need to have the X11 lib installed. 

You also must have the XTest lib installed which for Debian/Ubuntu can be installed with the below command:

`sudo apt-get install libxtst.dev`

Compile your driver with the below command:

`gcc boogie_board.c -lX11 -lXtst`

## Running

With your boogie board connected to your computer via usb, run your newly generated executable with elevated priveledges.

You can now control your cursor with the precision of a pen! Enjoy
