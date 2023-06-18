#include "GuiItems.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//loading animation thing here
GuiItems::LoadingAnimation::~LoadingAnimation() {
}


GuiItems::LoadingAnimation::LoadingAnimation(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f animationSize, sf::Vector2f animationPosition, sf::Vector2u spriteCount, float spriteLifeTime) {
	animation = Animation(spritesheet, spriteCount, spriteLifeTime);		//setting Animation class of button

	/*
	setting body's parameters
	*/
	body.setSize(animationSize);
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(spritesheet);
	body.setPosition(animationPosition);

	/*
	setting font style and its text
	*/
	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setCharacterSize(textSize);
	this->text.setPosition(animationPosition.x - textOffSet.x, animationPosition.y - textOffSet.y);
	this->text.setFillColor(sf::Color(0, 0, 0));
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


void GuiItems::LoadingAnimation::ResizeAndSetPosition(sf::Vector2f animationSize, sf::Vector2f animationPosition){
	body.setSize(animationSize);
	body.setOrigin(body.getSize() / 2.0f);
	body.setPosition(animationPosition);
}


void GuiItems::LoadingAnimation::Draw(sf::RenderWindow& window) {
	/*
	drawing body first so text can be on top of it
	*/
	window.draw(body);
	window.draw(text);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	isDisabled = false;
	/*
	setting button's size, texture and position
	*/
	body.setSize(buttonSize);
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(spritesheet);
	body.setPosition(buttonPosition);

	/*
	setting font style and its text
	*/
	this->text.setFont(*font);
	this->text.setString(text);
	this->text.setCharacterSize(textSize);
	this->text.setPosition(buttonPosition.x - textOffSet.x, buttonPosition.y - textOffSet.y);
	this->text.setFillColor(sf::Color(255, 255, 255));
}


void GuiItems::Button::ResetPositionAndText(std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonPosition){
	/*
	setting button's position
	*/
	body.setPosition(buttonPosition);

	/*
	setting font style and its text
	*/
	this->text.setString(text);
	this->text.setCharacterSize(textSize);
	this->text.setPosition(buttonPosition.x - textOffSet.x, buttonPosition.y - textOffSet.y);
}


void GuiItems::Button::Update(float deltaTime, sf::Event& events, sf::Vector2i mousePosition, Other::ProgrammeStage& stage, Other::ProgrammeStage setStageTo){
	int spriteRow = 1;											//parameter to select which row from spritesheet to use
	text.setFillColor(sf::Color(0, 0, 0));

	//if button is disabled
	if (isDisabled) {
		spriteRow = 0;
		text.setFillColor(sf::Color(193, 193, 193));
	}
	//if mouse on top of button
	else if ((mousePosition.x > (body.getPosition().x - body.getSize().x / 2.0f) && mousePosition.x < (body.getPosition().x + body.getSize().x / 2.0f)) && ((mousePosition.y > body.getPosition().y - body.getSize().y / 2.0f) && mousePosition.y < (body.getPosition().y + body.getSize().y / 2.0f))) {
		spriteRow = 2;
		text.setFillColor(sf::Color(255, 255, 255));
		
		//if button left clicked
		if (events.mouseButton.button == sf::Mouse::Left) {
			/*
			animation of button getting pressed maybe
			*/
			//if clicked button is released
			if (events.type == sf::Event::MouseButtonReleased) {
				stage = setStageTo;
			}
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


void GuiItems::Button::SetEnable(bool isDisabled){
	this->isDisabled = isDisabled;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//list stuff here
GuiItems::ListItem::~ListItem() {
}


GuiItems::ListItem::ListItem() {
}


GuiItems::ListItem::ListItem(sf::Texture* spritesheet, sf::Font* font, unsigned int count, Other::ProgrammesToUpdate& programme, sf::Vector2f listPosition, sf::Vector2u spriteCount, float spriteCurrentTime) {
	animation = Animation(spritesheet, spriteCount, spriteCurrentTime);		//setting Animation class of button

	/*
	setting list's size, texture and position
	*/
	body.setSize(sf::Vector2f(950.0f, 200.0f));
	body.setOrigin(body.getSize() / 2.0f);
	body.setTexture(spritesheet);
	body.setPosition(listPosition);

	spriteRow = 0;
	isSelected = false;
	
	//setting programme struct
	this->programme = programme;

	/*
	setting font style and its text
	*/
	text.setFont(*font);
	text.setString(std::to_wstring(count) + L".  " + programme.name + L"\n\n" 
		+ L"       текущая версия: " + programme.fromVersion + L"\n"
		+ L"    доступная версия: " + programme.toVersion);
	text.setCharacterSize(30);
	text.setOrigin(body.getSize() / 2.0f);
	text.setFillColor(sf::Color(0, 0, 0));

	textUpdateResult.setCharacterSize(18);
	textUpdateResult.setString(L"");
	textUpdateResult.setFont(*font);
	textUpdateResult.setOrigin(body.getSize() / 2.0f);
	textUpdateResult.setFillColor(sf::Color(0, 0, 0));
}


void GuiItems::ListItem::Update(float deltaTime, bool allowInteractivity, sf::Event& events, sf::Vector2i mousePosition, float moveBy) {
	//moving body and text
	body.setPosition(body.getPosition().x, body.getPosition().y + moveBy);
	text.setPosition(body.getPosition().x + 25.0, body.getPosition().y + 25.0);
	textUpdateResult.setPosition(body.getPosition().x + 420.0, body.getPosition().y + 75.0);
	
	//setting parameter to select which row from spritesheet to use
	if (!isSelected) {
		spriteRow = 0;
	}
	else {
		spriteRow = 1;
	}

	//if mouse on top of list
	if (!(mousePosition.y < 145.0f) && (mousePosition.x > (body.getPosition().x - body.getSize().x / 2.0f) && mousePosition.x < (body.getPosition().x + body.getSize().x / 2.0f)) && ((mousePosition.y > body.getPosition().y - body.getSize().y / 2.0f) && mousePosition.y < (body.getPosition().y + body.getSize().y / 2.0f))) {
		//if button left clicked
		if (events.mouseButton.button == sf::Mouse::Left && allowInteractivity) {
			/*
			animation of button getting pressed maybe
			*/
			//if clicked button is released
			if (events.type == sf::Event::MouseButtonReleased) {
				isSelected = not isSelected;
			}
		}
	}

	//updating animation class and selecting sprite to display
	animation.Update(spriteRow, deltaTime);
	body.setTextureRect(animation.textureRect);
}


void GuiItems::ListItem::setTextUpdateResult(std::wstring text){
	textUpdateResult.setString(text);
}


void GuiItems::ListItem::Draw(sf::RenderWindow& window) {
	/*
	drawing body first so text can be on top of it
	*/
	window.draw(body);
	window.draw(text);
	window.draw(textUpdateResult);
}


bool GuiItems::ListItem::IsSelected(){
	return isSelected;
}


float GuiItems::ListItem::getYPosition(){
	return body.getPosition().y;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//textboard stuff here
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