# C  Architecture Rebuild - a simple version

> Group: 10
>
> Arthur: 李天浩 16030199027
>
> https://github.com/Arch1n/CS6001L-Homework

## functions expected

Read a source code written in c, analysis the functions especially those call C MYSQL API, and generate two tables. One is about the relation of function and functions which are called in the previous function, another is about the relation of function and SQL tables which are modified in the function.

Use the so called HLK Clustering Algorithm to the matrix we generated in task1.

## functions we made

 Because of my limited ability and lack of time, we assume the input source code **NO DEFINITIONS**, **NO CUSTOM DATA TYPES**. **NO FUNCTION POINTERS**, **VERY SIMPLE SQL STATEMENTS**.

Under such assumptions, the task1 is finished.

About task2, I write a fake so called HLK Clustering Algorithm,so I can't get the correct answer :(

## details

**Process the calling of the functions as a graph .**

- use  adjacency table to store a graph.
- if in function Foo(), another function Bar() is called, we add a edge from Foo() to Bar(). 
- use Floyd-Warshall  algorithm to process the transitive closure of directed graphs .

**Process the code with regular expression .**

- regular expression is a powerful tool to process strings.

1. find all the function definitions and initialize the graph.
2. use stack to find out the implementation details of the function
3. find out the function calling and MYSQL API calling in each function and build the graph.


