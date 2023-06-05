#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "Other.h"

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
		used to change text
		*/
		void SetText(std::wstring text);


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

	/*
	Constructor(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonSize, sf::Vector2f buttonPosition, sf::Vector2u spriteCount, float spriteCurrentTime)0
	*/
	class Button{
	public:
		Button();
		Button(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonSize, sf::Vector2f buttonPosition, sf::Vector2u spriteCount, float spriteCurrentTime);
		~Button();


		/*
		checks if button got pressed and updates its sprite count if necessary
		*/
		void Update(float deltaTime, sf::Vector2i mousePosition, Other::ProgrammeStage& stage, Other::ProgrammeStage setStageTo);
		

		/*
		draws button body and its text into buffer frame
		*/
		void Draw(sf::RenderWindow& window);



	private:
		sf::Texture spritesheet;				//contains entire spritesheet
		sf::RectangleShape body;			//contain parameters related to body
		Animation animation;					//handles animation
		sf::Text text;								//contains text to display
		sf::Font font;								//contains font style used by text
	};


	/*
	allows for massive text board to display
	*/
	class TextBoard {
	public:
		~TextBoard();
		TextBoard(sf::Texture* texture, sf::Font* font);
		
		/*
		changes text in textboard
		*/
		void SetText(std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f position, sf::Vector2f size);
		
		/*
		draws textboard into buffer frame
		*/
		void Draw(sf::RenderWindow& window);

	private:
		sf::Texture texture;					//contains image
		sf::RectangleShape body;		//body size
		sf::Font font;							//contains font style
		sf::Text text;							//contains text to display
	};


	class ListItem {

	};
}
