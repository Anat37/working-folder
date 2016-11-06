#pragma once
#include<iostream>
#include<vector>
#include<queue>

#define INF 1000000000


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
		return ans;
	}
private:
	std::vector<std::vector<size_t> > graph;
	std::vector<std::vector<size_t> > stream;
	size_t stok;
	size_t istok;
	size_t vertex_cnt;

	bool bfs(size_t vertex) {
		std::queue<size_t> que;
		std::vector<size_t> vect;
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
};


void startA() {
	size_t n, s, t, c;
	size_t ans = 0;
	std::cin >> n;
	while (n != 0) {
		std::cin >> s >> t >> c;
		StreamWeb sw;
		--s;
		--t;
		sw.prepare(n, s, t);
		for (size_t i = 0; i < c; ++i) {
			size_t u, v, k;
			std::cin >> u >> v >> k;
			--u;
			--v; 
			sw.addEdge(u, v, k);
		}
		ans = sw.calcStream();
		std::cout << ans << std::endl;
		std::cin >> n;
	}
}

