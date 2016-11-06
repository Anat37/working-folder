#pragma once
#include<iostream>

int stringLen;
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
		graph[to][from] += value;
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
		for (size_t i = 0; i < vertex_cnt; ++i)
			ans += stream[istok][i];
		/*for (size_t i = 0; i < vertex_cnt; ++i) {
		for (size_t j = 0; j < vertex_cnt; ++j)
		std::cout << stream[i][j] << ' ';
		std::cout << std::endl;
		}*/
		return ans;
	}

private:
	std::vector<std::vector<int> > graph;
	std::vector<std::vector<int> > stream;
	std::vector<bool> used;
	size_t stok;
	size_t istok;
	size_t vertex_cnt;

	bool goDfs() {

		bool q = false;

		for (int i = 1; i < vertex_cnt - 1; ++i) {
			used.assign(vertex_cnt, false);
			int d = dfs(i, graph[0][i] - stream[0][i]);
			if (d > 0) {
				q = true;
				stream[0][i] += d;
				stream[i][0] -= d;
				//std::cout << d;
			}

		}
		return q;
	}
	int dfs(int v, int str) {        // Cmin Ч пропускна€ способность в текущем подпотоке
		if (v == stok)
			return str;
		if (str <= 0)
			return str;
		used[v] = true;
		int suka = (v - 1) / stringLen;
		int j = (v - 1) % stringLen;
		int u = (suka - 1)*stringLen + j + 1;
		if (suka - 1 >= 0) {
			if (!used[u] && stream[v][u] < graph[v][u]) {
				int delta = dfs(u, std::min(str, graph[v][u] - stream[v][u]));
				if (delta > 0) {
					stream[v][u] += delta;
					stream[u][v] -= delta;
					return delta;
				}
			}
		}
		u = (suka + 1)*stringLen + j + 1;
		if (u < vertex_cnt - 1) {
			if (!used[u] && stream[v][u] < graph[v][u]) {
				int delta = dfs(u, std::min(str, graph[v][u] - stream[v][u]));
				if (delta > 0) {
					stream[v][u] += delta;
					stream[u][v] -= delta;
					return delta;
				}
			}
		}

		u = suka*stringLen + j + 2;
		if (j < stringLen - 1) {
			if (!used[u] && stream[v][u] < graph[v][u]) {
				int delta = dfs(u, std::min(str, graph[v][u] - stream[v][u]));
				if (delta > 0) {
					stream[v][u] += delta;
					stream[u][v] -= delta;
					return delta;
				}
			}
		}
		u = suka*stringLen + j;
		if (j - 1 >= 0) {
			if (!used[u] && stream[v][u] < graph[v][u]) {
				int delta = dfs(u, std::min(str, graph[v][u] - stream[v][u]));
				if (delta > 0) {
					stream[v][u] += delta;
					stream[u][v] -= delta;
					return delta;
				}
			}
		}
		u = stok;
		//for (int u = 0; u < vertex_cnt; ++u) 

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
};

int valent(char c) {
	switch (c) {
	case 'H': return 1;
	case 'O': return 2;
	case 'N': return 3;
	case 'C': return 4;
	}
	return -1;
}

void startChemistry() {
	int n, m, in = 0, out = 0, ans, k = 0;
	StreamWeb sw;
	std::vector<std::vector<char> > vect;
	std::cin >> n >> m;
	stringLen = m;
	vect.resize(n);
	for (int i = 0; i < n; ++i) {
		vect[i].assign(m, '.');
	}

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			std::cin >> vect[i][j];

	/*
	for (int i = 0; i < n; ++i) {
	for (int j = 0; j < m; ++j)
	std::cout << vect[i][j];
	std::cout << std::endl;
	}*/

	sw.prepare(n*m + 2, 0, n*m + 1);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j) {
			if (vect[i][j] != '.') {
				k++;
				if (i + 1 < n && vect[i + 1][j] != '.')
					sw.addEdge(i*m + j + 1, (i + 1)*m + j + 1, 1);
				if (j + 1 < m && vect[i][j + 1] != '.')
					sw.addEdge(i*m + j + 1, i*m + j + 2, 1);
				if ((i - j) % 2 == 0) {
					sw.addEdge(0, i*m + j + 1, valent(vect[i][j]));
					in += valent(vect[i][j]);
				}
				else {
					sw.addEdge(i*m + j + 1, n*m + 1, valent(vect[i][j]));
					out += valent(vect[i][j]);
				}
				vect[i][j] = '.';
			}
		}
	if (k == 0) {
		std::cout << "Invalid";
		return;
	}
	if (in != out) {
		std::cout << "Invalid";
		return;
	}
	ans = sw.calcStream();
	//std::cout << ans << ' ' << in << ' ' << out <<std::endl;
	if (in == ans && out == ans)
		std::cout << "Valid";
	else
		std::cout << "Invalid";
}
