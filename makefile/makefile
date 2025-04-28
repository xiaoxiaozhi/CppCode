DIR = $(dir %.c) #结果 ./ 只能打印相对目录
$(info This is an info message $(dir %.c))
e.txt:c.txt d.txt
	cat c.txt d.txt > e.txt
c.txt:a.txt b.txt
	cat a.txt b.txt > c.txt

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
TARGET = main
$(TARGET):$(OBJS)
	gcc -o $(TARGET) $(OBJS)

%.o: %.c
	gcc -c -o $@ $<



.PHONY:clean cd1 cd2
clean:
	@-rm c.txt e.txt 
	@-rm *.o
	@-rm *.exe
cd1:
	pwd
	cd ..
	pwd
cd2:
	pwd;cd ..;pwd

has_error:
	-rm zzz.txt
	echo 'ok'