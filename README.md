# 8-bit-computer-emulator

This is a C++ based emulator of my [8-bit-computer](https://github.com/blurpy/8-bit-computer).

The goal is to make the emulator as realistic as possible. It's based on emulating the communication between the different parts of the computer so the state is accurate on every cycle. This means the instruction decoder does not change state of memory or registers, but rather directs which part can communicate over the bus at different points in time, like the real hardware do with the microcode in the EEPROMs. 

Please see the link above for documentation on how the computer works. Since the emulator tries to replicate that, no additional documentation is provided here.


## Current state

The emulation is done, and the programs that run on the real hardware will run unmodified on the emulator with the same result.

TODO:
* Some kind of user interface to visualize state
* Controls to allow modifying state and single step through programs
* Some more documentation in the code
* Unit tests


## Requirements

* cmake
* g++


## Build and run

Clone the repo first with git or download a zip of the repo.

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./src/8bit <program.asm>
```

Different programs are available in the programs directory.
