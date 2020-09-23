# Conway Game of Life

## made with vt100 terminal escape sequences.

### Compile and run

Clone the repository:

```
 git clone https://github.com/ArthurBacci64/game-of-life
 cd game-of-life
```

Compile `main.c` file.

#### With gcc

```
$ gcc main.c -o game-of-life
```

#### To run

```
$ ./game-of-life
```

### Install

To install you will need to compile before (see `compile and run`)

```
# cp ./game-of-life /usr/bin/game-of-life
# chmod 555 /usr/bin/game-of-life
```

### How to use it

You have your cursor represented by `__`. Press `E` to toggle the selected cell status. `WASD` to move the cursor, ` ` ( `SPACE` ) to Play/Stop and `Q` to quit.

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

- Multiplatform support ( to work with WindowsOS )
- Movement with arrow keys
- More apparent cursor
- Do a parameter parser
- Change the play velocity
- Key to compute one step
- Map function keys to put common shapes like `block`, `blinker`, `pulsar`, `glider`...