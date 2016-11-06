#pragma once

class StreamWeb {
public:
	void prepare(size_t n, size_t isto, size_t sto) {
		--sto;
		--isto;
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
		--from;
		--to;
		graph[from][to] += value;
	}

	size_t calcStream() {
		size_t ans = 0;
		while (bfs(istok)) {
			/*for (size_t i = 0; i < vertex_cnt; ++i) {
			for (size_t j = 0; j < vertex_cnt; ++j)
			std::cout << stream[i][j] << ' ' ;
			std::cout << std::endl;
			}*/
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

	void decEdge(size_t from, size_t to, size_t value) {
		--from;
		--to;
		if (stream[from][to] == graph[from][to]) {
			stream[from][to] -= value;
			stream[to][from] += value;
			decStream(from, to);
		}
		graph[from][to] -= value;
		/*for (size_t i = 0; i < vertex_cnt; ++i) {
		for (size_t j = 0; j < vertex_cnt; ++j)
		std::cout << stream[i][j] << ' ';
		std::cout << std::endl;
		}*/
	}


private:
	std::vector<std::vector<long long> > graph;
	std::vector<std::vector<long long> > stream;
	size_t stok;
	size_t istok;
	size_t vertex_cnt;

	bool bfs(size_t vertex) {
		std::queue<size_t> que;
		std::vector<long long> vect;
		std::vector<size_t> prev;
		vect.assign(vertex_cnt, 0);
		prev.assign(vertex_cnt, 0);
		que.push(vertex);
		vect[vertex] = INF;
		while (!que.empty()) {
			size_t v = que.front();
			size_t k;
			que.pop();
			if (v == stok && vect[v] > 0) {
				size_t val = vect[v];
				while (v != istok) {
					stream[prev[v]][v] += val;
					stream[v][prev[v]] -= val;
					v = prev[v];
				}
				return true;
			}
			if (v != stok)
				for (size_t i = 0; i < vertex_cnt; ++i) {
					if (graph[v][i] - stream[v][i] > 0) {
						k = std::min(vect[v], graph[v][i] - stream[v][i]);
						if (k > vect[i]) {
							prev[i] = v;
							vect[i] = k;
							que.push(i);
						}
					}
				}
		}
		return false;
	}

	void decStream(size_t from, size_t to) {
		std::queue<size_t> que;
		std::vector<bool> used;
		std::vector<size_t> prev;
		used.assign(vertex_cnt, false);
		prev.assign(vertex_cnt, 0);
		que.push(istok);
		while (!que.empty()) {
			size_t v = que.front();
			que.pop();
			used[v] = true;
			if (v == from) {
				while (v != istok) {
					stream[prev[v]][v] -= 1;
					stream[v][prev[v]] += 1;
					v = prev[v];
				}
				break;
			}
			for (size_t i = 0; i < vertex_cnt; ++i) {
				if (stream[v][i] > 0 && used[i] == false) {
					prev[i] = v;
					que.push(i);
				}
			}
		}
		used.assign(vertex_cnt, false);
		while (!que.empty())
			que.pop();
		que.push(to);
		while (!que.empty()) {
			size_t v = que.front();
			que.pop();
			used[v] = true;
			if (v == stok) {
				while (v != to) {
					stream[prev[v]][v] -= 1;
					stream[v][prev[v]] += 1;
					v = prev[v];
				}
				break;
			}
			for (size_t i = 0; i < vertex_cnt; ++i) {
				if (stream[v][i] > 0 && used[i] == false) {
					prev[i] = v;
					que.push(i);
				}
			}
		}
	}
};


void startA() {
	size_t n, c, u, v, k, m;
	size_t ans = 0;
	std::cin >> n >> c;
	StreamWeb sw;
	sw.prepare(n, 1, n);
	for (size_t i = 0; i < c; ++i) {
		std::cin >> u >> v >> k;
		sw.addEdge(u, v, k);
	}
	ans = sw.calcStream();
	std::cout << ans << std::endl;
	std::cin >> m;
	for (int i = 0; i < m; ++i) {
		std::cin >> u >> v >> k;
		if (u == 1) {
			sw.addEdge(v, k, 1);
			ans = sw.calcStream();
			std::cout << ans << std::endl;
		}
		else {
			sw.decEdge(v, k, 1);
			ans = sw.calcStream();
			std::cout << ans << std::endl;
		}
	}
}
