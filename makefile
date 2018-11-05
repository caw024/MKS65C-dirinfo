all: dir.c dir.h
	gcc -g dir.c
run: all
	./a.out
clean:
	rm a.out 
