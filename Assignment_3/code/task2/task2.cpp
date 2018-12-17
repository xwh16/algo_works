#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <istream>
#include <algorithm>

using namespace std;

vector<unsigned int> monkeys;
vector<unsigned int> bananas;

unsigned int solve()
{
    unsigned int i, move;
    unsigned int ans = 0;
    for (i = 0; i < monkeys.size(); i++)
    {
        move = abs((int)(monkeys[i] - bananas[i]));
        // printf("Assign monkey[%d] to banana[%d]. (%d second)\n",
        //        monkeys[i], bananas[i], move);
        ans = max(ans, move);
    }
    return ans;
}

int main()
{
    unsigned int i, temp;
    do
    {
        scanf("%d", &temp);
        monkeys.push_back(temp);
    } while (getchar() != '\n');
    do
    {
        scanf("%d", &temp);
        bananas.push_back(temp);
    } while (getchar() != '\n');
    sort(monkeys.begin(), monkeys.end());
    sort(bananas.begin(), bananas.end());
    printf("%d\n", solve());
    return 0;
}
