#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>


using namespace sf;

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    // Create a texture
    Texture textureBackground;

    // Load graphic
    textureBackground.loadFromFile("graphics/background.png");

    // Create Sprite
    Sprite spriteBackground;

    // Attach texture to sprite
    spriteBackground.setTexture(textureBackground);

    // Background position
    spriteBackground.setPosition(0, 0);

    // Create tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Create Bee sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false;
    float beeSpeed = 0.0f;

    // Create Cloud Sprite
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control time!
    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Magenta);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool paused = true;

    // Draw some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;

    // Choose Font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set font to message
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign message
    messageText.setString("Press Enter to Start!");
    scoreText.setString("Score : 0");

    // Set size of font
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Set color
    messageText.setFillColor(Color::Red);
    scoreText.setFillColor(Color::White);

    // Position Text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    while (window.isOpen()) {
        /*
        * Handle player input
        */
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;

            // Reset Time and Score
            score = 0;
            timeRemaining = 6;
        }

        /*
        * Update Scene
        */
        if (!paused) {
            // Measure time
            Time dt = clock.restart();

            // Time Bar functions
            timeRemaining -= dt.asSeconds();
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                paused = true;

                // Change message text
                messageText.setString("Out of time!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // Update Bee
            if (!beeActive) {
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;

                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;
                }
            }
           
            // Update First Cloud
            if (!cloud1Active) {
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 100) + 75;

                srand((int)time(0) * 10);
                float height = (rand() % 75);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else {
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                if (spriteCloud1.getPosition().x > 2000) {
                    cloud1Active = false;
                }
            }

            // Update Second Cloud
            if (!cloud2Active) {
                srand((int)time(0) * 10);
                cloud2Speed = (rand() % 100) + 100;

                srand((int)time(0) * 10);
                float height = (rand() % 250);
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else {
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                if (spriteCloud2.getPosition().x > 2000) {
                    cloud2Active = false;
                }
            }

            // Update Third Cloud
            if (!cloud3Active) {
                srand((int)time(0) * 10);
                cloud3Speed = (rand() % 100) + 125;

                srand((int)time(0) * 10);
                float height = (rand() % 350);
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else {
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                if (spriteCloud3.getPosition().x > 2000) {
                    cloud3Active = false;
                }
            }
            // Update score text
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());
        }
       
        /*
        * Draw Scene
        */
        // Clear everything from the last frame
        window.clear();

        // Draw game scene
        window.draw(spriteBackground);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee);

        // Draw Text
        window.draw(scoreText);
        if (paused) {
            window.draw(messageText);
        }

        // Draw Time Bar
        window.draw(timeBar);

        // Display scene
        window.display();

    }

    return 0;
}
