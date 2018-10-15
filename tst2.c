#include<mysql.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

static void output_error(MYSQL *mysql);

int main(int argc, char *argv[]) {
    MYSQL mysql;
    MYSQL_RES *result;
    MYSQL_ROW row;
    MYSQL_FIELD *fields;
    const char *host = "localhost";
    const char *user = "root";
    const char *password = "root";
    const char *database = "test";
    const int port = 3306;
    const char *socket = NULL;
    const int flag = 0;
    const char *sql;
    int num_fields;
    unsigned long *lengths;
    int i;
    //initialize the database   
    if (!mysql_init(&mysql)) {
        output_error(&mysql);
    }
    printf("mysql initialized successfully ! /n");
    //connect to the database;  
    if (!mysql_real_connect(&mysql, host, user, password, database, port, socket, flag)) {
        output_error(&mysql);
    }
    printf("mysql connect successfully! /n");
    printf("/n/n/nthe content of the table data in the database test/n");
    printf("-----------------------------------------------------------/n");

    mysql_query(&mysql , "select * from a");
    mysql_query(&mysql , "select * from a,b");
    mysql_query(&mysql , "select * from a, b");
    mysql_query(&mysql , "select * from a ,b");
    mysql_query(&mysql , "insert into e");
    mysql_query(&mysql , "create table c");
    mysql_query(&mysql , "create database d");
    mysql_query(&mysql , "drop table c");
    output_error(&mysql);

    //fetch the the result set of the query!          
    result = mysql_store_result(&mysql);
    if (result) {
        fields = mysql_fetch_fields(result);    // fetch the struct of result          
        num_fields = mysql_num_fields(result);  // fetch the number of result fields;  

        //lengths = mysql_fetch_lengths(result);      
        for (i = 0; i < num_fields; i++) {
            printf("%s/t", fields[i].name);
        }
        printf("/n");
        while (row = mysql_fetch_row(result)) {
            for (i = 0; i < num_fields; i++) {
                printf("%s /t", row[i]);
            }
            printf("/n");
        }
        //release the result of set  for release the memory  
        mysql_free_result(result);

    } else {
        output_error(&mysql);
    }
    printf("/n/n-----------------------------------------------------------/n");
    //close the connetion to the database  
    mysql_close(&mysql);
    return 0;
}

static void output_error(MYSQL *mysql) {
    fprintf(stderr, "errorno: %d /n", mysql_errno(mysql));
    fprintf(stderr, "error info: %s/n", mysql_error(mysql));
    exit(1);
}

