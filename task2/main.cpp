#include <bits/stdc++.h>
using namespace std;
struct node_t{
    size_t ori_id;
    string name;
    int mark;
    node_t(int a, string b):ori_id(a), name(b){mark = 0;}
};
map<pair<size_t , size_t> , bool> G;
vector<node_t> col, row;
map<string, size_t> id_col, id_row;

void init(const char*);
void work();
int SumR(size_t, size_t, size_t);
int SumC(size_t, size_t, size_t);
void print();

int main(int argc, char **argv){
    assert(argc > 1);
    init(argv[1]);
    work();
    print();
    return 0;
}

void work(){
    //freopen("out.txt", "w", stderr);
    int MarkR, MarkC;
    size_t M = row.size();
    size_t N = col.size();
    size_t I = col.size()>>1;
    size_t J ;
    int times = 20000;
    do {
        //cerr<<times<<" :I = "<<I<<endl;
        MarkR = 0;
        for(size_t id = 0; id < M; id++){
            if(SumR(id, 0, I) >= SumR(id, I, N))
                row[id].mark = 1;
            else
                row[id].mark = 2;
            if(MarkR != row[id].mark)
                MarkR = 1;
        }
        sort(row.begin(), row.end(), [](const node_t& a, const node_t& b){ return  a.mark < b.mark;});
        J = 0;
        for(const auto&c: row) J += c.mark == 1;
        //for(const auto&c: row) cerr<<c.name<<" ";cerr<<endl;
        //cerr<<times<<" :J = "<<J<<endl;
        MarkC = 0;
        for(size_t id = 0; id < N; id++){
            if(SumC(id, 0, J) >= SumC(id, J, M))
                col[id].mark = 1;
            else
                col[id].mark = 2;
            if(MarkC != col[id].mark)
                MarkC = 1;
        }
        sort(col.begin(), col.end(), [](const node_t& a, const node_t& b){ return  a.mark < b.mark;});
        I = 0;
        for(const auto&c: col) I += c.mark == 1;
        //for(const auto&c: col) cerr<<c.name<<" ";cerr<<endl;
    } while (times-- && (MarkR == 1 || MarkC == 1));
    cerr<<times<<endl;
}




void init(const char* filename){
    //freopen("out.txt", "w", stderr);
    ifstream fin(filename);
    string line, word;
    getline(fin, line);
    for(auto &c: line)
        if(c==',') c = ' ';
    stringstream ss(line);
    size_t id = 0;
    while(ss >> word){
        if(id_col.count(word) == 0)
            id_col[word] = id++;
        col.emplace_back(id_col[word], word);
    }
    ss.clear();
    id = 0;
    while (getline(fin, line)){
        for(auto &c: line){
            if(c==' ') c = '@';
            if(c==',') c = ' ';
        }
        stringstream ss(line);
        string name;
        ss >> name;
        if(id_row.count(name) == 0)
            id_row[name] = id++;
        row.emplace_back(id_row[name], name);
        int i=0;
        while(ss >> word){
            G.insert(make_pair(make_pair(id_row[name], i), word == "*"));
            i++;
        }
    }
    /*
    for(size_t i = 0; i < col.size(); i++){
        for(size_t j = 0; j < row.size(); j++){
            col[i].sum += G[make_pair(i, j)];
        }
        //cerr <<"colsum["<<i<<"] is " << col[i].sum <<endl;
    }
    for(size_t i = 0; i < row.size(); i++){
        for(size_t j = 0; j < col.size(); j++){
            row[i].sum += G[make_pair(j, i)];
        }

        //cerr <<"rowsum["<<i<<"] is " << row[i].sum <<endl;
    }
     */
}

int SumR(size_t id, size_t l, size_t r){
    int cnt = 0;
    id = row[id].ori_id;
    for(size_t j = l; j < r; j++){
        size_t j_ = col[j].ori_id;
        cnt += G[make_pair(id, j_)];
    }
    return cnt;
}

int SumC(size_t id, size_t l, size_t r){
    int cnt = 0;
    id = col[id].ori_id;
    for(size_t j = l; j < r; j++){
        size_t j_ = row[j].ori_id;
        cnt += G[make_pair(j_, id)];
    }
    return cnt;
}

void print(){
    freopen("out.csv", "w", stderr);
    cerr<<",";
    for(const auto& c: col)
        cerr<<c.name<<',';
    cerr<<'\n';
    for(const auto& c: row){
        cerr<<c.name<<',';
        for(const auto&d: col){
            cerr<<(G[make_pair(c.ori_id, d.ori_id)] ? '*': ' ');
            cerr<<',';
        }
        cerr<<'\n';
    }
}