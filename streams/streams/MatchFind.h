#pragma once
#include<iostream>
#include<vector>
#include<queue>
class MatchFind {
public:
	void prepare(int n) {
		graph.clear();
		graph.resize(n);
		match.clear();
		match.resize(n);
		match.assign(n, -1);
		notdeleted.assign(n, true);
		firstnot = 0;
		cnt = n;
	}

	void addEdge(int from, int to) {
		graph[from].push_back(to);
		graph[to].push_back(from);
		/*if (match[from] == -1 && match[to] == -1)
		{
		match[from] = to;
		match[to] = from;
		}*/
	}

	void precalc() {
		for (int i = 0; i < cnt; ++i) {
			if (match[i] == -1)
				for (int j = graph[i].size() - 1; j >= 0; --j)
					if (match[graph[i][j]] == -1) {
						match[i] = graph[i][j];
						match[graph[i][j]] = i;
						break;
					}
		}
	}

	long long calcMatch() {
		long long ans = 0;
		bool q = true;
		precalc();
		while (q) {
			q = false;
			for (int i = firstnot; i < cnt; ++i) {
				if (match[i] == -1 && notdeleted[i]) {
					used.assign(cnt, false);
					notdeleted[i] = dfs(i, 0, -1);
					q = q || notdeleted[i];
				}
				if (!q) {
					++firstnot;
				}
			}
		}
		for (int i = 0; i < cnt; ++i)
			if (match[i] != -1) ++ans;
		return ans / 2;
	}

	bool dfs(int vertex, short d, int par) {
		bool q = false;
		used[vertex] = true;
		if (d) {
			q = dfs(match[vertex], 0, vertex);
			if (!q) return false;
			else {
				match[vertex] = par;
				match[par] = vertex;
				return true;
			}
		}
		else {
			for (size_t i = 0; i < graph[vertex].size(); ++i) {
				if (match[graph[vertex][i]] == -1) {
					match[vertex] = graph[vertex][i];
					match[graph[vertex][i]] = vertex;
					return true;
				}
				else {
					if (used[graph[vertex][i]] == false)
						q = dfs(graph[vertex][i], 1, vertex);
					if (q) {
						match[vertex] = graph[vertex][i];
						match[graph[vertex][i]] = vertex;
						return true;
					}
				}
			}
		}
		return false;
	}
private:
	std::vector<std::vector<int > > graph;
	std::vector<int > match;
	std::vector<bool> used;
	std::vector<bool> notdeleted;
	size_t firstnot = 0;
	size_t cnt;
};

#define INF 1000000000



void startB() {
	long long n, a, b, m, k = 0, ans = 0;
	char c;
	MatchFind mf;
	std::vector<std::vector<long> > vect;
	std::cin >> n >> m >> a >> b;
	vect.resize(n);
	for (int i = 0; i < n; ++i) {
		vect[i].assign(m, -1);
	}

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
		{
			std::cin >> c;
			if (c == '*') {
				vect[i][j] = k;
				++k;
			}
		}

	/*
	for (int i = 0; i < n; ++i) {
	for (int j = 0; j < m; ++j)
	std::cout << vect[i][j];
	std::cout << std::endl;
	}*/



	if (a >= 2 * b) {
		std::cout << k * b;
		return;
	}
	mf.prepare(k);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			if (vect[i][j] != -1) {
				if (i + 1 < n && vect[i + 1][j] != -1)
					mf.addEdge(vect[i + 1][j], vect[i][j]);
				if (j + 1 < m && vect[i][j + 1] != -1)
					mf.addEdge(vect[i][j + 1], vect[i][j]);
				vect[i][j] = -1;
			}
		}
	ans = mf.calcMatch();
	ans = ans*a + b * (k - 2 * ans);
	std::cout << ans;
}
