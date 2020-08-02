#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <limits>
#include <math.h>
#include <unordered_set>
#include <chrono>

#define NMAX 5000010
// constants used to calculate the amount of iterations
#define niu 100.0
#define eps 1.0/100.0

using namespace std;

unordered_set <int> adjset[NMAX];
vector <int> adjlis[NMAX];
int V, E, k;
long long int W2[NMAX];

int EDGE_EXISTS(int w, int u)
{
	return (adjset[u].find(w) != adjset[u].end());
}

// Algorithm using all weights equal to 1
double alg1()
{
	int i, l = 0;
	for(i = 0; i < k; i++)
	{
		int j = rand() % V;
		if((int)adjlis[j].size() < 2)
			continue;
		int u = rand() % ((int)adjlis[j].size());
		int w = rand() % ((int)adjlis[j].size());
		while(w == u)  w = rand() % ((int)adjlis[j].size());
		if(EDGE_EXISTS(adjlis[j][w], adjlis[j][u])) 
			l++;
	}
	
	return (double)l/(double)k;
}

// Binary search used for sampling with desired distribution
int find_index(long long int vl)
{
	int in = 0, fin = V-1;
	while(in < fin)
	{
		int med = (in+fin)/2;
		if(W2[med] < vl)
			in = med+1;
		else
			fin = med;
	}
	return in;
}

//algorithm using weights equal to degree*(degree-1)/2
double alg2()
{
	random_device rd;
	mt19937_64 eng(rd());
	uniform_int_distribution<unsigned long long> distr;
	int i, l = 0;
	for(i = 0; i < k; i++)
	{
		long long int r = (distr(eng) % W2[V-1])+1;
		int j = find_index(r);
		int u = rand() % ((int)adjlis[j].size());
		int w = rand() % ((int)adjlis[j].size());
		while(w == u)  w = rand() % ((int)adjlis[j].size());
		if(EDGE_EXISTS(adjlis[j][w], adjlis[j][u])) l++;
	}
	return (double)l/k;
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
		if(adjset[a].find(b) != adjset[a].end())
			continue;
		adjlis[a].push_back(b);
		adjlis[b].push_back(a);
		adjset[a].insert(b);
		adjset[b].insert(a);
	}
	auto t1 = std::chrono::high_resolution_clock::now();
    k = ceil(log(2.0*niu)/(2.0*eps*eps));
	double coef1 = alg1();
	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
	printf("Average CC: %.15lf\n", coef1);
	printf("%ld micro seconds\n", duration);
	for(i = 0; i < V; i++) W2[i] = (long long int)adjlis[i].size()*(adjlis[i].size()-1)/2LL;
	for(i = 1; i < V; i++) W2[i] += W2[i-1];
	double coef2 = alg2();
	auto t3 = std::chrono::high_resolution_clock::now();
	auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t2 ).count();
	printf("Global CC: %.15lf\n", coef2);
	printf("%ld micro seconds\n", duration2);
}
