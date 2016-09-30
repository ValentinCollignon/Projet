all:Labirynthe

Labirynthe: endfile.o event.o initfile.o Labirynthe.o
	gcc -o Labirynthe endfile.o event.o initfile.o Labirynthe.o  `sdl-config --cflags --libs` -lm

endfile.o: endfile.c
	gcc -o endfile.o -c endfile.c `sdl-config --cflags --libs` -lm -W -Wall -ansi -pedantic 

event.o: event.c
	gcc -o event.o -c event.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 

initfile.o: initfile.c
	gcc -o initfile.o -c initfile.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 

Labirynthe.o: Labirynthe.c endfile.h initfile.h event.h
	gcc -o Labirynthe.o -c Labirynthe.c `sdl-config --cflags --libs`-lm -W -Wall -ansi -pedantic 
	
clean:
	rm -rf *.o

mrproper: clean
	rm -rf Labirynthe
