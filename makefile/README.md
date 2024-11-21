以廖雪峰的为准 https://liaoxuefeng.com/books/makefile/introduction/index.html
### 1. 基本语法 
Makefile由若干条规则（Rule）构成，每一条规则指出一个目标文件（Target），若干依赖文件（prerequisites），以及生成目标文件的命令
命令前必须以tab开头
```
目标：依赖、依赖......
    依赖生成目标的语法 
    依赖生成目标的语法 
    ......
```
以#开头的是注释，会被make命令忽略。
```
m.txt: a.txt b.txt
	cat a.txt b.txt > m.txt
```
这是一条简单的规则。我们怎么执行？`make 目标` 。 如果make后面没有跟目标就默认第一条 
把最终结果的规则放第一条，其他规则的顺序是无关紧要的，因为make执行时自动判断依赖。
```
$ make
make: `x.txt' is up to date.
make检测到x.txt已经是最新版本，无需再次执行，因为x.txt的创建时间晚于它依赖的m.txt和c.txt的最后修改时间。
```
### 2. 伪目标  
有时候不光要生成文件还需要其他操作，这就是伪目标。
```
clean:
    rm -f c.txt e.txt
    事实上上述规则也可以做到删除文件的目的，但是这样做会产生一个问题，如果有clean文件，该规则将不再被执行，（因为没有依赖文件，也就不会更新）
    所以就产生了伪目标，目标(clean)不再被认为是一个文件
```
.PHONY：name  声明伪目标. 伪目标可以是任意字符，常见的有clean和all
```
.PHONY:clean 
clean:
    rm -f c.txt e.txt
```
### 3. 执行环境
请看下面的规则
```
cd:
	pwd
	cd ..
	pwd
```
这条规则的输出是什么呢？两个pwd 打印相同的路径，这是因为规则的每条命令都在独立的shell环境执行，cd..命令影响不了第二条pwd的结果。
想要在一个环境中执行多条命令，命令中间用; 或者 &&隔开即可.
```
cd:
	pwd ;\
	cd ..;\
	pwd 
\或者这样便于浏览 
```
### 4. 控制打印
make会打印规则下的没一条命令，如果我们不想打印某一条命令，可以在命令前加上@，不打印单仍会执行
```
TARGET:OBJS
    @echo $@   增加@，不显示命令本身，只显示结果
```
### 5. 控制错误
正常情况下make在执行命令时，遇到错误就会中断执行。有些时候，我们想忽略错误，继续执行后续命令，可以在需要忽略错误的命令前加上-
```
has_error:
	-rm zzz.txt
	echo 'ok'
```
### 6. 隐式规则
请看下面的规则，即使把生成.o的规则去掉也能执行，这是因为make最初就是为了编译C程序而设计的，为了免去重复创建编译.o文件的规则，
make内置了隐式规则（Implicit Rule），即遇到一个xyz.o时，如果没有找到对应的规则，就自动应用一个隐式规则：
对于C程序来说，使用隐式规则有一个潜在问题，那就是无法跟踪.h文件的修改。如果我们修改了hello.h的定义，
由于隐式规则main.o: main.c并不会跟踪hello.h的修改，导致main.c不会被重新编译，这个问题我们放到后面解决。
```
world.out: hello.o main.o
	cc -o world.out hello.o main.o
```
### 7. 定义变量
目标和依赖在规则中出现很多次我们可以用变量来替代减少脚本大小
变量定义用变量名 = 值或者变量名 := 值，通常变量名全大写。引用变量用$(变量名)，非常简单。
$@表示目标文件，$^表示所有依赖文件
```
TARGET = world.out
OBJS = hello.o main.o
$(TARGET): $(OBJS)
	cc -o $(TARGET) $(OBJS)
```
### 8. 自动生成依赖
如果有一种方式能让make自动生成hello.o main.o这个“列表”，就更好了。注意到每个.o文件是由对应的.c文件编译产生的，
因此，可以让make先获取.c文件列表，再替换，得到.o文件列表：
```
$(wildcard %.c) 列出所有的.c文件
$(patsubst %.c,%.o,$(wildcard *.c)) 
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
TARGET = main 
$(TARGET):$(OBJS)
    gcc -o $(TARGET):$(OBJS)
%.o:%.c
    gcc -c -o $@:$<    
```


