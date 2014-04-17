#include <iostream>
#include "graph.h"
#include "graph.cpp"
#include <stdexcept>
using namespace std;

int main() {
	graph<int> g;
	node<int>* a=new node<int>(3);
	g.add_node(a);
	g.add_node(2);
	a->add_relation(g[1]);
	cout << g;
	
	graph<std::string> h;
	cin >> h;
	cout << h;
	return 0;
}