#ifndef GRAPH_H
#define GRAPH_H

#define TEMPLATE_MAGIC

#include <vector>
#include <iostream>

template<class T>
class graph;

template<class T>
std::ostream &operator<<(std::ostream& o, const graph<T>& g);

template<class T>
std::istream &operator>>(std::istream& i, graph<T>& g);

template<class T>
class node {
	T n;
	std::vector<node<T>*> relations;
public:
	node();
	node(T);
	~node();
	bool add_relation(node<T>*);
	bool delete_relation(node<T>*);
	void set_data(T&);
	T get_data() const;
	int rel_size();
	bool relate_with(node<T>*);
	std::vector<node<T>*> get_relations() const;
};

template<class T>
class graph {
	std::vector<node<T>*> nodes;
public:
	graph();
	graph(const graph<T>& g);
	~graph();
	int** adjacency_matrix() const;
	bool add_node(node<T>*);
	bool add_node(T);
	bool delete_node(T);
	bool delete_node(node<T>*);
	int size() const;
	node<T>* operator[](int);
	graph<T>& operator=(const graph& g);
	friend std::ostream &operator<< <> (std::ostream& o, const graph<T>& g);
	friend std::istream &operator>> <> (std::istream& i, graph<T>& g);
};

#endif
