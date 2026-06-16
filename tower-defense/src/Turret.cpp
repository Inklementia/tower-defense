#include "Turret.h"

const float Turret::speedAngular = MathUtils::angleDegToRad(180.0f);

Turret::Turret(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), angle(0.0f) {
	textureMain = TextureLoader::loadTexture(renderer, "Turret.bmp");
	textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
}

void Turret::update(float dT) {
	angle += speedAngular * dT;
	if (angle > MathUtils::angleDegToRad(360.0f))
		angle -= MathUtils::angleDegToRad(360.0f);
}

void Turret::draw(SDL_Renderer* renderer, int tileSize) {
	drawTextureWithOffset(renderer, textureShadow, 5, tileSize);
	drawTextureWithOffset(renderer, textureMain, 0, tileSize);
}

void Turret::drawTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* textureSelected,
	int offset, int tileSize) {
	if (renderer != nullptr && textureSelected != nullptr) {
		//Draw the image at the turret's position and angle and offset.
		int w, h;
		SDL_QueryTexture(textureSelected, NULL, NULL, &w, &h);
		SDL_Rect rect = {
			(int)(pos.x * tileSize) - w / 2 + offset,
			(int)(pos.y * tileSize) - h / 2 + offset,
			w,
			h };
		SDL_RenderCopyEx(renderer, textureSelected, NULL, &rect,
			MathUtils::angleRadToDeg(angle), NULL, SDL_FLIP_NONE);
	}
}

bool Turret::checkIfOnTile(int x, int y) {
	return ((int)pos.x == x && (int)pos.y == y);
}