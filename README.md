# Advent of Code 2022

## Building / Running

I originally told myself I'd do this in ASM as I need to learn some... but, you know.... ain't nobody got time for
that. Decided to do it in C++ as I need to write a bunch of it for my upcoming Augmented Normalcy project, and I've
only really written C++ for embedded.

### Windows

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..
$ ninja
$ ninja install
# now we can run things :D
$ .\day_1.exe
```