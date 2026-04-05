CC = gcc
CFLAGS = -I./include -I/usr/local/include
LDFLAGS = -L/usr/local/lib -loqs -lcrypto -lm

all: lib static_test

lib:
	$(CC) $(CFLAGS) -fPIC -c src/pqc_lib.c -o src/pqc_lib.o
	ar rcs libmypqc.a src/pqc_lib.o
	$(CC) -shared -o libmypqc.so src/pqc_lib.o $(LDFLAGS)

static_test:
	$(CC) $(CFLAGS) -o pqc_tester main.c libmypqc.a $(LDFLAGS)

clean:
	rm -f src/*.o *.a *.so pqc_tester
