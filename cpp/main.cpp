#include "header.h"

using namespace std;

int main()
{
    // Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "rectangles");
    
    // Font
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        return EXIT_FAILURE;

    // Text
    sf::Text text("Hello World", font, 28);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);

    // Text Button
    sf::RectangleShape button(sf::Vector2f(300.f, 100.f));
    button.setFillColor(sf::Color::Yellow);

    // variables
    bool n = false;
    int hackerman = 0, gameState = 0, currentEnemy = 0, freeze, level = 1, end = 1;
    double angle, spawnRate = 1.f;
    
    //SFML Variables
    sf::Vector2i localMouse;
    sf::Vector2f squarePos;
    sf::Vector2f displacement;
    sf::Time elapsedTime;
    sf::Time menuTime;

    // --------------Creating Game State-------------------
    // Enemy
    int enemies = 50;
    enemy white[MAXENEMIES];

    // Player
    player rectangle;

    // Start Clock
    sf::Clock clock;

    //----------------------------GAME START--------------------------
    while (window.isOpen())
    {
        // TIME
        elapsedTime = clock.getElapsedTime();

        // Get Entity Positions
        localMouse = sf::Mouse::getPosition(window);
        squarePos = rectangle.object.getPosition();

        // Check Player Health
        if (rectangle.health <= 0)
        {
            rectangle.state = DEAD;
            gameState = 0;
        }
        
        // Check Number of Enemies Left
        end = 1;
        if (currentEnemy >= enemies)
        {
            for (int i = 0; i < enemies; ++i)
                if (white[i].state != DEAD)
                {
                    end = 0;
                    break;
                }
            if (end)
            {
                currentEnemy = 0;
                gameState = 0;
                rectangle.state = DEAD;
                level++;
            }
        }
            
        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // X Button
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                switch( event.key.scancode)
                {
                    case sf::Keyboard::N:
                        if (event.key.control) hackerman = (hackerman + 1) % 2;
                        break;
                }
            }

            //----------------------------EVENTS-------------------------------
            if (gameState == 0)
            {
                if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::C && event.key.control)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Left: // Initializing Game Values at Start
                            if (button.getGlobalBounds().contains(localMouse.x, localMouse.y))
                            {
                                // Reset Game State
                                if (!(rectangle.health <= 0) && enemies < MAXENEMIES && level % 5 == 0) enemies += (50 * (level / 5));
                                if (rectangle.state == DEAD)
                                {
                                    if (!(rectangle.health <= 0)) spawnRate = 1 + (float)level / 20;
                                    game(&rectangle, white, enemies, level);
                                    currentEnemy = 0;
                                }
                                
                                freeze = 3;
                                gameState = 1;
                            }
                            break;
                    }
                }
            }
            else if (gameState)
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.scancode)
                    {
                        case sf::Keyboard::Scan::C: 
                            if (event.key.control) gameState = 0;
                            break;
                    }
                }
            }
            
        }

        window.clear();

        //---------------DEVELOPER STATS--------------------
        if (hackerman)
        {
            // Enemy Movement Speed
            for (int i = 0; i < enemies; ++i)
                if (white[i].state != DEAD)
                {
                    text.setString(to_string(white[i].mv));
                    displayString(&window, &text, sf::Vector2f(white[i].object.getPosition().x - (text.getLocalBounds().width / 2.f), white[i].object.getPosition().y - (white[i].object.getGlobalBounds().height) - 10.f));
                }
            
            // Current Player Movemenet Speed
            text.setString("Movespeed: " + to_string(rectangle.mv));
            displayString(&window, &text, sf::Vector2f(0.f, 50.f));

            // Displacement between Player and Mouse
            text.setString("Displacement: " + to_string(displacement.x) + ", " + to_string(displacement.y));
            displayString(&window, &text, sf::Vector2f(0.f, 100.f));

            // Current Shield Angle around PLayer
            text.setString("Angle: " + to_string(rectangle.shield.getRotation()));
            displayString(&window, &text, sf::Vector2f(0.f, 150.f));

            // Current Spawnrate
            text.setString("Spawnrate: " + to_string(1000 / spawnRate));
            displayString(&window, &text, sf::Vector2f(0.f, 200.f));
        }

        //------------------------LEVEL STATS---------------------------
        if (gameState)
        {
            // Current Enemy
            text.setString("Enemies Left: " + to_string(enemies - currentEnemy));
            displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, text.getLocalBounds().height + 100.f));

            // Current Player HP
            text.setString("Health: " + to_string(rectangle.health));
            displayString(&window, &text, sf::Vector2f(squarePos.x - text.getLocalBounds().width / 2.f, squarePos.y - 105.f));

            // Pause Application
            text.setString(sf::String("Pause Session with ctrl + C"));
            displayString(&window, &text, sf::Vector2f(0.f, 1000.f));
        }

        //--------------------------GENERAL STATS----------------------
        // Level
        text.setString("Level: " + to_string(level));
        displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, text.getLocalBounds().height));

        // Elapsed Time
        text.setString("Time: " + to_string((int)elapsedTime.asSeconds()));
        displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, text.getLocalBounds().height + 50.f));

        if (!gameState) // MENU
        {
            // Start Text
            text.setString((rectangle.health > 0) ? "Start Level!" : "Try Again!");
            displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, (HEIGHT - text.getLocalBounds().height) / 2.f));
            
            // Start Button
            button.setPosition(text.getPosition().x - 75, text.getPosition().y - 33);
            button.getGlobalBounds().contains(localMouse.x, localMouse.y) ? button.setFillColor(sf::Color::Yellow) : button.setFillColor(sf::Color::Blue);
            window.draw(button);
            window.draw(text);

            // Exit Application
            text.setString(sf::String("Exit application with ctrl + C"));
            displayString(&window, &text, sf::Vector2f(0.f, 1000.f));
        }
        else if (gameState)
        {
            if (freeze < 0)
            {
                //------------------------------------------GAME ENGINE--------------------------------
                //PLAYER LOGIC
                // Player Movement
                //mv = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? BASEMV * 0.50 : BASEMV;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    rectangle.object.move(-rectangle.mv, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    rectangle.object.move(rectangle.mv, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    rectangle.object.move(0.f, -rectangle.mv);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    rectangle.object.move(0.f, rectangle.mv);

                // Check if Squares at edge of window
                boundary(&rectangle.object, WIDTH, HEIGHT);
                
                // Calculating Shield Angle and Position
                displacement = sf::Vector2f((float)localMouse.x - squarePos.x, (float)localMouse.y - squarePos.y);
                angle = atan2((double)displacement.y, (double)displacement.x) * (180.f/PI);
                rectangle.shield.setRotation(angle);
                rectangle.shield.setPosition(squarePos.x, squarePos.y);
                
                //ENEMY LOGIC
                // Spawning New Enemies
                if (!(elapsedTime.asMilliseconds() % (int)(1000 / spawnRate)) && currentEnemy < enemies)
                {
                    white[currentEnemy].state = ALIVE;
                    currentEnemy++;
                }

                // Checking each enemy state
                for (int i = 0; i < enemies; ++i)
                {
                    if (white[i].state != DEAD)
                    {
                        // Enemy Movement
                        if (white[i].object.getPosition().x > squarePos.x)
                            white[i].object.move(-white[i].mv, 0.f);
                        if (white[i].object.getPosition().y > squarePos.y)
                            white[i].object.move(0.f, -white[i].mv);    
                        if (white[i].object.getPosition().x < squarePos.x)
                            white[i].object.move(white[i].mv, 0.f);
                        if (white[i].object.getPosition().y < squarePos.y)
                            white[i].object.move(0.f, white[i].mv);
                        
                        // Collisions
                        bool objectCol = collision(&rectangle.object, &white[i].object);
                        bool shieldCol = collision(&rectangle.shield, &white[i].object);
                        if (objectCol || shieldCol)
                        {
                            if (objectCol) rectangle.health--;
                            white[i].object.setPosition(-200.f, -200.f);
                            white[i].state = DEAD;
                        } 
                    }
                }
            }
            
            //---------------------DRAW ENTITIES------------------
            // Drawing Player
            window.draw(rectangle.object);
            window.draw(rectangle.shield);

            // Drawing Enemies
            for (int i = 0; i < enemies; ++i)
                if (white[i].state != DEAD) window.draw(white[i].object);
            
            // Update Freeze
            if (elapsedTime.asMilliseconds() % 1000 == 0)
                freeze--;
        }

        // Display Window
        window.display();
    }

    return EXIT_SUCCESS;
}