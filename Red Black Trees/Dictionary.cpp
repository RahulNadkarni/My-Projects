/*********************************************************************************
 *  * *  * * Rahul Nadkarni, rnadkarn
 * * *  * * 2022 Spring CSE101 PA8
 * * *  * * Dictionary.cpp
 * * *  * * Dictionary ADT
 *************************************************************************************/
#include "Dictionary.h"
#include <iostream>
#include <stdexcept>
#include <string>
//void Transplant(Dictionary& T);
//void setValueHelp(Dictionary& T, keyType k, valType v);

//node struct
Dictionary::Node::Node(keyType k, valType v){
    key = k; 
    val = v; 
    parent = nullptr; 
    left = nullptr; 
    right = nullptr;
    color = 0; 
}

//print inorder walk of the tree
void Dictionary::inOrderString(std::string& s, Dictionary::Node* R) const{
    if(R!= nil){
	inOrderString(s,R->left); 
	s += R->key; 
	s += " : ";
        std::string value = std::to_string(R->val); 
        s+= value;  
	s += "\n";
	inOrderString(s,R->right);  
    	
    }
}
//print preorder walk of the tree
void Dictionary::preOrderString(std::string& s, Dictionary::Node* R) const{
    if(R != nil){
        s+=(R->key); 
	s+="\n"; 
	preOrderString(s,R->left); 
	preOrderString(s,R->right);
    }
}
//copy the tree in a preorder fashion and sets the root
void Dictionary::preOrderCopy(Dictionary::Node*R, Dictionary::Node* N){
     if(R != N){
	setValue(R->key,R->val); 
	preOrderCopy(R->left,N); 
   	preOrderCopy(R->right,N); 
        }
}   
//deletes the tree in a postorder fashion
void Dictionary::postOrderDelete(Dictionary::Node* R){
    if(R == nil){
 	return; 
    } 
    postOrderDelete(R->left); 
    postOrderDelete(R->right); 
    delete(R);  
}
//searches for a key k in the tree
Dictionary::Node* Dictionary::search(Dictionary::Node* R, keyType k) const{
    if(R == nil || k.compare(R->key) == 0){
	return R; 
    }
    else if ( k.compare(R->key) < 0){
	return search(R->left, k); 
    }
    else{
	return search(R->right, k); 
    }
}
//finds the left most node of the tree
Dictionary::Node* Dictionary::findMin(Dictionary::Node* R){
	while(R->left != nil){
	    R = R->left;
	}
    return R; 
}
//finds the right most node of the tree
Dictionary::Node* Dictionary::findMax(Dictionary::Node* R){
        while(R->right != nil){
	    R = R->right; 
	}
    return R; 
}
//finds the successor of the node in the tree
Dictionary::Node* Dictionary::findNext(Dictionary::Node* N){
    if(N->right != nil){
	return findMin(N->right); 
    }
    Node* y = N->parent; 
    while(y != nil && N == y->right){
	N = y; 
        y = y->parent; 
    }
    return y; 
} 
//finds the predecessor of the node in the tree
Dictionary::Node* Dictionary::findPrev(Dictionary::Node* N){
    if(N->left != nil){
	return findMax(N->left); 
    }
    Node* y = N->parent; 
    while(y != nil && N == y->left){
	N = y; 
 	y = y->parent; 
    } 
    return y;
}
//rotate left
void Dictionary::LeftRotate(Dictionary::Node* R,Dictionary:: Node* x){
    Node* y = x->right; 
    x->right = y->left; 
    if(y->left != nil){
	y->left->parent = x; 
    }
    y->parent = x->parent; 
    if(x->parent == nil){
	root = y; 
    }
    else if(x == x->parent->left){ 
 	x->parent->left = y;
    }
    else{
	x->parent->right = y; 
    }
    y->left = x; 
    x->parent = y; 
}
//rotate right
void Dictionary::RightRotate(Dictionary::Node* R, Dictionary::Node* x){
    Node* y = x->left; 
    
    x->left = y->right; 
    if(y->right != nil){
	y->right->parent = x; 
    }
    y->parent = x->parent; 
    if(x->parent == nil){
	root = y; 
    }
    else if(x == x->parent->right){
	x->parent->right = y; 
    }
    else{
	x->parent->left = y; 
    }
    y->right = x; 
    x->parent = y;
}
//insert fix up rotations
void Dictionary::RB_InsertFixUp(Dictionary::Node* R, Dictionary::Node* z){
     while(z->parent->color == 1){
	if(z->parent == z->parent->parent->left){
	    Node* y = z->parent->parent->right; 
	    if(y->color == 1){
		z->parent->color = 0; 
		y->color = 0; 
		z->parent->parent->color = 1; 
		z = z->parent->parent; 
	    }
	    else{
		if(z == z->parent->right){
		    z = z->parent; 
		    LeftRotate(root,z); 
		}
		z->parent->color = 0; 
		z->parent->parent->color = 1; 
		RightRotate(root,z->parent->parent); 
	    }
	}
	else{
	    Node* y = z->parent->parent->left; 
	    if(y->color == 1){
	 	z->parent->color = 0; 
		y->color = 0; 
		z->parent->parent->color = 1; 
		z = z->parent->parent; 
	    }
	    else{
	        if(z == z->parent->left){
		    z = z->parent; 
		    RightRotate(root,z); 
		}
		z->parent->color = 0;
		z->parent->parent->color = 1; 
		LeftRotate(root,z->parent->parent); 
	    }
	}
    }
  root->color = 0; 
}
//transplant for delete 
void Dictionary::RB_Transplant(Dictionary::Node* R, Dictionary::Node* u, Dictionary::Node* v){
    if(u->parent == nil){
	root = v; 
    }
    else if(u == u->parent->left){
	u->parent->left = v; 
    }
    else{
	u->parent->right = v; 
    }
    v->parent = u->parent; 
}
//delete
void Dictionary::RB_DeleteFixUp(Dictionary::Node* R, Dictionary::Node* x){
    while(x != root && x->color == 0){
	if(x == x->parent->left){
	    Node* w = x->parent->right; 
	    if(w->color == 1){
		w->color = 0;
		x->parent->color = 1; 
		LeftRotate(root,x->parent); 
		w = x->parent->right; 
	    }
	    if(w->left->color == 0 && w->right->color == 0){
		w->color = 1; 
		x =x->parent; 
	    }
	    else{
	        if(w->right->color == 0){
		    w->left->color = 0; 
		    w->color = 1; 
		    RightRotate(root,w); 
		    w = x->parent->right; 
	  	}
		w->color = x->parent->color; 
		x->parent->color = 0 ; 
		w->right->color = 0; 
		LeftRotate(root,x->parent); 
		x = root; 
	   }
        }
	else{
	    Node* w = x->parent->left; 
	    if(w->color == 1){
		w->color = 0; 
		x->parent->color = 1; 
		RightRotate(root,x->parent); 
		w = x->parent->left; 		
	    }
	    if(w->right->color == 0 && w->left->color == 0){
		w->color = 1; 
		x = x->parent; 
	    }
	    else{
		if(w->left->color == 0){
		    w->right->color = 0; 
		    w->color = 1; 
		    LeftRotate(root,w); 
		    w = x->parent->left; 
	   	}
		w->color = x->parent->color; 
		x->parent->color = 0; 
		w->left->color = 0; 
		RightRotate(root,x->parent); 
		x= root; 
	   }
     }
  }
 x->color = 0; 
}
void Dictionary::RB_Delete(Dictionary::Node* R, Dictionary::Node* z){
    Node* y = z; 
    int y_og = y->color; 
    Node* x; 
    if(z->left == nil){
	x = z->right; 	
   	RB_Transplant(root,z,z->right);
    }
    else if(z->right == nil){
	x = z->left; 
	RB_Transplant(root,z,z->left); 
    }
    else{
	y = findMin(z->right); 
	y_og = y->color; 
	x = y->right; 
	if(y->parent == z){
	    x->parent = y;
	}
	else{
	    RB_Transplant(root,y,y->right); 
	    y->right = z->right; 
	    y->right->parent = y; 
	}
	RB_Transplant(root,z,y); 
	y->left = z->left; 
	y->left->parent = y; 
	y->color = z->color; 
   }
   if(y_og == 0){
	RB_DeleteFixUp(root,x); 
   }
}
//constructor
Dictionary::Dictionary(){
    nil = new Node("dummy",-1000000);  
    root = nil;
    current = nil; 
    num_pairs = 0; 
}
//copy constructor
Dictionary::Dictionary(const Dictionary& D){  
    nil = new Node("dummy",-1000000); 
    root = nil; 
    current = nil;
    num_pairs = 0;
    preOrderCopy(root,nil);
}
//destructor
Dictionary::~Dictionary(){
    clear(); 
}
//return size 
int Dictionary::size() const{
     return num_pairs;
}
//true if it contains k, false else 
bool Dictionary::contains(keyType k) const{
    if(num_pairs == 0){
	return false; 
    }
    if(search(root,k) != nil){
        return true; 
    }
    return false;  
}
//returns the value of the key that was looked up
valType& Dictionary::getValue(keyType k) const{
    return search(root,k)->val; 
}
//true if current is not nil, false else
bool Dictionary::hasCurrent() const{
	if(current != nil){
	    return true; 
	}
	return false; 
}
//return's current's key if not nil
keyType Dictionary::currentKey() const{
    if(hasCurrent()){
        return current->key; 
    }
    else{
	throw std::logic_error("current is nil");
    }
}
//returns current's value if not nil
valType& Dictionary::currentVal() const{
    if(hasCurrent()){
	return current->val;
    }
    throw std::logic_error("current is nil");       
}
//clear the dictionary by calling postorderdelete
void Dictionary::clear(){
    if(num_pairs == 0 && root == nil && current == nil){
	return; 
    }
    postOrderDelete(root);
    num_pairs = 0;
    current = nil;      
}
//inserts a value into the tree
void Dictionary::setValue(keyType k, valType v){
	Node* z = new Node(k,v);
	Node* y = nil; 
	Node* x = root; 
	num_pairs += 1; 
        while(x != nil){
	    if(x->key == k){
	        x->val = v; 
	  	num_pairs -= 1; 
		RB_InsertFixUp(root,x); 
		return; 
	    }
	    y = x;
	    if((z->key).compare(x->key) < 0){
		x = x->left; 
	    }
	    else if(x->key == k){
		x->val = v;  
	        return; 
	    }
	    else{
		x = x->right; 
	    }
        }
        z->parent = y; 
	if(y == nil){
	    root = z; 
	}
	else if(z->key.compare(y->key) < 0){
	    y->left = z; 
	}
	else if(z->key.compare(y->key) > 0){
	    y->right = z; 
	}
	z->left = nil; 
	z->right = nil; 
 	z->color = 1;
	RB_InsertFixUp(root,z); 
}
//deletes a value from the tree
void Dictionary::remove(keyType k){
    num_pairs -= 1;
    Node* s = search(root,k);
    if(s->key == current->key && s->val == current->val){
	current = nil; 
    }  
    RB_Delete(root,s); 
}
//set current to the minimum
void Dictionary::begin(){
    if(num_pairs != 0){
	current = findMin(root); 
    }
    else{
        current = nil; 
    }
}
//set current to the maximum
void Dictionary::end(){
   if(num_pairs != 0){
	current = findMax(root);
	std::cout<<current->key; 
   }
   else{
       current = nil; 
    }
}
//set current to its successor
void Dictionary::next(){
   if(current != findMax(root)){
	current = findNext(current); 
   }
   else{
      current = nil;
   }
}
//sets current to its predecessor 
void Dictionary::prev(){
    if(current != findMin(root)){
	current = findPrev(current);   
        std::cout<<current->key; 
    }
    else{
        current = nil; 
    }
}
//inorder string 
std::string Dictionary::to_string() const{
    std::string s = ""; 
    inOrderString(s,root); 
    return s; 
}
//preorder string
std::string Dictionary::pre_string() const{
    std::string s = ""; 
    preOrderString(s,root); 
    return s; 
}
//see if two dictionaries are equal to each other
bool Dictionary::equals(const Dictionary& D) const{
    std::string s = ""; 
    std::string d = ""; 
    inOrderString(s,root); 
    D.inOrderString(d,D.root);
    std::string a = ""; 
    std::string b = ""; 
    preOrderString(a,root); 
    D.preOrderString(b,D.root);
    if(s == d && a == b){
	return true; 
    }
    return false; 
}
//operators 
std::ostream& operator<<( std::ostream& stream, Dictionary& D){
    return stream << D.Dictionary::to_string(); 
}
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.Dictionary::equals(B);
} 
Dictionary &Dictionary::operator=(const Dictionary& D){
    if(this != &D){
	Dictionary temp = D; 
    
    std::swap(root,temp.root); 
    std::swap(current,temp.current); 
    std::swap(num_pairs,temp.num_pairs);
    }
    return *this;  
}

