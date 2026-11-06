#include "Block.h"
#include <iostream>
#include <vector>
#include <algorithm>

typedef struct
{
    Block *prev;
    int height;
    int rotation;
    int n_blocks;
} Placement;

int main()
{
    size_t n_blocks;
    std::cin >> n_blocks;
    std::vector<Block> blocks;
    blocks.reserve(n_blocks);
    std::string s_x, s_y, s_z;

    std::vector<Placement> dp(n_blocks);

    for (size_t i = 0; i < n_blocks; i++)
    {
        std::cin >> s_x >> s_y >> s_z;

        blocks.emplace_back(s_x, s_y, s_z);
        Block &b = blocks.back();

        int rotation;

        // A torre pode começar apenas com este bloco.
        dp[i] = {nullptr, b.get_max_rotation(&rotation), rotation, 1};

        for (size_t k = 0; k < i; k++)
        {
            int current_rotation = 0;

            if (b.is_placable(blocks[k], &current_rotation))
            {
                int new_height = dp[k].height + b.get_z();

                if (new_height > dp[i].height)
                {
                    dp[i] = {&blocks[k], new_height, current_rotation, dp[k].n_blocks + 1};
                }
            }
        }
    }

    auto max = std::max_element(
        dp.begin(),
        dp.end(),
        [](const Placement &a, const Placement &b)
        {
            return a.height < b.height;
        });

    std::cout << max->height << " " << max->n_blocks << std::endl;

    return 0;
}