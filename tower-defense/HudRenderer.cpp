#include "HudRenderer.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#include <fstream>

namespace {
constexpr const char *FONT_PATH = "Data/Fonts/Crooker.otf";
unsigned char *fontBuffer = nullptr;
stbtt_fontinfo fontInfo{};
bool fontLoaded = false;

float getScale(int fontSize) {
  return stbtt_ScaleForPixelHeight(&fontInfo, (float)fontSize);
}

int measureGlyphRunWidth(const std::string &text, int fontSize) {
  if (!fontLoaded || text.empty())
    return 0;

  const float scale = getScale(fontSize);
  int width = 0;
  for (unsigned char c : text) {
    int advance = 0, lsb = 0;
    stbtt_GetCodepointHMetrics(&fontInfo, c, &advance, &lsb);
    width += (int)(advance * scale);
  }
  return width;
}

void drawStringBitmap(SDL_Renderer *renderer, int x, int y,
                      const std::string &text, UiColor color, int fontSize) {
  if (!fontLoaded || text.empty())
    return;

  const float scale = getScale(fontSize);
  int ascent = 0, descent = 0, lineGap = 0;
  stbtt_GetFontVMetrics(&fontInfo, &ascent, &descent, &lineGap);
  const int baseline = y + (int)(ascent * scale);
  int xPos = x;

  for (unsigned char c : text) {
    int advance = 0, lsb = 0;
    stbtt_GetCodepointHMetrics(&fontInfo, c, &advance, &lsb);

    int gw = 0, gh = 0, xoff = 0, yoff = 0;
    unsigned char *bitmap = stbtt_GetCodepointBitmap(&fontInfo, scale, scale, c,
                                                     &gw, &gh, &xoff, &yoff);
    if (bitmap != nullptr && gw > 0 && gh > 0) {
      SDL_Surface *surface =
          SDL_CreateRGBSurfaceWithFormat(0, gw, gh, 32, SDL_PIXELFORMAT_RGBA32);
      if (surface != nullptr) {
        Uint32 *pixels = (Uint32 *)surface->pixels;
        for (int row = 0; row < gh; row++) {
          for (int col = 0; col < gw; col++) {
            const unsigned char alpha = bitmap[(size_t)row * gw + col];
            pixels[(size_t)row * gw + col] =
                SDL_MapRGBA(surface->format, color.r, color.g, color.b, alpha);
          }
        }

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (texture != nullptr) {
          SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
          const SDL_Rect dst = {xPos + xoff, baseline + yoff, gw, gh};
          SDL_RenderCopy(renderer, texture, NULL, &dst);
          SDL_DestroyTexture(texture);
        }
      }
      stbtt_FreeBitmap(bitmap, nullptr);
    }

    xPos += (int)(advance * scale);
  }
}
} // namespace

bool HudRenderer::init() {
  std::ifstream file(FONT_PATH, std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return false;

  const std::streamsize size = file.tellg();
  if (size <= 0)
    return false;

  file.seekg(0, std::ios::beg);
  fontBuffer = new unsigned char[(size_t)size];
  if (!file.read((char *)fontBuffer, size)) {
    delete[] fontBuffer;
    fontBuffer = nullptr;
    return false;
  }

  if (!stbtt_InitFont(&fontInfo, fontBuffer,
                      stbtt_GetFontOffsetForIndex(fontBuffer, 0))) {
    delete[] fontBuffer;
    fontBuffer = nullptr;
    return false;
  }

  fontLoaded = true;
  return true;
}

void HudRenderer::shutdown() {
  delete[] fontBuffer;
  fontBuffer = nullptr;
  fontLoaded = false;
}

int HudRenderer::measureTextWidth(const std::string &text, int fontSize) {
  return measureGlyphRunWidth(text, fontSize);
}

void HudRenderer::drawText(SDL_Renderer *renderer, int x, int y,
                           const std::string &text, UiColor color,
                           int fontSize) {
  drawStringBitmap(renderer, x, y, text, color, fontSize);
}

void HudRenderer::drawTextCentered(SDL_Renderer *renderer, int centerX, int y,
                                   const std::string &text, UiColor color,
                                   int fontSize) {
  drawStringBitmap(renderer, centerX - measureGlyphRunWidth(text, fontSize) / 2,
                   y, text, color, fontSize);
}

void HudRenderer::drawPanel(SDL_Renderer *renderer, int x, int y, int w, int h,
                            UiColor color) {
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  const SDL_Rect rect = {x, y, w, h};
  SDL_RenderFillRect(renderer, &rect);
}

void HudRenderer::drawCheckerboard(SDL_Renderer *renderer, int width,
                                   int height, int tileSize, UiColor light,
                                   UiColor dark) {
  for (int ty = 0; ty < (height + tileSize - 1) / tileSize; ty++) {
    for (int tx = 0; tx < (width + tileSize - 1) / tileSize; tx++) {
      const UiColor &c = ((tx + ty) % 2 == 0) ? light : dark;
      SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
      const SDL_Rect rect = {tx * tileSize, ty * tileSize, tileSize, tileSize};
      SDL_RenderFillRect(renderer, &rect);
    }
  }
}
