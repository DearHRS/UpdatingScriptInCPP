#include "Animation.h"

Animation::~Animation() {
}

Animation::Animation() {};

Animation::Animation(sf::Texture* texture, sf::Vector2u spriteCount, float spriteLifeTime) {
	this->spriteCount = spriteCount;
	this->spriteLifeTime = spriteLifeTime;									//how long one sprite lasts on screen
	spriteCurrentTime = 0.0f;														//animation cycle counter
	spriteCurrent.x = 0;																	//current selected sprite

	//getting size of sprite from spritesheet
	textureRect.width = texture->getSize().x / float(spriteCount.x);
	textureRect.height = texture->getSize().y / float(spriteCount.y);
}


void Animation::Update(int spriteRow, float deltaTime) {
	spriteCurrent.y = spriteRow;													//which animation cycle
	spriteCurrentTime += deltaTime;											//increasing Current animation cycle by second by sf::clock

	if (spriteLifeTime <= spriteCurrentTime) {							//if sprites lifetime is exceeded then switch to next
		spriteCurrentTime -= spriteLifeTime;
		spriteCurrent.x++;
		//spriteCurrent.x += 1 + int(spriteCurrentTime / spriteLifeTime);			//this was supposed to skip sprites if cycle was too slow (not working as intended)

		if (spriteCurrent.x >= spriteCount.x) {								//resetting sprites if outside of animation cycle
			spriteCurrent.x = 0;
		}

		/*
		fixating texture vertically
		*/
		textureRect.top = spriteCurrent.y * textureRect.height;

		/*
		fixating texture horizontally
		*/
		textureRect.left = spriteCurrent.x * textureRect.width;
		textureRect.width = abs(textureRect.width);
	}
}
