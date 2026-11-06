#ifndef BLOCK_H
#define BLOCK_H

#include <string>

class Block
{
private:
    int x, y, z;

public:
    Block(const std::string &s_x, const std::string &s_y, const std::string &s_z);

    int get_x() { return x; }
    int get_y() { return y; }
    int get_z() { return z; }

    void rotate();

    bool is_placable(const Block &b, int *rotation);

    int get_max_rotation(int *rotation);
};

#endif