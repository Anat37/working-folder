#include"HeaderTest.h"

int main() {
	test(500);
	string a;
	string b;
	cin >> a >> b;
	vector< vector<size_t> > ans = calculateSuffixPrefixLCS(a, b);
	for (size_t i = 0; i < ans.size(); ++i) {
		for (size_t j = 0; j < ans[i].size(); ++j)
			cout << ans[i][j] << ' ';
		cout << std::endl;
	}
	cin >> a;
	return 0;
}