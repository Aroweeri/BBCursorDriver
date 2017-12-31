# Boogie Board Cursor Driver

## Installation/Compilation

To compile this driver you will need to have the X11 lib installed. 

You also must have the XTest lib installed which for Debian/Ubuntu can be installed with the
below command:

	sudo apt-get install libxtst.dev

Compile your driver with the below command:

	gcc -o bbcursordriver boogie_board.c -lX11 -lXtst

## Running

Before running your driver you need to find out what device file represents your boogie board. On
Ubuntu 16.04 I found that /dev/usb/hiddev0 was the one for me. You can simply cat the file with
your boogie board plugged in and press buttons/draw on it to see if any data is coming through:

	sudo cat /dev/usb/hiddev0

With your boogie board connected to your computer via usb, run your newly generated executable with
elevated priveledges, giving an argument to the device file that represents your boogie board:

	sudo bbcursordriver /dev/usb/hiddev0

You may find that your cursor freezes and the boogie board becomes unresponsive at random
intervals. This is a known problem and I've been investigating it for some time. The current
solution is to kill the process and restart it.

You can now control your cursor with the precision of a pen! Enjoy.
The orientation to place the boogie board for it to match with your screen is with the plug/cord
facing up.

Something to note:

The driver will manage to function with any screen resolution, but results may vary. With a square
display or perhaps one that is being used in portrait mode, you will find that moving the pen to
the left/right will be much more drastic a movement than up/down. These differences will likely be
irrelevant due to the feedback you'll be recieving from watching your cursor move on the screen.
