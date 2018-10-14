#include <bits/stdc++.h>
#include "mygraph.h"
using namespace std;

graph_t G;

void read_code(istream&, string&);
void parsing_func(const string&);

int main(int argc, char** argv) {
    assert(argc == 2);
    ifstream fin(argv[1]);
    string code;
    read_code(fin, code);

    return 0;
}




void read_code(istream &in, string &code){
    code.clear();
    string line;
    while(getline(in, line)){
        code += line;
    }
}