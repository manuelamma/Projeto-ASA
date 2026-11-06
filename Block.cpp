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

int Block::get_max_rotation()
{
    int max_rotation = 0, max_z = -1;

    for (int i = 0; i < 3; i++)
    {
        if (z > max_z)
        {
            max_z = z;
            max_rotation = i;
        }

        rotate();
    }
    return max_rotation;
}

bool Block::is_placable(const Block &b, int *rotation)
{
    for (int i = 0; i < 3; i++)
    {
        if (b.x > x && b.y > y)
            return true;

        rotate();
        (*rotation)++;
    }
    (*rotation) = 0;
    return false;
}