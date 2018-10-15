# C  Architecture Rebuild - a simple version

> Group: 10
>
> Arthur: 李天浩 16030199027
>
> https://github.com/Arch1n/CS6001L-Homework

## functions expected

Read a source code written in c, analysis the functions especially those call C MYSQL API, and generate two tables. One is about the relation of function and functions which are called in the previous function, another is about the relation of function and SQL tables which are modified in the function.

## functions we made

 Because of my limited ability and lack of time, we assume the input source code **NO DEFINITIONS**, **NO CUSTOM DATA TYPES**. **NO FUNCTION POINTERS**, **VERY SIMPLE SQL STATEMENTS**.

Under such assumptions, the task is finished.

## details

**Process the calling of the functions as a graph .**

- use  adjacency table to store a graph.
- if in function Foo(), another function Bar() is called, we add a edge from Foo() to Bar(). 
- use Floyd-Warshall  algorithm to process the transitive closure of directed graphs .

**Process the code with regular expression. **

- regular expression is a powerful tool to process strings.

1. find all the function definitions and initialize the graph.
2. use stack to find out the implementation details of the function
3. find out the function calling and MYSQL API calling in each function and build the graph.

## Code

### main.cpp

the entrance of the program, about the function processing the code and build the graph

```c++
#include <bits/stdc++.h>
#include "mygraph.h"
#include "mycsv.h"

using namespace std;

graph_t G_ff, G_ft;


void load_code(string&, const char*);

string get_func_body(const string&, const smatch&);
void find_func_def(const string&);
void find_func_body(const string&);
string get_func_name(const string&);

void find_func_call(const string&, const smatch&);
void find_sql_table(const string&, const smatch&);

void print_csv();

int main(int argc, char **argv) {
    assert(argc == 2);
    freopen("out.txt", "w", stderr);
    string code;
    load_code(code, argv[1]);
    find_func_def(code);
    find_func_body(code);
    print_csv();
    return 0;
}

void find_sql_table(const string& body, const smatch& info){
    string from = get_func_name(info.str());
    regex word_regex("(mysql_query\\()([a-zA-Z0-9&_]+)(\\s?)(,)(\\s?)(\")(.+)(\"\\);)");
    auto words_begin = sregex_iterator(body.begin(), body.end(), word_regex);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        string sql, word, result;
        size_t idx = 0;
        while(match_str[idx++] != '\"');
        while(match_str[idx++] != '\"')
            sql += match_str[idx-1];
        for(auto &c: sql)
            c = tolower(c);
        //cerr << sql;
        stringstream ss(sql);
        ss>>word;
        if(word == "select"){
            ss >> word;
            ss >> word;
            assert(word == "from");
            while(ss>>word) {
                if(word == "where")
                    break;
                word += ' ';
                result += word;
            }
        } else if(word == "insert"){
            ss >> word;
            assert(word == "into");
            ss >> word;
            result = word;
        } else if(word == "create"){
            ss >> word;
            if(word == "table"){
                ss >> word;
                result = word;
            } else {
                continue;
            }
        } else if(word == "drop"){
            ss >> word;
            if(word == "table"){
                ss >> word;
                result = word;
            } else {
                continue;
            }
        }
        for(auto &c: result){
            if(c == ',')
                c = ' ';
        }
        ss.clear();
        stringstream res(result);
        while(res >> word){
            cerr << "add table " << word << " to func "<< from<<endl;
            G_ft.add_edge(from, word);
        }
    }
}

void find_func_call(const string& body, const smatch& info){
    regex word_regex("(?!\\b(if|while|for)\\b)\\b[a-zA-Z0-9_]*(?=\\s*)(\\()(.+)(\\))([,;])");
    auto words_begin = sregex_iterator(body.begin(), body.end(), word_regex);
    auto words_end = sregex_iterator();
    string from = get_func_name(info.str());
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        string to = get_func_name(string(" ")+match_str);
        if(G_ff.has_func(to)){
            G_ff.add_edge(from, to);
            cerr<<"add edge :\t"<<from<<"\t->\t"<<to<<endl;
        }
    }
}

string get_func_body(const string& s, const smatch& info){
    string this_func_name = get_func_name(info.str());
    long idx_l = info.position();
    while(s[idx_l++] != '{');
    long idx_r = idx_l;
    idx_l --;
    stack<char> S;
    S.push('{');
    while(!S.empty()){
        const char& cur = s[idx_r];
        if(cur == '(' || cur == '{')
            S.push(cur);
        else if(cur == ')' && S.top() == '(')
            S.pop();
        else if(cur == '}' && S.top() == '{')
            S.pop();
        idx_r ++;
    }
    return s.substr(idx_l, idx_r-idx_l);
    //cerr<<this_func_name<<":\n"<<s.substr(idx_l, idx_r-idx_l)<<"\n----------\n";
}

void find_func_body(const string& s){
    regex word_regex("[a-zA-Z0-9_]+\\*?( )([a-zA-Z0-9_]*)(\\s?)(\\()(.*)(\\))(\\s?)([\\{])");
    //regex word_regex("[a-zA-Z0-9_](\\*?)+( )(?!\\b(if|while|for)\\b)\\b\\w*(?=\\s*\\()(\\()(.+)(\\))(\\s?)(\\{)");
    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        string body = get_func_body(s, match);
        cerr <<get_func_name(match_str)<<":\n----------------------------------------------------\n";
        find_func_call(body, match);
        find_sql_table(body, match);
        cerr<<"\n----------------------------------------------------\n";
    }
}

void find_func_def(const string& s){
    regex word_regex("[a-zA-Z0-9_]+\\*?( )([a-zA-Z0-9_]*)(\\s?)(\\()(.*)(\\))(\\s?)([;\\{])");
    //regex word_regex("[a-zA-Z0-9_]+\\*?( )(?!\\b(if|while|for)\\b)\\b[a-zA-Z0-9_]*(?=\\s*\\()(\\()(.+)(\\))([;\\{])");
    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        G_ff.add_def(get_func_name(match_str));
        //cerr <<get_func_name(match_str) << "\t\t"<<match_str << '\n';
    }
}

string get_func_name(const string& s){
    string ans;
    size_t idx = 0;
    while(s[idx++] != ' ');
    while(s[idx++] != '(') ans += s[idx - 1];
    return ans;
}

void load_code(string &code, const char* filename){
    ifstream in(filename);
    string tmp, line;
    while(getline(in, tmp)){
        bool flag = true;
        line.clear();
        for(const auto& c: tmp){
            if(c != ' ' )
                flag = false;
            if(flag && c == ' ')
                continue;
            line += c;
        }
        line += '\n';
        code += line;
    }
}
```

