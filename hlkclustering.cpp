#include <bits/stdc++.h>
using namespace std;
struct node_t{
    int ori_id;
    int mark;
};
map<pair<int, int> ,bool > G;
vector<node_t> col, row;

void work(istream &, ostream &);

int main(int argc, char **argv){
    assert(argc > 1);
    work(ifstream(argv[1]), ofstream(argv[2]));
    return 0;
}

void work(istream &in, ostream &out){

}
