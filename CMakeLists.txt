project("my_cmake" CXX)#cmake 工程名字
add_executable(out main.cpp swap.cpp)#生成可执行文件(文件名，cpp列表......)
add_definitions("-Wall -g")  
#Ctrl shift p 搜索cmake config 选择一个编译器 gcc 8.1.0 
#配置好后CppCode目录生成build文件，cd build目录 ,执行cmake .. 生成Makefile文件,执行mingw32-make.exe(实际就是执行Makefile，生成可执行文件)，linux下执行make命令