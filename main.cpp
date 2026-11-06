#include "Block.h"
#include <iostream>
#include <vector>
#include <algorithm>

typedef struct
{
    Block *prev;
    int height;
    int rotation;

} Placement;

int main()
{
    size_t n_blocks;
    std::cin >> n_blocks;
    std::vector<Block> blocks;
    blocks.reserve(n_blocks);
    std::vector<Block> torre;
    std::vector<Placement> dp;

    std::string s_x, s_y, s_z;
    for (size_t i = 0; i < n_blocks; i++)
    {
        std::cin >> s_x;
        std::cin >> s_y;
        std::cin >> s_z;
        Block b(s_x, s_y, s_z);
        if (i == 0)
        {
            dp[i] = {&b, b.get_z(), b.get_max_rotation()};
        }
        int max_height = -1;
        int current_rotation = 0;
        for (size_t k = 0; k < i; k++)
        {
            if (blocks[k].is_placable(b, &current_rotation) && dp[k].height > max_height)
            {
                dp[i] = {
                    &b, dp[k].height + b.get_z(), current_rotation};
                max_height = dp[i].height + b.get_z();
            }
        }
    }
    Placement maior = *std::max_element(
        dp.begin(),
        dp.end(),
        [](const Placement &a, const Placement &b)
        {
            return a.height < b.height;
        });

    std::cout << maior.height << '\n'; // 15

    return 0;
}