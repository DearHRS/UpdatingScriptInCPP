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
//remove, only testing



#include <string>
#include <vector>
#include <thread>
#include <chrono>
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
    backgroundTextureMainMenu.loadFromFile("Assets\\animations\\pngFiles\\mainBackground.png");
    background.setSize(sf::Vector2f(screenHSize, screenVSize));
    background.setTexture(&backgroundTextureMainMenu);
    background.setPosition(sf::Vector2f(0.0f, 0.0f));

    /*
    setting loading animations
    */
    sf::Texture loadingSpritesheet;
    loadingSpritesheet.loadFromFile("Assets\\animations\\pngFiles\\loadingSpriteSheet1.png");
    GuiItems::LoadingAnimation loadingAnimation(&loadingSpritesheet, &font, L"testing", 12, sf::Vector2f(12.0f, 15.0f), sf::Vector2f(200.0f, 200.0f), sf::Vector2f((screenHSize)/2, (screenVSize) / 2), sf::Vector2u(10, 1), 0.1f);


    /*
    setting buttons
    */
    sf::Texture buttonSpriteSheet;
    buttonSpriteSheet.loadFromFile("Assets\\animations\\pngFiles\\button.png");
    GuiItems::Button closeButton(&buttonSpriteSheet, &font, L"закрыть", 24, sf::Vector2f(40.0f, 15.0f), sf::Vector2f(200.0f, 80.0f), sf::Vector2f((screenHSize) / 2, (screenVSize) * 4 / 5), sf::Vector2u(1, 2), 0.1f);


    /*
    setting textboard
    */
    sf::Texture textBoardTexture;
    textBoardTexture.loadFromFile("Assets\\animations\\pngFiles\\textBoard.png");
    GuiItems::TextBoard textBoard(&textBoardTexture, &font);


    /*
    predefined programme stages
    used for cycling through stages of programmes
    */
    Other::ProgrammeStage stage = Other::ProgrammeStage::stageGetSetCHCP; 


    /*
    parameters to store data
    */
    std::wstring chcp[2] = {L"", L"65001" };                                              //stores current encoding codepage and one to use
    std::vector<Other::ProgrammesToUpdate> programmesToUpdate;      //stores name, id, current version and available version for programmes to update


    /*
    threads
    initializer
    */
    std::thread chcpThread;                                 //used to load and set chcp
    std::thread hasAdminPrivilegesThread;        //used to check for admin privileges
    std::thread isConnectedToNetThread;           //used to check for net connectivity
    std::thread hasSourceThread;                        //used to check if msstore source is allowed on winget
    std::thread acceptMsstoreSourceThread;      //used to make user accept msstore
    std::thread getUpdateListThread;                  //used to get update list

    /*
    threads finished
    markers
    */
    Other::threadBoolResult chcpThreadResult = Other::threadBoolResult::notFinished;
    Other::threadBoolResult hasAdminPrivilegesResult = Other::threadBoolResult::notFinished;
    Other::threadBoolResult isConnectedToNetResult = Other::threadBoolResult::notFinished;
    Other::threadBoolResult hasSourceResult = Other::threadBoolResult::notFinished;
    Other::threadBoolResult getUpdateThreadResult = Other::threadBoolResult::notFinished;


    std::string dummy;

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
        case Other::ProgrammeStage::stageGetSetCHCP:
            //assigning task for chcpThread
            chcpThread = std::thread(WindowCommands::GetSetCHCP, std::ref(chcp[0]), std::string(chcp[1].begin(), chcp[1].end()), std::ref(chcpThreadResult));

            //changing text of loading screen
            loadingAnimation.SetText(L"Настройка кодировки символов\n        командной строки...", 24, sf::Vector2f((screenHSize) / 2, (screenVSize) * 5 / 6), sf::Vector2f(175.0f, 85.0f));
            stage = Other::ProgrammeStage::stageCheckAdminPrivileges;
            break;

        /*
        main part of stage 0
        waiting for chcp to change
        checking if programme has admin privileges
            if true:
                begin checking netconnection
        */
        case Other::ProgrammeStage::stageCheckAdminPrivileges:
            switch (chcpThreadResult){
            //running this segment of code while chcp is still doing its thing
            case Other::threadBoolResult::notFinished:
                if (hasAdminPrivilegesResult == Other::threadBoolResult::True) {
                    isConnectedToNetThread = std::thread(WindowCommands::IsConnectedToNet, std::ref(isConnectedToNetResult));

                    stage = Other::ProgrammeStage::stageCheckNet;
                }

                //if no admin privileges
                else if (hasAdminPrivilegesResult == Other::threadBoolResult::False) {
                    //clearing screen from loading animation
                    window.draw(background);

                    //displaying textboard
                    textBoard.SetText(L"Это программное обеспечение должно запускаться от имени \nадминистратора. Пожалуйста, перезагрузитесь под именем \nадминистратора!", 24, sf::Vector2f(320.0f, 85.0f), sf::Vector2f((screenHSize) / 2, (screenVSize) * 3 / 6), sf::Vector2f(800.0f, 500.0f));
                    textBoard.Draw(window);

                    //displaying button
                    closeButton.Update(deltaTime, mousePosition, stage, Other::ProgrammeStage::stageCloseWindow);
                    closeButton.Draw(window);
                }
                break;

            //running this segment of code when chcp is done
            case Other::threadBoolResult::True:
                loadingAnimation.SetText(L"  проверка доступа к \nправам администратора...", 24, sf::Vector2f((screenHSize) / 2, (screenVSize) * 5 / 6), sf::Vector2f(130.0f, 85.0f));
                hasAdminPrivilegesThread = std::thread(WindowCommands::HasAdminPrivileges, std::ref(hasAdminPrivilegesResult));
                chcpThreadResult = Other::threadBoolResult::notFinished;
                break;

            case Other::False:
                /*
                illegal state
                */
                break;
            }
            break;


        //resolving net connection issues and begin checking if winget has msstore accepted
        case Other::ProgrammeStage::stageCheckNet:
            switch (isConnectedToNetResult){
            case Other::threadBoolResult::notFinished:
                loadingAnimation.SetText(L"Пожалуйста подождите, проверка \n      подключения к Интернету...", 24, sf::Vector2f((screenHSize) / 2, (screenVSize) * 5 / 6), sf::Vector2f(175.0f, 85.0f));
                break;

            case Other::threadBoolResult::True:
                /*
                loadingAnimation.SetText(L"Пожалуйста подождите, проверка \n   разрешен ли источник msstore", 24, sf::Vector2f((screenHSize) / 2, (screenVSize) * 5 / 6), sf::Vector2f(175.0f, 85.0f));
                hasSourceThread = std::thread(WindowCommands::IsSourcemsstoreAllowed, std::ref(hasSourceResult));

                stage = Other::ProgrammeStage::stageCheckSourcemsstore;
                */

                stage = Other::ProgrammeStage::stageGenerateUpdateListInitializer;
                break;

            case Other::threadBoolResult::False:
                //clearing screen from loading animation
                window.draw(background);

                //displaying textboard
                textBoard.SetText(L"Вы не подключены к Интернету, пожалуйста, перезапустите\n программу при подключении к Интернету!", 24, sf::Vector2f(300.0f, 85.0f), sf::Vector2f((screenHSize) / 2, (screenVSize) * 3 / 6), sf::Vector2f(800.0f, 500.0f));
                textBoard.Draw(window);

                //displaying button
                closeButton.Update(deltaTime, mousePosition, stage, Other::ProgrammeStage::stageCloseWindow);
                closeButton.Draw(window);
                break;
            }

            break;

        //checking if msstore source is allowed on winget
        case Other::ProgrammeStage::stageCheckSourcemsstore:
            switch (hasSourceResult){
            case Other::True:
                /*
                if source is allowed
                */
                break;
            case Other::False:
                stage = Other::ProgrammeStage::stageSourcemsstoreFailed;
                WindowCommands::SetCHCP(std::string(chcp[0].begin(), chcp[0].end()));
                acceptMsstoreSourceThread = std::thread(system, "winget upgrade");
                break;
            }         

            break;


        //if msstore source on winget not allowed
        case Other::ProgrammeStage::stageSourcemsstoreFailed:
            //clearing screen from loading animation
            window.draw(background);

            //displaying textboard
            textBoard.SetText(L"   Пожалуйста, примите источник msstore в \nкоммандном строке и перезапустите программу.", 24, sf::Vector2f(250.0f, 85.0f), sf::Vector2f((screenHSize) / 2, (screenVSize) * 3 / 6), sf::Vector2f(800.0f, 500.0f));
            textBoard.Draw(window);

            //displaying button
            closeButton.Update(deltaTime, mousePosition, stage, Other::ProgrammeStage::stageCloseWindow);
            closeButton.Draw(window);
            break;


        //generating to update list
        case Other::ProgrammeStage::stageGenerateUpdateListInitializer:
            getUpdateListThread = std::thread(WindowCommands::GenerateUpdateReport, std::cref(programmesToUpdate),std::cref(getUpdateThreadResult));

            stage = Other::ProgrammeStage::stageGenerateUpdateList;
            break;

        //dealing with found list
        case Other::ProgrammeStage::stageGenerateUpdateList:
            switch (getUpdateThreadResult){
            case Other::notFinished:
                loadingAnimation.SetText(L"Пожалуйста подождите, проверка \nпоиск программ для обновления", 24, sf::Vector2f((screenHSize) / 2, (screenVSize) * 5 / 6), sf::Vector2f(175.0f, 85.0f));
                break;

            //if something to update found
            case Other::True:

                go finish list item radio button thing and add it here

                then add a button that just leads to updating those programmes 

                break;
            
            //if nothing to update was found
            case Other::False:
                //clearing screen from loading animation
                window.draw(background);

                //displaying textboard
                textBoard.SetText(L"Все программы обновлены, ничего не \nнайдено для обновления!", 24, sf::Vector2f(250.0f, 85.0f), sf::Vector2f((screenHSize) / 2, (screenVSize) * 3 / 6), sf::Vector2f(800.0f, 500.0f));
                textBoard.Draw(window);

                //displaying button
                closeButton.Update(deltaTime, mousePosition, stage, Other::ProgrammeStage::stageCloseWindow);
                closeButton.Draw(window);
                break;
            }

            break;


        //initializer of stage 2
        case Other::ProgrammeStage::stage20:
            
            break;


        default:
            break;
        
        case Other::ProgrammeStage::stageCloseWindow:
            window.close();
            break;
        }

        
        window.display();
    }

    return 0;
}
