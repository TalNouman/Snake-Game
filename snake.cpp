#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>

#include "snake.hpp"


Point::Point(size_t x, size_t y, char val) : m_x(x), m_y(y), m_val(val) {}

/* -------------------- */

Point::~Point() {}

/* -------------------- */

Board::Board(size_t rows, size_t columns) : 
                m_board(), m_rlimit(rows), m_climit(columns)
{
    m_board.resize(rows, std::vector<Point>(columns, Point(rows, columns)));
    for(size_t i = 1; i < rows - 1; ++i)
    {
        for(size_t j = 1; j < columns - 1; ++j)
        {
            m_board[i][j].m_val = ' ';
        }
    }
}

/* -------------------- */

Board::~Board() {}

/* -------------------- */

void Board::AddSnake(const std::vector<Point>& snake)
{
    for(size_t i = 0; i < snake.size(); ++i)
    {
        m_board[snake[i].m_x][snake[i].m_y].m_val = snake[i].m_val;
    }
}

/* -------------------- */
void Board::AddFood(char food)
{
    size_t r = 0;
    size_t c = 0;
    
    while(m_board[r][c].m_val != ' ')
    {
        r = rand() % m_rlimit;
        c = rand() % m_climit;
    }

    m_board[r][c].m_val = food;
}

/* -------------------- */

void Board::UpdateBoard(Point* remove, Point* change_val, Point* add)
{
    if(remove != NULL)
    {
        m_board[remove->m_x][remove->m_y].m_val = ' ';
    }
    m_board[change_val->m_x][change_val->m_y].m_val = '$';
    m_board[add->m_x][add->m_y].m_val = add->m_val;
}


/* -------------------- */

const std::vector<std::vector<Point> > Board::GetBoard()
{
    return m_board;
}

/* -------------------- */

size_t Board::GetRLimit()
{
    return m_rlimit;
}

/* -------------------- */

size_t Board::GetCLimit()
{
    return m_climit;
}

/* -------------------- */

Food::Food(char food) : m_food(food) {}

/* -------------------- */

Food::~Food() {}

/* -------------------- */

char Food::GetFood()
{
    return m_food;
}

/* -------------------- */

Snake::Snake(size_t starting_size, size_t rlimit, size_t climit) :
                m_snake()
{
    size_t r = 1, c = 1;
    for(r, c; r < rlimit, c < climit, starting_size > 1; ++c)
    {
        m_snake.push_back(Point(r, c, '$'));
        --starting_size;
    }
    m_snake.push_back(Point(r, c, '@'));
}

/* -------------------- */

Snake::~Snake() {}

/* -------------------- */

std::vector<Point> Snake::GetSnake()
{
    return m_snake;
}

/* -------------------- */

void Snake::IncreaseSnake(Point add)
{
    m_snake.back().m_val = m_snake.front().m_val;
    m_snake.push_back(add);
}

/* -------------------- */

void Snake::UpdateSnake(Point add)
{
    m_snake.erase(m_snake.begin());
    IncreaseSnake(add);
}

/* -------------------- */

Manager::Manager() : m_manage_board(),
                    m_manage_snake(3, m_manage_board.GetRLimit(), 
                        m_manage_board.GetCLimit()),
                    m_manage_food() {}

/* -------------------- */

Manager::~Manager() {}

/* -------------------- */

void Manager::SetPoint(Point* point, size_t x, size_t y, char val)
{
    point->m_x = x;
    point->m_y = y;
    point->m_val = val;
}

/* -------------------- */

void Manager::PrintBoard()
{
    for(size_t i = 0; i < m_manage_board.GetBoard().size(); ++i) 
    {
        for(size_t j = 0; j < m_manage_board.GetBoard()[i].size(); ++j) 
        {
            std::cout << m_manage_board.GetBoard()[i][j].m_val;
        } 

        std::cout << std::endl;
    }
}

/* -------------------- */

void Manager::Run()
{
    m_manage_board.AddSnake(m_manage_snake.GetSnake());
    m_manage_board.AddFood(m_manage_food.GetFood());
    PrintBoard();
    MoveSnake();
}

/* -------------------- */

void Manager::ManageUpdates(Point* to_check)
{
    if(m_manage_board.GetBoard()[to_check->m_x][to_check->m_y].m_val == ' ')
    {
        m_manage_board.UpdateBoard(&m_manage_snake.GetSnake().front(),
                        &m_manage_snake.GetSnake().back(), to_check);
        m_manage_snake.UpdateSnake(*to_check);
    }
    else if(m_manage_board.GetBoard()[to_check->m_x][to_check->m_y].m_val == '*' ||
            m_manage_board.GetBoard()[to_check->m_x][to_check->m_y].m_val == '$')
    {
        exit(1);
    }
    else
    {
        m_manage_board.UpdateBoard(NULL, &m_manage_snake.GetSnake().back(), to_check);
        m_manage_snake.IncreaseSnake(*to_check);
        m_manage_board.AddFood(m_manage_food.GetFood());
    }
}

/* -------------------- */

char Manager::getCharacter() 
{
    char ch = 0;
    struct termios oldTermios, newTermios;

    // Disable terminal line buffering
    tcgetattr(STDIN_FILENO, &oldTermios);
    newTermios = oldTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);

    // Read a character
    read(STDIN_FILENO, &ch, 1);

    // Restore the terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermios);

    return ch;
}

/* -------------------- */

void Manager::MoveSnake()
{
    Point new_point;
    char input = getCharacter();

    while(input != 'q')
    {
        switch (input)
        {
        case 'w': //up
            SetPoint(&new_point, m_manage_snake.GetSnake().back().m_x - 1,
                        m_manage_snake.GetSnake().back().m_y,
                        m_manage_snake.GetSnake().back().m_val);
            ManageUpdates(&new_point);
            break;
        case 'a': //left
            SetPoint(&new_point, m_manage_snake.GetSnake().back().m_x,
                        m_manage_snake.GetSnake().back().m_y - 1,
                        m_manage_snake.GetSnake().back().m_val);
            ManageUpdates(&new_point);
            break;
        case 'd': //right
            SetPoint(&new_point, m_manage_snake.GetSnake().back().m_x,
                        m_manage_snake.GetSnake().back().m_y + 1,
                        m_manage_snake.GetSnake().back().m_val);
            ManageUpdates(&new_point);
            break;
        case 'z': //down
            SetPoint(&new_point, m_manage_snake.GetSnake().back().m_x + 1,
                        m_manage_snake.GetSnake().back().m_y,
                        m_manage_snake.GetSnake().back().m_val);
            ManageUpdates(&new_point);
            break;       
        default:
            break;
        }

        std::cout << "\033[2J\033[H";
        PrintBoard();
        input = getCharacter();
    }     
}

/* -------------------- */

