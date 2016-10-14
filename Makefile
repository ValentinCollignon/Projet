all:Labyrinthe

Labyrinthe: endfile.o event.o initfile.o Labyrinthe.o
	gcc -o Labyrinthe endfile.o event.o initfile.o Labyrinthe.o  `sdl-config --cflags --libs` -lm

endfile.o: endfile.c
	gcc -o endfile.o -c endfile.c `sdl-config --cflags --libs` -lm -W -Wall -ansi -pedantic 

event.o: event.c
	gcc -o event.o -c event.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 

initfile.o: initfile.c
	gcc -o initfile.o -c initfile.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 

Labyrinthe.o: Labyrinthe.c endfile.h initfile.h event.h
	gcc -o Labyrinthe.o -c Labyrinthe.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 

clean:
	rm -rf *.o

mrproper: clean
	rm -rf Labyrinthe
