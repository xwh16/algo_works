#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

unsigned int N;         // the number of people
unsigned int L;         // the weight limit of a single boat
vector<unsigned int> P; // people's weights

unsigned int solve()
{
    unsigned int i = 0;            // small ptr
    unsigned int j = P.size() - 1; // large ptr
    unsigned int ans = 0;
    while (i < j)
    {
        unsigned int p1 = P[i];
        unsigned int p2 = P[j];
        if (p1 + p2 <= L)
        {
            // 2 on the same boat
            // printf("[%d]=%d and [%d]=%d goes together\n", i, p1, j, p2);
            i++;
            j--;
        }
        else
        {
            // larger one goes alone
            // printf("[%d]=%d goes alone\n", j, p2);
            j--;
        }
        ans++;
    }
    // last guy to go
    if (i == j)
    {
        ans++;
    }
    return ans;
}

int main()
{
    unsigned int i, temp;
    scanf("%d", &N);
    scanf("%d", &L);
    for (i = 0; i < N; i++)
    {
        scanf("%d", &temp);
        P.push_back(temp);
    }
    sort(P.begin(), P.end());
    printf("%d\n", solve());
    return 0;
}