#include "Dictionary.h"
#include <iostream>
#include <stdexcept>
#include <string>


int main(void){
   Dictionary D = Dictionary(); 
   D.setValue("a",1);
   D.setValue("b",2);
   D.setValue("c",3); 
   D.setValue("e",5); 
   D.to_string();  
}
