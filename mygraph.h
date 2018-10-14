#ifndef UNTITLED_MYGRAPH_H
#define UNTITLED_MYGRAPH_H

#include <bits/stdc++.h>
using namespace std;
using graph_base = map<string, set<string>>;
struct graph_t{
    graph_base G;
    map<string, bool> vis;
    void add_edge(string, string);
    void set_unvisited();
};

void graph_t::add_edge(string from, string to) {
    if(!G.count(from))
        G[from] = new
    G[from].insert(to);
}

void graph_t::set_unvisited() {
    vis.clear();
}
#endif //UNTITLED_MYGRAPH_H
