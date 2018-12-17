#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int T; // the number of input groups
vector<unsigned int> N;

unsigned int solve(unsigned int len)
{
    unsigned int i, j;
    unsigned int maxVal = 1;
    vector<unsigned int> d;
    d.resize(len, 1);
    for (i = 0; i < len; i++)
    {
        unsigned int ai, aj;
        ai = N[i];
        for (j = 0; j < i; j++)
        {
            aj = N[j];
            if (aj < ai)
            {
                d[i] = (d[j] + 1) > d[i] ? (d[j] + 1) : d[i];
                maxVal = d[i] > maxVal ? d[i] : maxVal;
            }
        }
        // printf("d[%d]=%d\n", ai, d[i]);
    }
    return maxVal;
}

// use binary search to reduce time
// comlexity of the inner loop to O(nlogn)
unsigned int solve2(unsigned int len)
{
    unsigned int i;
    unsigned int maxVal = 1;
    vector<unsigned int> d;
    // 数组d保存的是长度为k的序列
    // 该序列位于末尾位置的元素最小
    // 的取值, 数组d本身的序列不具
    // 实际意义
    for (i = 0; i < len; i++)
    {
        unsigned int ai, aj;
        unsigned int left, mid, right;
        if (d.size() == 0)
        {
            d.push_back(N[i]);
        }
        else
        {
            vector<unsigned int>::iterator it = lower_bound(d.begin(), d.end(), N[i]);
            if (it == d.end()) {
                d.push_back(N[i]);
            }
            else {
                *it = N[i];
            }
        }
        // printf("d[%d]=%d\n", ai, d[i]);
        // printf("LIS[%d]: ", d.size());
        // copy(d.begin(), d.end(), ostream_iterator<unsigned int>(cout, " "));
        // printf("\n");
    }
    return d.size();
}

int main()
{
    unsigned int i, j;
    unsigned int len; // the length of sequence
    unsigned int temp;
    scanf("%d", &T);
    for (i = 0; i < T; i++)
    {
        scanf("%d", &len);
        for (j = 0; j < len; j++)
        {
            scanf("%d", &temp);
            N.push_back(temp);
        }
        printf("%d\n", solve2(len));
        N.clear();
    }

    return 0;
}