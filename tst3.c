#include <mysql.h>
#include <stdio.h>

void A(int z){
    mysql_query(a,"select * from a,aa,aaa");
}

void B(int z){
    mysql_query(a,"select * from a,b,c");
}

void C(int z){
    mysql_query(a,"select * from b,d,e");
}

void D(int z){
    mysql_query(a,"select * from d,dd,ddd");
}

void E(int z){
    mysql_query(a,"select * from e,ee,a,b,c");
}

void ABC(int z){
    A(1);
    B(2);
    C(3);
}

void DE(int z){
    D(z);
    E(z);
}

void ABCDE(int z){
    ABC(z);
    DE(z);
}

int main(){
    ABCDE(111);
    return 0;
}
