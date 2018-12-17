#include <cmath>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

unsigned int N;          // the event takes N days to complete
unsigned int M;          // the number of kinds of volunteers
float z = 0;             // the optimal value
vector<float> b;         // the demands for volunteers
vector<float> c;         // the recruit fee for volunteers
vector<vector<float> > A; // the available shedule (N x M) for volunteers
vector<unsigned int> Bv; // the basis vector

void print_table()
{
    unsigned int i, j;
    printf("=================================>\n");
    printf("      ");
    for (i = 0; i < c.size(); i++)
    {
        printf("x%-7d ", i);
    }
    printf("\n");

    printf("c   ");
    for (i = 0; i < c.size(); i++)
    {
        printf("%8.4f ", c[i]);
    }
    printf("| %8.4f ", z);
    printf("\n");

    for (i = 0; i < A.size(); i++)
    {
        printf("x%-2d ", Bv[i]);
        for (j = 0; j < A[i].size(); j++)
        {
            printf("%8.4f ", A[i][j]);
        }
        printf("| %8.4f ", b[i]);
        printf("\n");
    }

    printf("Basis=(");
    for (i=0; i<Bv.size(); i++)
    {
        printf(" x%d ", Bv[i]);
    }
    printf(")\n");
    printf("<=================================\n");
}

// 在多项式的角度, 用第l行去消去e变量
inline int pivot(int e, int l)
{
    // printf("\nPivot: using non-basis e:x%d to replace l:%d,x%d\n", e, l, Bv[l]);
    // print_table();

    unsigned int i, j;
    // 1. 将A[l][e]变为单位1
    //    Scaling缩放操作
    float Ale = A[l][e];    // 暂存A[l][e]
    b[l] = b[l] / Ale;
    for (j = 0; j < A[l].size(); j++)
    {
        A[l][j] = A[l][j] / Ale;
    }
    // 2. A中其余行减去A[i][e]*Aj消元
    for (i=0; i<A.size(); i++)
    {
        if (i == l) continue;
        float Aie = A[i][e];    // 暂存A[i][e]
        b[i] = b[i] - Aie*b[l];
        for (j=0; j<A[i].size(); j++)
        {
            A[i][j] = A[i][j] - Aie*A[l][j];    // 消去A[i][e], 对第l行乘上A[i][e]
        }
    }
    // 3. 目标函数c减去c[e]*Aj消元
    float ce = c[e];    // 暂存c[e]
    z = z - c[e]*b[l];
    for (j=0; j<c.size(); j++)
    {
        c[j] = c[j] - ce*A[l][j];
    }
    // 4. 更新基向量与非基向量
    //    将Bv中的e替换为l
    Bv[l] = e;

    // print_table();

    return 0;
}

int do_simplex()
{
    unsigned int i, j;
    while (true)
    {
        unsigned int e, l, mark = 0;
        // 1. 寻找进入变量e
        for (j=0; j<c.size(); j++)
        {
            if (c[j] < 0) 
            {
                mark = 1;
                e = j;
                break;
            }
        }
        // 没有系数为负的ck
        if (!mark)
        {
            // 输出最优解
            // printf("Found optimal solution!\n");
            return 0;
        }
        // 2. 在Ae中根据Bland原则挑选退出变量l
        float theta = INFINITY, tmp;
        for (i=0; i<A.size(); i++)
        {
            if (A[i][e] > 0)
            {
                tmp = b[i]/A[i][e];
                if (tmp < theta)
                {
                    theta = tmp;
                    l = i;
                }
            }
        }
        if (isinf(theta))
            return 1;   // 目标函数没有上界
        // 3. 执行Pivot操作
        pivot(e, l);
    }
}

int init_simplex()
{
    // 初始化稀疏矩阵
    unsigned int i, j;
    for (i = 0; i < A.size(); i++)
    {
        A[i].resize(M + N, 0);
        A[i][M + i] = 1;
    }
    c.resize(M + N, 0);
    // 初始向量基
    for (i = M; i < M + N; i++)
    {
        Bv.push_back(i);
    }
    
    // 首先找到约束集中b最小的项
    unsigned int l; // l标记最小项下标
    float min_b = 0;
    for (i = 0; i < b.size(); i++)
    {
        if (b[i] < min_b)
        {
            min_b = b[i];
            l = i;
        }
    }

    // printf("Slack matrix:\n");
    // print_table();
    // printf("minimum b_%d=%f\n", l, min_b);

    // 1.0 基本解是可行解
    if (min_b >= 0)
    {
        return 0;
    }
    // 1.1 基本解不是可行解
    else
    {
        vector<float> c_original;
        // 构造辅助L_aux
        for (i = 0; i < A.size(); i++)
        {
            // 增加-x0项至末尾
            A[i].push_back(-1);
        }
        // 替换L_aux的目标函数
        c_original.resize(M + N + 1, 0);
        c_original[M+N] = 1;
        c.swap(c_original);
        // 执行一次pivot操作使约束条件大于0
        pivot(M+N, l);  // l是行号
        // Simplex求解
        assert(do_simplex() == 0);
        // 检查L_aux的值
        if (z == 0)
        {
            // 消除辅助变量
            for (i=0; i<A.size(); i++)
            {
                A[i].pop_back();
            }
            // 还原目标函数
            c.swap(c_original);
            // print_table();
            // 检查目标函数中基Basis对应不为0的ck
            for (i=0; i<Bv.size(); i++)
            {
                if (c[Bv[i]] != 0)
                {
                    // 执行高斯消元操作
                    float tmp = c[Bv[i]];   // c[Bv[i]]除以1
                    for (j=0; j<c.size(); j++)
                    {
                        c[j] = c[j] - tmp*A[i][j];
                    }
                    z = z - tmp*b[i];
                }
            }
            // print_table();
            return 0;
        }
        else
        {
            // 没有可行解
            return 1;
        }
    }
}

int simplex()
{
    // printf("Initializing Simplex.\n");
    assert(init_simplex() == 0);

    // printf("Solving Simplex.\n");
    assert(do_simplex() == 0);

    return (-1.0 * z);
}

int main()
{
    unsigned int i, d;
    scanf("%d %d", &N, &M);
    A.resize(N);
    b.resize(N); // N constraints
    c.resize(M); // target function
    for (i = 0; i < N; i++)
    {
        A[i].resize(M, 0);
        scanf("%d", &d);
        b[i] = -1.0 * d;
    }
    for (i = 0; i < M; i++)
    {
        unsigned int Si, Fi, Ci;
        scanf("%d %d %d", &Si, &Fi, &Ci);
        c[i] = Ci;
        for (d = Si - 1; d < Fi && d < N; d++)  // TODO: ??? 啥玩意
        {
            A[d][i] = -1;
        }
    }

    printf("%.0f\n", abs((float)simplex()));

    return 0;
}
