all:
	gcc -I./include -I/usr/local/include -o pqc_tester main.c src/pqc_lib.c /usr/local/lib/liboqs.a -lcrypto -lm

clean:
	rm -f pqc_tester *.o *.a *.so
