.PHONY : clean
all : hello.c
	gcc hello.c -o hello

run :
	./hello

clean :
	rm -rf hello
