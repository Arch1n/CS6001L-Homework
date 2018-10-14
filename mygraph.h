#ifndef UNTITLED_MYGRAPH_H
#define UNTITLED_MYGRAPH_H

#include <bits/stdc++.h>
using namespace std;

using func = string;
using edge_list = set<func>;
using graph_base = map<string, edge_list *>;


struct graph_t{
    graph_base G;
    map<string, bool> vis;
    void add_def(string);
    void add_edge(string, string);
    void set_unvisited();
};

void graph_t::add_def(string cur) {
    if(!G.count(cur))
        G[cur] = new edge_list;
}

void graph_t::add_edge(string from, string to) {
    add_def(from);
    add_def(to);
    G[from]->insert(to);
}

void graph_t::set_unvisited() {
    vis.clear();
}
#endif //UNTITLED_MYGRAPH_H
