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
            auto p = G_ft.G[name];
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
