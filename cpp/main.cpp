#include "header.h"

using namespace std;

int main()
{
    // Window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "ECHOES OF APOLAKI: THE TALE OF MIGGY");

    // Font
    sf::Font font;
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        return EXIT_FAILURE;

    // Music
    sf::Music background_music[2];
    if(!background_music[0].openFromFile("assets/Wild_Horseradish_Jam.wav"))
        return EXIT_FAILURE;
    sf::Music background_music2;
    if(!background_music[1].openFromFile("assets/Valley_Comes_Alive.wav"))
        return EXIT_FAILURE;

    // Sound Effects
    // Sun Shot
    sf::SoundBuffer buffer[3];
    if (!buffer[0].loadFromFile("assets/Laser_Gun.wav"))
        return EXIT_FAILURE;
    sf::Sound sun_shot_sound;
    sun_shot_sound.setBuffer(buffer[0]);

    // Time Slow
    if (!buffer[1].loadFromFile("assets/Time_Slow.wav"))
        return EXIT_FAILURE;
    sf::Sound time_slow_sound;
    time_slow_sound.setBuffer(buffer[1]);
    time_slow_sound.setVolume(25.f);

    // Shield Hit
    if (!buffer[2].loadFromFile("assets/Shield_Hit_Fast.wav"))
        return EXIT_FAILURE;
    sf::Sound shield_hit_sound;
    shield_hit_sound.setBuffer(buffer[2]);
    shield_hit_sound.setVolume(25.f);

    // Text
    sf::Text text("Hello World", font, 28);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.0f);

    // Text Button
    sf::RectangleShape button(sf::Vector2f(300.f, 100.f));
    button.setFillColor(sf::Color::Cyan);

    // Textures and Sprites
    // Forest
    sf::Texture texture[3];
    sf::Sprite background_image[3];
    if (!texture[0].loadFromFile("assets/Forest.jpg"))
        return EXIT_FAILURE;
    background_image[0].setTexture(texture[0]);
    background_image[0].setScale(sf::Vector2f(
        WIDTH / background_image[0].getLocalBounds().width, 
        HEIGHT / background_image[0].getLocalBounds().height
    ));
    
    // Swamp
    if (!texture[1].loadFromFile("assets/swamp.jpg"))
        return EXIT_FAILURE;
    background_image[1].setTexture(texture[1]);
    background_image[1].setScale(sf::Vector2f(
        WIDTH / background_image[1].getLocalBounds().width,
        HEIGHT / background_image[1].getLocalBounds().height
    ));

    // Sky
    if (!texture[2].loadFromFile("assets/Sky.jpg"))
        return EXIT_FAILURE;
    background_image[2].setTexture(texture[2]);
    background_image[2].setScale(sf::Vector2f(
        WIDTH / background_image[2].getLocalBounds().width,
        HEIGHT / background_image[2].getLocalBounds().height
    ));

    // Player
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("assets/player.png"))
        return EXIT_FAILURE;
    sf::Sprite player_sprite(player_texture);
    player_sprite.setOrigin(sf::Vector2f(player_sprite.getGlobalBounds().width / 2.f, player_sprite.getGlobalBounds().height / 2.f));
    player_sprite.setScale(sf::Vector2f(0.2, 0.2));

    // variables
    int hackerman = 0, gameState = 0, tutorial = 0, currentEnemy = 0, freeze, level = 1, end = 1, music = 0, background = 0;
    double angle, spawnRate = 1.f;
    
    //SFML Variables
    sf::Vector2i localMouse;
    sf::Vector2f squarePos;
    sf::Vector2f displacement;
    sf::Time elapsedTime;

    // --------------Creating Game State-------------------
    // Enemy
    int enemies = 50;
    entity white[MAXENTITIES];

    // Player
    player rectangle;

    // Skills
    effect time_slow(0, 0, 10.f, 15000, 0);
    manifest barrier(0, 30000, 6000); // Time is in Milliseconds | 1 Second = 1000 Millisecond
    manifest sun_shot(3, 100, 1);

    // Start Clock
    sf::Clock clock;

    // Play background music
    background_music[0].play();

    //----------------------------GAME START--------------------------
    while (window.isOpen())
    {
        // Loop Background Music
        if (background_music[music].getStatus() == sf::SoundSource::Stopped)
        {
            music = (music + 1) % 2;
            background_music[music].play();
        }

        // TIME
        elapsedTime = clock.getElapsedTime();

        // Get Entity Positions
        localMouse = sf::Mouse::getPosition(window);
        squarePos = rectangle.body.getPosition();

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
                window.setMouseCursorGrabbed(false);
                level++;
                if (level % 10 == 0)
                    background = (background + 1) % 3;
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
                switch(event.key.scancode)
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
                                if (!(rectangle.health <= 0) && enemies < MAXENTITIES && level % 5 == 0) enemies += (20 * (level / 5));
                                if (rectangle.state == DEAD)
                                {
                                    if (!(rectangle.health <= 0)) spawnRate = 1 + (float)level / 15;
                                    game(&rectangle, white, enemies, level);
                                    currentEnemy = 0;

                                    // Reset Skills
                                    time_slow.reset();
                                    barrier.reset(&rectangle);
                                }
                                window.setMouseCursorGrabbed(true);
                                freeze = 0;
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
                            if (event.key.control)
                            {
                                window.setMouseCursorGrabbed(false);
                                gameState = 0;
                            }
                            break;
                        case sf::Keyboard::Scan::Q:
                            if (time_slow.cooldown)
                            {
                                time_slow.timeAtCast = elapsedTime.asMilliseconds();
                                time_slow.time_slow(white, enemies);
                                time_slow.cooldown = false;
                                time_slow_sound.play();
                            }
                            break;
                        case sf::Keyboard::Scan::E:
                            if (barrier.cooldown)
                            {
                                barrier.active = true;
                                barrier.timeAtCast = elapsedTime.asMilliseconds();
                                barrier.barrier(&rectangle);
                                barrier.cooldown = false;
                            }
                            break;
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Left:
                            if (sun_shot.cooldown)
                            {
                                sun_shot.timeAtCast = elapsedTime.asMilliseconds();
                                sun_shot.sun_shot(angle, squarePos);
                                sun_shot.cooldown = false;
                                sun_shot_sound.play();
                            } 
                    }
                }
            }
        }

        window.clear();
        //Background Image
        window.draw(background_image[background]);
        

        //---------------DEVELOPER STATS--------------------
        if (hackerman)
        {
            // Enemy Movement Speed
            for (int i = 0; i < enemies; ++i)
                if (white[i].state != DEAD)
                {
                    text.setString(to_string(white[i].mv));
                    displayString(&window, &text, sf::Vector2f(white[i].body.getPosition().x - (text.getLocalBounds().width / 2.f), white[i].body.getPosition().y - (white[i].body.getGlobalBounds().height) - 10.f));
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

            // Time Slow
            text.setString("[Q] Time Slow: " + to_string((time_slow.cooldown) ? 0 : (time_slow.timer - (elapsedTime.asMilliseconds() - time_slow.timeAtCast)) / 1000.f));
            displayString(&window, &text, sf::Vector2f(0.f, HEIGHT - 50.f));

            // Barrier
            text.setString("[E] Barrier: " + to_string((barrier.cooldown) ? 0 : (barrier.timer - (elapsedTime.asMilliseconds() - barrier.timeAtCast)) / 1000.f));
            displayString(&window, &text, sf::Vector2f(0.f, HEIGHT - 100.f));

            // Barrier Duration
            if (barrier.active)
            {
                text.setString("Duration: " + to_string((barrier.duration - (elapsedTime.asMilliseconds() - barrier.timeAtCast)) / 1000.f));
                displayString(&window, &text, sf::Vector2f(WIDTH - text.getLocalBounds().width - 10.f, 0.f));
            }

            // Sun Shot
            text.setString("[Left Click] Sun Shot: " + to_string((sun_shot.cooldown) ? 0 : (sun_shot.timer - (elapsedTime.asMilliseconds() - sun_shot.timeAtCast)) / 1000.f));
            displayString(&window, &text, sf::Vector2f(0.f, HEIGHT - 150.f));

            // Pause Application
            text.setString(sf::String("Pause Session with ctrl + C"));
            displayString(&window, &text, sf::Vector2f(WIDTH - text.getLocalBounds().width, 1000.f));
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
            if (!tutorial)
            {
                // Start Text
                text.setString((rectangle.health > 0) ? "Start Level!" : "Try Again!");
                displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, (HEIGHT - text.getLocalBounds().height) / 2.f));
                
                // Start Button
                button.setPosition(text.getPosition().x - 75, text.getPosition().y - 33);
                button.getGlobalBounds().contains(localMouse.x, localMouse.y) ? button.setFillColor(sf::Color::Yellow) : button.setFillColor(sf::Color::Blue);
                window.draw(button);
                window.draw(text);

                // Tutorial Text
                text.setString("Tutorial!");
                displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, ((HEIGHT - text.getLocalBounds().height) / 2.f) + button.getGlobalBounds().height + 50.f));

                // Tutorial Button
                button.setFillColor(sf::Color::Green);
                button.setPosition(text.getPosition().x - 75, text.getPosition().y - 33);

                // Exit Application
                text.setString(sf::String("Exit application with ctrl + C"));
                displayString(&window, &text, sf::Vector2f(0.f, 1000.f));
            }
            else
            {
                // Start Text
                //text.setString((rectangle.health > 0) ? "Start Level!" : "Try Again!");
                //displayString(&window, &text, sf::Vector2f((WIDTH - text.getLocalBounds().width) / 2.f, (HEIGHT - text.getLocalBounds().height) / 2.f));


                
            }
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
                    rectangle.body.move(-rectangle.mv, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    rectangle.body.move(rectangle.mv, 0.f);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    rectangle.body.move(0.f, -rectangle.mv);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    rectangle.body.move(0.f, rectangle.mv);

                // Check if Squares at edge of window
                boundary(&rectangle.body, WIDTH, HEIGHT);
                
                // Calculating Shield Angle and Position
                displacement = sf::Vector2f((float)localMouse.x - squarePos.x, (float)localMouse.y - squarePos.y);
                angle = atan2((double)displacement.y, (double)displacement.x) * (180.f/PI); // Utilizes Basic Trigonometry
                rectangle.shield.setRotation(angle);
                rectangle.shield.setPosition(squarePos.x, squarePos.y);

                // Setting Player Sprite to Center of Hitbox
                player_sprite.setPosition(sf::Vector2f(squarePos.x, squarePos.y));

                // Checking Skill States
                time_slow.check_cooldown(elapsedTime.asMilliseconds());
                barrier.check_cooldown(elapsedTime.asMilliseconds());
                sun_shot.check_cooldown(elapsedTime.asMilliseconds());
                
                // Barrier Duration
                if (barrier.active && elapsedTime.asMilliseconds() - barrier.timeAtCast >= barrier.duration)
                {
                    barrier.active = false;
                    barrier.barrier(&rectangle);
                }

                // Sun Shot Movement
                for (int i = 0; i < MAXENTITIES; ++i)
                {
                    if (sun_shot.object[i].state == ALIVE)
                    {
                        // Get trajectory of bullet using polar coordinates with r being the movement speed
                        float x = sun_shot.object[i].mv * cos(sun_shot.get_sun_shot_angle(i) / (180.f/PI));
                        float y = sun_shot.object[i].mv * sin(sun_shot.get_sun_shot_angle(i) / (180.f/PI));
                        sun_shot.object[i].body.move(x, y);

                        // Delete bullet if it goes outside screen
                        sf::Vector2f bullet = sun_shot.object[i].body.getPosition();
                        if (bullet.x > WIDTH || bullet.x < 0 || bullet.y > HEIGHT || bullet.y < 0)
                        {   
                            sun_shot.object[i].state = DEAD;
                            sun_shot.object[i].body.setPosition(-200.f, -200.f);
                        }
                    }
                }
                
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
                        if (white[i].body.getPosition().x > squarePos.x)
                            white[i].body.move(-white[i].mv, 0.f);
                        if (white[i].body.getPosition().y > squarePos.y)
                            white[i].body.move(0.f, -white[i].mv);    
                        if (white[i].body.getPosition().x < squarePos.x)
                            white[i].body.move(white[i].mv, 0.f);
                        if (white[i].body.getPosition().y < squarePos.y)
                            white[i].body.move(0.f, white[i].mv);
                        
                        // Collisions
                        bool objectCol = collision(&rectangle.body, &white[i].body);
                        bool shieldCol = collision(&rectangle.shield, &white[i].body);
                        if (objectCol || shieldCol)
                        {
                            if (objectCol) rectangle.health--;
                            if (shieldCol) shield_hit_sound.play();
                            white[i].body.setPosition(-200.f, -200.f);
                            white[i].state = DEAD;
                            continue;
                        }

                        // Sun Shot Collisions
                        for (int j = 0; j < MAXENTITIES; ++j)
                        {
                            if (sun_shot.object[j].state != DEAD)
                            {
                                if (collision(&white[i].body, &sun_shot.object[j].body))//; <------------ I wasted 30 minutes wondering why the enemies insta die.
                                {
                                    white[i].body.setPosition(0.f, 0.f);
                                    white[i].state = DEAD;
                                    sun_shot.object[j].body.setPosition(-200.f, -200.f);
                                    sun_shot.object[j].state = DEAD;
                                    continue;
                                }
                            }
                        }
                    }
                }
            }
            
            //---------------------RENDER------------------
            // Drawing Player
            window.draw(rectangle.body);
            window.draw(player_sprite);
            window.draw(rectangle.shield);

            // Drawing Enemies
            for (int i = 0; i < enemies; ++i)
                if (white[i].state != DEAD) window.draw(white[i].body);
            
            // Drawing Other Entites
            // Sun Shot bullets
            for (int i = 0; i < MAXENTITIES; ++i)
                if (sun_shot.object[i].state != DEAD) window.draw(sun_shot.object[i].body);

            // Update Freeze
            if (elapsedTime.asMilliseconds() % 1000 == 0)
                freeze--;
        }

        // Display Window
        window.display();
    }

    return EXIT_SUCCESS;
}