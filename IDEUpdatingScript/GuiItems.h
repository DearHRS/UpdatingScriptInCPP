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
		void SetText(std::wstring text, unsigned int textSize, sf::Vector2f textPosition, sf::Vector2f textOffSet);


		/*
		resize and reposition
		*/
		void ResizeAndSetPosition(sf::Vector2f animationSize, sf::Vector2f animationPosition);

		/*
		draws button body and its text
		*/
		void Draw(sf::RenderWindow& window);

	private:
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
		~Button();
		Button();
		Button(sf::Texture* spritesheet, sf::Font* font, std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonSize, sf::Vector2f buttonPosition, sf::Vector2u spriteCount, float spriteCurrentTime);


		/*
		changes position and text of button
		*/
		void ResetPositionAndText(std::wstring text, unsigned int textSize, sf::Vector2f textOffSet, sf::Vector2f buttonPosition);


		/*
		checks if button got pressed and updates its sprite count if necessary
		*/
		void Update(float deltaTime, sf::Event& events, sf::Vector2i mousePosition, Other::ProgrammeStage& stage, Other::ProgrammeStage setStageTo);
		

		/*
		draws button body and its text into buffer frame
		*/
		void Draw(sf::RenderWindow& window);


		/*
		sets buttons to enable / disabled
		*/
		void SetEnable(bool isDisabled);


	private:
		bool isDisabled;							//stores if button is disabled

		sf::RectangleShape body;			//contain parameters related to body
		Animation animation;					//handles animation
		sf::Text text;								//contains text to display
		sf::Font font;								//contains font style used by text
	};


	class ListItem {
	public:
		~ListItem();
		ListItem();
		ListItem(sf::Texture* spritesheet, sf::Font* font, unsigned int count, Other::ProgrammesToUpdate& programme, sf::Vector2f listPosition, sf::Vector2u spriteCount, float spriteCurrentTime);

		Other::ProgrammesToUpdate programme;			//contains data of programme

		/*
		checks if list got pressed or cursor is howering on top of it, updates its sprite count if necessary
		and moves list depending on cursor
		*/
		void Update(float deltaTime, bool allowInteractivity, sf::Event& events, sf::Vector2i mousePosition, float moveBy);

		/*
		sets text for get update result
		*/
		void setTextUpdateResult(std::wstring text);

		/*
		draws list body and its text into buffer frame
		*/
		void Draw(sf::RenderWindow& window);

		/*
		return if selected
		*/
		bool IsSelected();

		/*
		returns y position of list
		*/
		float getYPosition();


	private:
		bool isSelected;												//used to hold state of being selected or not

		sf::Texture spritesheet;									//contains entire spritesheet
		sf::RectangleShape body;								//contain parameters related to body
		Animation animation;										//handles animation
		int spriteRow;													//handles which row of spritesheet is played

		sf::Text text;													//contains text to display
		sf::Text textUpdateResult;								//contains update result to display
		sf::Font font;													//contains font style used by text
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
		sf::RectangleShape body;		//body size
		sf::Font font;							//contains font style
		sf::Text text;							//contains text to display
	};
}