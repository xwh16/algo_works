#include <cmath>
#include <cassert>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>

#define MAX 0x7FFFFFFF

using namespace std;

unsigned int N;                  // the number of vertices
unsigned int M;                  // the number of edges
vector<vector<unsigned int> > G;  // the capacity graph
vector<vector<unsigned int> > Gf; // the residual capacity graph
vector<vector<unsigned int> > F;          // the flow on edges

// the layered network
vector<unsigned int> Vf;        // vector set
vector<vector<unsigned int> > Ef;   // edge set

unsigned int S, T; // source S and sink T

bool build_network()
{
    unsigned int i, w;
    vector<unsigned int> df(N); // store node layer number
    queue<unsigned int> Q;  // bfs queue
    // initialize BFS
    Q.push(S);
    for (i=1; i<N; i++)
    {
        df[i] = MAX;
    }
    // initialize layered network
    Vf.clear();
    Ef.clear();
    Ef.resize(N);
    Vf.push_back(S);
    while (!Q.empty())
    {
        unsigned int v = Q.front(); 
        Q.pop();
        for (w=0; w<N; w++)
        {
            if (Gf[v][w] > 0)   // a valid edge in Gf
            {
                if (df[w] == MAX)   // unvisited in BFS
                {
                    Q.push(w);
                    df[w] = df[v] + 1;
                    Vf.push_back(w);
                    Ef[v].push_back(w);
                }
                else if (df[w] == df[v] + 1) // visited
                {
                    Ef[v].push_back(w);
                }
            }
        }
    }
    return df[T] < MAX;
}

unsigned int dfs(vector<unsigned int> *pred, unsigned int source)
{
    unsigned int i;
    pred->push_back(source);
    if (Ef[source].size() == 0)
        return 1;
    return dfs(pred, Ef[source][0]) + 1;
}

unsigned int augment(vector<unsigned int> *path)
{
    unsigned int i, r;
    unsigned int b = MAX;
    vector<unsigned int> &p = *path;
    // find bottleneck edge
    for (i=0; i<path->size()-1; i++)
    {
        unsigned int u = p[i];
        unsigned int v = p[i+1];
        if (Gf[u][v] < b)
        {
            b = Gf[u][v];
            r = i;  // (r, r+1) is the bottoleneck edge
        }
    }
    // augment flow by b on p
    for (i=0; i<path->size()-1; i++)
    {
        unsigned int u = p[i];
        unsigned int v = p[i+1];
        F[u][v] += b;
        Gf[u][v] -= b;
        Gf[v][u] += b;
    }
    // remove bottleneck edge
    for (i=0; i<Ef[p[r]].size(); i++)
    {
        if (Ef[p[r]][i] == p[r+1])
        {
            Ef[p[r]].erase(Ef[p[r]].begin()+i);
            break;
        }
    }
    return b;
}


unsigned int blocking_flow()
{
    unsigned int i, j;
    unsigned int bf = 0;
    while (!Ef[S].empty())
    {
        vector<unsigned int> path;
        // using dfs to find a s-t path in Nf
        dfs(&path, S-1);
        if (path.back() == S-1)
        {
            // p is a s-t path in Nf
            // augment p using bf
            bf += augment(&path);
        }
        else
        {
            // delete last node in p
            Ef[path.back()].clear();
        }
    }
    return bf;
}

unsigned int dinic()
{
    unsigned int i, j;
    unsigned int flow = 0;
    do
    {
        // 1. construct a layered network
        //    using BFS
        bool reachable = build_network();
        if (!reachable)
            break;
        // 2. find a blocking flow bf in Nf
        //    using DFS 
        flow += blocking_flow();
    } while (true);
    return flow;
}

int main()
{
    unsigned int i, j;

    scanf("%d %d %d %d", &N, &M, &S, &T);

    G.resize(N);
    Gf.resize(N);
    F.resize(N);
    for (i = 0; i < N; i++)
    {
        G[i].resize(N, 0);
        Gf[i].resize(N, 0);
        F[i].resize(N, 0); // initialize f(e)=0
    }

    for (i = 0; i < M; i++)
    {
        unsigned int u, v, c;
        scanf("%d %d %d", &u, &v, &c);
        G[u - 1][v - 1] = c;
        Gf[u - 1][v - 1] = c;
    }

    printf("%d\n", dinic());

    return 0;
}
