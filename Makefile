all:
	g++ src/main.cpp -lSDL2 -lSDL2_image -o build/game.exe
run:
	./build/game.exe