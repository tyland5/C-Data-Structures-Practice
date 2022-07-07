#include <iostream>
#include <string> 
using namespace std;


//if i want to accurately track depth, each node would have to include a depth. would help with implementing an AVL tree
//kind of overkill for a binary search tree though
template <class T>
class BSTNode{
      template <class U> friend class BinarySearchTree;
      protected:
        BSTNode *left = nullptr;
        BSTNode *right = nullptr;
        BSTNode *parent; //necessary for removal
        T value;

        BSTNode(T val){
          value = val;
        }
};

template <class T>
class BinarySearchTree{
  private:
    BSTNode<T> *root = nullptr;
    int (*comparator)(T,T);

    string inorderTraversalHelper(BSTNode<T> *node){
      string output = "";
      if(node->left != nullptr){
        output += inorderTraversalHelper(node->left);// recursive call. 
      }
      
      output += to_string(node -> value) + " - ";
      
      if(node->right != nullptr){
        output += inorderTraversalHelper(node->right);
      }

      if(node == root){ 
        output += "end";
      }
      return output;
    }

    BSTNode<T> *find(T value){
      BSTNode<T> *currentNode = root;

      while(currentNode != nullptr){
        if(comparator(value, currentNode -> value) > 0){
          currentNode = currentNode -> right;
        }
        else if(comparator(value, currentNode -> value) < 0){
          currentNode = currentNode -> left;
        }
        else{
          return currentNode;
        }
      }
      return nullptr;
    }

    
    BSTNode<T> *leftMostNode(BSTNode<T> *node){
      while(node->left != nullptr){
        node = node -> left;
      }
      return node;
    }
    
    BSTNode<T> *rightMostNode(BSTNode<T> *node){
      while(node->right != nullptr){
        node = node -> right;
      }
      return node;
    }
  public:
    
    BinarySearchTree(int(*function)(T,T)){
      comparator = function;
    }

    void insert(T value){
      BSTNode<T> *node = new BSTNode(value); //VERY IMPORTANT. need to dynamically allocate, otherwise errors because stack
      
      if(root == nullptr){
        root = node;
        root->parent = nullptr; 
        return;
      }

      BSTNode<T> *currentNode = root;
      int counter = 1;

      while(true){
        counter += 1;
        if(comparator(value, currentNode -> value) > 0){
          if(currentNode -> right == nullptr){
            currentNode -> right = node;
            node -> parent = currentNode;
            break;
          }
          currentNode = currentNode -> right;
        }

        else{ //if <=
          if(currentNode -> left == nullptr){
            currentNode -> left = node;
            node -> parent = currentNode;
            break;
          }
          currentNode = currentNode -> left;
        }
      }

      //depth = depth < counter ? counter : depth;
    }


    void remove(T val){
      BSTNode<T> *node = find(val);
      if(node != nullptr){
        BSTNode<T> *replacement = node;
        
        if(node->left != nullptr){
          replacement = rightMostNode(node->left);
          cout << "replacement node is: " << replacement -> value << "\n";
          //update replacement's child node, replacement's parent node, and then itself
          if(replacement != node->left){
            if(replacement->left != nullptr){
              replacement -> left-> parent = replacement -> parent;
              replacement -> parent -> right = replacement -> left;
            }
            else{
              node -> left -> right = nullptr;
            }
            replacement -> left = node -> left;
            replacement -> left -> parent = replacement;
          }
          replacement -> parent = node -> parent;
          replacement->right = node ->right;
          
          //need to check if it's root node
          if(root == node){
            root = replacement;
            delete node;
            return;
          }

          //update parent node
          if(comparator(replacement -> value, node -> parent -> value) <= 0){
            node ->parent -> left = replacement;
          }
          else{
             node ->parent -> right = replacement;
          }

          delete node;
        }

        //done
        else if(node->right != nullptr){
          replacement = leftMostNode(node -> right);
          
          cout << "replacement node is: " << replacement -> value << "\n";
          //update replacement's child node and then itself
          if(replacement != node->right){
            if(replacement->right != nullptr){
              replacement-> right -> parent = replacement -> parent;
              replacement -> parent -> left = replacement -> right;
            }
            else{
              node -> right -> left = nullptr;
            }
            replacement -> right = node -> right;
            replacement -> right -> parent = replacement;
          }

          replacement -> parent = node -> parent;

          //need to check if it's root node
          if(root == node){
            root = replacement;
            delete node;
            return;
          }

          //update parent node
          if(comparator(replacement -> value, node -> parent -> value) <= 0){
            node ->parent -> left = replacement;
          }
          else{
             node ->parent -> right = replacement;
          }

          delete node; //free space
        }

        else{
          //need to check if it's root node
          if(root == node){
            root = nullptr;
            delete node;
            return;
          }

          if(comparator(replacement -> value, node -> parent -> value) <= 0){
            node ->parent -> left = nullptr;
          }
          else{
             node ->parent -> right = nullptr;
          }

          delete node;
        }
      }
      else{
        cout << "This node could not be removed as it does not exist\n";
      }
    }

    
    string inOrderTraversalString(){
      if(root == nullptr){
        return "Empty Tree\n";
      }
      return inorderTraversalHelper(root);
    }
};


int compareInt(int x, int y){
  return x - y;
}

//simple
void leftRemoval(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(12);
  bst.insert(6);
  bst.insert(8);
  bst.insert(9);
  bst.insert(10);
  bst.insert(11);
  bst.remove(6);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

//harder
void leftRemoval2(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(12);
  bst.insert(6);
  bst.insert(8);
  bst.insert(9);
  bst.insert(10);
  bst.insert(11);
  bst.insert(7);
  bst.remove(6);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

void leftRemoval3(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(13);
  bst.insert(7);
  bst.insert(9);
  bst.insert(10);
  bst.insert(11);
  bst.insert(12);
  bst.insert(8);
  bst.insert(6);
  bst.remove(7);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}


void rightRemoval1(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(6);
  bst.insert(8);
  bst.insert(10);
  bst.remove(8);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

void rightRemoval2(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(6);
  bst.insert(8);
  bst.insert(15);
  bst.insert(10);
  bst.remove(8);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

void rightRemoval3(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(6);
  bst.insert(8);
  bst.insert(15);
  bst.insert(10);
  bst.insert(12);
  bst.remove(8);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

void rootRemoval(){
  BinarySearchTree<int> bst(compareInt);
  bst.insert(6);
  bst.insert(5);
  bst.insert(7);
  bst.remove(6);
  string tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
  
  bst.remove(5);
  tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";

  bst.remove(7);
  tree = bst.inOrderTraversalString();
  cout <<  tree + "\n";
}

//g++ -g BST.cpp -o BST. make sure to include -g flag to utilize gdb properly
//testing
int main() 
{
  leftRemoval();
  leftRemoval2();
  leftRemoval3();
  rightRemoval1();
  rightRemoval2();
  rightRemoval3();
  rootRemoval();
  return 0;
}

