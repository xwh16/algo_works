// C++ implementation of Dinic's Algorithm
#include <bits/stdc++.h>
using namespace std;

// A structure to represent a edge between
// two vertex
struct Edge
{
    int v; // Vertex v (or "to" vertex)
           // of a directed edge u-v. "From"
           // vertex u can be obtained using
           // index in adjacent array.

    int flow; // flow of data in edge

    int C; // capacity

    int rev; // To store index of reverse
             // edge in adjacency list so that
             // we can quickly find it.
};

// Residual Graph
class Graph
{
    int V;      // number of vertex
    int *level; // stores level of a node
    vector<Edge> *adj;

  public:
    Graph(int V)
    {
        adj = new vector<Edge>[V];
        this->V = V;
        level = new int[V];
    }

    // add edge to the graph
    void addEdge(int u, int v, int C)
    {
        // Forward edge : 0 flow and C capacity
        Edge a{v, 0, C, adj[v].size()};

        // Back edge : 0 flow and 0 capacity
        Edge b{u, 0, 0, adj[u].size()};

        adj[u].push_back(a);
        adj[v].push_back(b); // reverse edge
    }

    bool BFS(int s, int t);
    int sendFlow(int s, int flow, int t, int ptr[]);
    int DinicMaxflow(int s, int t);
};

// Finds if more flow can be sent from s to t.
// Also assigns levels to nodes.
bool Graph::BFS(int s, int t)
{
    for (int i = 0; i < V; i++)
        level[i] = -1;

    level[s] = 0; // Level of source vertex

    // Create a queue, enqueue source vertex
    // and mark source vertex as visited here
    // level[] array works as visited array also.
    list<int> q;
    q.push_back(s);

    vector<Edge>::iterator i;
    while (!q.empty())
    {
        int u = q.front();
        q.pop_front();
        for (i = adj[u].begin(); i != adj[u].end(); i++)
        {
            Edge &e = *i;
            if (level[e.v] < 0 && e.flow < e.C)
            {
                // Level of current vertex is,
                // level of parent + 1
                level[e.v] = level[u] + 1;

                q.push_back(e.v);
            }
        }
    }

    // IF we can not reach to the sink we
    // return false else true
    return level[t] < 0 ? false : true;
}

// A DFS based function to send flow after BFS has
// figured out that there is a possible flow and
// constructed levels. This function called multiple
// times for a single call of BFS.
// flow : Current flow send by parent function call
// start[] : To keep track of next edge to be explored.
//		 start[i] stores count of edges explored
//		 from i.
// u : Current vertex
// t : Sink
int Graph::sendFlow(int u, int flow, int t, int start[])
{
    // Sink reached
    if (u == t)
        return flow;

    // Traverse all adjacent edges one -by - one.
    for (; start[u] < adj[u].size(); start[u]++)
    {
        // Pick next edge from adjacency list of u
        Edge &e = adj[u][start[u]];

        if (level[e.v] == level[u] + 1 && e.flow < e.C)
        {
            // find minimum flow from u to t
            int curr_flow = min(flow, e.C - e.flow);

            int temp_flow = sendFlow(e.v, curr_flow, t, start);

            // flow is greater than zero
            if (temp_flow > 0)
            {
                // add flow to current edge
                e.flow += temp_flow;

                // subtract flow from reverse edge
                // of current edge
                adj[e.v][e.rev].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}

// Returns maximum flow in graph
int Graph::DinicMaxflow(int s, int t)
{
    // Corner case
    if (s == t)
        return -1;

    int total = 0; // Initialize result

    // Augment the flow while there is path
    // from source to sinkS
    while (BFS(s, t) == true)
    {
        // store how many edges are visited
        // from V { 0 to V }
        int *start = new int[V + 1];

        // while flow is not zero in graph from S to D
        while (int flow = sendFlow(s, INT_MAX, t, start))

            // Add path flow to overall flow
            total += flow;
    }

    // return maximum flow
    return total;
}

int N; // the size of chessboard
int M; // the number of obstacles

int onBoard(int x, int y)
{
    return x >= 0 && x < N && y >= 0 && y < N; 
}

// Driver program to test above functions
int main()
{
    int i, j, k;
    vector<vector<int> > broken;
    vector<vector<int> > nodeID;

    scanf("%d %d", &N, &M);
    broken.resize(N);
    nodeID.resize(N);
    for (i = 0; i < N; i++)
    {
        broken[i].resize(N, 0);
        nodeID[i].resize(N, 0);
    }

    for (i = 0; i < M; i++)
    {
        int x, y;
        scanf("%d %d", &x, &y);
        broken[x-1][y-1] = 1;
    }

    int odd = 0, even = 0;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (!broken[i][j] && (i+j)&1)
                nodeID[i][j] = ++odd;
            else if (!broken[i][j] && (i+j)%2==0)
                nodeID[i][j] = ++even;
        }
    }
    int S = 0, T = odd + even + 1;
    Graph g(T+1);
    int move[8][2] = {
        -2, -1,
        -2,  1,
         2, -1,
         2,  1,
        -1, -2,
        -1,  2,
         1, -2,
         1,  2
    };
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if ((i+j)&1 && !broken[i][j])
            {
                // 奇点集
                // printf("Edge: %d-%d\n", S, nodeID[i][j]);
                g.addEdge(S, nodeID[i][j], 1);
                for (k = 0; k < 8; k++)
                {
                    int x = i + move[k][0];
                    int y = j + move[k][1];
                    if (onBoard(x,y) && (x+y)%2==0 && !broken[x][y])
                    {
                        // printf("Mid Edge: %d-%d\n", nodeID[i][j], nodeID[x][y]+odd);
                        g.addEdge(nodeID[i][j], nodeID[x][y]+odd, INT_MAX);
                    }
                }
            }
            else if ((i+j)%2==0 && !broken[i][j])
            {
                // 偶点集
                // printf("Edge: %d-%d\n", nodeID[i][j]+odd, T);
                g.addEdge(nodeID[i][j]+odd, T, 1);
            }
        }
    }
    // printf("T=%d\n",T);
    cout << N*N - M - g.DinicMaxflow(S, T) << endl;
    return 0;
}
