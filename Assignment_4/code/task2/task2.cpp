#include <cmath>
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;

unsigned int N;          // the number of constraints
unsigned int M;          // the number of variables
float z = 0;             // the optimal value
vector<float> b;         // the constraint values
vector<float> c;         // the checking numbers
vector<vector<float> > A; // the equation matrix
vector<unsigned int> Bv; // the basis vectors

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
    for (i = 0; i < Bv.size(); i++)
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
    float Ale = A[l][e]; // 暂存A[l][e]
    b[l] = b[l] / Ale;
    for (j = 0; j < A[l].size(); j++)
    {
        A[l][j] = A[l][j] / Ale;
    }
    // 2. A中其余行减去A[i][e]*Aj消元
    for (i = 0; i < A.size(); i++)
    {
        if (i == l)
            continue;
        float Aie = A[i][e]; // 暂存A[i][e]
        b[i] = b[i] - Aie * b[l];
        for (j = 0; j < A[i].size(); j++)
        {
            A[i][j] = A[i][j] - Aie * A[l][j]; // 消去A[i][e], 对第l行乘上A[i][e]
        }
    }
    // 3. 目标函数c减去c[e]*Aj消元
    float ce = c[e]; // 暂存c[e]
    z = z - c[e] * b[l];
    for (j = 0; j < c.size(); j++)
    {
        c[j] = c[j] - ce * A[l][j];
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
        for (j = 0; j < c.size(); j++)
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
        for (i = 0; i < A.size(); i++)
        {
            if (A[i][e] > 0)
            {
                tmp = b[i] / A[i][e];
                if (tmp < theta)
                {
                    theta = tmp;
                    l = i;
                }
            }
        }
        if (isinf(theta))
            return 1; // 目标函数没有上界
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
        c_original[M + N] = 1;
        c.swap(c_original);
        // 执行一次pivot操作使约束条件大于0
        pivot(M + N, l); // l是行号
        // Simplex求解
        assert(do_simplex() == 0);
        // 检查L_aux的值
        if (z == 0)
        {
            // 消除辅助变量
            for (i = 0; i < A.size(); i++)
            {
                A[i].pop_back();
            }
            // 还原目标函数
            c.swap(c_original);
            // print_table();
            // 检查目标函数中基Basis对应不为0的ck
            for (i = 0; i < Bv.size(); i++)
            {
                if (c[Bv[i]] != 0)
                {
                    // 执行高斯消元操作
                    float tmp = c[Bv[i]]; // c[Bv[i]]除以1
                    for (j = 0; j < c.size(); j++)
                    {
                        c[j] = c[j] - tmp * A[i][j];
                    }
                    z = z - tmp * b[i];
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
    float s[5][2];
    M = 6;         // 5个x + 1个d变量(x6)
    N = 5 * 2 + 4; // 5个x范围约束 + 4个d约束
    A.resize(N);
    b.resize(N, 0); // N constraints
    c.resize(M, 0); // target function
    for (i = 0; i < N; i++)
    {
        A[i].resize(M, 0);
    }

    for (i = 0; i < 5; i++)
    {
        scanf("%f %f", &s[i][0], &s[i][1]);
    }

    // x1
    A[0][0] = 1;
    b[0] = s[0][1];
    A[1][0] = -1;
    b[1] = -1.0 * s[0][0];
    // x2
    A[2][1] = 1;
    b[2] = s[1][1];
    A[3][1] = -1;
    b[3] = -1.0 * s[1][0];
    // x3
    A[4][2] = 1;
    b[4] = s[2][1];
    A[5][2] = -1;
    b[5] = -1.0 * s[2][0];
    // x4
    A[6][3] = 1;
    b[6] = s[3][1];
    A[7][3] = -1;
    b[7] = -1.0 * s[3][0];
    // x5
    A[8][4] = 1;
    b[8] = s[4][1];
    A[9][4] = -1;
    b[9] = -1.0 * s[4][0];

    // -x1+x2+d
    A[10][0] = 1;
    A[10][1] = -1;
    A[10][5] = 1;
    b[10] = 0;
    // -x2+x3+d
    A[11][1] = 1;
    A[11][2] = -1;
    A[11][5] = 1;
    b[11] = 0;
    // -x3+x4+d
    A[12][2] = 1;
    A[12][3] = -1;
    A[12][5] = 1;
    b[12] = 0;
    // -x4+x5+d
    A[13][3] = 1;
    A[13][4] = -1;
    A[13][5] = 1;
    b[13] = 0;

    // minimize -d
    c[5] = -1;

    simplex();

    vector<float> ans;
    ans.resize(M + N, 0);
    for (i=0; i<Bv.size(); i++)
    {
        ans[Bv[i]] = b[i];
    }

    for (i=0; i<5-1; i++)
    {
        printf("%.2f ", ans[i]);
    }
    printf("%.2f\n", ans[i]);

    return 0;
}
