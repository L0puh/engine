# ENGINE

## set up:

1. download dependencies: 

    - [glfw3.0](https://www.glfw.org/download): manage window
    - [glad.c & glad.h](https://glad.dav1d.de/generated/tmpz09wqj4oglad/glad.zip): loader  
    - [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h): convert images to textures
    - [glm 1.0.0](https://glm.g-truc.net/0.9.8/index.html): do math
    - [imgui](https://github.com/ocornut/imgui): debug & UI (for now?)

2. run:

```sh
mkdir build
cd build
cmake ..
make
./engine 
```

