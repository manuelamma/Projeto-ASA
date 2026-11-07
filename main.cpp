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

    // Ordena por a decrescente e, em caso de empate, por b crescente.
    // Evita empilhar blocos com a mesma dimensão a. 
    std::sort(orients.begin(), orients.end(),
    [](const Orient &x, const Orient &y) {
        if (x.a != y.a) return x.a > y.a;
        return x.b < y.b;
    });

    size_t n_orients = orients.size();
    std::vector<Placement> dp(n_orients);

    std::vector<int> values;
    for (auto &o : orients) values.push_back(o.b);
    std::sort(values.begin(), values.end(), std::greater<int>());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    // Guarda os índices das melhores soluções de programação dinâmica.
    std::vector<int> bit(values.size() + 1, -1);

    for (size_t i = 0; i < n_orients; i++)
    {
        Orient &o = orients[i];

        int pos = std::lower_bound(values.begin(), values.end(), o.b, std::greater<int>()) - values.begin() + 1;
        // Procura a melhor torre com ambas as dimensões da base maiores.
        int best = -1;
        for (int j = pos - 1; j > 0; j -= j & -j)
            if (bit[j] != -1 && (best == -1 || dp[bit[j]].height > dp[best].height))
                best = bit[j];
        // Calcula a altura máxima da torre terminada na orientação atual.
        dp[i] = {best == -1 ? nullptr : &dp[best], &o, o.h + (best == -1 ? 0 : dp[best].height), 1 + (best == -1 ? 0 : dp[best].n_blocks)};
        for (int j = pos; j < (int)bit.size(); j += j & -j)
            if (bit[j] == -1 || dp[i].height > dp[bit[j]].height) bit[j] = (int)i;
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