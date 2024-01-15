/*********************************************************************************
 *  *  * *  * * Rahul Nadkarni, rnadkarn
 *  * * *  * * 2022 Spring CSE101 PA8
 *  * * *  * * WordFrequency.cpp
 *  * * *  * * Count frequency of words in an infile
 **************************************************************************************/
#include<iostream>
#include<fstream>
#include "Dictionary.h"
#include <sstream>
#include <string.h>
#include <algorithm>
#include <locale>
int main(int argc, char *argv[]){
    std::ifstream fp; 
    std::ofstream op; 
    fp.open(argv[1]); 
    op.open(argv[2]);  
    std::string line; 
    std::string delim = " \n\t\\\"\',<.>/?;:[{]}|`~!@#$^&*()%-_=+0123456789";
    Dictionary D = Dictionary(); 
    std::string tokenBuffer;
    std::string token; 
    size_t begin,end,len; 
    while(getline(fp,line)){
	len = line.length();  
	tokenBuffer = "";
	begin = std::min(line.find_first_not_of(delim,0),len); 
	end  = std::min(line.find_first_of(delim,begin),len); 
	token = line.substr(begin,end-begin); 
	while(token != ""){
	   int l = token.length(); 
	   for(int i = 0; i <l; i++){
	      token[i] = std::tolower(token[i]); 
	} 
	   if(D.contains(token)){
		int value = D.getValue(token); 
		D.setValue(token,value+1); 
	   }
	   else{
		D.setValue(token,1); 
	   }
	   begin = std::min(line.find_first_not_of(delim,end+1),len); 
	   end = std::min(line.find_first_of(delim,begin),len); 
	   token = line.substr(begin,end-begin); 
	}
    }
    op<<D.to_string()<<std::endl; 
}
