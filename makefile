CC  = gcc
IC2 = -Wall
BIN = CrossWords
INC = includes/
CFLAGS = `sdl-config --libs --cflags` -lSDL_image -lSDL_ttf -lSDL_mixer
CSDL = `sdl-config --libs --cflags`

$(BIN): main.o moteur.o STJ_SDL.o
	$(CC) $(IC2) main.o moteur.o STJ_SDL.o -o $(BIN) -lm $(CFLAGS)

main.o: main.c $(INC)Head.h $(INC)STJ_SDL.h
	$(CC) $(IC2) -c main.c $(CSDL)
	
moteur.o: moteur.c $(INC)Head.h
	$(CC) $(IC2) -c moteur.c $(CSDL)
	
STJ_SDL.o: STJ_SDL.c $(INC)STJ_SDL.h $(INC)Head.h
	$(CC) $(IC2) -c STJ_SDL.c $(CSDL)

clean:
	@rm -f *.o
	@rm $(BIN)