#include "graph.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>
/* class node */
template <class T>
node<T>::node() {}

template <class T>
node<T>::node(T n) : n(n) {}

template <class T>
node<T>::~node() {
	relations.clear();
}

template <class T>
bool node<T>::add_relation(node<T>* a) {
	if (!this->relate_with(a)) {
		relations.push_back(a);
		if (!a->relate_with(this)) 
			a->relations.push_back(this);
		return true;
	}
	return false;
}

template <class T>
void node<T>::set_data(T& _n) { 
     n = _n;
}

template <class T>
T node<T>::get_data() const {
	return n;
}

template <class T>
bool node<T>::relate_with(node<T>* n) {
	for (int i=0; i<relations.size(); i++)
		if (n==relations[i]) return true;
	return false;
}

template <class T>
int node<T>::rel_size() {
	return relations.size(); 
}

template <class T>
bool node<T>::delete_relation(node<T>* n) {
	for (int i=0; i<rel_size(); i++)
		if (n==relations[i]) { 
			relations.erase(relations.begin()+i);
			for (int j=0; j<n->rel_size(); j++)
				if (this==n->relations[j])
					n->relations.erase(n->relations.begin()+j);
			return true;
		}
	return false;
}

template <class T>
std::vector<node<T>*> node<T>::get_relations() const {
	return relations;
}

/* class graph */
template <class T>
graph<T>::graph() {}

template <class T>
graph<T>::graph(const graph<T>& g) {
	std::vector<node<T>*> v;
	for (int i=0; i < g.size(); i++) {
		node<T>* new_node = new node<T>(g.nodes[i]->get_data());
		v.push_back(new_node);
	}
	this->nodes = v;
	int** a = g.adjacency_matrix();
	for (int i=0; i < size(); i++) {
		for (int j=i; j < size(); j++)
			if (a[i][j]==1) nodes[i]->add_relation(nodes[j]);
	}
}

template <class T>
graph<T>::~graph() {
	for (int i=0; i<size(); i++) 
		delete nodes[i];
	nodes.clear();
}

template <class T>
int graph<T>::size() const {
	return nodes.size();
}

template <class T>
bool graph<T>::add_node(node<T>* a) {
	if (a!=0) {
		int c=0;
		for (int i=0; i<size(); i++) {
			if (nodes[i]->get_data()==a->get_data()) c++;
		}
		if (c>1) return false;

		nodes.push_back(a);

		for (int i=0; i<size(); i++) {
		    if (a->relate_with(nodes[i])) 
				nodes[i]->add_relation(a);   
			for (int j=0; j<a->rel_size(); j++)
				if ((std::find(nodes.begin(), nodes.end(), a->get_relations()[j])==nodes.end()) && (a->get_relations()[j]!=0)) 
					add_node(a->get_relations()[j]); 
		}
		return true;
	}
}

template <class T>
bool graph<T>::add_node(T n) {
	node<T>* _n = new node<T>(n);
	return add_node(_n);
}

template <class T>
bool graph<T>::delete_node(T n) {
	for (int i=0; i<size(); i++) {
		if (nodes[i]->get_data()==n) { 
			for (int j=0; j<nodes[i]->rel_size(); j++)
				nodes[i]->get_relations()[j]->delete_relation(nodes[i]);
			nodes.erase(nodes.begin()+i);
			return true;
		}
	} 
	return false;
}

template <class T>
bool graph<T>::delete_node(node<T>* n) {
	for (int i=0; i<size(); i++) {
		if (nodes[i]==n) { 
			for (int j=0; j<nodes[i]->rel_size(); j++)
				nodes[i]->get_relations()[j]->delete_relation(nodes[i]);
			nodes.erase(nodes.begin()+i);
			return true;
		}
	} 
	return false;
}

template <class T>
int** graph<T>::adjacency_matrix() const {
	int** a = new int*[size()];
	for (int i=0; i<size(); i++) 
		a[i] = new int[size()];

	for (int i=0; i<size(); i++)
		for (int j=i; j<size(); j++)
			if (nodes[i]->relate_with(nodes[j])) {
				a[i][j]=1;
				a[j][i]=1;
			}
			else {
				a[i][j]=0;
				a[j][i]=0;
			}
	return a;
}

template <class T>
node<T>* graph<T>::operator[](int i) {
	return nodes[i];
}

template <class T>
graph<T>& graph<T>::operator=(const graph<T>& g) {
	if (this != &g) {
		for (int i=0; i<size(); i++) 
			delete nodes[i];
		nodes.clear();		

		std::vector<node<T>*> v;
		for (int i=0; i < g.size(); i++) {
			node<T>* new_node = new node<T>(g.nodes[i]->get_data());
			v.push_back(new_node);
		}
		this->nodes = v;
		int** a = g.adjacency_matrix();
		for (int i=0; i < size(); i++) {
			for (int j=i; j < size(); j++)
				if (a[i][j]==1) nodes[i]->add_relation(nodes[j]);
		}
		return *this;
	}
}

template <class T>
std::ostream &operator<<(std::ostream& o, const graph<T>& g) {
	if (g.size()==0) 
		o<<"Graph is empty!\n";
	else {
		o<<"Nodes' data in graph:\n";
		for (int i=0;i<g.size();i++)
			o<<g.nodes[i]->get_data()<<" ";
		o<<"\nAdjacency matrix for graph(order of nodes is noted above):\n";
		int** adj = g.adjacency_matrix();
		for (int i=0;i<g.size();i++) {
			for (int j=0;j<g.size();j++)
				o<<adj[i][j];
			o<<std::endl;
		}
		return o;
	}
}

template <class T>
std::istream &operator>>(std::istream& i, graph<T>& g) {
	if (i.good()) {
		int size, cur, cur2;
		T elem;
		std::string rel;
		std::vector<node<T>*> nodes;
		i>>size;
		for (int j=0; j<size; j++) {
			i>>elem;
			nodes.push_back(new node<T>(elem));
		}
		while (i>>rel) {
			if (rel=="end") break;
			if ((rel[0]=='(') && (rel[rel.size()-1]==')')) {
				std::stringstream ss(rel.substr(1,rel.size()-1));
				ss>>cur;
				if (ss.peek() == ',')
       				ss.ignore();
				else {
					i.setstate(std::ios::failbit);
					throw std::logic_error("Broken thread\n");
				}
				ss>>cur2;
				nodes[cur]->add_relation(nodes[cur2]);
			}
			else {
				i.setstate(std::ios::failbit);
				throw std::logic_error("Broken thread\n");
			}
		}
		
		for (int j=0; j<g.size(); j++) 
			delete g.nodes[j];
		g.nodes.clear();
			
		g.nodes = nodes;
		return i;
	} 
	
	else throw std::logic_error("Broken thread\n");
}