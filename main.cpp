#include "Block.h"
#include <iostream>
#include <vector>
#include <algorithm>

typedef struct placement
{
    struct placement *prev;
    Block *b;
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
        dp[i] = {nullptr, &b, b.get_max_rotation(&rotation), rotation, 1};

        for (size_t k = 0; k < i; k++)
        {
            int current_rotation = 0;
            int new_height;

            if (b.is_placable(blocks[k], &current_rotation, &new_height))
            {
                new_height += dp[k].height;

                if (new_height > dp[i].height)
                {
                    dp[i] = {&dp[k], &blocks[i], new_height, current_rotation, dp[k].n_blocks + 1};
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

    max->b->print_rotated_block(max->rotation);

    Placement *p = max->prev;

    while (true)
    {
        if (p == nullptr)
            break;

        p->b->print_rotated_block(p->rotation);

        p = p->prev;
    }

    return 0;
}