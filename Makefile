obj-m+=ebbchar.o

all:
	make -C /lib/module/$(shell uname -r)/build/M=$(PWD) modules
	$(CC) testebbchar.c -o test
clean:
	make -C /lib/module/$(shell uname -r)/build/M=$(PWD) clean
	rm test