### mygraph.h

a data structure about graph

```c++
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
    inline bool has_func(string s){return G.count(s);}
    void add_def(string);
    void add_edge(string, string);
    void set_unvisited();
};


void graph_t::add_def(string cur) {
    if(!has_func(cur))
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

```



### mycsv.h

output the result to csv files.

```c++
#ifndef CS6001L_HOMEWORK_MYCSV_H
#define CS6001L_HOMEWORK_MYCSV_H

#include "mygraph.h"
#include <bits/stdc++.h>

extern graph_t G_ff, G_ft;

void print_csv(){
    int id = 1, idf_max , idt_max;
    map<string, int> func_to_int, table_to_int;
    map<int, string> int_to_func, int_to_table;
    for(const auto& func: G_ff.G){
        func_to_int[func.first] = id;
        int_to_func[id] = func.first;
        id++;
    }
    idf_max = id;
    id = 1;
    for(const auto& func: G_ft.G){
        for(auto &table: *(func.second)){
            if(!table_to_int.count(table)){
                table_to_int[table] = id;
                int_to_table[id] = table;
                id++;
            }
        }
    }
    for(const auto& k: G_ff.G){
        for(const auto& i: G_ff.G){
            for(const auto& j: G_ff.G){
                edge_list & i_ = *(i.second);
                edge_list & k_ = *(k.second);
                if(i_.count(k.first) && k_.count(j.first))
                    i_.insert(j.first);
            }
        }
    }
    idt_max = id;
    ofstream fout("ff.csv");
    fout<<',';
    for(int i=1; i<idf_max; i++){
        fout<<int_to_func[i]<<',';
    }
    fout<<'\n';
    for(int i=1; i<idf_max; i++){
        fout<<int_to_func[i]<<',';
        for(int j=1; j<idf_max; j++){
            edge_list &i_ = *(G_ff.G[int_to_func[i]]);
            func j_ = int_to_func[j];
            if(i_.count(j_))
                fout<<'*';
            else fout<<' ';
            fout<<',';
        }
        fout<<'\n';
    }
    fout.close();
    fout.open("ft.csv");
    fout<<',';
    for(int i=1; i<idt_max; i++){
        fout<<int_to_table[i]<<',';
    }
    fout<<'\n';
    for(int i=1; i<idf_max; i++){
        func name = int_to_func[i];
        fout<< name <<',';
        set<string> H;
        auto p = G_ft.G[name];
        set<func> lis;
        if(p != NULL){
            lis = *(G_ft.G[name]);
            for(auto& c: lis)
                H.insert(c);
        }
        lis = *(G_ff.G[name]);
        for(const auto& c: lis){
            auto p = G_ft.G[c];
            if(p != NULL){
                auto lis_ = *p;
                for(const auto& ele: lis_)
                    H.insert(ele);
            }
        }
        for(int j=1; j<idt_max; j++){
            if(H.count(int_to_table[j]))
                fout<<'*';
            else
                fout<<' ';
            fout<<',';
        }
        fout<<'\n';
    }

}

#endif //CS6001L_HOMEWORK_MYCSV_H

```



