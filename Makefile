all: game-of-life

game-of-life: main.c
	$(CC) -o game-of-life main.c -Wall -W -pedantic -std=c99

clean:
	rm game-of-life

install:
	$(CC) -o /usr/bin/game-of-life main.c -Wall -W -pedantic -std=c99

uninstall:
	rm -f /usr/bin/game-of-life ~/.local/bin/game-of-life


install-locally:
	$(CC) -o ~/.local/bin/game-of-life main.c -Wall -W -pedantic -std=c99


