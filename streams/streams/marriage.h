#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>


const int INF = std::numeric_limits<int>::max();
struct Edge {
	int vertex;
	int cap, val, stream;
	size_t back;
	int num;
};


class findMin {
public:
	std::vector<std::vector <Edge>> graph;
	int stock;
	int source;
	int vertexCnt;
	int strCnt;

	void addEdge(int from, int to, int str, int value, int num) {
		Edge r1 = { to, str, value, 0, graph[to].size(), num };
		Edge r2 = { from, str, value, 0, graph[from].size(), num };
		graph[from].push_back(r1);
		graph[to].push_back(r2);
	}

	void prepare(int n, int sour, int sto) {
		stock = sto;
		source = sour;
		vertexCnt = n;
		graph.resize(n);
	}

	void calc(int k) {
		int stream = 0, cost = 0;
		while (stream < k) {
			std::vector<int> id(vertexCnt, 0);
			std::vector<int> d(vertexCnt, INF);
			std::vector<int> q(vertexCnt);
			std::vector<int> p(vertexCnt);
			std::vector<size_t> p_rib(vertexCnt);
			std::vector<bool> usedStr(vertexCnt);
			int qh = 0, qt = 0;
			q[qt++] = source;
			d[source] = 0;
			while (qh != qt) {
				int v = q[qh++];
				id[v] = 2;
				if (qh == vertexCnt)  qh = 0;
				for (size_t i = 0; i<graph[v].size(); ++i) {
					Edge& r = graph[v][i];
					if (r.stream < r.cap && d[v] + r.val < d[r.vertex] && r.stream >= 0) {
						d[r.vertex] = d[v] + r.val;
						if (id[r.vertex] == 0) {
							q[qt++] = r.vertex;
							if (qt == vertexCnt)  qt = 0;
						}
						else if (id[r.vertex] == 2) {
							if (--qh == -1)  qh = vertexCnt - 1;
							q[qh] = r.vertex;
						}
						id[r.vertex] = 1;
						p[r.vertex] = v;
						p_rib[r.vertex] = i;
					}
					if (r.stream < r.cap && d[v] - r.val < d[r.vertex] && r.stream < 0) {
						d[r.vertex] = d[v] - r.val;
						if (id[r.vertex] == 0) {
							q[qt++] = r.vertex;
							if (qt == vertexCnt)  qt = 0;
						}
						else if (id[r.vertex] == 2) {
							if (--qh == -1)  qh = vertexCnt - 1;
							q[qh] = r.vertex;
						}
						id[r.vertex] = 1;
						p[r.vertex] = v;
						p_rib[r.vertex] = i;
					}
				}
			}
			if (d[stock] > INF / 10)  break;
			for (int v = stock; v != source; v = p[v]) {
				int pv = p[v];
				size_t pr = p_rib[v], r = graph[pv][pr].back;
				if (graph[pv][pr].stream < 0) {
					cost -= 2 * graph[pv][pr].val;
				}
				++graph[pv][pr].stream;
				--graph[v][r].stream;
				cost += graph[pv][pr].val;
			}
			++stream;
		}
		if (stream < k) {
			std::cout << -1;
			return;
		}
		/*cost = 0;
		for (int i = 0; i < vertexCnt; ++i)
		for (int j = 0; j < graph[i].size(); ++j)
		if (graph[i][j].stream > 0)
		cost += graph[i][j].val;*/
		printAns(cost, k);
	}

	void printAns(int cost, int k) {
		printf("%.5f\n", (double)cost / (double)k);
		std::vector<int> temp;
		for (int i = 0; i < k; ++i) {
			temp.clear();
			int vertex = source;
			while (vertex != stock) {
				for (int j = graph[vertex].size() - 1; j >= 0; --j)
					if (graph[vertex][j].num != -1 && graph[vertex][j].stream == 1) {
						temp.push_back(graph[vertex][j].num);
						graph[vertex][j].num = -1;
						vertex = graph[vertex][j].vertex;
						break;
					}
					else graph[vertex].pop_back();
			}
			std::cout << temp.size() << ' ';
			for (int j = 0; j < temp.size(); ++j)
				std::cout << temp[j] << ' ';
			std::cout << std::endl;
		}
	}
};
void startMarr() {
	int n, m, k, x, y, c;
	std::cin >> n >> m >> k;
	findMin fm;
	fm.prepare(n, 0, n - 1);
	for (int i = 0; i < m; ++i) {
		std::cin >> x >> y >> c;
		--x;
		--y;
		fm.addEdge(x, y, 1, c, i + 1);
	}
	fm.calc(k);

}