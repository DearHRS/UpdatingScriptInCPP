#include "GuiItems.h"
#include <iostream>

GuiItems::LoadingAnimation::~LoadingAnimation() {
}


GuiItems::LoadingAnimation::LoadingAnimation(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f animationSize, sf::Vector2f animationPosition, sf::Vector2u spriteCount, float spriteLifeTime) {
	animation = Animation(spritesheet, spriteCount, spriteLifeTime);		//setting Animation class of button

	/*
	setting font style and its text
	*/
	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setCharacterSize(textSize);
	this->text.setOrigin(body.getSize() / 2.0f);
	this->text.setPosition(animationPosition.x - textOffSet.x, animationPosition.y - textOffSet.y);
	this->text.setFillColor(sf::Color(0, 0, 0));

	/*
	setting body's parameters
	*/
	body.setSize(animationSize);
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(spritesheet);
	body.setPosition(animationPosition);
}


void GuiItems::LoadingAnimation::Update(float deltaTime) {
	//updating animation class and selecting sprite to display
	animation.Update(0, deltaTime);
	body.setTextureRect(animation.textureRect);
}


void GuiItems::LoadingAnimation::SetText(std::wstring text, unsigned int textSize, sf::Vector2f textPosition, sf::Vector2f textOffSet) {
	/*
	setting text's position, size and text
	*/
	this->text.setPosition(textPosition.x - textOffSet.x, textPosition.y - textOffSet.y);
	this->text.setCharacterSize(textSize);
	this->text.setString(text);
}


void GuiItems::LoadingAnimation::Draw(sf::RenderWindow& window) {
	/*
	drawing body first so text can be on top of it
	*/
	window.draw(body);
	window.draw(text);
}


/*
Button logic below here
*/
GuiItems::Button::~Button(){
}

//empty constructor made so i could use vector to load all buttons but game crashes if vector used
GuiItems::Button::Button() {

}

GuiItems::Button::Button(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonSize, sf::Vector2f buttonPosition, sf::Vector2u spriteCount, float spriteCurrentTime){
	
	animation = Animation(spritesheet, spriteCount, spriteCurrentTime);		//setting Animation class of button

	/*
	setting font style and its text
	*/
	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setCharacterSize(textSize);
	this->text.setOrigin(body.getSize() / 2.0f);
	this->text.setPosition(buttonPosition.x - textOffSet.x, buttonPosition.y - textOffSet.y);
	this->text.setFillColor(sf::Color(255, 255, 255));

	/*
	setting button's size, texture and position
	*/
	body.setSize(buttonSize);
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(spritesheet);
	body.setPosition(buttonPosition);
}


void GuiItems::Button::Update(float deltaTime, sf::Vector2i mousePosition, Other::ProgrammeStage& stage, Other::ProgrammeStage setStageTo){
	int spriteRow = 0;											//parameter to select which row from spritesheet to use
	text.setFillColor(sf::Color(0, 0, 0));

	//if mouse on top of button
	if ((mousePosition.x > (body.getPosition().x - body.getSize().x / 2.0f) && mousePosition.x < (body.getPosition().x + body.getSize().x / 2.0f)) && ((mousePosition.y > body.getPosition().y - body.getSize().y / 2.0f) && mousePosition.y < (body.getPosition().y + body.getSize().y / 2.0f))) {
		spriteRow = 1;
		text.setFillColor(sf::Color(255, 255, 255));
		
		//if button left clicked
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			stage = setStageTo;
		}
	}

	//updating animation class and selecting sprite to display
	animation.Update(spriteRow, deltaTime);
	body.setTextureRect(animation.textureRect);
}


void GuiItems::Button::Draw(sf::RenderWindow& window){
	/*
	drawing body first so text can be on top of it
	*/
	window.draw(body);
	window.draw(text);
}


GuiItems::TextBoard::~TextBoard(){
}


GuiItems::TextBoard::TextBoard(sf::Texture* texture, sf::Font* font){
	/*
	setting textboards texture
	*/
	body.setTexture(texture);

	/*
	setting text's font and fill colour
	*/
	this->text.setFont(*font);
	this->text.setFillColor(sf::Color(0, 0, 0));
}


void GuiItems::TextBoard::SetText(std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f position, sf::Vector2f size){
	/*
	setting body's position and size
	*/
	body.setSize(size);
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(position);

	/*
	setting text's position, size and text
	*/
	this->text.setPosition(position.x - textOffSet.x, position.y - textOffSet.y);
	this->text.setCharacterSize(textSize);
	this->text.setString(text);
}


void GuiItems::TextBoard::Draw(sf::RenderWindow& window){
	/*
	drawing body first so text can be on top of it
	*/
	window.draw(body);
	window.draw(text);
}
