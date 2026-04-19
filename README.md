# 🚀 Multiplayer Physics Arena (C++ + SDL2)

A 2D physics-based multiplayer arena game built from scratch in **C++ using SDL2**, featuring dynamic gravity switching and recoil-based movement.

---

## Gameplay

Players fight inside a bounded arena with platforms while the game randomly switches between **normal gravity** and **zero gravity** modes.

### Core Mechanics

* **Dynamic Gravity System**

  * Normal Gravity → Players fall and move normally
  * Zero Gravity → No direct movement, players must use recoil to move
  * Random switching creates unpredictable gameplay

* **Recoil-Based Movement**

  * Shooting applies force in the opposite direction
  * Used as the primary movement mechanic in zero gravity

* **Combat System**

  * Infinite bullets with fire rate limit
  * Bullet collision and damage system
  * Health-based elimination

* **Platform Arena**

  * Static platforms for positioning
  * Boundary constraints to keep players inside arena

---

## Tech Stack

* **C++**
* **SDL2**
* **SDL2_image**
* Custom game loop and physics (no external engine)

---

## Features

* ✅ Delta-time based movement
* ✅ Custom physics system (velocity, gravity, air drag)
* ✅ Axis-separated collision detection (AABB)
* ✅ Gravity + Zero Gravity modes
* ✅ Recoil-based movement system
* ✅ Shooting system with rate limiting
* ✅ Two-player local multiplayer
* ✅ Health system
* ✅ Texture rendering

---

## Controls

### Player 1

* Move: `W A S D`
* Shoot: `Space`

### Player 2

* Move: `Arrow Keys`
* Shoot: `r`

---

## ▶️ Build & Run

### Requirements

* SDL2
* SDL2_image
* g++

### Install (Windows using MSYS2)

Install SDL2 and SDL2_image using MSYS2 packages.

### Compile

```bash
g++ src/main.cpp -lSDL2 -lSDL2_image -o build/game.exe
```

### Run

```bash
./build/game.exe
```

---

##  Project Structure

```
multiplayer-arena/
├── src/
├── include/
├── assets/
│   └── images/
├── build/
├── README.md
├── Makefile
└── .gitignore
```

---

## 📸 Screenshot

*Gravity mode*

![alt text](sky.png.png)

*Zero Gravity mode*

![alt text](sapce_.png.png)
---

## Future Improvements

*  Online multiplayer (networking)
*  360° aiming system
*  Moving platforms
*  Sound effects
*  Score system and match timer
*  Improved animations

---

##  What I Learned

* Game loop design
* Physics simulation (gravity, velocity, damping)
* Collision detection & resolution
* Real-time input handling
* Rendering using SDL2
* Structuring C++ projects

---

## Author

P Sai Maneesh

---

## Note

This project was built to strengthen **C++ system-level and game development skills** without using heavy game engines.

---
