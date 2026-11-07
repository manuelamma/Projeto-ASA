#include <cstdio>
#include <vector>
#include <algorithm>

// Orientação de um tipo de bloco: base a x b (com a <= b) e altura h.
typedef struct orient
{
    int a, b, h;
} Orient;

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

        // Basta guardar a base ordenada (a <= b): se uma base cabe sobre outra
        // nalguma rotação, também cabe com ambas as bases ordenadas.
        orients.push_back({d[1], d[2], d[0]});
        orients.push_back({d[0], d[2], d[1]});
        orients.push_back({d[0], d[1], d[2]});
    }

    // Ordenar por a decrescente: qualquer orientação que possa ficar por baixo
    // de outra tem a estritamente maior, logo aparece antes no vetor.
    std::sort(
        orients.begin(),
        orients.end(),
        [](const Orient &x, const Orient &y)
        {
            return x.a > y.a;
        });

    size_t n_orients = orients.size();
    std::vector<Placement> dp(n_orients);

    for (size_t i = 0; i < n_orients; i++)
    {
        Orient &o = orients[i];

        // A torre pode começar apenas com esta orientação.
        dp[i] = {nullptr, &o, o.h, 1};

        for (size_t k = 0; k < i; k++)
        {
            const Orient &below = orients[k];

            if (o.a < below.a && o.b < below.b)
            {
                int new_height = dp[k].height + o.h;

                if (new_height > dp[i].height)
                {
                    dp[i] = {&dp[k], &o, new_height, dp[k].n_blocks + 1};
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

    printf("%d %d\n", max->height, max->n_blocks);

    // Seguir prev percorre a torre do topo para a base; guardar e imprimir ao contrário.
    std::vector<Orient *> tower;
    tower.reserve(max->n_blocks);

    for (Placement *p = &*max; p != nullptr; p = p->prev)
        tower.push_back(p->o);

    for (auto it = tower.rbegin(); it != tower.rend(); ++it)
        printf("%d %d %d\n", (*it)->a, (*it)->b, (*it)->h);

    return 0;
}
