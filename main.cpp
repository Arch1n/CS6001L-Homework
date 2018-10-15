#include <bits/stdc++.h>
#include "mygraph.h"


using namespace std;

graph_t G_ff, G_ft;


void load_code(string&, const char*);

string get_func_body(const string&, const smatch&);
void find_func_def(const string&);
void find_func_body(const string&);
string get_func_name(const string&);

void ana_func(const string&, const smatch&);
void find_func_call(const string&, const smatch&);
void find_sql_table(const string&);

int main(int argc, char **argv) {
    assert(argc == 2);
    freopen("out.txt", "w", stderr);
    string code;
    load_code(code, argv[1]);
    find_func_def(code);
    //find_func_body(code);
    //cout<<s<<endl;

    //std::regex self_regex("\\w+", std::regex_constants::ECMAScript | std::regex_constants::icase);
    //if (std::regex_search(s, self_regex)) {
    //    std::cout << "Text contains the phrase 'regular expressions'\n";
    //}



//    std::regex long_word_regex("(\\w{7,})");
//    std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
//    std::cout << new_s << '\n';
}

void find_func_call(const string& body,const smatch& info){
    regex word_regex("(?!\\b(if|while|for)\\b)\\b[a-zA-Z0-9_]+(?=\\s*)(\\()(.+)(\\))([,;])");
    auto words_begin = sregex_iterator(body.begin(), body.end(), word_regex);
    auto words_end = sregex_iterator();
//    std::cout << "Found "
//              << std::distance(words_begin, words_end)
//              << " words\n";
    string from = get_func_name(info.str());
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        smatch match = *i;
        string match_str = match.str();
        string to = get_func_name(string(" ")+match_str);
        cerr<<"add edge :\t"<<from<<"\t->\t"<<to<<endl;
        //cerr <<match.position() << "\t\t"<<match_str << '\n';
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
    regex word_regex("[a-zA-Z0-9_](\\*?)+( )(?!\\b(if|while|for)\\b)\\b\\w+(?=\\s*\\()(\\()(.+)(\\))(\\s?)(\\{)");
    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = sregex_iterator();

    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        string body = get_func_body(s, match);
        cerr <<get_func_name(match_str)<<":\n----------------------------------------------------\n";
        find_func_call(body, match);
        cerr<<"\n----------------------------------------------------\n";
        //G_ff.add_def(get_func_name(s));
        //cerr <<get_func_name(match_str) << "\t\t"<<match_str << '\n';
    }
}

void find_func_def(const string& s){
    regex word_regex("[a-zA-Z0-9_]+\\*?( )(?!\\b(if|while|for)\\b)\\b\\w+(?=\\s*\\()(\\()(.+)(\\))([;\\{])");
    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        G_ff.add_def(get_func_name(s));
        cerr <<get_func_name(match_str) << "\t\t"<<match_str << '\n';
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