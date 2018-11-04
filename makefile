all: dir.o
	gcc dir.o
run:
	./a.out
clean:
	rm a.out
dir.o:
	gcc -c dir.c
