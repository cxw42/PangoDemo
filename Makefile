all: main

main: main.c
	gcc -o $@ $< `pkg-config --cflags --libs gtk+-3.0`
