# Alarm-Board

A simple alarm clock with the purpose of being ran in a Raspberry PI written in C with the raylib library.

## Build Instructions

The only two requirements that are required that should be installed in your system is: Raylib and pigpio. Other than that, to build for general testing a simple `make run` is enough.

For PI access and testing, pigpio requires the program to be ran under root, so `sudo make run` needs to be ran.

## Things to do
- Multi alarm sound selection.
- Custom themes (Probably palette selection)
- Expand to be a bedside dashboard.
    - ~~Weather forecast (Most likely using libcurl and custom JSON implementation)~~.