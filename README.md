## Game Description

A Jetpack Joyride like / flappy bird like 2D game wrtiien in OpenGL.  Navigate through a hazardous laboratory, collect coins, and avoid deadly zappers while trying to achieve the highest score possible!


## Installation and Running

### Prerequisites
- OpenGL
- CMake
- C++ compiler

### Building the Game

1. Clone the repository:
```bash
git clone https://github.com/ishwarbb/JetpackJoyride.git
cd JetpackJoyride
```

2. Create and enter build directory:
```bash
mkdir build
cd build
```

3. Generate build files:
```bash
cmake ..
```

4. Compile the game:
```bash
make
```

5. Run the game:
```bash
./app
```


## Features

### Core Gameplay
- Three progressively challenging levels
- Physics-based movement system with gravity and jetpack thrust
- Randomly generated obstacles (zappers) and collectible coins
- Lives system with 9 lives
- Score tracking based on coins collected
- Distance-based level progression (1000 units per level)

### Visual Elements
- Glowing effects for zappers and active jetpack
- Visual cues for level progression (laboratory environment)
- On-screen HUD displaying:
  - Current level
  - Distance traveled
  - Coin count/score
  - Lives remaining

### Controls
- UP ARROW: Activate jetpack to move upward
- Natural falling motion when key is released
- Character runs automatically when on ground

### Game Rules
- Complete 1000 distance units to advance to the next level
- Collect coins to increase your score
- Avoid zappers - each collision costs one life
- Starting lives: 9
- Respawn position: Top-left of screen
- Game ends when all lives are depleted or upon completing all levels


## Credits
- Core game engine components adapted from learnopengl tutorials
- Original game concept by Halfbrick Studios

## Known Issues
- Be cautious when respawning, as immediate zapper contact will result in consecutive life loss

## Development Notes
- Implements core Jetpack Joyride mechanics while maintaining playability
- Features physics-based movement system
- Includes dynamic obstacle generation

Have fun!
