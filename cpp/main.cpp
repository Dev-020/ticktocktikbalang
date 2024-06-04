#include "header.h"

using namespace std;

int main()
{
    // Window
    sf::RenderWindow window(sf::VideoMode(1600, 1050), "rectangles");
    
    // Font
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        return EXIT_FAILURE;

    // Text
    string s = "Hello World";
    sf::Text text(s, font, 28);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);

    // Text Button
    sf::RectangleShape button(sf::Vector2f(300.f, 100.f));
    button.setFillColor(sf::Color::Yellow);

    // variables
    bool n = false;
    int hackerman = 0, gameState = 0, currentEnemy = 0, freeze, level = 1, end = 1;
    double angle;
    
    //SFML Variables
    sf::Vector2i localMouse;
    sf::Vector2f squarePos;
    sf::Vector2f displacement;
    sf::Time elapsedTime;
    sf::Time menuTime;

    // --------------Initial Game State-------------------
    // Enemy
    int enemies = 100;
    enemy white[enemies];

    // Player
    player rectangle;

    // Start Clock
    sf::Clock clock;

    // Game Start
    while (window.isOpen())
    {
        // TIME
        elapsedTime = clock.getElapsedTime();

        // Calculate Needed Values
        localMouse = sf::Mouse::getPosition(window);
        squarePos = rectangle.object.getPosition();
        displacement = sf::Vector2f((float)localMouse.x - squarePos.x, (float)localMouse.y - squarePos.y);
        angle = atan2((double)displacement.y, (double)displacement.x) * (180.f/PI);

        // Check Player Health
        if (rectangle.health <= 0)
            gameState = 0;
        
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
                rectangle.health = 0;
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

            // Checked Events Per Game State
            if (gameState == 0)
            {
                if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::C && event.key.control)
                    window.close();
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Left:
                            if (button.getGlobalBounds().contains(localMouse.x, localMouse.y))
                            {
                                // Reset Game State
                                if (rectangle.health <= 0)
                                {
                                    // Generating Initial Stats of Enemy
                                    for (int i = 0; i < enemies; ++i)
                                    {
                                        white[i].object.setPosition((rand() % 2) ? WIDTH : 0, rand() % HEIGHT);
                                        white[i].mv = ((rand() % (1 + (2 * level))) / 10.f) + MINMV;
                                        white[i].state = DEAD;
                                    }
                                    currentEnemy = 0;

                                    // Setting Initial Position of Player
                                    rectangle.object.setPosition(800.f, 525.f);
                                    rectangle.shield.setPosition(squarePos.x, squarePos.y);
                                    rectangle.health = 4 + (2 * level);
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

        //---------------DEVELOPER TOOLS--------------------
        if (hackerman)
        {
            // Enemy MV
            s = "Enemy MV: " + to_string(white[currentEnemy].mv);
            text.setPosition(0.f, 0.f);
            text.setString(s);
            window.draw(text);

            // Current Player Movemenet Speed
            s = "Movespeed: " + to_string(rectangle.mv);
            text.setString(s);
            text.setPosition(0.f, 100.f);
            window.draw(text); 

            // Displacement
            s = "Displacement: " + to_string(displacement.x) + ", " + to_string(displacement.y);
            text.setString(s);
            text.setPosition(0.f, 200.f);
            window.draw(text);

            // Current Shield Rotation
            s = "Angle: " + to_string(rectangle.shield.getRotation());
            text.setString(s);
            text.setPosition(0.f, 250.f);
            window.draw(text);

        }

        //------------------------LEVEL STATS---------------------------
        if (gameState)
        {
            // Current Enemy
            s = "Enemies: " + to_string(enemies - currentEnemy);
            text.setPosition(0.f, 50.f);
            text.setString(s);
            window.draw(text);

            // Current Player HP
            s = "Health: " + to_string(rectangle.health);
            text.setString(s);
            text.setPosition(0.f, 150.f);
            window.draw(text);

            // Pause Application
            text.setString(sf::String("Pause Session with ctrl + C"));
            text.setPosition(0.f, 1000.f);
            window.draw(text);
        }

        //--------------------------GENERAL STATS----------------------

        // Level
        s = "Level: " + to_string(level);
        text.setString(s);
        text.setPosition((WIDTH - text.getLocalBounds().width) / 2.f, text.getLocalBounds().height);
        window.draw(text);

        // Elapsed Time
        s = "Time: " + to_string(elapsedTime.asSeconds());
        text.setString(s);
        text.setPosition(0.f, 300.f);
        window.draw(text);

        if (!gameState) // MENU
        {
            // Play Button
            text.setString(sf::String("Play Game!"));
            text.setPosition((WIDTH - text.getLocalBounds().width) / 2.f, (HEIGHT - text.getLocalBounds().height) / 2.f);
            button.setPosition(text.getPosition().x - 75, text.getPosition().y - 33);
            button.getGlobalBounds().contains(localMouse.x, localMouse.y) ? button.setFillColor(sf::Color::Yellow) : button.setFillColor(sf::Color::Blue);
            window.draw(button);
            window.draw(text);

            // Exit Application
            text.setString(sf::String("Exit application with ctrl + C"));
            text.setPosition(0.f, 1000.f);
            window.draw(text);
        }
        else if (gameState)
        {
            if (freeze < 0)
            {
                //---------------------------PLAYER STATS---------------------------------------
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
                
                // Set Shield in Proper Position
                rectangle.shield.setRotation(angle);
                rectangle.shield.setPosition(squarePos.x, squarePos.y);
                
                //-------------------------------ENEMY STATS--------------------------------------------
                // Spawning New Enemies
                if (!(elapsedTime.asMilliseconds() % (1000 / level)) && currentEnemy <= enemies)
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
            for (int i = 0; i < enemies - 1; ++i)
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