#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <vector>
#include <fcntl.h>
#include <fstream>

using namespace std;
bool gameOver;
const int width = 30;
const int height = 10;

enum director { STOP, LEFT, RIGHT, UP, DOWN };
director dir;
int head_x, head_y, fruit_x, fruit_y, score = 0;

char getKey()   
{
    struct termios oldt, newt;
    char ch;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    ch = getchar();
    fcntl(STDIN_FILENO, F_SETFL, oldt.c_lflag);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void setup(vector<pair<int, int>> &snake);
void draw(vector<pair<int, int>> &snake);
void input();
void logic(vector<pair<int, int>> &snake);

int main()
{
    vector<pair<int, int>> snake;
    setup(snake);
    
    while (!gameOver)
    {
        draw(snake);
        input();
        logic(snake);
        usleep(130000);
    }

    cout << "Game Over!" << endl;
    cout << "Your final score: " << score << endl;

    fstream myfile("score.txt", ios::in | ios::out);
    string str;
    int pre_score = 0;

    myfile >> str >> str >> str >> pre_score;

    if (score > pre_score)
    {
        myfile.close();
        myfile.open("score.txt", ios::out | ios::trunc);
        myfile << "Your highest score: " << score << "   ";
    }

    myfile.close();
    return 0;
}

void setup(vector<pair<int, int>> &snake)
{
    gameOver = false;
    dir = RIGHT;

    head_x = width / 2;
    head_y = height / 2;
    snake.push_back({head_x, head_y});
    
    fruit_x = 1 + (rand() % (width - 2));
    fruit_y = 1 + (rand() % (height - 2));

}

void draw(vector<pair<int, int>> &snake)
{
    system("clear");
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (x == 0 || x == width - 1)           
            {
                cout << '#';
            }
            else if (y == 0 || y == height - 1)     
            {
                cout << '#';
            }
            else if (y == fruit_y && x == fruit_x)  
            {
                cout << '*';
            }
            else                        
            {
                bool printed = false;
                for (int i = 0; i < snake.size(); i++)
                {
                    if (x == snake[i].first && y == snake[i].second)
                    {
                        cout << (i == 0 ? 'O' : 'o');
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
    switch (getKey())
    {
    case 'a': dir = LEFT; break;
    case 'd': dir = RIGHT; break;
    case 'w': dir = UP; break;
    case 's': dir = DOWN; break;
    case 'q': gameOver = true; return;
    default:  break;
    }
}

void logic(vector<pair<int, int>> &snake)
{
    int pre_X = snake[0].first;
    int pre_Y = snake[0].second;

    switch (dir)
    {
    case LEFT:  pre_X--; break;
    case RIGHT: pre_X++; break;
    case UP:    pre_Y--; break;
    case DOWN:  pre_Y++; break;
    default:    break;
    }

    if (pre_X <= 0)
    {
        pre_X = width;
    }
    else if (pre_X >= width)
    {
        pre_X = 0;
    }
    if (pre_Y <= 0)
    {
        pre_Y = height;
    }
    else if (pre_Y >= height)
    {
        pre_Y = 0;
    }
    
    for (int i = 0; i < snake.size(); i++)
    {
        if (pre_X == snake[i].first && pre_Y == snake[i].second)
        {
            gameOver = true;
            return;
        }
    }

    snake.insert(snake.begin(), {pre_X, pre_Y});

    if (pre_X == fruit_x && pre_Y == fruit_y)
    {
        score += 10;
        fruit_x = 1 + (rand() % (width - 2));
        fruit_y = 1 + (rand() % (height - 2));
    }
    else
    {
        snake.pop_back();
    } 
}