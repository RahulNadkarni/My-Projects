/*********************************************************************************
 * * *  * * Rahul Nadkarni, rnadkarn
 * * *  * * 2022 Spring CSE101 PA8
 * * *  * * Order.cpp
 * * *  * * Order File
 *************************************************************************************/
#include<iostream>
#include<fstream>
#include<string>
#include "Dictionary.h"

int main(int argc, char *argv[]){
    std::ifstream fp;    //open files
    std::ofstream op;
    fp.open(argv[1]);
    op.open(argv[2]);
    int val = 0; //value = line number 
    std::string key;  //key = read 
    Dictionary D = Dictionary();   //new dictionary
    while(std::getline(fp,key)){   //read file
	val += 1;  //value 
        D.setValue(key,val);    //set the value 
    }
    op << D.to_string()<<std::endl;   //print inorder 
    op << D.pre_string();  //print post order
    fp.close(); 
    op.close(); 
}
