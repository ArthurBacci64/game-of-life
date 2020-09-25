# Conway Game of Life

## made with NCurses.

<a href="https://asciinema.org/a/361528" target="_blank"><img src="https://asciinema.org/a/361528.svg" /></a>

### Compile and run

Clone the repository:

```
$ git clone https://github.com/ArthurBacci64/game-of-life
$ cd game-of-life
$ make
```

#### To run

```
$ ./game-of-life
```

### Install

To install globally ( to all users ), run this as ROOT:

```
# make install
```

If you want to install only to your user:

```
$ make install-locally
```

To uninstall:

```
# make uninstall
```

( if you only installed locally you can run without sudo )


### How to use it

| FUNCTION | KEY(S) |
| ----------- | ----------- |
| QUIT | `CTRL + 'c'` |
| PLAY/STOP | 'p' |
| PLAY1STEP | 's' |
| MOVE LEFT | 'h', `CTRL+'B'`, `KEY_LEFT` |
| MOVE RIGHT | 'l', `CTRL+'F'`, `KEY_RIGHT` |
| MOVE DOWN | 'j', `CTRL+'N'`, `KEY_DOWN` |
| MOVE UP | 'k', `CTRL+'P'`, `KEY_UP` |
| CHANGE BLOCK STATUS | `ESPACE` |
| CLEAR GRID | 'c' |


### Contribute

If you like to contribute, just do a pull request with the following steps:

1. Fork this repository into you Github account
2. `git clone https://github.com/[ Your github username ]/game-of-life && cd game-of-life`
3. Do the changes
4. `git add .`
5. `git commit`
6. `git push`
7. In Github you can see on your forked repository a option to do a pull request

### To-do list

- [X] Movement with arrow keys
- [x] More apparent cursor
- [ ] Do a parameter parser
- [ ] Change the play velocity
- [x] Key to compute one step
- [ ] Add auto reload on screen size change, trying to preserve the max amount of data in the grid 
- [x] Make a Makefile
- [X] Start using NCurses
- [ ] Make a panel with some status like number of live cells, number of death cells, canvas size...
- [ ] Make a menu on press `F1` to insert common shapes like block, blinker, pulsar, glider...
- [ ] Colorize it!
- [ ] Mouse compatibility
