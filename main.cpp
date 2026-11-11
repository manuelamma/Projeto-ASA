/*
 * Projeto ASA 26/27 - Grupo AL048
 * Manuel André (ist1118928) e Bruno Santos (ist1118057)
 *
 * Torre de altura máxima com programação dinâmica e Fenwick Tree, O(N log N).
 */

#include <cstdio>
#include <vector>
#include <algorithm>
#include <functional>

// Orientação de um bloco: base a x b (a <= b) e altura h.
typedef struct orient
{
    int a, b, h;
} Orient;

// Melhor torre com a orientação o no topo; prev é o bloco de baixo.
typedef struct placement
{
    struct placement *prev;
    Orient *o;
    int height;
    int n_blocks;
} Placement;

int main()
{
    int n_blocks;
    if (scanf("%d", &n_blocks) != 1)
        return 0;

    std::vector<Orient> orients;
    orients.reserve(3 * (size_t)n_blocks);

    for (int i = 0; i < n_blocks; i++)
    {
        int d[3];
        if (scanf("%d %d %d", &d[0], &d[1], &d[2]) != 3)
            return 0;

        std::sort(d, d + 3);

        // Uma orientação por cada dimensão usada como altura.
        orients.push_back({d[1], d[2], d[0]});
        orients.push_back({d[0], d[2], d[1]});
        orients.push_back({d[0], d[1], d[2]});
    }

    // a decrescente; nos empates, b crescente (impede empilhar blocos com o mesmo a).
    std::sort(orients.begin(), orients.end(),
              [](const Orient &x, const Orient &y)
              {
                  if (x.a != y.a)
                      return x.a > y.a;
                  return x.b < y.b;
              });

    size_t n_orients = orients.size();
    std::vector<Placement> dp(n_orients);

    // Valores distintos de b, por ordem decrescente.
    std::vector<int> values;
    values.reserve(n_orients);

    for (const Orient &o : orients)
        values.push_back(o.b);

    std::sort(values.begin(), values.end(), std::greater<int>());
    values.erase(std::unique(values.begin(), values.end()), values.end());

    // Fenwick Tree: índice da torre mais alta em dp, ou -1.
    std::vector<int> bit(values.size() + 1, -1);

    for (size_t i = 0; i < n_orients; i++)
    {
        Orient &o = orients[i];

        // Posições anteriores a pos têm b maior.
        int pos = std::lower_bound(values.begin(), values.end(), o.b, std::greater<int>()) - values.begin() + 1;

        // Torre mais alta onde o bloco cabe.
        int best = -1;

        for (int j = pos - 1; j > 0; j -= j & -j)
        {
            if (bit[j] != -1 && (best == -1 || dp[bit[j]].height > dp[best].height))
                best = bit[j];
        }

        if (best == -1)
            dp[i] = {nullptr, &o, o.h, 1};
        else
            dp[i] = {&dp[best], &o, dp[best].height + o.h, dp[best].n_blocks + 1};

        // Regista a nova torre na Fenwick Tree.
        for (int j = pos; j < (int)bit.size(); j += j & -j)
        {
            if (bit[j] == -1 || dp[i].height > dp[bit[j]].height)
                bit[j] = (int)i;
        }
    }

    auto max = std::max_element(dp.begin(), dp.end(),
                                [](const Placement &x, const Placement &y)
                                {
                                    return x.height < y.height;
                                });

    printf("%d %d\n", max->height, max->n_blocks);

    // prev vai do topo para a base, por isso imprime-se ao contrário.
    std::vector<Orient *> tower;
    tower.reserve(max->n_blocks);

    for (Placement *p = &*max; p != nullptr; p = p->prev)
        tower.push_back(p->o);

    for (auto it = tower.rbegin(); it != tower.rend(); ++it)
        printf("%d %d %d\n", (*it)->a, (*it)->b, (*it)->h);

    return 0;
}