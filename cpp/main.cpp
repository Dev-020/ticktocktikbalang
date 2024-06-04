#include "header.h"

using namespace std;

namespace 
{
    const unsigned int WIDTH = 1600;
    const unsigned int HEIGHT = 1050;
    const float BASEMV = 0.5;
}   

int main()
{
    // Window
    sf::RenderWindow window(sf::VideoMode(1600, 1050), "rectangles");

    // Thread
    //sf::Thread thread(&menu);
    //thread.launch();
    
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
    
    // rectangle
    int num = 3;
    sf::RectangleShape rectangle[num];
    for (int i = 0; i < num; ++i)
    {
        rectangle[i].setPosition(800.f, 700.f - 100 * i);
        rectangle[i].setSize(sf::Vector2f(100.f, 100.f));
    }
    rectangle[0].setFillColor(sf::Color::Green);
    rectangle[1].setFillColor(sf::Color::Red);
    rectangle[2].setFillColor(sf::Color::Blue);

    // variables
    bool n = false;
    int c = 0;
    float mv = BASEMV;
    int menu = 1;
    
    //SFML Variables
    sf::Vector2i localMouse;

    // Game
    while (window.isOpen())
    {
        // Get Position of Pointer relative to window
        localMouse = sf::Mouse::getPosition(window);

        // Events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // X Button
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
                
            // Button Presses
            if (menu)
            {
                if (event.type == sf::Event::KeyPressed && event.key.scancode == sf::Keyboard::Scan::C)
                    window.close();
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    switch (event.mouseButton.button)
                    {
                        case sf::Mouse::Left:
                            if (button.getGlobalBounds().contains(localMouse.x, localMouse.y)) menu = 0;
                            break;
                    }
                }
            }
            else
            {
                if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.scancode)
                    {
                        case sf::Keyboard::Scan::C: 
                            if (event.key.control) menu = 1;
                            for (int i = 0; i < num; ++i)
                                rectangle[i].setPosition(800.f, 700.f - 100 * i);
                            c = 0;
                            break;
                        case sf::Keyboard::Scan::N: n = true; break;
                        case sf::Keyboard::Scan::Num1: c = 0; break;
                        case sf::Keyboard::Scan::Num2: c = 1; break;
                        case sf::Keyboard::Scan::Num3: c = 2; break;
                    }
                }
                else if (event.type == sf::Event::KeyReleased)
                {
                    switch (event.key.scancode)
                    {
                        case sf::Keyboard::Scan::N: n = false; break;
                    }
                }
            }
            
        }

        window.clear();

        if (menu)
        {
            // Play Button
            text.setString(sf::String("Play Game!"));
            text.setPosition((WIDTH - text.getLocalBounds().width) / 2.f, (HEIGHT - text.getLocalBounds().height) / 2.f);
            button.setPosition(text.getPosition().x - 75, text.getPosition().y - 33);
            button.getGlobalBounds().contains(localMouse.x, localMouse.y) ? button.setFillColor(sf::Color::Yellow) : button.setFillColor(sf::Color::Blue);
            window.draw(button);
            window.draw(text);

            // Text
            text.setString(sf::String("Exit application with ctrl + C"));
            text.setPosition(0.f, 0.f);
            window.draw(text);
        }
        else
        {
            // Text Output
            s = (n) ? "N is Pressed" : "N is not Pressed";
            text.setPosition(0.f, 0.f);
            text.setString(s);
            window.draw(text);

            switch (c)
            {
                case 0: s = "Green Square"; break;
                case 1: s = "Red Square"; break;
                case 2: s = "Blue Square"; break;
            }
            text.setPosition(0.f, 50.f);
            text.setString(s);
            window.draw(text);

            s = "Movespeed: " + to_string(mv);
            text.setString(s);
            text.setPosition(0.f, 100.f);
            window.draw(text); 

            s = "Go back to menu with ctrl + C";
            text.setString(s);
            text.setPosition(0.f, 150.f);
            window.draw(text);

            // Moving Rectangles
            mv = (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) ? BASEMV * 0.50 : BASEMV;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                rectangle[c].move(-mv, 0.f);
                for (int i = 0; i < num; ++i)
                    for (int j = 0; j < num; ++j)
                        if (j != i && collision(&rectangle[i], &rectangle[j])) rectangle[c].move(mv, 0.f);
            }  
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                rectangle[c].move(mv, 0.f);
                for (int i = 0; i < num - 1; ++i)
                    for (int j = 0; j < num; ++j)
                        if (j != i && collision(&rectangle[i], &rectangle[j])) rectangle[c].move(-mv, 0.f);
            } 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                rectangle[c].move(0.f, -mv);
                for (int i = 0; i < num - 1; ++i)
                    for (int j = 0; j < num; ++j)
                        if (j != i && collision(&rectangle[i], &rectangle[j])) rectangle[c].move(0.f, mv);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                rectangle[c].move(0.f, mv);
                for (int i = 0; i < num - 1; ++i)
                    for (int j = 0; j < num; ++j)
                        if (j != i && collision(&rectangle[i], &rectangle[j])) rectangle[c].move(0.f, -mv);
            }

            // Check if Squares at edge of window
            boundary(&rectangle[c], WIDTH, HEIGHT);

            // Drawing rectangles
            window.draw(rectangle[0]);
            window.draw(rectangle[1]);
            window.draw(rectangle[2]);
        }

        window.display();

    }

    return EXIT_SUCCESS;
}