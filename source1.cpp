#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <fstream>
using namespace std;

const int width = 35;
const int height = 12;

bool gameOver;
enum direction {STOP, LEFT, RIGHT, UP, DOWN};
direction dir;
int head_r, head_c, fruit_r, fruit_c;

char getKey()   
{
    struct termios old, newt;
    char ch;
    
    tcgetattr(STDIN_FILENO, &old);
    newt = old;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    ch = getchar();
    fcntl(STDIN_FILENO, F_SETFL, old.c_lflag);

    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    return ch;
}

void setup(vector<pair<int, int>> &snake);
void draw(vector<pair<int, int>> &snake);
void input();
void logic(vector<pair<int, int>> &snake, unsigned int &time, int &score);

int main()
{
    vector<pair<int, int>> snake;
    setup(snake);
    unsigned int time = 130000;
    int score = 0;

    while (!gameOver)
    {
        system("clear");
        draw(snake);
        input();
        logic(snake, time, score);
        usleep(time);   
    }

    cout << "Your score: " << score << endl;

    fstream myfile;
    string str;
    int best_scr;

    myfile.open("score.txt", ios::in);
    myfile >> str >> str >> str >> best_scr;
    myfile.close();

    if (score > best_scr)
    {
        cout << "Your highest score: " << score << endl;
        myfile.open("score.txt", ios::out | ios::trunc);
        myfile << "Your highest score: " << score << "   ";
    }
    else
    {
        cout << "Your highest score: " << best_scr << endl;

    }
    myfile.close();

    cout << "Game Over!" << endl;
    return 0;
}

void setup(vector<pair<int, int>> &snake)
{
    gameOver = false;
    dir = RIGHT;

    head_r = height / 2;
    head_c = width / 2;
    snake.push_back({head_r, head_c});

    fruit_r = (rand() % (height - 2)) + 1;
    fruit_c = (rand() % (width - 2)) + 1;
}

void draw(vector<pair<int, int>> &snake)
{
    for (int i = 0; i < height; i++)    // hang
    {
        for (int j = 0; j < width; j++)     // cot
        {
            if (i == 0 || i == height - 1)
            {
                cout << '#';
            }
            else if (j == 0 || j == width - 1)
            {
                cout << '#';
            }
            else if (i == fruit_r && j == fruit_c)
            {
                cout << '*';
            }
            else
            {
                bool printed = false;
                for (int k = 0; k < snake.size(); k++)
                {
                    if (i == snake[k].first && j == snake[k].second)
                    {
                        if (k == 0)
                        {
                            cout << 'O';
                        }
                        else
                        {
                            cout << 'o';
                        }
                        printed = true;
                        break;
                    }
                }
                if (!printed)
                {
                    cout << ' ';
                }
            }
        }
        cout << endl;
    }
}

void input()
{
    switch (tolower(getKey()))
    {
    case 'w':
        if (dir == DOWN)
        {
            break;
        }
        dir = UP;
        break;
    case 'a':
        if (dir == RIGHT)
        {
            break;
        }
        dir = LEFT;
        break;
    case 's':
        if (dir == UP)
        {
            break;
        }
        dir = DOWN;
        break;
    case 'd':
        if (dir == LEFT)
        {
            break;
        }
        dir = RIGHT;
        break;
    case 'q':
        gameOver = true;
        return;
    default:
        break;
    }
}

void logic(vector<pair<int, int>> &snake, unsigned int &time, int &score)
{
    int pre_r = snake[0].first;
    int pre_c = snake[0].second;

    switch (dir)
    {
    case UP:
        pre_r--;
        break;
    case LEFT:
        pre_c--;
        break;
    case DOWN:
        pre_r++;
        break;
    case RIGHT:
        pre_c++;
        break;
    }

    if (pre_r <= 0)
    {
        pre_r = height - 1;
    }
    else if (pre_r >= height - 1)
    {
        pre_r = 0;
    }
    
    if (pre_c <= 0)
    {
        pre_c = width - 1;
    }
    else if (pre_c >= width - 1)
    {
        pre_c = 0;
    }
    

    snake.insert(snake.begin(), {pre_r, pre_c});
    if (pre_r == fruit_r && pre_c == fruit_c)
    {
        score += 10;
        fruit_r = (rand() % (height - 2)) + 1;
        fruit_c = (rand() % (width - 2)) + 1;
        time -= 2000;
    }
    else
    {
        snake.pop_back();
    }
    
    for (int i = 1; i < snake.size(); i++)
    {
        if (pre_r == snake[i].first && pre_c == snake[i].second)
        {
            gameOver = true;
            return;
        }
    }
}
