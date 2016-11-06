#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<functional>
#include<set>
#include<cassert>
const long long INF = std::numeric_limits<long long>::max();
struct Edge {
	int vertex;
	int cap;
	long long val;
	int stream;
	size_t back;
	int num;
};

class StreamWeb {
public:
	void prepare(size_t n, size_t isto, size_t sto) {
		graph.clear();
		stream.clear();
		vertex_cnt = n;
		graph.resize(n);
		for (size_t i = 0; i < n; ++i)
			graph[i].assign(n, 0);
		stream.resize(n);
		for (size_t i = 0; i < n; ++i)
			stream[i].assign(n, 0);
		istok = isto;
		stok = sto;
	}

	void addEdge(size_t from, size_t to, size_t value) {
		graph[from][to] = value;
		graph[to][from] = 0;
	}

	size_t calcStream() {
		size_t ans = 0;
		while (goDfs()) {
			/*for (size_t i = 0; i < vertex_cnt; ++i) {
			for (size_t j = 0; j < vertex_cnt; ++j)
			std::cout << graph[i][j] << ' ' ;
			std::cout << std::endl;
			}
			std::cout << std::endl;*/
		}
		for (size_t i = 0; i < vertex_cnt; ++i)
			ans += stream[istok][i];
		/*for (size_t i = 0; i < vertex_cnt; ++i) {
		for (size_t j = 0; j < vertex_cnt; ++j)
		std::cout << stream[i][j] << ' ';
		std::cout << std::endl;
		}*/
		return ans;
	}


	std::vector<std::vector<int> > graph;
	std::vector<std::vector<int> > stream;
	std::vector<bool> used;
	size_t stok;
	size_t istok;
	size_t vertex_cnt;

	bool goDfs() {

		bool q = false;

		for (int i = 0; i < vertex_cnt - 1; ++i) {
			used.assign(vertex_cnt, false);
			int d = dfs(i, graph[istok][i] - stream[istok][i]);
			if (d > 0) {
				q = true;
				stream[istok][i] += d;
				stream[i][istok] -= d;
				//std::cout << d;
			}

		}
		return q;
	}
	int dfs(int v, int str) {
		if (v == stok)
			return str;
		if (str <= 0)
			return str;
		used[v] = true;

		for (int u = 0; u < vertex_cnt; ++u)
			if (!used[u] && stream[v][u] < graph[v][u]) {
				int delta = dfs(u, std::min(str, graph[v][u] - stream[v][u]));
				if (delta > 0) {
					stream[v][u] += delta;
					stream[u][v] -= delta;
					return delta;
				}
			}
		return 0;
	}
	void noflow() {
		for (int i = 0; i < vertex_cnt; ++i) {
			for (int j = 0; j < vertex_cnt; ++j)
				stream[i][j] = 0;
		}
	}
};



class findMin {
public:
	std::vector<std::vector <Edge>> graph;
	std::vector<long long> dist;
	std::vector<int> par;
	std::vector<int> par_rib;
	std::vector<long long> pot;
	int stock;
	int source;
	int vertexCnt;

	void addEdge(int from, int to, int str, long long value) {
		Edge r1 = { to, str, value, 0, graph[to].size(), -1 };
		Edge r2 = { from, 0, -value, 0, graph[from].size(), -1 };
		graph[from].push_back(r1);
		graph[to].push_back(r2);
	}

	void prepare(int n, int sour, int sto) {
		stock = sto;
		source = sour;
		vertexCnt = n;
		graph.resize(n);
	}

	void potCalc() {
		pot.assign(vertexCnt, INF);
		pot[source] = 0;
		for (int i = 0; i < vertexCnt; ++i) {
			for (int j = 0; j < vertexCnt; ++j) {
				for (int k = 0; k < graph[j].size(); ++k) {
					if (graph[j][k].stream < graph[j][k].cap && pot[j] != INF && pot[j] + graph[j][k].val < pot[graph[j][k].vertex]) {
						pot[graph[j][k].vertex] = pot[j] + graph[j][k].val;
					}
				}
			}
		}
	}

