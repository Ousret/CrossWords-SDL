CC  = gcc -Wall -Wextra
BIN = CrossWords
INC = include/
CFLAGS =`sdl-config --libs --cflags` -lSDL_image -lSDL_ttf -lESDL -lpthread -lfmodex -lcrypto #-lGL -lGLU -lGLEW -lSDL2_gpu 
CSDL = `sdl-config --cflags`

$(BIN): main.o engine.o mliste.o save.o
	$(CC) main.o engine.o mliste.o save.o -o $(BIN) -lm $(CFLAGS)

main.o: main.c $(INC)engine.h
	$(CC) -c main.c $(CSDL)

engine.o: engine.c $(INC)engine.h
	$(CC) -c engine.c

mliste.o: mliste.c $(INC)mliste.h
	$(CC) -c mliste.c

save.o: save.c $(INC)save.h
	$(CC) -c save.c

clean:
	@rm -f *.o
	@rm $(BIN)