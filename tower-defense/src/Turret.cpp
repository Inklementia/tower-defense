#include "Turret.h"
#include "Unit.h"

const float Turret::speedAngular = MathUtils::angleDegToRad(180.0f), Turret::weaponRange = 5.0f;

Turret::Turret(SDL_Renderer* renderer, Vector2D setPos) :
	pos(setPos), angle(0.0f) {
	textureMain = TextureLoader::loadTexture(renderer, "Turret.bmp");
	textureShadow = TextureLoader::loadTexture(renderer, "Turret Shadow.bmp");
}

void Turret::update(float dT, std::vector<std::shared_ptr<Unit>>& listUnits) {
	//Check if a target has been found but is no longer alive or is out of weapon range.
	if (auto unitTargetSP = unitTarget.lock()) {
		if (unitTargetSP->getIsAlive() == false ||
			(unitTargetSP->getPos() - pos).magnitude() > weaponRange) {
			//Then reset it.
			unitTarget.reset();
		}
	}

	//Find a target if needed.
	if (unitTarget.expired())
		unitTarget = findEnemyUnit(listUnits);

	if (auto unitTargetSP = unitTarget.lock()) {
		//Determine the direction normal to the target.
		Vector2D directionNormalTarget = (unitTargetSP->getPos() - pos).normalize();

		//Determine the angle to the target.
		float angleToTarget = directionNormalTarget.angleBetween(Vector2D(angle));

		//Update the angle as required.
		if (abs(angleToTarget) > 0.0f) {
			//Determine the angle to move this frame.
			float angleMove = -copysign(speedAngular * dT, angleToTarget);
			if (abs(angleMove) > abs(angleToTarget)) {
				//It will point directly at it's target this frame.
				angle = directionNormalTarget.angle();
			}
			else {
				//It won't reach it's target this frame.
				angle += angleMove;
			}
		}
	}
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

std::weak_ptr<Unit> Turret::findEnemyUnit(std::vector<std::shared_ptr<Unit>>& listUnits) {
	//Find the closest enemy unit to this turret.
	std::weak_ptr<Unit> closestUnit;
	float distanceClosest = 0.0f;

	//Loop through the entire list of units.
	for (auto& unitSelected : listUnits) {
		//Ensure that the selected unit exists.
		if (unitSelected != nullptr) {
			//Calculate the distance to the selected unit.
			float distanceCurrent = (pos - unitSelected->getPos()).magnitude();
			//Check if the unit is within range, and no closest unit has been found or the 
			//selected unit is closer than the previous closest unit.
			if (distanceCurrent <= weaponRange &&
				(closestUnit.expired() || distanceCurrent < distanceClosest)) {
				//Then set the closest unit to the selected unit.
				closestUnit = unitSelected;
				distanceClosest = distanceCurrent;
			}
		}
	}

	return closestUnit;
}