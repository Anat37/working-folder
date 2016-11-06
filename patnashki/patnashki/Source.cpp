#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<queue>


#define CURR_X 0
#define CURR_Y 1
#define LAST_X 2
#define LAST_Y 3


class Table {
public:
	int sum_dist;
	std::vector<std::vector<char> > table;
	std::vector<int>  pos;
	std::string route;

	Table() {};

	Table (const Table& another) 
		:sum_dist(another.sum_dist)
		,table(another.table)
		,pos(another.pos)
		,route(another.route) {};

	Table(int turn_in,
		std::vector<std::vector<char> > table_in,
		std::vector<int>  pos_in,
		std::string route_in)
		:table(table_in)
		,pos(pos_in)
		,route(route_in)
	{
		sum_dist = calcDist(turn_in);
	};
	
	Table& operator =(const Table& another) {
		sum_dist = another.sum_dist;
		table = another.table;
		pos = another.pos;
		route = another.route;
		return *this;
	};

	int calcDist(int turn){
		int returnValue = turn;
		std::vector<bool> used(9 - 1, false);
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (table[i][j] != '0') {
					returnValue += abs((table[i][j] - '1') / 3 - i) + abs((table[i][j] - '1') % 3 - j);
				}
			}
		}
		return returnValue;
	}

	bool checkWin(){
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				if (table[i][j] == '0') {
					continue;
				}
				if (table[i][j] != i * 3 + j + 1 + '0') {
					return false;
				}
			}
		}
		return true;
	}

	void swap(Table& another){
		std::swap(sum_dist, another.sum_dist);
		table.swap(another.table);
		pos.swap(another.pos);
		std::swap(route, another.route);
	}

};

bool operator < (const Table& first, const Table& second) {
	if (first.sum_dist == second.sum_dist)
		return (first.route.length() < second.route.length());
	else
		return (first.sum_dist > second.sum_dist);
}

bool checkPos(std::vector<std::vector<char> >& board)
{
	int transNum = 0;
	std::vector<char> vect(9);
	for (int i = 0; i < board.size(); ++i) {
		for (int j = 0; j < board[i].size(); ++j) {
			if (i % 2 == 0) 
				vect[i * 3 + j] = board[i][j];
			else 
				vect[i * 3 + 3 - j - 1] = board[i][j];
		}
	}
	for (int i = 0; i < 9; ++i) {
		if (vect[i] == '0') 
			continue;
		for (int j = i + 1; j < 9; ++j) {
			if (vect[j] == '0') 
				continue;
			if (vect[i] > vect[j]) 
				++transNum;
		}
	}
	vect.clear();
	if (transNum % 2 == 0)
		return false;
	return true;
}

int A_star(std::vector<std::vector<char> >& start_board, std::string& actions, short int x_pos, short int y_pos)
{
	int turn = 0;
	std::priority_queue<Table> queue;
	std::vector<std::pair<short int, short int> > steps;
	steps.push_back(std::make_pair(0, 1));
	steps.push_back(std::make_pair(1, 0));
	steps.push_back(std::make_pair(-1, 0));
	steps.push_back(std::make_pair(0, -1));
	std::vector<char> moves{ 'R', 'D', 'U', 'L' };
	std::vector<std::vector<char> > cur_state_board;
	std::vector<int> positions{x_pos, y_pos, x_pos, y_pos};
	queue.push(Table(turn, start_board, positions, actions));
	while (!queue.empty()) {
		Table current = queue.top();
		queue.pop();
		cur_state_board = current.table;
		positions = current.pos;
		actions = current.route;
		turn = actions.size();
		if (positions[CURR_X] == 3 - 1 && positions[CURR_Y] == 3 - 1 && current.checkWin())
			return turn;
		for (size_t i = 0; i < moves.size(); ++i) {
			int x = steps[i].first, y = steps[i].second;
			if ((-1 < positions[CURR_X] + x) && (positions[CURR_X] + x < 3)
				&& (-1 < positions[CURR_Y] + y) && (positions[CURR_Y] + y < 3)
				&& !((positions[LAST_X] == positions[CURR_X] + x) && (positions[LAST_Y] == positions[CURR_Y] + y))) {
				std::swap(cur_state_board[positions[CURR_X] + x][positions[CURR_Y] + y],
					cur_state_board[positions[CURR_X]][positions[CURR_Y]]);
				std::vector < int > new_positions{positions[CURR_X] + x, positions[CURR_Y] + y,
														positions[CURR_X], positions[CURR_Y] };
				queue.push(Table(turn + 1, cur_state_board, new_positions, actions + moves[i]));
				std::swap(cur_state_board[positions[CURR_X] + x][positions[CURR_Y] + y],
					cur_state_board[positions[CURR_X]][positions[CURR_Y]]);
			}

		}


	}
	return turn;
}

int main()
{
	std::ifstream fin("puzzle.in");
	std::ofstream fout("puzzle.out");
	std::vector<std::vector<char> > board(3);
	short int x_pos, y_pos;
	for (int i = 0; i < 3; ++i) {
		char read_temp;
		for (int j = 0; j < 3; ++j) {
			fin >> read_temp;
			board[i].push_back(read_temp);
			if (read_temp == '0') {
				x_pos = i;
				y_pos = j;
			}
		}
	}
	if (!checkPos(board)) {
		fout << -1;
	}
	else {
		std::string actions = "";
		int turn = A_star(board, actions, x_pos, y_pos);
		fout << turn << "\n";
		fout << actions;
	}
	fin.close();
	fout.close();
	return 0;
}