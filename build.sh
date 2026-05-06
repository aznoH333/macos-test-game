eval cc game.c $(pkg-config --libs --cflags raylib) -o game
./game
rm game.o
rm game
