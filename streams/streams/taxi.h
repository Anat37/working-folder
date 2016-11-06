#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

const int INF = std::numeric_limits<int>::max();

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

struct Order {
	Order(int sx, int sy, int fx, int fy, int ts)
		:placeStartX(sx)
		, placeStartY(sy)
		, placeFinishY(fy)
		, placeFinishX(fx)
		, timeStart(ts)
	{
		finCalc();
	}
	int placeStartX, placeFinishX;
	int placeStartY, placeFinishY;
	int timeStart, timeFinish;
	void finCalc() {
		timeFinish = timeStart + std::abs(placeStartX - placeFinishX)
			+ std::abs(placeStartY - placeFinishY);
	}
};

void startTaxi() {
	int n, a, b, sx, sy, fx, fy;
	std::vector<Order > vect;
	MatchFind mc;
	std::cin >> n;

	for (int i = 0; i < n; ++i) {
		scanf("%d:%d", &a, &b);
		std::cin >> sx >> sy >> fx >> fy;
		vect.push_back(Order(sx, sy, fx, fy, a * 60 + b));
	}

	mc.prepare(2 * n);

	for (int i = 0; i < n; ++i)
		for (int j = i + 1; j < n; ++j) {
			if (vect[i].timeFinish + std::abs(vect[i].placeFinishX - vect[j].placeStartX)
				+ std::abs(vect[i].placeFinishY - vect[j].placeStartY) < vect[j].timeStart)
				mc.addEdge(i, n + j);
		}
	std::cout << n - mc.calcMatch();
}

