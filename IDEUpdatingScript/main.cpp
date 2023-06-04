/* 
IDEUpdatingScript.cpp : This file contains the 'main' function. Program execution begins and ends there.
To add:
    +-1. way to parse cmd result (should be done in different thread othwerise gui is stuck until winget is done with its stuff)
    +2. sfml library for interface
    -3. way to understand cmd result and decipher which are IDE
    -4. display it to user via sfml
    -5. way for user to choose IDEs to update
    -6. way to select particular IDEs and update them using "winget upgrade >programme id here< --force > Temp\\updatingAttempt.txt"


    chcp 866 rushian cmd encoding
*/

//remove, only testing
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
//remove, only testing



#include <string>
#include <vector>
#include <thread>
#include <SFML/Graphics.hpp>

#include "WindowCommands.h"
#include "Other.h"
#include "Animation.h"
#include "GuiItems.h"



int main(){
    //remove, only testing
    
    _setmode(_fileno(stdin), 0x00020000);   //setting console to use unicode hex characters on input and output 
    _setmode(_fileno(stdout), 0x00020000);  //setting console to use unicode hex characters on input and output 
    //remove, only testing


    /*
    Window Parameters
    */
    static float screenHSize = 1000.0f;         //screen horizontal size
    static float screenVSize = 600.0f;          //screen vertical size
    sf::RenderWindow window(sf::VideoMode(screenHSize, screenVSize), "IDE Updating Script", sf::Style::Close);
    
    /*
    parameters used by everyone
    */
    sf::Event events;                                       // varaible to store events happening during execution
    sf::Vector2i mousePosition;                      //struct to store positions of mouse
    float deltaTime = 0.0f;                              //time most definitely only used for movements in platformer game
    sf::Clock clock;                                        //repeating clock
    //sf::View pov(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(screenHSize, screenVSize));
    sf::Font font;                                              //font style
    font.loadFromFile("Assets\\text\\TimesNewRoman.ttf");


    /*
    setting background
    */
    sf::RectangleShape background;
    sf::Texture backgroundTextureMainMenu;
    backgroundTextureMainMenu.loadFromFile("Assets\\mainBackground.png");
    background.setSize(sf::Vector2f(screenHSize, screenVSize));
    background.setTexture(&backgroundTextureMainMenu);
    background.setPosition(sf::Vector2f(0.0f, 0.0f));

    /*
    setting loading animations
    */
    sf::Texture loadingSpritesheet;
    loadingSpritesheet.loadFromFile("Assets\\animations\\pngFiles\\loadingSpriteSheet1.png");
    GuiItems::LoadingAnimation loadingAnimation(&loadingSpritesheet, &font, L"testing", 12, sf::Vector2f(12.0f, 15.0f), sf::Vector2f(200.0f, 200.0f), sf::Vector2f((screenHSize)/2, (screenVSize) / 2), sf::Vector2u(10, 1), 0.1f);


    enum ProgrammeStage {stage00, stage01, stage02, stage15, stage20};  //predefined programme stages
    ProgrammeStage stage = stage00;                                                             //used for cycling through stages of programmes


    /*
    parameters to store data
    */
    std::wstring chcp[2] = {L"", L"65001" };                                              //stores current encoding codepage and one to use
    std::vector<Other::ProgrammesToUpdate> programmesToUpdate;      //stores name, id, current version and available version for programmes to update


    /*
    threads
    initializer
    */
    std::thread workHorse;                      //used for random minial tasks that I don't want to dedicate threads for
    std::thread chcpThread;                     //used to load and set chcp
    

    /*
    threads finished
    markers
    */
    Other::threadBoolResult workHorseResult = Other::threadBoolResult::notFinished;
    Other::threadBoolResult chcpThreadResult = Other::threadBoolResult::notFinished;


    while (window.isOpen()){
        /*
            updating objects that needed to be updated after every cycle
        */
        window.clear(sf::Color(255, 255, 255));                     //clearing all from old buffer
        deltaTime = clock.restart().asSeconds();                    //resetting time since last cycle

        //limiting fps to 30
        if (1.f / deltaTime > 30.f) {
            float wait = (1.f/30.f - deltaTime) * 1000;
            std::this_thread::sleep_for(std::chrono::milliseconds(long(wait)));
            deltaTime = 1.f / 30.f;
        }
        //if fps less than 30 then increasing delta time to what it should have been
        else if (1.f / deltaTime < 30.f) {
            deltaTime = 1.f / 30.f + deltaTime;
        }

        mousePosition = sf::Mouse::getPosition(window);     //variables that need to be constantly updated

        window.draw(background);

        loadingAnimation.Update(deltaTime);
        loadingAnimation.Draw(window);

        /*
            polling events
        */
        while (window.pollEvent(events)) {
            switch (events.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        //different stages of programme handling
        switch (stage){
        
         /*
        initializer of stage 0
        launching thread to fetch data and going to main stage 0
        */
        case stage00:
            chcpThread = std::thread(WindowCommands::GetSetCHCP, std::ref(chcp[0]), std::string(chcp[1].begin(), chcp[1].end()), std::ref(chcpThreadResult));          //assigning task for chcpThread

            stage = stage01;
            break;

        /*
        main part of stage 0
        waiting for chcp to change
        checking if programme has admin privileges
            if true:
                begin checking netconnection
        */
        case stage01:
            switch (chcpThreadResult){
            case Other::threadBoolResult::notFinished:
                if (workHorseResult == Other::threadBoolResult::True) {
                    workHorseResult = Other::threadBoolResult::notFinished;
                    workHorse = std::thread(WindowCommands::IsConnectedToNet, std::ref(workHorseResult));
                    stage = stage02;
                }
                else if (workHorseResult == Other::threadBoolResult::False) {
                    window.draw(background);

                    /*
                    exit button and asking user to restart programme with admin privledges
                    */
                }
                break;

            case Other::threadBoolResult::True:
                workHorse = std::thread(WindowCommands::HasAdminPrivileges, std::ref(workHorseResult));
                chcpThreadResult = Other::threadBoolResult::notFinished;
                break;

            case Other::False:
                /*
                impossible state
                */
                break;
            }
            break;


        //resolving net connection issues and begin checking if winget has msstore accepted
        case stage02:
            if (workHorseResult == Other::threadBoolResult::True) {
                stage = stage15;
            }
            else {
                /*
                telling user to connect to internet and restart application with admin privileges
                */
            }

            break;

        case stage15:
            break;


        //initializer of stage 2
        case stage20:
            
            break;


        default:
            break;
        }


        window.display();
    }

    return 0;
}
