#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <queue>

#define NMAX 1000010
// constants used in calculating the amount of iterations
#define delt 0.1
#define eps 0.02
#define ce 0.5

using namespace std;

queue <int> qq;
vector <int> adjlis[NMAX];
vector <pair<int, int> > shortp[NMAX];
int V, E, sz = 0, k;
double b[NMAX];
int dis[NMAX], sigm[NMAX];

//Estimate Diameter
int calcvdg()
{
	int vi = rand() % V;
	memset(dis, -1, sizeof dis);
	qq.push(vi);
	dis[vi] = 0;
	int mx1 = 0, mx2 = 0;
	while(!qq.empty())
	{
		int cur = qq.front();
		qq.pop();
		int i, ss = adjlis[cur].size();
		for(i = 0; i < ss; i++)
		{
			int nx = adjlis[cur][i];
			if(dis[nx] != -1)
				continue;
			dis[nx] = dis[cur] + 1;
			qq.push(nx);
			if(dis[nx] > mx1) mx2 = mx1, mx1 = dis[nx];
			else if(dis[nx] > mx2) mx2 = dis[nx];
		}
	}
	return mx1 + mx2;
}

//Calculate Shortest paths
int CompSP(int in, int fin)
{
	memset(sigm, 0, sizeof sigm);
	memset(dis, -1, sizeof dis);
	qq.push(in);
	shortp[in].clear();
	dis[in] = 0;
	sigm[in] = 1;
	while(!qq.empty())
	{
		int cur = qq.front();
		qq.pop();
		if(cur == fin) break;
		int i, ss = adjlis[cur].size();
		for(i = 0; i < ss; i++)
		{
			int nx = adjlis[cur][i];
			if(dis[nx] == dis[cur] + 1)
			{
				sigm[nx] += sigm[cur];
				shortp[nx].push_back(make_pair(sigm[nx],cur));
				continue;
			}
			else if(dis[nx] != -1)
				continue;
			shortp[nx].clear();
			shortp[nx].push_back(make_pair(sigm[cur], cur));
			sigm[nx] = sigm[cur];
			dis[nx] = dis[cur] + 1;
			qq.push(nx);
		}
	}
	
	while(!qq.empty()) qq.pop();
	return sigm[fin];
}

// Sample next node in a Shortest path
int samplesig(int u, int cur)
{
	int vl = (rand() % sigm[cur]) + 1;
	int in = 0, fin = shortp[cur].size()-1;
	while(in < fin)
	{
		int med = (in+fin)/2;
		if(shortp[cur][med].first < vl)
			in = med + 1;
		else
			fin = med;
	}
	return shortp[cur][in].second;
}

int main()
{
	srand(time(NULL));
	scanf("%d%d", &V, &E);
	int i;
	for(i = 0; i < E; i++)
	{
		int a, b;
		scanf("%d%d", &a, &b);
		adjlis[a].push_back(b);
		adjlis[b].push_back(a);
	}
	for(i = 0; i < V; i++)
		b[i] = 0;
	int vdg = calcvdg();
	int r = (ce/(eps*eps))*(floor(log2(vdg-2))+log(1.0/delt));
	printf("%d\n", r);
	for(i = 0; i < r; i++)
	{
		int u = rand() % V;
		int v = rand() % V;
		while(u == v) v = rand() % V;
		int Suv = CompSP(u, v);
		if(Suv != 0)
		{
			int w = v;
			while(w != u)
			{
				int z = samplesig(u, w);
				if(z != u)
					b[z] += 1.0/r;
				w = z;
			}
		}
	}
	for(i = 0; i < V; i++)
		printf("%d: %lf\n", i, b[i]);
}
