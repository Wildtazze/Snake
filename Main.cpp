#include <iostream>
#include <random>
#include <thread>
#include <conio.h>

using namespace std;

#define SNAKEH -1
#define TILE 0
#define APPLE 4

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define HWALL 1
#define VWALL 2
#define CORNER 3

#define BOARDSIZE 17

void apple(int board[BOARDSIZE + 1][BOARDSIZE + 1]);
void setUp(int board[BOARDSIZE + 1][BOARDSIZE + 1], int appleX, int appleY, int snakeX, int snakeY);
int logic(int board[BOARDSIZE + 1][BOARDSIZE + 1], int snakeY, int snakeX);
void printBoard(int board[BOARDSIZE + 1][BOARDSIZE + 1]);
void input();
int moveSnake(int tGone, int board[BOARDSIZE + 1][BOARDSIZE + 1], int snakeLen);
void collectApple(int snakeLen, int board[BOARDSIZE + 1][BOARDSIZE + 1]);

int facing = RIGHT;
int state = 1;

int main(void)
{
    int board[BOARDSIZE + 1][BOARDSIZE + 1];
    int snakeLen = 2;
    int snakeX = (BOARDSIZE + 1) / 2;
    int snakeY = (BOARDSIZE + 1) / 2;
    int appleX = -1;
    int appleY = -1;
    

    setUp(board, appleX, appleY, snakeX, snakeY);
    thread t1(input);
    do{
        _sleep(500);
        printBoard(board);
    }while(logic(board, snakeY, snakeX) == 1);
    state = 0;
    t1.join();
    cout << "Oops We Crashed!";
    return 0;
}

void setUp(int board[BOARDSIZE + 1][BOARDSIZE + 1], int appleX, int appleY, int snakeX, int snakeY)
{
    for(int i = 0; i <= BOARDSIZE; i++)
    {
        for(int j = 0; j <= BOARDSIZE; j++)
        {
            board[i][j] = TILE;
            if(i == 0 || i == BOARDSIZE) board[i][j] = HWALL;
            else if(j == 0 || j == BOARDSIZE) board[i][j] = VWALL;
        }
    }
    board[0][0] = CORNER;
    board[BOARDSIZE][0] = CORNER;
    board[0][BOARDSIZE] = CORNER;
    board[BOARDSIZE][BOARDSIZE] = CORNER;

    apple(board);

    board[snakeY][snakeX] = SNAKEH;
    board[snakeY][snakeX - 1] = SNAKEH - 1;
}

void input()
{
    while(state == 1)
    {
        if(_kbhit() != 0)
        {
            switch(_getch())
            {
                case('w'):
                    if(facing == DOWN) break;
                    facing = UP;
                    break;
                case('a'):
                    if(facing == RIGHT) break;
                    facing = LEFT;
                    break;
                case('s'):
                    if(facing == UP) break;
                    facing = DOWN;
                    break;
                case('d'):
                    if(facing == LEFT) break;
                    facing = RIGHT;
                    break;
            }
        }
    }
}

void apple(int board[BOARDSIZE + 1][BOARDSIZE + 1])
{
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> dist(1, BOARDSIZE - 2);

    int xrand, yrand;
    
    do{
        xrand = dist(eng) + 1;
        yrand = dist(eng) + 1;
        if(board[yrand][xrand] == TILE) board[yrand][xrand] = APPLE;
    }while(board[yrand][xrand] != APPLE);
}

void printBoard(int board[BOARDSIZE + 1][BOARDSIZE + 1])
{
    for(int i = 0; i <= BOARDSIZE; i++)
    {
        for(int j = 0; j <= BOARDSIZE; j++)
        {
            switch(board[i][j])
            {
                case TILE:
                    cout << "  ";
                    break;
                case CORNER:
                    cout << "+";
                    break;
                case HWALL:
                    cout << "--";
                    break;
                case VWALL:
                    cout << "|";
                    break;
                case APPLE:
                    cout << " @";
                    break;
                case SNAKEH:
                    cout << " O";
                    break;
                default:
                    if(board[i][j] < -1 ) cout << " o";
                    break;
            }
        }
        cout << endl;
    }
}
int logic(int board[BOARDSIZE + 1][BOARDSIZE + 1], int snakeY, int snakeX, int snakeLen)
{
    switch(facing)
    {
        case RIGHT:
            if(board[snakeY][snakeX + 1] == APPLE) 
            {
                collectApple(snakeLen, board);
                return 1;
            }
            if(board[snakeY][snakeX + 1] != TILE) return 0;
            else moveSnake(0, board, snakeLen);
            snakeX++;
            break;
        case LEFT:
            if(board[snakeY][snakeX - 1] == APPLE) 
            {
                collectApple(snakeLen, board);
                board[snakeY][snakeX] = SNAKEH;
                return 1;
            }
            if(board[snakeY][snakeX - 1] != TILE) return 0;
            else moveSnake(0, board, snakeLen);
            snakeX--;
            break;
        case UP:
            if(board[snakeY + 1][snakeX] == APPLE)  
            {
                collectApple(snakeLen, board);
                board[snakeY][snakeX] = SNAKEH;
                return 1;
            }
            if(board[snakeY + 1][snakeX] != TILE) return 0;
            else moveSnake(0, board, snakeLen);
            snakeY--;
            break;
        case DOWN:
            if(board[snakeY - 1][snakeX] == APPLE)  
            {
                collectApple(snakeLen, board);
                board[snakeY][snakeX] = SNAKEH;
                return 1;
            }
            if(board[snakeY - 1][snakeX] != TILE) return 0;
            else moveSnake(0, board, snakeLen);
            snakeY++;
            break;
    }

    board[snakeY][snakeX] = SNAKEH;

    return 1;
}

int moveSnake(int tGone, int board[BOARDSIZE + 1][BOARDSIZE + 1], int snakeLen)
{
    for(int i = 1; i <= BOARDSIZE - 1; i++)
    {
        for(int j = 1; j <= BOARDSIZE - 1; j++)
        {
            if(tGone == 0 && board[i][j] == -1 * snakeLen)
            {
                board[i][j] = TILE;
                tGone = 1;
            }
            else if(board[i][j] * -1 > 0) board[i][j]--;
        }
    }
    return 0;
}

void collectApple(int snakeLen, int board[BOARDSIZE + 1][BOARDSIZE + 1])
{
    snakeLen++;
    apple(board);
    moveSnake(1, board, snakeLen);
}
