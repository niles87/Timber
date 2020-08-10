#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

// Function Declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];
const int CLOUDS = 3;
Sprite cloud[CLOUDS];

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

    int cloudSpeed[CLOUDS];
    bool cloudActive[CLOUDS];
    for (int i = 0; i < CLOUDS; i++) {
        cloud[i].setTexture(textureCloud);
        cloud[i].setPosition(0, 200 * i);
        cloudActive[i] = false;
        cloudSpeed[i] = 0;
    }

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

    // Load Sound FX
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sounds/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sounds/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

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

                chop.play();
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

                chop.play();
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

                outOfTime.play();
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
           
            // Update Clouds
            for (int i = 0; i < CLOUDS; i++) {
                if (!cloudActive[i]){
                    srand((int)time(0)* (10 * i + 10));
                    cloudSpeed[i] = (rand() % 100);

                    srand((int)time(0) * 10);
                    float height = (rand() % (100 * i + 100));
                    cloud[i].setPosition(-200, height);
                    cloudActive[i] = true;
                }
                else {
                    cloud[i].setPosition(cloud[i].getPosition().x + (cloudSpeed[i] * dt.asSeconds()), cloud[i].getPosition().y);
                    if (cloud[i].getPosition().x > 2000) {
                        cloudActive[i] = false;
                    }
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

            // Handle Flying log
            if (logActive) {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // Handle player death 
            if (branchPositions[5] == playerSide) {
                paused = true;
                acceptInput = false;

                // Draw GraveStone
                spriteRIP.setPosition(525, 760);
                spritePlayer.setPosition(2000, 660);

                // Change Message Text
                messageText.setString("SQUISHED!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                death.play();
            }
        }
       
        /*
        * Draw Scene
        */
        // Clear everything from the last frame
        window.clear();

        // Draw game scene
        window.draw(spriteBackground);
        for (int i = 0; i < CLOUDS; i++) {
            window.draw(cloud[i]);
        }
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