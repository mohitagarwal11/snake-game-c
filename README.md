# 🐍 Snake Game in C (Terminal-Based)

A simple terminal-based Snake game written in C, playable on both **Windows** and **Linux/macOS**.

Most of the things i learnt in this project are based on and around the terminal manipulation for games in linux and windows. While input handling for both at the same time
is confusing, once done it is much simplier to just copy and paste and make a snippet out of it to nvr type that shit again!


## To Play

- Use **W, A, S, D** keys to move the snake.
- Eat food (`00`) to grow.
- Avoid hitting walls (`##`) or yourself (`@@`).
- Press **Q** to quit the game.


## Compile & Run

**Linux/macOS:**
```
gcc snake.c -o snake
./snake
```

**Windows (with MinGW):**
```
gcc snake.c -o snake.exe
snake.exe
```

> On Linux/macOS, the terminal switches to raw input mode for real-time controls.


## Features

- Real-time keyboard input
- Multiple food items on screen
- Self and wall collision detection
- Cross-platform support(linux/win)
- Simple ASCII visuals

