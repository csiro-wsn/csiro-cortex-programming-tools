csiro-cortex-programming-tools
==============================

Programming tools for wireless sensor nodes based on ARM Cortex-M3.

## Content

This directory contains a build script (build.sh) to compile the following tools:

1. The command line version of the BOSSA flash programming utility (http://www.shumatech.com/web/products/bossa). It contains Opal-specific patches such as toggling LEDs while programming and rebooting the node after programming.

2. The command line tools "opal-erase" and "opal-erase-and-reset" to control the Opal node when connected to the Opal debug board.

## Build Instructions:

1. Open a shell and change to the top directory of this repository.
2. Build the toolchain by running "./build.sh" Binary files will be written to the "build" folder.
3. Move the files within the "build" folder to the final location where you would like to install the binaries (e.g. "/opt/csiro-cortex-programming-tools/" or "$HOME/csiro-cortex-programming-tools").
3. Add the location of the binaries to the PATH environment variable of your shell. This can be done by adding the following line to your $HOME/.bashrc file if you are using Bash:
```
export PATH=$PATH:$HOME/csiro-cortex-programming-tools
```
4. In order to setup USB device permissions using udev rules copy "40-csiro-opal.rules" from the "config" folder to "/etc/udev/rules.d/".

## Troubleshooting for OSX:
1. to compile opal tools for OSX, you might want to use the following flags
```
CFLAGS="-L/opt/local/lib/ -I/opt/local/include/"
```

## Contact: 

* [Philipp Sommer](mailto:Philipp.Sommer@csiro.au)
