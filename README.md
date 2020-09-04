# bot2

# Build for MinGW

## Install GLEW
```
cd dependencies\glew-2.1.0\build
cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\MinGW
mingw32-make all
mingw32-make install
```

## Install GLFW
```
cd dependencies\glfw-3.2.1
cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\MinGW
mingw32-make all
mingw32-make install
```

## Install rapidjson and stb_image.h
```
cp -r dependencies\rapidjson C:\MinGW\include
cp dependencies\stb_image.h C:\MinGW\include
```
