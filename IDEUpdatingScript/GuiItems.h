#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"

namespace GuiItems {
	/*
	Constructor(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f animationSize, sf::Vector2f animationPosition, sf::Vector2u spriteCount, float spriteLifeTime)
	*/
	class LoadingAnimation {
	public:
		LoadingAnimation(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f animationSize, sf::Vector2f animationPosition, sf::Vector2u spriteCount, float spriteLifeTime);
		~LoadingAnimation();


		/*
		checks if button got pressed and updates its sprite count if necessary
		*/
		void Update(float deltaTime);


		/*
		draws button body and its text
		*/
		void Draw(sf::RenderWindow& window);

	private:
		sf::Texture spritesheet;				//contains entire spritesheet
		sf::RectangleShape body;			//contain parameters related to body
		Animation animation;					//handles animation
		sf::Text text;								//contains text to display
		sf::Font font;								//contains font style used by text
	};


	class Button{
	public:
		Button();
		Button(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonSize, sf::Vector2f buttonPosition, sf::Vector2u spriteCount, float spriteCurrentTime);
		~Button();


		/*
		checks if button got pressed and updates its sprite count if necessary
		*/
		void Update(float deltaTime, sf::Vector2i mousePosition, float& stage, float destination);
		

		/*
		draws button body and its text
		*/
		void Draw(sf::RenderWindow& window);



	private:
		sf::Texture spritesheet;				//contains entire spritesheet
		sf::RectangleShape body;			//contain parameters related to body
		Animation animation;					//handles animation
		sf::Text text;								//contains text to display
		sf::Font font;								//contains font style used by text
	};


	class TextBoard {

	};


	class ListItem {

	};
}
