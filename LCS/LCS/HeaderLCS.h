#pragma once
#include<string>
#include<vector>
using std::string;
using std::vector;

vector< vector<size_t> > transpon(const vector< vector<size_t> >& matrix) {
	vector< vector<size_t> > ans;
	size_t n = matrix.size() - 1;
	size_t m = matrix[0].size() - 1;
	ans.resize(m);
	for (auto& val : ans)
		val.resize(n);
	for (size_t i = 0; i < m; ++i)
		for (size_t j = 0; j < n; ++j)
			ans[i][j] = matrix[j + 1][i];
	return ans;
}

void criticalPoints(vector< vector<size_t> >& hor, vector< vector<size_t> >& vert, const string &fir, const string &sec) {
	size_t n = vert.size() - 1;
	size_t m = hor.size() - 1;
	for (auto& val : vert)
		val.push_back(0);
	for (size_t i = 0; i <= m; ++i)
		hor[i].push_back(i + 1);

	for (size_t i = 0; i < n; ++i)
		for (size_t j = 0; j < m; ++j)
		{
			size_t v = vert[i][j];
			size_t h = hor[j][i];
			if (v < h)
				if (fir[i] == sec[j]) {
					hor[j].push_back(v);
					vert[i].push_back(h);
				}
				else {
					hor[j].push_back(h);
					vert[i].push_back(v);
				}
			else {
				hor[j].push_back(v);
				vert[i].push_back(h);
			}
		}
}


vector< vector<size_t> > ans—alc(const vector< vector<size_t> >& hor, const vector< vector<size_t> >& vert) {
	size_t n = vert.size() - 1;
	size_t m = hor.size() - 1;
	vector< vector<size_t> > ans;
	ans.resize(n + 1);
	for (auto& val : ans)
		val.resize(m + 1);
	for (auto& val : ans[0])
		val = 0;
	for (auto& val : ans)
		val[m] = 0;

	for (size_t i = 1; i <= n; ++i) {
		size_t k = vert[i - 1][m];
		for (int j = m - 1; (j >= k && j >= 0); --j)
			ans[i][j] = ans[i - 1][j];
		for (int j = k - 1; j >= 0; --j)
			ans[i][j] = ans[i - 1][j] + 1;
	}
	return ans;
}


vector< vector<size_t> > calculateSuffixPrefixLCS(const string &sec, const string &fir) {
	size_t n = fir.size(), m = sec.size();
	vector< vector<size_t> > vert;
	vert.resize(n + 1);
	vector< vector<size_t> > hor;
	hor.resize(m + 1);
	
	criticalPoints(hor, vert, fir, sec);

	vector< vector<size_t> > ans = ans—alc(hor, vert);
	
	return transpon(ans);
}

void suffCalc(vector< vector<size_t> >& c, vector< vector<size_t> >& ans, size_t i, const string& a, const string& b) {
	size_t n = ans.size() - 1;
	size_t m = ans[0].size() - 1;
	for (auto& val : c)
		val[i] = 0;
	for (auto& val : c[0])
		val = 0;

	for (int ii = 1; ii <= n; ii++)
		for (int j = i + 1; j <= m; j++) {
			if (a[ii - 1] == b[j - 1]) {
				c[ii][j] = c[ii - 1][j - 1] + 1;
			}
			else if (c[ii - 1][j] >= c[ii][j - 1]) {
				c[ii][j] = c[ii - 1][j];
			}
			else {
				c[ii][j] = c[ii][j - 1];
			}
		}
	for (size_t j = 1; j <= n; ++j)
		ans[j].push_back(c[j][m]);
}

vector<vector<size_t> > naive(const string& b, const string& a) {
	const size_t n = a.size();
	const size_t m = b.size();
	vector< vector<size_t> > ans;
	ans.resize(n + 1);
	ans[0].resize(m + 1);
	for (size_t i = 0; i <= m; ++i)
		ans[0][i] = 0;
	vector< vector<size_t> > c;
	c.resize(n + 1);
	for (auto& val : c)
		val.resize(m + 1);

	for (size_t i = 0; i <= m; ++i){
		suffCalc(c, ans, i, a, b);
		}
	
	return transpon(ans);
}