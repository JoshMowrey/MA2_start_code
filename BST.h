#ifndef __BST_H
#define __BST_H

#include <iostream>
#include <limits>
using namespace std;


/*
 *  Data structure for a single tree node
 */
template <typename T>
struct Node {
public:
	T value;
	Node* left;
	Node* right;

	Node(T val) {
		this->value = val;
		this->left = nullptr;
		this->right = nullptr;
	}

	~Node()
	{
		this->value = 0;
		this->left = nullptr;
		this->right = nullptr;
	}
};

/*
 * Binary Search Tree (BST) class implementation
 */
template <typename T>
class BST {

protected:
	Node<T>* _root;         // Root of the tree nodes

	/* Add new T val to the tree */
	void addHelper(Node<T>* root, T val) {
		if (root->value > val) {
			if (!root->left) {
				root->left = new Node<T>(val);
			}
			else {
				addHelper(root->left, val);
			}
		}
		else {
			if (!root->right) {
				root->right = new Node<T>(val);
			}
			else {
				addHelper(root->right, val);
			}
		}
	}

	/* Print tree out in inorder (A + B) */
	void printInOrderHelper(Node<T>* root) {
		if (!root) return;
		printInOrderHelper(root->left);
		cout << root->value << ' ';
		printInOrderHelper(root->right);
	}

	/* Return number of nodes in tree */
	int nodesCountHelper(Node<T>* root) {
		if (!root) {
			return 0;
		}
		else {
			return 1 + nodesCountHelper(root->left) + nodesCountHelper(root->right);
		}
	}

	/* Return height of tree (root == nullptr -> 0) */
	int heightHelper(Node<T>* root) {
		if (!root) {
			return -1;
		}
		else {
			return 1 + max(heightHelper(root->left), heightHelper(root->right));
		}
	}

	/* Delete a given <T> value from tree */
	bool deleteValueHelper(Node<T>* parent, Node<T>* current, T value) {
		if (!current) return false;
		if (current->value == value) {
			if (current->left == nullptr || current->right == nullptr) {
				Node<T>* temp = current->left;
				if (current->right) temp = current->right;
				if (parent) {
					if (parent->left == current) {
						parent->left = temp;
					}
					else {
						parent->right = temp;
					}
				}
				else {
					this->_root = temp;
				}
			}
			else {
				Node<T>* validSubs = current->right;
				while (validSubs->left) {
					validSubs = validSubs->left;
				}
				T temp = current->value;
				current->value = validSubs->value;
				validSubs->value = temp;
				return deleteValueHelper(current, current->right, temp);
			}
			delete current;
			return true;
		}
		return deleteValueHelper(current, current->left, value) ||
			deleteValueHelper(current, current->right, value);
	}
	//Used for recursively moving through the bst and freeing each node
	void destroy(Node<T>* Next){
		if (Next->right != nullptr){
			destroy(Next->right);
		}
		if (Next->left != nullptr){
			destroy(Next->left);
		}
		free(Next);
	}
	//used to recursively check for a value
	bool contains(T value, Node<T>* current_node){
		if (value > current_node->value){
			if (current_node->right != nullptr){
				return contains(value, current_node->right);
			}
		}
		else if (value < current_node->value){
			if (current_node->left != nullptr){
				return contains(value, current_node->left);
			}
		}
		else if (value == current_node->value){
			return true;
		}
		return false;
	}
	void printLevelOrderHelper(int level, Node<T>* current_node){
		if (level != -1){
			if (current_node->left != nullptr){
				cout << current_node->left->value << " ";
			}
			if (current_node->right != nullptr){
				cout << current_node->right->value << " ";
				
			}
		}		
		if (current_node->left != nullptr || current_node->right != nullptr){
			cout << "\n";
		}
		if (current_node->left != nullptr){
			printLevelOrderHelper(level-1, current_node->left);
		}
		if (current_node->right != nullptr){
			printLevelOrderHelper(level-1, current_node->right);
		}
	}
	int find_depth(){
		if (_root != nullptr){
			return find_depth(0,_root);
		}
		else return -1;
	}
	int find_depth(int current, Node<T>* currentNode){
		int rightdepth = 0, leftdepth = 0;
		if (currentNode->left != nullptr){
			leftdepth = find_depth(current+1, currentNode->left);
		}
		if (currentNode->right != nullptr){
			rightdepth = find_depth(current+1, currentNode->right);
		}
		if (rightdepth == 0 && leftdepth == 0){
			return current;
		}
		else if (rightdepth >= leftdepth){
			return rightdepth;
		}	
		else{
			return leftdepth;
		}
	}
	/********************************* PUBLIC API *****************************/
public:

	BST() : _root(nullptr) { }               // Basic initialization constructor

	/**
	 * Destructor - Needs to free *all* nodes in the tree
	 * TODO: Implement Destructor
	 */
	~BST() {
	    if (_root->right != nullptr){
			destroy(_root->right);
		}
		if (_root->left != nullptr){
			destroy(_root->left);
		}
		free(_root);
	}

	/* Public API */
	void add(T val) {
		if (this->_root) {
			this->addHelper(this->_root, val);
		}
		else {
			this->_root = new Node<T>(val);
		}
	}

	void print() {
		printInOrderHelper(this->_root);
	}

	/**
	 * Print the nodes level by level, starting from the root
	 * TODO: Implement printLevelOrder
	 */
	void printLevelOrder() {
		if (_root != nullptr){
			cout<<_root->value<<"\n"; 
			printLevelOrderHelper(find_depth(), _root);
		}
	}

	int nodesCount() {
		return nodesCountHelper(this->_root);
	}

	int height() {
		return heightHelper(this->_root);
	}

	/**
	 * Print the maximum path in this tree
	 * TODO: Implement printMaxPath
	 */
	void printMaxPath() {
		Node<T>* current = _root;
		for (int i = 0; i < find_depth()+1; i++){
			cout << current->value << " ";
			current = current->left;
		}
	}

	bool deleteValue(T value) {
		return this->deleteValueHelper(nullptr, this->_root, value);
	}

	/**
	 * Find if the BST contains the value
	 * TODO: Implement contains
	 */
	bool contains(T value) {
	    if (value > _root->value){
			return contains(value, _root->right);
		}
		else if (value < _root->value){
			return contains(value, _root->right);
		}
		else if (value == _root->value){
			return true;
		}
		else {
			return false;
		}
	}
};

#endif

