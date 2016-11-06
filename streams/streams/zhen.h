#pragma once
class StreamWeb {
public:
	void prepare(size_t n, size_t isto, size_t sto) {
		graph.clear();
		vertex_cnt = n;
		graph.resize(n);
		istok = isto;
		stok = sto;
	}

	void noflow() {
		for (int i = 0; i < vertex_cnt; ++i) {
			for (int j = 0; j < graph[i].size(); ++j)
				graph[i][j].stream = 0;
		}
	}

	void addEdge(size_t from, size_t to, size_t value) {
		Edge r1 = { to, value, 0, graph[to].size() };
		Edge r2 = { from, 0, 0, graph[from].size() };
		graph[from].push_back(r1);
		graph[to].push_back(r2);
	}

	size_t calcStream() {
		size_t ans = 0;
		while (goDfs()) {
			/*for (size_t i = 0; i < vertex_cnt; ++i) {
			for (size_t j = 0; j < vertex_cnt; ++j)
			std::cout << stream[i][j] << ' ' ;
			std::cout << std::endl;
			}*/
		}
		for (size_t i = 0; i < graph[istok].size(); ++i)
			ans += graph[istok][i].stream;
		/*for (size_t i = 0; i < vertex_cnt; ++i) {
		for (size_t j = 0; j < vertex_cnt; ++j)
		std::cout << stream[i][j] << ' ';
		std::cout << std::endl;
		}*/
		return ans;
	}

	void delEdge(size_t from, size_t to, size_t value) {
		int rib, src_rib;
		for (int i = 0; i < graph[from].size(); ++i) {
			if (graph[from][i].vertex == to) {
				graph[from][i].cap = 0;
				rib = i;
			}
			if (graph[from][i].vertex == istok) {
				src_rib = graph[from][i].back;
			}
		}
		if (graph[from][rib].stream > 0) {
			graph[to][graph[from][rib].back].stream = 0;
			graph[from][rib].stream = 0;
			--graph[istok][src_rib].stream;
			graph[to][0].stream = 0;
		}
	}
	std::vector<std::vector<Edge> > graph;
	std::vector<bool> used;
	size_t stok;
	size_t istok;
	size_t vertex_cnt;

	bool goDfs() {

		bool q = false;

		for (int i = 0; i < graph[istok].size(); ++i) {
			used.assign(vertex_cnt, false);
			int d = dfs(graph[istok][i].vertex, graph[istok][i].cap - graph[istok][i].stream);
			if (d > 0) {
				q = true;
				graph[istok][i].stream += d;
				graph[graph[istok][i].vertex][graph[istok][i].back].stream -= d;
				//std::cout << d;
			}

		}
		return q;
	}