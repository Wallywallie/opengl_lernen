# 环境配置
## 安装glfw
由于预编译文件与本地编译器产生了不兼容的问题，本项目自己编译glfw
1. 源码下载
2. `makedir build` -> `cd build` -> `cmake .. -G "MinGW Makefiles"` -> `mingw32-make`
3. `build/src/libglfw.a`复制到项目路径下