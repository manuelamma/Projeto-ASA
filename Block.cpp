#include <iostream>
#include <sstream>
#include "Block.h"

Block::Block(const std::string &s_x, const std::string &s_y, const std::string &s_z)
{
    x = std::stoi(s_x);
    y = std::stoi(s_y);
    z = std::stoi(s_z);
}

void Block::rotate()
{
    int tmp_x = x;
    int tmp_y = y;
    x = z;
    z = tmp_y;
    y = tmp_x;
}

int Block::get_max_rotation(int *rotaion)
{
    int max_z = -1;

    for (int i = 0; i < 3; i++)
    {
        if (z > max_z)
        {
            max_z = z;
            (*rotaion) = i;
        }

        rotate();
    }
    return max_z;
}

bool Block::is_placable(const Block &b, int *rotation, int *new_z)
{
    for (int i = 0; i < 3; i++)
    {
        if (b.x > x && b.y > y)
        {
            (*new_z) = z;
            return true;
        }

        rotate();
        (*rotation)++;
    }
    (*rotation) = 0;
    return false;
}

void Block::print_rotated_block(int rotation)
{
    switch (rotation)
    {
    case 0:
        std::cout << x << ' ' << y << ' ' << z << std::endl;
        break;

    case 1:
        std::cout << y << ' ' << z << ' ' << x << std::endl;
        break;

    case 3:
        std::cout << z << ' ' << x << ' ' << y << std::endl;
        break;

    default:
        break;
    }
}