# Conway Game of Life

## made with vt100 terminal escape sequences.

### Not working at Windows OS

This code uses the `termios.h` header that's present on unix-like system only

Feel free to do a pull request adding Windows OS support

### Compile and run

Clone the repository:

```
 git clone https://github.com/ArthurBacci64/game-of-life
 cd game-of-life
 make
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
| QUIT | 'q' |
| PLAY/STOP | 'p' |
| PLAY1STEP | 's' |
| MOVE LEFT | 'h' |
| MOVE RIGHT | 'l' |
| MOVE DOWN | 'j' |
| MOVE UP | 'k' |
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

- [ ] Multiplatform support ( to work with WindowsOS )
- [ ] Movement with arrow keys
- [x] More apparent cursor
- [ ] Do a parameter parser
- [ ] Change the play velocity
- [x] Key to compute one step
- [ ] Map function keys to put common shapes like `block`, `blinker`, `pulsar`, `glider`...
- [ ] Add auto reload on screen size change, trying to preserve the max amount of data in the grid 
- [x] Make a Makefile
