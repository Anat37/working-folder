#pragma once
const long long INF = std::numeric_limits<long long>::max();
struct Edge {
	int vertex;
	int cap;
	long long val;
	int stream;
	size_t back;
	int num;
};


struct vert {
	vert(int a, long long b)
		:v(a)
		, dist(b) {}
	vert() {}
	int v;
	long long dist;
	bool operator() (const vert& a, const vert& b) {
		return a.dist > b.dist;
	}
};
std::vector<long long> dist;
struct cmp {
	bool operator() (const int a, const int b) {
		if (dist[a] == dist[b])
			return a > b;
		else
			return dist[a] < dist[b];
	}
};

class findMin {
public:
	std::vector<std::vector <Edge>> graph;

	std::vector<int> par;
	std::vector<int> par_rib;
	std::vector<long long> pot;
	int stock;
	int source;
	int vertexCnt;


	void addEdge(int from, int to, int str, long long value, int num) {
		Edge r1 = { to, str, value, 0, graph[to].size(), num };
		Edge r2 = { from, 0, -value, 0, graph[from].size(),  num };
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
		bool q = true;;
		pot.assign(vertexCnt, INF);
		pot[source] = 0;
		for (int i = 0; (i < vertexCnt) && (q == true); ++i) {
			q = false;
			for (int j = 0; j < vertexCnt; ++j) {
				for (int k = 0; k < graph[j].size(); ++k) {
					if (graph[j][k].stream < graph[j][k].cap && pot[j] != INF && pot[j] + graph[j][k].val < pot[graph[j][k].vertex]) {
						pot[graph[j][k].vertex] = pot[j] + graph[j][k].val;
						q = true;
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
		std::set<int, cmp> q;
		used.assign(vertexCnt, false);
		dist.assign(vertexCnt, INF);
		par.assign(vertexCnt, -1);
		par_rib.assign(vertexCnt, -1);
		dist[source] = 0;
		q.insert(source);
		while (!q.empty()) {
			int vertex = -1;
			long long d = INF;
			/*for (int j = 0; j < vertexCnt; ++j) {
			if (used[j] == false && dist[j] < d) {
			d = dist[j];
			vertex = j;
			}
			}*/
			vertex = *q.begin();
			q.erase(q.begin());

			if (vertex == -1)
				break;
			used[vertex] = true;
			if (dist[vertex] == INF)
				break;
			for (int j = 0; j < graph[vertex].size(); ++j)
				if (graph[vertex][j].stream < graph[vertex][j].cap && used[graph[vertex][j].vertex] == false) {
					int u = graph[vertex][j].vertex;
					if (dist[u] > dist[vertex] + graph[vertex][j].val + pot[vertex] - pot[u]) {
						q.erase(u);
						par[u] = vertex;
						par_rib[u] = j;
						dist[u] = dist[vertex] + graph[vertex][j].val + pot[vertex] - pot[u];
						q.insert(u);
					}
				}
		}
	}

	long long calc(int k) {
		int stream = 0;
		long long cost = 0;
		long long delta = 0;
		potCalc();
		while (stream < k) {
			deikstraPathFind();
			if (dist[stock] > INF / 10)  break;
			delta = 0;
			for (int v = stock; v != source; v = par[v]) {
				int pv = par[v];
				size_t pr = par_rib[v], r = graph[pv][pr].back;
				delta += graph[pv][pr].val;
			}
			if (delta > 0)
				break;
			cost += delta;
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
		return cost;
	}
};

void startMAXpar() {
	int n, m, k, a, b;
	std::cin >> n >> m >> k;
	std::vector<int> right;
	std::vector<int> left;
	std::vector<int> ans;
	right.resize(m);
	left.resize(n);
	findMin fm;
	fm.prepare(n + m + 2, n + m, n + m + 1);
	for (int i = 0; i < n; ++i) {
		std::cin >> left[i];
	}
	for (int i = 0; i < m; ++i) {
		std::cin >> right[i];
	}
	for (int i = 0; i < k; ++i) {
		std::cin >> a >> b;
		--a;
		--b;
		fm.addEdge(a, n + b, 1, -left[a] - right[b], i + 1);
	}
	for (int i = 0; i < n; ++i) {
		fm.addEdge(n + m, i, 1, 0, -1);
	}
	for (int i = 0; i < m; ++i) {
		fm.addEdge(n + i, n + m + 1, 1, 0, -1);
	}
	std::cout << -fm.calc(n + m) << std::endl;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < fm.graph[i].size(); ++j)
			if (fm.graph[i][j].vertex != n + m && fm.graph[i][j].stream == 1) {
				ans.push_back(fm.graph[i][j].num);
				break;
			}
	}
	std::cout << ans.size() << std::endl;
	for (int i = 0; i < ans.size(); ++i)
		std::cout << ans[i] << ' ';
}
