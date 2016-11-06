#pragma once
#include<vector>
#include<algorithm>

class findMin {
public:
	std::vector<std::vector <Edge>> graph;
	std::vector<int> dist;
	std::vector<int> par;
	std::vector<int> par_rib;
	std::vector<int> pot;
	int stock;
	int source;
	int vertexCnt;
	int strCnt;

	void addEdge(int from, int to, int str, int value) {
		Edge r1 = { to, str, value, 0, graph[to].size() };
		Edge r2 = { from, 0, -value, 0, graph[from].size() };
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
			int d = INF;
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

	int calc(int k) {
		int stream = 0, cost = 0;
		potCalc();
		while (stream < k) {
			deikstraPathFind();
			if (dist[stock] > INF / 10)  break;
			for (int v = stock; v != source; v = par[v]) {
				int pv = par[v];
				size_t pr = par_rib[v], r = graph[pv][pr].back;
				++graph[pv][pr].stream;
				--graph[v][r].stream;
				cost += graph[pv][pr].val;
			}
			for (int i = 0; i < vertexCnt; ++i) {
				if (pot[i] < INF)
					pot[i] += dist[i];
			}
			++stream;
		}
		if (stream < k) {
			std::cout << -1;
			return 0;
		}
		return cost;
		/*cost = 0;
		for (int i = 0; i < vertexCnt; ++i)
		for (int j = 0; j < graph[i].size(); ++j)
		if (graph[i][j].stream > 0)
		cost += graph[i][j].val;*/
		//printAns(cost, k);
	}
};