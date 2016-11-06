#pragma once
struct Edge {
	int vertex;
	int cap;
	long long val;
	int stream;
	size_t back;
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
		bool q = true;
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

struct Order {
	Order(long long val, long ts, long long tf)
		:value(val)
		, timeStart(ts)
		, last(tf)
	{
		finCalc();
	}
	long long value;
	long long timeStart, timeFinish;
	long long last;
	void finCalc() {
		timeFinish = timeStart + last - 1;
	}
};

void startAndroid() {
	int n, k;
	long long val, a, b;
	std::vector<Order > vect;
	findMin mf;
	std::cin >> n >> k;

	for (int i = 0; i < n; ++i) {
		std::cin >> a >> b >> val;
		vect.push_back(Order(val, a, b));
	}

	mf.prepare(2 * n + 2, 0, 2 * n + 1);

	for (int i = 0; i < n; ++i)
		mf.addEdge(i + 1, n + i + 1, 1, -vect[i].value);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			if (vect[i].timeFinish < vect[j].timeStart)
				mf.addEdge(n + i + 1, j + 1, 1, 0);
		}
	for (int i = 0; i < n; ++i)
		mf.addEdge(0, i + 1, 1, 0);
	for (int i = 0; i < n; ++i)
		mf.addEdge(n + i + 1, 2 * n + 1, 1, 0);
	mf.calc(k);
	std::vector<int> ans;
	ans.assign(n, 0);
	for (int i = 1; i <= n; ++i)
		for (int j = 0; j < mf.graph[i].size(); ++j) {
			if (mf.graph[i][j].stream == 1)
				ans[mf.graph[i][j].vertex - n - 1] = 1;
		}
	for (int i = 0; i < n; ++i)
		std::cout << ans[i] << ' ';
}
