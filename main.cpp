/* gu ji ming tian jiu kan bu dao zhe xie dai ma le
 * yin wei ming tian chong gou
 * GGWP
 *
 *\w+( )(?!\b(if|while|for)\b)\b\w+(?=\s*\()(\()(.+)(\))(;)
 * */

#include <bits/stdc++.h>
#include "mygraph.h"
using namespace std;

graph_t G;

void read_code(istream&, string&);
void find_func(const string&);
void parsing_func(const string&);

int main(int argc, char** argv) {
    assert(argc == 2);
    ifstream fin(argv[1]);
    string code;
    read_code(fin, code);

    return 0;
}

/* TYPE FUNCNAME (ARGUMENT){ STATEMENT;}
 * TYPE FUNCNAME (ARGUMENT);
 *
 *
 * */
void find_func(const string& code){
    stack<char> S;
    deque<string> history;
    stringstream ss(code);
    string word;
    while(ss>>word){

    }

}
void parsing_func(const string& code){

}

void read_code(istream &in, string &code){
    code.clear();
    string line;
    while(getline(in, line)){
        code += line;
    }
}
