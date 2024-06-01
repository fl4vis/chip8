INCLUDES= -I./include
FLAGS= -g
all:
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -o ./bin/main `sdl2-config --cflags --libs`
clean:
	rm ./bin/*