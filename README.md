# BareBonesRenderer
Minimal C++ wireframe renderer built entirely on the CPU, using only standard C libraries and the Windows API—no external graphics libraries. Designed for learning the fundamentals of 3D rendering from scratch

## Features

![Wireframe rendering demo](./assets/gifs/demo.gif)

-- loads .obj files (inside the /assets)
-- basic camera movement with WASD and mouse
-- wireframe rendering

## How to build

its just some CMake tbh

```bash
mkdir build
cd build
cmake ..
cmake --build .