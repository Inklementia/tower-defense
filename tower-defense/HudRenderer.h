#pragma once
#include "GameConfig.h"
#include "SDL.h"
#include <string>

struct UiColor {
  int r, g, b, a;

  constexpr UiColor(int setR, int setG, int setB, int setA)
      : r(setR), g(setG), b(setB), a(setA) {}
};

class HudRenderer {
public:
  static bool init();
  static void shutdown();

  static void drawText(SDL_Renderer *renderer, int x, int y,
                       const std::string &text, UiColor color,
                       int fontSize = 16);
  static void drawTextCentered(SDL_Renderer *renderer, int centerX, int y,
                               const std::string &text, UiColor color,
                               int fontSize = 16);
  static int measureTextWidth(const std::string &text, int fontSize = 16);
  static void drawPanel(SDL_Renderer *renderer, int x, int y, int w, int h,
                        UiColor color);
  static void drawHudBox(SDL_Renderer *renderer, int x, int y, int w, int h,
                         UiColor border);
  static bool isSaveButtonAt(int x, int y);
  static void drawCheckerboard(SDL_Renderer *renderer, int width, int height,
                               int tileSize, UiColor light, UiColor dark);
};
