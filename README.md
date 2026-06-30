# ⏳ Stuck in a Time Loop

A unique puzzle game built with **C++** and **Raylib**.  
Navigate through 5 levels, solve puzzles, and escape the time loop!

---

## 🎮 Features

- **Time loop mechanic** — press Space to create a clone of yourself
- **5 unique levels** — each with different puzzles and challenges
- **Clone management** — your clones persist and can interact with the environment
- **Puzzle solving** — press buttons, unlock doors, and find your way out
- **Randomized elements** — doors may lead to different outcomes
- **Countdown timers** — some clones have limited lifetimes
- **Rich visual style** — custom sprites for characters, doors, buttons, and walls

---

## 🛠️ Technologies Used

- **C++** — core logic
- **Raylib** — 2D rendering, input handling, audio

---

## 🚀 How to Build

### Requirements

Install Raylib:

bash
# Ubuntu / Debian
sudo apt install libraylib-dev

# Or build from source
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake ..
make
sudo make install

### Compile

g++ -o time_loop time_loop.cpp \
    -lraylib -lm -lpthread -ldl -lrt -lX11

### Run

./time_loop


🎮 Controls
Key	Action
Arrow Keys	Move the player
Space	Create a time loop clone
Close Window	Exit the game


🧠 How It Works

1.    Player — moves with arrow keys

2.    Time loop — pressing Space creates a clone at a random position

3.    Clones — replicate your movements and can interact with buttons

4.    Levels — each level has a locked door and a puzzle to solve

5.    Buttons — press buttons to unlock doors

6.    Level 5 — randomized door outcomes, including victory, setbacks, and resets
