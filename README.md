# bot2

sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

## Installation Guide for MinGW
1. _Install MinGW and build tools_
   
   Suppose MinGW is installed to directory C:/MinGW
   Install cmake and ninja 

2. _Install glew_

   Build glew using the following commands:
      ```
      $cd dependencies
      $unzip glew-2.1.0.zip
      $cd glew-2.1.0/build/cmake
      $cmake -G Ninja .
      $ninja
      ```
   Copy glew-2.1.0/build/cmake/lib/libglew32.a to C:/MinGW/lib manually.
   Copy glew-2.1.0/include/GL to C:/MinGW/include manually.

3. _Install glfw_

   Build glfw using the following commands:
      ```
      $cd dependencies
      $unzip glfw-3.2.1
      $cd glfw-3.2.1
      $cmake -G Ninja .
      $ninja
      ```
   Copy glfw-3.2.1/src/libglfw3.a to C:/MinGW/lib manually.
   Copy glfw-3.2.1/include/GLFW to C:/MinGW/include manually.
   
4. _Install gvim for windows_

   Copy dependencies/gvimrc to the installation directory of gvim
