# tower-defense

Tower Defense game in C++ and SDL2.

## Project structure

```
tower-defense/
├── *.cpp, *.h          # game source
├── Data/
│   ├── Assets/         # textures (.bmp)
│   └── Sounds/         # audio (.ogg)
├── include/
│   ├── SDL2/           # SDL2 headers
│   └── SDL2_mixer/     # SDL_mixer headers
├── lib/                # SDL2.lib, SDL2main.lib, SDL2_mixer.lib
├── SDL2.dll            # runtime dependencies
├── SDL2_mixer.dll
├── libogg-0.dll
└── tower-defense.vcxproj
```

SDL libraries are bundled locally in `include/` and `lib/`. After build, DLLs and the `Data/` folder are copied to the output directory.

## Build

Open `tower-defense.sln` in Visual Studio, select **x64** (Debug or Release), build and run.
