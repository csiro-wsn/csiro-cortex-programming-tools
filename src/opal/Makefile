all:
	mkdir -p bin
	gcc $(CFLAGS) -o bin/opal-list opal.c opal-list.c -lftdi
	gcc $(CFLAGS) -o bin/opal-erase-and-reset opal.c opal-erase-and-reset.c -lftdi
	gcc $(CFLAGS) -o bin/opal-reset opal.c opal-reset.c -lftdi
	gcc $(CFLAGS) -o bin/opal-init opal.c opal-init.c -lftdi
	gcc $(CFLAGS) -o bin/opal-start opal.c opal-start.c -lftdi
	gcc $(CFLAGS) -o bin/opal-stop opal.c opal-stop.c -lftdi


