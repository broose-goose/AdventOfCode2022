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

### Mac

```
$ brew install cmake
$ mkdir build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
$ make install
# now we can run things D:
$ ./day_4.exe
```

## Multiple Repos

Well, I have this set up on my personal github so I can work on it in my comfy CLion env. B/c of this, I need to:

```
$ git pull origin main
$ git push aledade_gitlab main
```