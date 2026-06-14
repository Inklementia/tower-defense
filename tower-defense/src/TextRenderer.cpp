#include "TextRenderer.h"
#include <SDL_ttf.h>
#include <cstdio>

static TTF_Font* font = nullptr;

bool TextRenderer::init() {
	if (TTF_Init() != 0)
		return false;

	font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 16);
	return font != nullptr;
}

void TextRenderer::quit() {
	if (font != nullptr) {
		TTF_CloseFont(font);
		font = nullptr;
	}
	TTF_Quit();
}

void TextRenderer::drawNumber(SDL_Renderer* renderer, int number, int tileX, int tileY, int tileSize) {
	if (font == nullptr || renderer == nullptr)
		return;

	char text[8];
	snprintf(text, sizeof(text), "%d", number);

	SDL_Color color = { 20, 20, 20, 255 };
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
	if (surface == nullptr)
		return;

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_Rect rect = {
		tileX + (tileSize - surface->w) / 2,
		tileY + (tileSize - surface->h) / 2,
		surface->w,
		surface->h
	};
	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	SDL_DestroyTexture(texture);
}
