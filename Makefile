CC = gcc
CFLAGS = -W -ansi -std=c99 -g -Wunused-parameter
LIBS = -L./SDL2_ttf/.libs -L./SDL2_image/.libs -lm
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf -lSDL2_image
INCLUDES = -I./SDL2_ttf -I./SDL2_image
EXEC = main
SRC = main.c renderer.c map.c player.c vector2.c SDL_utils.c sprite.c building.c roads.c menu.c game.c
OBJ = $(SRC:.c=.o)

all: $(EXEC)
main: $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LIBS) $(LDFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
clean:
	rm -rf *.o *~
mrproper: clean
	rm -rf $(EXEC)
