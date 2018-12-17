#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <vector>
#include <limits>
#include <iterator>
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int m, n, topK;
vector<vector<unsigned int > > grid;

int solve()
{
	unsigned int i, j, k;
	vector<vector<unsigned int> > score1;
	vector<vector<unsigned int> > score2;
	vector<vector<unsigned int> > &p1 = score1;
	vector<vector<unsigned int> > &p2 = score2;
	score1.resize(n);
	score2.resize(n);
	score1[0].push_back(grid[0][0]);
	for (j = 1; j < n; j++)
	{
		score1[j].push_back(grid[0][j] + score1[j-1][0]);
	}
	for (i = 1; i < m; i++)
	{
		p2[0].clear();
		p2[0].push_back(p1[0][0] + grid[i][0]);
		for (j = 1; j < n; j++)
		{
			p2[j].clear();
			// consider its left grid
			for (k = 0; k < p2[j - 1].size(); k++)
			{
				p2[j].push_back(p2[j - 1][k] + grid[i][j]);
			}
			// then its top grid
			for (k = 0; k < p1[j].size(); k++)
			{
				p2[j].push_back(p1[j][k] + grid[i][j]);
			}
			sort(p2[j].begin(), p2[j].end());
			// memory pruning
			if (p2[j].size() > topK)
			{
				p2[j].erase(p2[j].begin(), p2[j].begin() + p2[j].size() - topK);
			}
		}
		swap(p1, p2);
	}
	sort(p1[n - 1].begin(), p1[n - 1].end());
	for (i = 0; i < topK; i++)
	{
		printf("%d", p1[n - 1][p1[n-1].size() - 1 - i]);
		if (i < topK - 1)
			printf(" ");
	}
	return 0;
}

int main()
{
	unsigned int i, j;
	scanf("%d %d\n%d", &m, &n, &topK);
	grid.resize(m);
	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			unsigned int temp;
			scanf("%d", &temp);
			grid[i].push_back(temp);
		}
	}
	solve();
	return 0;
}