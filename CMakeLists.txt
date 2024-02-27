project("my_cmake" CXX)#cmake 工程名字
set(src_list main.cpp swap.cpp)
add_executable(out ${src_list})#生成可执行文件(文件名，cpp列表......)，这种方式好像是针对cpp构建，如果有.a或者.so就是另一种方式了
add_definitions("-Wall -g")    #这是什么东西？？？

#Ctrl shift p 搜索cmake config 选择一个编译器 gcc 8.1.0 
#配置好后CppCode 在根目录执行cmake，会生成很多中间文件，为了让项目更加简洁，通常会创建build文件 cd build 执行cmake ..意思是执行build外层的尺码可文件
#中间文件会生成在当前执行cmake命令的文件夹。vscode好像会自动生成build
#配置好后CppCode目录生成build文件，cd build目录 ,执行cmake .. 生成Makefile文件,执行mingw32-make.exe(实际就是执行Makefile，生成可执行文件)，linux下执行make命令