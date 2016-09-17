.DEFAULT_GOAL := all
static:
	gcc -g -I. -c promises.c -o promises.o
	ar rcs libpromises.a promises.o
shared:
	gcc -g -I. -c -fPIC promises.c -o promises.o
	gcc -shared -Wl,-soname,libpromises.so.1 -o libpromises.so.1.0.0 promises.o
check: all
	gcc -g -o promise-test -I. -L. promise-test.c -lpthread -lpromises
	./promise-test
clean:
	-rm *.o *.a *.so.* promise-test
all: static shared
