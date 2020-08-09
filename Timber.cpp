#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace sf;

// Function Declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

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

    // Set up branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    // Set texture for each branch
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // Player Sprite 
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;

    // Gravestone Sprite
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Axe Sprite
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Lineup Axe with Tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Chopped Piece Sprite
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Log variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // control player input
    bool acceptInput = false;

    while (window.isOpen()) {
        /*
        * Handle player input
        */
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyReleased && !paused) {
                // allow input
                acceptInput = true;

                // Hide axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;

            // Reset Time and Score
            score = 0;
            timeRemaining = 6;
            
            // Create Branches 
            for (int i = 0; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
            }

            // Hide RIP
            spriteRIP.setPosition(675, 2000);

            spritePlayer.setPosition(580, 720);
            acceptInput = true;
        }
        if (acceptInput) {
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                playerSide = side::RIGHT;
                score++;

                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;
            }
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                playerSide = side::LEFT;
                score++;

                timeRemaining += (2 / score) + .15;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                updateBranches(score);

                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;
            }
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

            // Update branches 
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;
                if (branchPositions[i] == side::LEFT) {
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT) {
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);
                }
                else {
                    branches[i].setPosition(3000, height);
                }
            }
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
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }
        window.draw(spriteTree);
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
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

void updateBranches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }
    // Spawn New Branch
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}