	void pathFind() {
		std::vector<int> id(vertexCnt, 0);
		dist.assign(vertexCnt, INF);
		std::vector<int> q(vertexCnt);
		par.assign(vertexCnt, -1);
		par_rib.assign(vertexCnt, -1);
		int qh = 0, qt = 0;
		q[qt++] = source;
		dist[source] = 0;
		while (qh != qt) {
			int v = q[qh++];
			id[v] = 2;
			if (qh == vertexCnt)  qh = 0;
			for (size_t i = 0; i<graph[v].size(); ++i) {
				Edge& r = graph[v][i];
				if (r.stream < r.cap && dist[v] + r.val < dist[r.vertex]) {
					dist[r.vertex] = dist[v] + r.val;
					if (id[r.vertex] == 0) {
						q[qt++] = r.vertex;
						if (qt == vertexCnt)  qt = 0;
					}
					else if (id[r.vertex] == 2) {
						if (--qh == -1)  qh = vertexCnt - 1;
						q[qh] = r.vertex;
					}
					id[r.vertex] = 1;
					par[r.vertex] = v;
					par_rib[r.vertex] = i;
				}
			}
		}
	}

	void deikstraPathFind() {
		std::vector<bool> used;
		used.assign(vertexCnt, false);
		dist.assign(vertexCnt, INF);
		par.assign(vertexCnt, -1);
		par_rib.assign(vertexCnt, -1);
		dist[source] = 0;
		for (int i = 0; i < vertexCnt; ++i) {
			int vertex;
			long long d = INF;
			for (int j = 0; j < vertexCnt; ++j) {
				if (used[j] == false && dist[j] < d) {
					d = dist[j];
					vertex = j;
				}
			}
			used[vertex] = true;
			if (dist[vertex] == INF)
				break;
			for (int j = 0; j < graph[vertex].size(); ++j)
				if (graph[vertex][j].stream < graph[vertex][j].cap && used[graph[vertex][j].vertex] == false) {
					int u = graph[vertex][j].vertex;
					if (dist[u] > dist[vertex] + graph[vertex][j].val + pot[vertex] - pot[u]) {
						par[u] = vertex;
						par_rib[u] = j;
						dist[u] = dist[vertex] + graph[vertex][j].val + pot[vertex] - pot[u];
					}
				}
		}
	}

	long long calc(int k) {
		int stream = 0;
		long long cost = 0;
		potCalc();
		while (stream < k) {
			deikstraPathFind();
			if (dist[stock] >= INF)  break;
			for (int v = stock; v != source; v = par[v]) {
				int pv = par[v];
				size_t pr = par_rib[v], r = graph[pv][pr].back;

				++graph[pv][pr].stream;
				--graph[v][r].stream;
			}
			for (int i = 0; i < vertexCnt; ++i) {
				if (pot[i] < INF)
					pot[i] += dist[i];
			}
			++stream;
		}
		for (int i = 0; i < vertexCnt; ++i)
			for (int j = 0; j < graph[i].size(); ++j)
				if (graph[i][j].stream > 0)
					cost += graph[i][j].val;
		return cost;
	}
};

void startKpar() {
	int n, k;
	std::cin >> n >> k;
	k = std::min(n, k);
	std::vector<std::vector<int> > matr;
	std::vector<std::vector<int> > ans;
	std::vector<std::set<int> > dol;
	dol.resize(n);
	matr.resize(n);
	ans.resize(k);
	findMin fm;
	StreamWeb sw;
	fm.prepare(2 * n + 2, 2 * n, 2 * n + 1);
	sw.prepare(2 * n + 2, 2 * n, 2 * n + 1);
	for (int i = 0; i < n; ++i) {
		matr[i].resize(n);
		for (int j = 0; j < n; ++j) {
			std::cin >> matr[i][j];
			fm.addEdge(i, n + j, 1, matr[i][j]);
		}
	}
	for (int i = 0; i < n; ++i) {
		fm.addEdge(2 * n, i, k, 0);
		sw.addEdge(2 * n, i, 1);
	}
	for (int i = 0; i < n; ++i) {
		fm.addEdge(n + i, 2 * n + 1, k, 0);
		sw.addEdge(n + i, 2 * n + 1, 1);
	}
	std::cout << fm.calc(n*k) << std::endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < fm.graph[i].size(); ++j)
			if (fm.graph[i][j].vertex != 2 * n && fm.graph[i][j].stream == 1) {
				sw.addEdge(i, fm.graph[i][j].vertex, 1);
				//std::cout << i << ' ' << fm.graph[i][j].vertex << std::endl;;
			}
	}
	for (int i = 0; i < k; ++i) {
		sw.calcStream();
		for (int j = 0; j < n; ++j) {
			for (int it = n; it < 2 * n; ++it)
				if (sw.stream[j][it] == 1) {
					std::cout << it - n + 1 << ' ';
					sw.graph[j][it] = 0;
					break;
				}
		}
		sw.noflow();
		std::cout << std::endl;
	}
}


