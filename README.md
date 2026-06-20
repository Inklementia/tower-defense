# Tower Defense

A simple Tower Defense game built from scratch in **C++** using **SDL2** and **SDL2_mixer**.

The project was created while following the YouTube playlist:

**Make a Tower Base Defense Game C++ and SDL2 from Scratch**  
https://www.youtube.com/playlist?list=PL1v1Rwp_7pRzmjGqrvj7umNz84EKfKmi0

## Overview

This project demonstrates the basic structure of a 2D tower defense game using SDL2.  
It includes rendering, game loop logic, basic asset loading, sound support, and gameplay systems written in C++.

The game uses local SDL2 dependencies, so the required headers, libraries, and runtime DLL files are included directly in the repository.

## Features

- C++ game architecture
- SDL2 rendering
- SDL2_mixer audio support
- Basic tower defense gameplay
- Creative sandbox mode
- Custom level creation
- Save sandbox layout
- Texture loading from `.bmp` assets
- Sound effects from `.ogg` files

<img width="600" alt="Screenshot 2026-06-20 115455" src="https://github.com/user-attachments/assets/61caa90c-02df-41ab-ad6e-c9a730828813" />
<img width="600" alt="Screenshot 2026-06-20 115807" src="https://github.com/user-attachments/assets/7efb801c-49f6-49f0-bc22-46c256f4cc27" />
<img width="600" alt="Screenshot 2026-06-20 115752" src="https://github.com/user-attachments/assets/7dbc59a5-85c9-4670-843f-9f75567bdbcd" />


## Project Structure

```text
tower-defense/
├── *.cpp, *.h          # Game source files
├── Data/
│   ├── Assets/         # Texture assets (.bmp)
│   └── Sounds/         # Audio files (.ogg)
├── include/
│   ├── SDL2/           # SDL2 header files
│   └── SDL2_mixer/     # SDL2_mixer header files
├── lib/                # SDL2.lib, SDL2main.lib, SDL2_mixer.lib
├── SDL2.dll            # SDL2 runtime dependency
├── SDL2_mixer.dll      # SDL2_mixer runtime dependency
├── libogg-0.dll        # OGG audio runtime dependency
├── tower-defense.sln   # Visual Studio solution
└── tower-defense.vcxproj
