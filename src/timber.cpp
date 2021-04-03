#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>

// function declarations
void update_branches(int seed);
int constexpr NUM_BRANCHES = 6;
sf::Sprite branches[NUM_BRANCHES];

enum class side { LEFT, RIGHT, NONE };
side branch_positions[NUM_BRANCHES];

int main()
{
    // create a video object and create a window
    //sf::VideoMode vm = sf::VideoMode::getDesktopMode();
    //sf::RenderWindow window(vm, "Timber!!!");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber!!!");

    // create the background
    sf::Texture bg_texture;
    bg_texture.loadFromFile("../graphics/background.png");
    sf::Sprite bg_sprite;
    bg_sprite.setTexture(bg_texture);
    bg_sprite.setPosition(0,0);
    
    // make a tree
    sf::Texture tree_texture;
    tree_texture.loadFromFile("../graphics/tree.png");
    sf::Sprite tree_sprite;
    tree_sprite.setTexture(tree_texture);
    tree_sprite.setPosition(810,0);

    // prepare the bee
    sf::Texture bee_texture;
    bee_texture.loadFromFile("../graphics/bee.png");
    sf::Sprite bee_sprite;
    bee_sprite.setTexture(bee_texture);
    bee_sprite.setPosition(0,800);
    // active bee?
    bool bee_active = false;
    float bee_speed = 0.0f;

    // make three cloud sprites
    sf::Texture cloud_texture;
    cloud_texture.loadFromFile("../graphics/cloud.png");
    sf::Sprite cloud1_sprite, cloud2_sprite, cloud3_sprite;
    cloud1_sprite.setTexture(cloud_texture);
    cloud2_sprite.setTexture(cloud_texture);
    cloud3_sprite.setTexture(cloud_texture);
    // position the clouds on the left at different heights
    cloud1_sprite.setPosition(0,0);
    cloud2_sprite.setPosition(0,150);
    cloud3_sprite.setPosition(0,300);
    // active clouds?
    bool cloud1_active = false;
    float cloud1_speed = 0.0f;
    bool cloud2_active = false;
    float cloud2_speed = 0.0f;
    bool cloud3_active = false;
    float cloud3_speed = 0.0f;
    
    // let's control time itself
    sf::Clock clock;
    // time bar
    sf::RectangleShape time_bar;
    float time_bar_start_width = 400, time_bar_height = 80;
    time_bar.setSize(sf::Vector2f(time_bar_start_width, time_bar_height));
    time_bar.setFillColor(sf::Color::Red);
    time_bar.setPosition((1920 / 2) - time_bar_start_width / 2, 980);
    sf::Time game_time_total;
    float time_remaining = 6.0f;
    float time_bar_width_per_second = time_bar_start_width / time_remaining;

    // game paused?
    bool paused = true;

    // draw some text
    int score = 0;
    sf::Text message_text, score_text;

    // load font
    sf::Font font;
    font.loadFromFile("../fonts/KOMIKAP_.ttf");
    message_text.setFont(font);
    score_text.setFont(font);

    // display the message
    message_text.setString("Press Enter to start!");
    score_text.setString("Score = 0");
    message_text.setCharacterSize(75);
    score_text.setCharacterSize(100);
    message_text.setFillColor(sf::Color::White);
    score_text.setFillColor(sf::Color::White);

    // position the text
    sf::FloatRect text_rect = message_text.getLocalBounds();
    message_text.setOrigin(text_rect.left + 
                            text_rect.width / 2.0f,
                            text_rect.top +
                            text_rect.height / 2.0f);
    message_text.setPosition(1920 / 2.0f, 1080 / 2.0f);
    score_text.setPosition(20, 20);

    // prepare six branches
    sf::Texture branch_texture;
    branch_texture.loadFromFile("../graphics/branch.png");
    // set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(branch_texture);
        branches[i].setPosition(-2000, -2000);
        // position the sprite in the dead centre
        branches[i].setOrigin(200, 20);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //Respond to key pressed events
            if (event.type == sf::Event::EventType::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Return)
                {
                    paused = false;
                    // reset the time and the score
                    score = 0;
                    time_remaining = 6;
                }
            }
        }

        if (!paused)
        {
            // clear the last frame
            window.clear();
            // update the scene
            sf::Time dt = clock.restart();

            // subtract the amount of time remaining
            time_remaining -= dt.asSeconds();
            time_bar.setSize(sf::Vector2f(time_bar_width_per_second * 
                                            time_remaining, time_bar_height));
            if(time_remaining <= 0.0f)
            {
                // pause the game
                paused = true;
                message_text.setString("Out of time!!!");
                // reposition the text based on its new size
                sf::FloatRect text_rect = message_text.getLocalBounds();
                message_text.setOrigin(text_rect.left +
                                        text_rect.width / 2.0f,
                                        text_rect.top +
                                        text_rect.height / 2.0f);
                message_text.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }

            // launch the bee
            if (!bee_active)
            {
                //how fast is the bee
                srand((int)time(0));
                bee_speed = (rand() % 200) + 200;
                // how high is the bee
                srand((int)time(0)*10);
                float height = (rand() % 500) + 500;
                bee_sprite.setPosition(2000, height);
                bee_active = true;
            }
            else
            {
                bee_sprite.setPosition(
                    bee_sprite.getPosition().x - (bee_speed * dt.asSeconds()),
                    bee_sprite.getPosition().y);
                
                // has the bee reached the left-hand edge of the screen?
                if(bee_sprite.getPosition().x < -100)
                {
                    bee_active = false;
                }
            }

            // manage the clouds
            // cloud 1
            if (!cloud1_active)
            {
                // how fast is the cloud
                srand((int)time(0) * 10);
                cloud1_speed = (rand() % 200);
                // how high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                cloud1_sprite.setPosition(-200, height);
                cloud1_active = true;
            }
            else
            {
                cloud1_sprite.setPosition(
                    cloud1_sprite.getPosition().x +
                    (cloud1_speed * dt.asSeconds()),
                    cloud1_sprite.getPosition().y);
                
                // has the cloud reached the edge of the screen?
                if (cloud1_sprite.getPosition().x > 1920)
                {
                    cloud1_active = false;
                }
            }

            // cloud 2
            if (!cloud2_active)
            {
                // how fast is the cloud
                srand((int)time(0) * 20);
                cloud2_speed = (rand() % 200);
                // how high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                cloud2_sprite.setPosition(-200, height);
                cloud2_active = true;
            }
            else
            {
                cloud2_sprite.setPosition(
                    cloud2_sprite.getPosition().x +
                    (cloud2_speed * dt.asSeconds()),
                    cloud2_sprite.getPosition().y);
                
                // has the cloud reached the edge of the screen?
                if (cloud2_sprite.getPosition().x > 1920)
                {
                    cloud2_active = false;
                }
            }

            // cloud 3
            if (!cloud3_active)
            {
                // how fast is the cloud
                srand((int)time(0) * 30);
                cloud3_speed = (rand() % 200);
                // how high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                cloud3_sprite.setPosition(-200, height);
                cloud3_active = true;
            }
            else
            {
                cloud3_sprite.setPosition(
                    cloud3_sprite.getPosition().x +
                    (cloud3_speed * dt.asSeconds()),
                    cloud3_sprite.getPosition().y);
                
                // has the cloud reached the edge of the screen?
                if (cloud3_sprite.getPosition().x > 1920)
                {
                    cloud3_active = false;
                }
            }

            // update the score text
            std::stringstream ss;
            //ss << "Score = " << score;
            ss << "Score = " << time_remaining;
            score_text.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branch_positions[i] == side::LEFT)
                {
                    // move to the left side and flip the sprite
                    branches[i].setPosition(610, height);
                    branches[i].setRotation(180);
                }
                else if (branch_positions[i] == side::RIGHT)
                {
                    // move sprite to the right side
                    branches[i].setPosition(1330, height);
                    branches[i].setRotation(0);
                }
                else
                {
                    // hide the branch
                    branches[i].setPosition(3000,height);
                }
            }
        }

        // draw the scene
        window.draw(bg_sprite);
        window.draw(cloud1_sprite);
        window.draw(cloud2_sprite);
        window.draw(cloud3_sprite);
        // draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        window.draw(tree_sprite);
        window.draw(bee_sprite);

        // draw the score and time bar
        window.draw(score_text);
        window.draw(time_bar);
        if (paused)
        {
            // draw the message
            window.draw(message_text);
        }

        // show everything we just drew
        window.display();
    }
    
    return 0;
}

void update_branches(int seed)
{
    for (int j = NUM_BRANCHES - 1; j > 0 ; j--)
    {
        branch_positions[j] = branch_positions[j - 1];
    }

    // spawn a new branch at position 0
    // LEFT, RIGHT, or NONE
    srand((int)time(0)+seed);
    int r = (rand() % 5);
    switch (r)
    {
    case 0:
        branch_positions[0] = side::LEFT;
        break;
    case 1:
        branch_positions[0] = side::RIGHT;
        break;
    default:
        branch_positions[0] = side::NONE;
        break;
    }
}