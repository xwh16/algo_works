#include <cmath>
#include <cassert>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

#define MAX 0x7FFFFFFF

using namespace std;

typedef struct
{
    unsigned int id;
    unsigned int dest;
    unsigned int capacity;
    unsigned int mark; // 0 for normal, 1 for residual
} Edge;

unsigned int N;         // the number of vertices
unsigned int M;         // the number of edges
vector<vector<Edge> > G; // the capacity graph
vector<vector<Edge> > Gf; // the residual capacity graph
vector<unsigned int> F; // the flow on edges

unsigned int S, T; // source S and sink T

unsigned int print_path(vector<unsigned int> *path)
{
    unsigned int i;
    for (i = 0; i < path->size(); i++)
    {
        printf("%d ", (*path)[i]);
    }
    printf("\n");
}

vector<unsigned int> *find_path(unsigned int source, unsigned int sink)
{
    unsigned int i, j;
    vector<unsigned int> nodes;
    vector<unsigned int> discovered;
    vector<unsigned int> pred;
    vector<unsigned int> *path = new vector<unsigned int>();
    nodes.push_back(source);
    discovered.resize(N, 0);
    pred.resize(N, 0);
    while (!nodes.empty())
    {
        unsigned int cur = nodes.back();
        nodes.pop_back();
        printf("==> dfs at node %d\n", cur);
        if (!discovered[cur])
        {
            discovered[cur] = 1;
            for (i = 0; i < G[cur].size(); i++)
            {
                Edge edge = G[cur][i];
                if (discovered[edge.dest])
                    continue;
                unsigned int flow = F[edge.id];
                unsigned int residue = edge.capacity - flow;
                printf("Edge %d-%d, cap=%d flow=%d\n", cur, edge.dest, edge.capacity, F[edge.id]);
                if (edge.mark == 0 && residue == 0)
                {
                    // this is a normal path
                    continue;
                }
                else if (edge.mark == 1 && flow == 0)
                {
                    // this is a residual path
                    continue;
                }
                nodes.push_back(edge.dest);
                pred[edge.dest] = cur;
                if (edge.dest == sink)
                {
                    nodes.clear();
                    break;
                }
            }
        }
    }
    printf("<== dfs done\n");
    unsigned int cur = sink;
    while (true)
    {
        // printf("cur=%d\n", cur);
        path->insert(path->begin(), cur);
        if (cur == source)
            break;
        cur = pred[cur];
    }
    return path;
}

unsigned int augment(vector<unsigned int> *path, unsigned int flow)
{
    unsigned int i, j;
    vector<unsigned int> &p = *path;
    vector<unsigned int> e(p.size());
    // 1. find the bottleneck of path p
    unsigned int b = MAX;
    for (i = 0; i < p.size() - 1; i++)
    {
        unsigned int u, v;
        u = p[i];
        v = p[i + 1];
        for (j = 0; j < G[u].size(); j++)
        {
            if (v == G[u][j].dest)
                break;
        }
        e[i] = j;
        Edge edge = G[u][j];
        if (edge.mark == 0)
        {
            // a normal path
            b = min(b, edge.capacity - F[edge.id]);
        }
        else if (edge.mark == 1)
        {
            // a residual path
            b = min(b, F[edge.id]);
        }
        printf("Edge %d-%d, cap=%d flow=%d\n", u, v, edge.capacity, F[edge.id]);
    }
    printf("b=%d\n", b);
    // 2. augment flow on path by (b)
    for (i = 0; i < p.size() - 1; i++)
    {
        unsigned int u;
        u = p[i];
        Edge edge = G[u][e[i]];
        if (edge.mark == 0)
        {
            // a normal path
            F[edge.id] += b;
        }
        else if (edge.mark == 1)
        {
            // a residual path
            F[edge.id] -= b;
        }
    }
    return flow + b;
}

unsigned int ford_fulkerson()
{
    unsigned int i, j;
    unsigned int flow = 0;
    F.resize(0); // initialize f(e)=0
    do
    {
        // 1. find a s-t path in residual grah Gf
        //    using DFS
        vector<unsigned int> *path = find_path(S - 1, T - 1);
        print_path(path);
        if (path->size() == 0)
            break;
        // 2. augment flow with selected s-t path
        flow = augment(path, flow);
        printf("MaxFlow=%d\n", flow);
        getchar();
    } while (true);
    return flow;
}

int main()
{
    unsigned int i, j;

    scanf("%d %d %d %d", &N, &M, &S, &T);
    G.resize(N);
    F.resize(M, 0);
    for (i = 0; i < M; i++)
    {
        unsigned int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        G[u - 1].push_back({i, v - 1, c, 0});
        G[v - 1].push_back({i, u - 1, c, 1});
    }

    ford_fulkerson();

    return 0;
}
