#ifndef __SNAKE__
#define __SNAKE__

#include <cstddef>
#include <vector>
#include <unordered_map>

class Point
{
public: 
    Point(size_t x = 0, size_t y = 0, char val = '*');
    ~Point();

    size_t m_x;
    size_t m_y;
    char m_val;
}; 

/**********************/

class Snake
{
public:
    Snake(size_t starting_size = 3, size_t rlimit = 15, 
            size_t climit = 20);
    ~Snake();
    void IncreaseSnake(Point add);
    void UpdateSnake(Point add);
    std::vector<Point> GetSnake();

private:
    std::vector<Point> m_snake;
};

/**********************/

class Food
{
public:
    Food(char food = '@');
    ~Food();
    char GetFood();

private:
    char m_food;
};

/**********************/

class Board
{
public:
    Board(size_t rows = 15, size_t columns = 20);   
    ~Board();
    void AddSnake(const std::vector<Point>& snake);
    void AddFood(char food);
    void UpdateBoard(Point* remove, Point* change_val, Point* add); 
    const std::vector<std::vector<Point> > GetBoard();
    size_t GetRLimit();
    size_t GetCLimit();

private:
    std::vector<std::vector<Point> > m_board;
    size_t m_rlimit;
    size_t m_climit;
};

/**********************/

class Manager
{
public:
    Manager();
    ~Manager();
    char getCharacter();
    void SetPoint(Point* point, size_t x, size_t y, char val);
    void PrintBoard();
    void ManageUpdates(Point* to_check);
    void MoveSnake();
    void Run();

private:
    Board m_manage_board;
    Snake m_manage_snake;
    Food m_manage_food;

};


#endif //__SNAKE__