# simple-paint
Simple paint application written using Raylib

## Requirements
- CMake 3.16+
- C++20

## Build & Run
1. Create build directory:
```sh
mkdir build
cd build
```

2. Build:
```sh
cmake ..
cmake --build .
```

3. Run:
```sh
./Paint
```

## Features
- Color switching (mouse selection or with arrow left/arrow right)
- Changing brush size (mouse wheel up/down)
- Clear (C)
- Save (CTRL + S)
- Tools:
	- Brush (B)
	- Eraser (E)
	- Rectangle (R)
	- Line (L)
    - Circle (O)
    - Bucket Fill (F)
