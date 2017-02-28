#define _CRT_SECURE_NO_WARNINGS


#include<vector>
#include<algorithm>
#include<iostream>
#include<string>
#include<memory.h>
#include<queue>

static const int alphabet = 30;

struct Vertex {
public:
	std::vector<int> next;
	int ends;
	bool isTerminal;
	int parent;
	char letter;
	int suffixLink;
	std::vector<int> go;

	Vertex(int _ends = 0,bool isTerminal_ = false, int parent_ = 0,
		char letter_ = ' ', int suffixLink_ = -1)
		: ends(_ends)
		, next(alphabet, -1)
		, isTerminal(isTerminal_)
		, parent(parent_)
		, letter(letter_)
		, suffixLink(suffixLink_)
		, go(alphabet, -1)
	{}
};

class Trie {
public:

	Trie() : vertex(1, Vertex()) {}

	long long sum = 0;

	void addString(const std::string& str) {
		int current = 0;
		for (auto current_char : str) {
			if (vertex[current].next[current_char] == -1) {
				vertex[current].next[current_char] = size++;
				vertex.push_back(Vertex(false, current, current_char, -1));
			}
			current = vertex[current].next[current_char];
		}
		++vertex[current].ends;
	}

	bool find_cycle() {
		cycle_ = false;
		used.assign(vertex.size(), 0);
		find_cycle_int(0);
		return cycle_;
	}

	void buildLinks() {
		std::queue<int> queue;
		queue.push(0);
		while (!queue.empty()) {
			int current_vertex = queue.front();
			queue.pop();
			getLink(current_vertex);
			vertex[current_vertex].ends += vertex[vertex[current_vertex].suffixLink].ends;
			for (auto i : vertex[current_vertex].next) {
				if (i != -1) {
					queue.push(i);
				}
			}
		}
	}


	int calculate(char c, int current_vertex) {
		int to = go(current_vertex, c);
		sum += vertex[to].ends;
		return to;
	}


private:

	int size = 1;

	std::vector<Vertex> vertex;

	bool cycle_;

	std::vector<char> used;

	int getLink(int current) {
		if (vertex[current].suffixLink == -1) {
			if (current == 0 || vertex[current].parent == 0) {
				vertex[current].suffixLink = 0;
			}
			else {
				vertex[current].suffixLink = go(getLink(vertex[current].parent),
					vertex[current].letter);

			}
		}
		return vertex[current].suffixLink;
	}

	int go(int current, char cc) {
		if (vertex[current].go[cc] == -1) {
			if (vertex[current].next[cc] == -1) {
				vertex[current].go[cc] = current == 0 ? 0 : go(getLink(current), cc);
			}
			else {
				vertex[current].go[cc] = vertex[current].next[cc];
			}
		}
		return vertex[current].go[cc];
	}

	void find_cycle_int(int current) {
		used[current] = 1;
		if (cycle_) {
			return;
		}
		for (char c = 0; c <= 1; ++c) {
			int to = go(current, c);
			if (used[to] == 1) {
				cycle_ = true;
				return;
			}
			if (used[to] == 0 && !vertex[to].isTerminal) {
				find_cycle_int(go(current, c));
				if (cycle_)
					return;
			}
		}
		used[current] = -1;
	}


};

void PToZ(std::vector<int>& P, std::vector<int>& Z) {
	int n = P.size();
	for (int i = 1; i < n; i++) // 1 этап.
		if (P[i])
			Z[i - P[i] + 1] = P[i];

	Z[0] = n;

	if (Z[1])
		for (int i = 1; i < Z[1]; i++)
			Z[i + 1] = Z[1] - i;

	int t;
	for (int i = Z[1] + 1; i < n - 1; i++)
	{
		t = i;
		if (Z[i] && !Z[i + 1])
			for (int j = 1; j < Z[i] && Z[i + j] <= Z[j]; j++)
			{
				Z[i + j] = std::min(Z[j], Z[i] - j);
				t = i + j;
			}
		i = t;
	}
}


std::vector<int> suff;
std::vector<std::vector<int>> c;

std::vector<int> z_function(const std::string& s, int begin) {
	int n = s.length() - begin;
	std::vector<int> z(static_cast<std::size_t>(n));
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		if (i <= r)
			z[i] = std::min(r - i + 1, z[i - l]);
		while (i + z[i] < n && s[z[i] + begin] == s[i + z[i] + begin])
			++z[i];
		if (i + z[i] - 1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}
	return z;
}

std::vector<int> buildLCP(std::string& str, std::vector<int>& suf) {
	int n = str.length();
	std::vector<int> lcp(n);
	std::vector<int> pos(n);

	for (int i = 0; i < n; ++i) {
		pos[suf[i]] = i;
	}

	int k = 0;
	for (int i = 0; i < n; ++i) {
		if (k > 0)
			k--;
		if (pos[i] == n - 1) {
			lcp[n - 1] = -1;
			k = 0;
		}
		else {
			int j = suf[pos[i] + 1];
			while (std::max(i + k, j + k) < n && str[i + k] == str[j + k]) {
				k++;
			}
			lcp[pos[i]] = k;
		}
	}
	return lcp;
}


std::vector<std::vector<std::pair<int, char>>> g;
Trie trie;

void dfs(int v, int curv, char c) {
	if (c != 0) {
		curv = trie.calculate(c - 'a', curv);
	}
	for (auto to : g[v]) {
		dfs(to.first, curv, to.second);
	}
}


int main() {

freopen("input.txt", "rt", stdin);
freopen("output.txt", "wt", stdout);

	std::ios_base::sync_with_stdio(0);
	std::cin.tie(0);
	int n;
	std::cin >> n;
	g.resize(n);

	for (int i = 0; i < n; ++i) {
		int k;
		std::cin >> k;
		for (int j = 0; j < k; ++j) {
			int to;
			char c;
			std::cin >> to >> c;
			g[i].push_back({ to - 1, c });
		}
	}
	int m;
	std::cin >> m;
	std::string s;

	for (int i = 0; i < m; ++i) {
		
		std::cin >> s;
		for (auto& c : s)
			c -= 'a';
		trie.addString(s);
	}
	trie.buildLinks();
	dfs(0, 0, 0);
	std::cout << trie.sum;

	return 0;
}