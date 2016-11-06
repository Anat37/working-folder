#pragma once
int dynamic(int n, int k, std::vector<std::vector<int>>& matrix, std::vector<std::vector<int>>& vals) {
	if (vals[n][k] != -1)
		return vals[n][k];
	if (n == 1) {
	}
	int ans = INF;
	for (int i = 0; i < matrix.size(); ++i)
		if (i != k)
			ans = std::min(ans, dynamic(n - 1, i, matrix, vals));
	return vals[n][k] = ans;
}

void startMatrix() {
	int n;
	std::cin >> n;
	std::vector<std::vector<int> > matr;
	std::vector<std::vector<int> > vals;
	matr.resize(n);
	vals.resize(n + 1);
	for (int i = 0; i < n; ++i) {
		matr[i].resize(n);
		vals[i].assign(n + 1, -1);
		for (int j = 0; j < n; ++j)
			std::cin >> matr[i][j];
	}
	int ans = INF;
	for (int i = 0; i < n; ++i)
		ans = std::min(ans, dynamic(n - 1, i, matr, vals) + matr[n - 1][i]);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; j++)
			std::cout << vals[i][j] << ' ';
		std::cout << std::endl;
	}

	std::cout << ans;

}