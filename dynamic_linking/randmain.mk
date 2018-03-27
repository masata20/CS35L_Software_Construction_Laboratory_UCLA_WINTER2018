# Builds the program randmain using three types of linking.


randlibhw.so: randlibhw.c
	$(CC) $(CFLAGS) -shared -fPIC randlibhw.c -o randlibhw.so
randlibsw.so: randlibsw.c
	$(CC) $(CFLAGS) -shared -fPIC randlibsw.c -o randlibsw.so
randmain: randmain.o randcpuid.o
	$(CC) $(CFLAGS) -ldl -Wl,-rpath=$(PWD) randmain.o randcpuid.o -o randmain
