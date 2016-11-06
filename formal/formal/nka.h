#pragma once
#include<vector>
#include<set>
#include<string>


struct Node;

struct Edge {
	char symbol;
	Node* destination;

	Edge(){}
	Edge(Node* to, char c)
		:symbol(c)
		, destination(to)
	{};

	bool operator() (const Edge& a, const Edge& b){
		if (a.destination == b.destination)
			return a.symbol < b.symbol;
		return a.destination < b.destination;
	}
};

struct Node {
	Node() {
		edges.clear();
	};

	std::set<Edge, Edge> edges;
	bool finish;
	int id;

	void addEdge(Node* to, char c) {
		edges.insert(Edge(to, c));
	}
	void delEpsEdge(Node* to) {
		edges.erase(Edge(to, '1'));
	}
	bool operator() (const Edge& a, const Edge& b) {
		if (a.destination == b.destination)
			return a.symbol < b.symbol;
		return a.destination < b.destination;
	}
};

class NKA {
public:
	NKA(char c) {
		startNode = new Node();
		Node* fin = new Node();
		startNode->addEdge(fin, c);
		fin->finish = true;
		finish.insert(fin);
	}
	NKA() {
		startNode = new Node();
		startNode->finish = true;
		finish.insert(startNode);
	}

	void concatNKA(NKA& other) {
		for (std::set<Node*>::iterator it = finish.begin(); it != finish.end(); ++it) {
			(*it)->addEdge(other.startNode, '1');
		}
		finish.clear();
		finish = other.finish;
		other.finish.clear();
		other.startNode = NULL;
	}

	void plusNKA(NKA& other) {
		for (std::set<Node*>::iterator it = other.finish.begin(); it != other.finish.end(); ++it) {
			finish.insert(*it);
		}
		startNode->addEdge(other.startNode, '1');
		other.finish.clear();
		other.startNode = NULL;
	}

	void klin() {
		Node* newStart = new Node();
		for (std::set<Node*>::iterator it = finish.begin(); it != finish.end(); ++it) {
			(*it)->addEdge(newStart, '1');
		}
		newStart->addEdge(startNode, '1');
		startNode = newStart;
		finish.insert(startNode);
	}

	void transformToPrefix() {
		Node* oldStart = startNode;
		used.clear();
		lastId = 0;
		startNode = new Node();
		used.insert(startNode);
		list.insert(startNode);
		startNode->id = lastId++;
		delDFS(oldStart);
		transDFS(oldStart, oldStart);
		cnt = used.size() + 1;
	}

	bool tryFit(std::string str) {
		dynamic.resize(cnt + 1);
		for (int i = 0; i <= cnt; ++i) {
			dynamic[i].assign(str.size() + 1, -1);
		}
		return tryDFS(startNode, str, 0);
	}

	~NKA() {
		if (startNode != NULL && list.empty())
			delDFS(startNode);
		for (std::set<Node*>::iterator it = list.begin(); it != list.end(); ++it) {
			delete *it;
		}
	}

private:
	Node* startNode;
	std::set<Node*> finish;
	std::set<Node*> used;
	std::set<Node*> list;
	int cnt;
	int lastId;
	std::vector<std::vector<short> > dynamic;
	
	bool transDFS(Node* node, Node* nonEps) {
		std::set<Edge, Edge> edges = node->edges;
		bool q;
		used.insert(node);
		node->id = lastId++;
		startNode->addEdge(node, '1');
		for (std::set<Edge, Edge>::iterator it = edges.begin(); it != edges.end(); ++it) {
			if (used.find(it->destination) != used.end()) {
				continue;
			}
			if (it->symbol == '1') {
				q = transDFS(it->destination, nonEps);
				if (q)
					finish.insert(node);
			}
			else {
				if (node != nonEps) {
					nonEps->addEdge(it->destination, it->symbol);
				}
				transDFS(it->destination, it->destination);
			}
		}
		return finish.find(node) != finish.end();
	}

	void delDFS(Node* node) {
		std::set<Edge, Edge>& edges = node->edges;
		list.insert(node);
		for (std::set<Edge, Edge>::iterator it = edges.begin(); it != edges.end(); ++it) {
			if (list.find(it->destination) == list.end())
				delDFS(it->destination);
		}
	}

	bool tryDFS(Node* node, std::string& str, int ind) {
		if (ind == str.size()) {
			if (finish.find(node) != finish.end())
				return dynamic[node->id][ind] = true;
			else
				return dynamic[node->id][ind] = false;
		}
		if (dynamic[node->id][ind] != -1)
			return dynamic[node->id][ind];
		std::set<Edge, Edge> edges = node->edges;
		bool q = false;
		dynamic[node->id][ind] = 3;
		for (std::set<Edge, Edge>::iterator it = edges.begin(); it != edges.end(); ++it) {
			if (it->symbol == str[ind])
				q = q || tryDFS(it->destination, str, ind + 1);
			if (it->symbol == '1') {
				if (dynamic[it->destination->id][ind] == 3) {
					node->delEpsEdge(it->destination);
					continue;
				}
				q = q || tryDFS(it->destination, str, ind);
			}
			if (q)
				return dynamic[node->id][ind] = q;
		}
		return dynamic[node->id][ind] = q;
	}
};