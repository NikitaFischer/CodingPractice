/**********************************************************
 * AVL Tree Implementation file
 *
 *  ** riffed and modified heavily by Aaron Crandall - 2016
 *  ** All implementation in header because of templating
 *
 **********************************************************/

#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <queue>
#include <iostream>

using namespace std;

template<class T>
class AVLNode {
public:
    // Default blank AVLNode constructor
    AVLNode() {
        left = right = nullptr;
        height = 0;
    }

    // Constructor with provided element (data) and children
    AVLNode(const T& el, AVLNode *l = nullptr, AVLNode *r = nullptr) {
        key = el;
        left = l;
        right = r;
        height = 0;
    }

    T key;                  // Key to compare/insert on of type <T>
    AVLNode *left, *right;  // Children of this node
    int height;             // Height of this node in the tree
};

/*
 * Main AVLTree class that stores AVLNode objects
 *  -- REQUIRES: implementation of rotation functions
 *  -- REQUIRES: implementation of node heights
 *  -- REQUIRES: implementation of contains function
 *  -- REQUIRES: implementation of remove function
 */
template<class T>
class AVLTree {
  public:
    AVLTree() { root = nullptr; }
  
    void insert(const T& key) { insert(root, key); }
    void printTree() { printTree(root); }

    bool contains( const T& key ){ return(contains(root, key)); }
    void remove( const T& key ){ remove(root, key); }
  
  private:
    AVLNode<T>* root;

    AVLNode<T>* findMin(AVLNode<T>*& node)
    {
	AVLNode<T>* temp = node;

	while (temp->left != NULL)
	{
		temp = temp->left;
	}

	return temp;
    }

    int height(AVLNode<T>*node)const //returning height of node or -1 if nullptr
    {
	return node == nullptr ? -1 : node->height;
    }

    void rotateWithLeftChild(AVLNode<T>* &node)
    {
	AVLNode<T>*k1 = node->left;
	node->left = k1->right;
	k1->right = node;
	node->height = max(height(node->left), height(node->right)) + 1;
	k1->height = max(height(k1->left), node->height)+1;
	node = k1;
    }

    void rotateWithRightChild(AVLNode<T>* &node)
    {
	AVLNode<T>*k1 = node->right; //k1 is the subset node
	node->right = k1->left;
	k1->left = node;
	node->height = max(height(node->right), height(node->left)) + 1;
	k1->height = max(height(k1->right), node->height)+1;
	node = k1;
    }

    void doubleWithLeftChild(AVLNode<T>* &newnode)
    {
	rotateWithRightChild(newnode->left);
	rotateWithLeftChild(newnode);
    }

    void doubleWithRightChild(AVLNode<T>* &newnode)
    {
	rotateWithLeftChild(newnode->right);
	rotateWithRightChild(newnode);
    }

    /*
     *  Insert function needs updating!
     *  This function needs to update heights as it returns from recursive insert.
     *  If the heights of a node are more than 2 different, rotate to fix
     *  After fixing, repair the heights of all nodes in the rotated tree
     */
    void insert(AVLNode<T>* &node, const T& key) 
   {
        if(node == nullptr) 
	{
          node = new AVLNode<T>(key, nullptr, nullptr);
      	} 
	else if(key < node->key) 
	{
          insert(node->right, key);
      	} 
	else if (node->key < key)
	{
          insert(node->left, key);
      	}
	
	balance(node);
    }

	static const int ALLOWED_IMBALANCE = 1;

	void balance(AVLNode<T>* &node)
	{
		if(node == nullptr)
		 return;

		if(height(node->left) - height(node->right) > ALLOWED_IMBALANCE)
			if(height(node->left->left) >= height(node->left->right))
			  rotateWithLeftChild(node);
			else
			   doubleWithLeftChild(node);
		else
		{
			if(height(node->right) - height(node->left) > ALLOWED_IMBALANCE)
			   {
				if(height(node->right->right) >= height(node->right->left))
				  {
				     rotateWithRightChild(node);
				  }
				else
				  {
				     doubleWithRightChild(node);
				  }
			    }
	        }
		node->height = max (height(node->left), height(node->right)) + 1;
	}
				

    // MUST IMPLEMENT CONTAINS
    bool contains(AVLNode<T>* root, const T& key) 
       {
	 AVLNode<T>*ptemp = root; //temporary node that stores root
	 while(1) //1 is true
	  {
		if(!ptemp) //ptemp == NULL
		{
			return false;
		}
		else if(ptemp->key > key)
		{
			ptemp = ptemp->left;
		}
		else if(ptemp->key < key)
		{
			ptemp = ptemp->right;
		}
		else if(ptemp->key == key)
		{
			return true;
		}
	  }
      //cout << "CONTAINS STILL UNIMPLEMENTED" << endl;
      //return(false);
    }

    // MUST IMPLEMENT REMOVE
    void remove( AVLNode<T>* node, const T& key) {
	
	if (node == nullptr)
	{
		return;
	}

	if (key < node->key)
	{
		remove(node->left, key);
	}

	else if(node->key < key)
	{
		remove(node->right, key);
	}

	else
	{
		if ((node->left == NULL) && (node->right == NULL))
		{
			delete node;
			node = NULL;
			//Case 2: one child
		}
		else if (node->left == NULL)
		{
			AVLNode<T>* temp = node;
			node = node->right;
			delete temp;
		}
		else if (node->right == NULL)
		{
			AVLNode<T>* temp = node;
			node = node->left;
			delete temp;
		}
		else
		{
			AVLNode<T>* temp = findMin(node->right);
			node->key = temp->key;
			remove(node->right, temp->key);
		}
        }

	balance(node);
  }

    // Should do a level order printout with actual depth (no alignment)
    void printTree(AVLNode<T>* node) {
      queue<AVLNode<T>*> bufQueue;
      int curr_height = node->height;
      bufQueue.push(node);
      while( !bufQueue.empty() ) {
        AVLNode<T>* curr = bufQueue.front();
        if( curr->left  != nullptr ){ bufQueue.push(curr->left ); }
        if( curr->right != nullptr ){ bufQueue.push(curr->right); }
        if( curr->height < curr_height ){
          //cout << endl; 
          curr_height = curr->height;
        }
        cout << curr->key << " ";
        bufQueue.pop();
      }
      cout << endl;
    }
  // end private
};

#endif
