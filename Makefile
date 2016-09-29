Labirynthe:	Labirynthe.c
	gcc Labirynthe.c `sdl-config --cflags --libs` -o Labirynthe -lm
clean:
	rm Labirynthe
