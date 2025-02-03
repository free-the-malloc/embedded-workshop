# Some embedded fun!

## AVR Toolchain

For AVR source files, the avr-GNU toolchain is necessary. For MacOS, run the following in the terminal:

    brew tap osx-cross/avr
    brew install avr-gcc

You'll also want avrDude which handles the flashing of the compiled hex code onto the microcontroller.

    brew install avrdude

To configure VS Code for this environment, see this [link](https://www.tonymitchell.ca/posts/use-vscode-with-avr-toolchain/) and follow the Linux instructions. 

The developer guides published by Microchip are relatively comprehensive. Here are some resources.

- For details on the different digital ports, look [here](https://developerhelp.microchip.com/xwiki/bin/view/products/mcu-mpu/8-bit-avr/structure/ioports/).

- For help with any avr-C functions, consult the man pages by plugging into the terminal <code>man \<function name\></code>.

### Using the Makefile

To compile and flash some C source code to the AVR, you will need to determine the port in which to access the microchip.

For MacOS, this may be determined by opening a terminal session and entering the following command:

    ls /dev/

Look through the list for a device that looks something like <code>cu.usb*</code>.

Now, make sure you're in the project directory and open a terminal session. Call the following command:

    make deploy TARGET=<src filename> PORT=<microcontroller port>

Where target is your source file name (without the .c extension) and PORT is the device port we just located (make sure you include /dev/).

If you want to open a serial monitor for the device, enter the following in a terminal session:

    cat /dev/<device port>

