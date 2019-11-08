# Boogie Board Cursor Driver

## Installation/Compilation

To compile this driver you will need to have the X11 lib installed. 

You also must have the XTest lib installed which for Debian/Ubuntu can be installed with the
below command:

	sudo apt-get install libxtst.dev

Compile with `make`


## Running

After compiling the program, you only need to run it with

sudo ./bbcursordriver

The program automatically detects the presence of a boogie board rip to read from.

You can now control your cursor with the precision of a pen! Enjoy.
The orientation to place the boogie board for it to match with your screen is with the plug/cord
facing up.

Something to note:

The driver will manage to function with any screen resolution, but results may vary. With a square
display or perhaps one that is being used in portrait mode, you will find that moving the pen to
the left/right will be much more drastic a movement than up/down. These differences will likely be
irrelevant due to the feedback you'll be recieving from watching your cursor move on the screen.
