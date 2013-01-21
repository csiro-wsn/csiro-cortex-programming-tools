all:
	mkdir -p bin
	${CROSS_COMPILE}gcc -o bin/opal-list opal.c opal-list.c -lftdi
	${CROSS_COMPILE}gcc -o bin/opal-erase-and-reset opal.c opal-erase-and-reset.c -lftdi
	${CROSS_COMPILE}gcc -o bin/opal-reset opal.c opal-reset.c -lftdi
	${CROSS_COMPILE}gcc -o bin/opal-init opal.c opal-init.c -lftdi
	${CROSS_COMPILE}gcc -o bin/opal-start opal.c opal-start.c -lftdi
	${CROSS_COMPILE}gcc -o bin/opal-stop opal.c opal-stop.c -lftdi


