#include <iostream>
#include <string> 
using namespace std;


template <class T>
class AVLNode{
      template <class U> friend class AVLTree;
      protected:
        AVLNode *left = nullptr;
        AVLNode *right = nullptr;
        AVLNode *parent; //necessary for removal
        int height;
        T value;

        AVLNode(T val){
          value = val;
        }
      public:
        int getHeight(){return height;}
};

template <class T>
class AVLTree{
  private:
    AVLNode<T> *root = nullptr;
    int (*comparator)(T,T);

    string inorderTraversalHelper(AVLNode<T> *node){
      string output = "";
      if(node->left != nullptr){
        output += inorderTraversalHelper(node->left);// recursive call. 
      }
      
      output += to_string(node -> value) + " with height: " + to_string(node->height) + "\n";
      
      if(node->right != nullptr){
        output += inorderTraversalHelper(node->right);
      }

      if(node == root){ 
        output += "end\n\n";
      }
      return output;
    }

    
    AVLNode<T> *leftMostNode(AVLNode<T> *node){
      while(node->left != nullptr){
        node = node -> left;
      }
      return node;
    }
    
    AVLNode<T> *rightMostNode(AVLNode<T> *node){
      while(node->right != nullptr){
        node = node -> right;
      }
      return node;
    }

    //returns node if rotations are required, nullptr if not
    AVLNode<T> *adjustHeights(AVLNode<T> *node){
      while(node != nullptr){
        
        if(node->left != nullptr && node -> right != nullptr){
          if(abs(node->right->height - node->left->height) == 2){
              return node;
          }
          else if(node->right->height - node->left->height == 0){
              return nullptr;
          }
          else{ //there's a difference of 1
              node -> height += 1;
              node = node -> parent;
          }
        }

        else if(node -> left != nullptr){
          if(node -> left -> height == 2){
              return node;
          }
          else{
              node -> height += 1;
              node = node -> parent;
          }
        }

        else{
          if(node -> right -> height == 2){
              return node;
          }
          else{
              node -> height += 1;
              node = node -> parent;
          }
        }
      }
      return node; //will be nullptr
    }

    AVLNode<T> *postRemovalAdjust(AVLNode<T> *node){
      while(node != nullptr){
        if(node->left != nullptr && node -> right != nullptr){
          if(abs(node->right->height - node->left->height) == 2){
              node->height -= 1; //necessary based on how I implemented code
              return node;
          }
          else if(node->right->height - node->left->height == 0){
              node->height -= 1;
              node = node->parent;
          }
          else{ //there's a difference of 1
              //no height change should be necessary since removing and have other height that's greater
              node = node -> parent;
          }
        }

        else if(node -> left != nullptr){
          if(node -> left -> height == 2){
              node->height -=1;
              return node;
          }
          else{
              node -> height -= 1;
              node = node -> parent;
          }
        }

        else{
          if(node -> right -> height == 2){
              node->height -=1;
              return node;
          }
          else{
              node -> height -= 1;
              node = node -> parent;
          }
        }
      }
      return node; //nullptr
    }

    //the node passed will be the node the rotation is based off of
    // "single rotation"
    void rotateLeft(AVLNode<T> *node){
      if(node-> right -> left != nullptr){ //mostly for complicated single and double rotations
        AVLNode<T> *grandchild = node -> right -> left;
        AVLNode<T> *childNode = node -> right;

        
        if(childNode->right != nullptr && childNode->right->height > grandchild->height){
          if(node->parent == nullptr){
            root = childNode;
          }
          else if(comparator(node->value, node->parent->value) <= 0){
            node -> parent -> left = childNode;
          }
          else{
            node -> parent -> right = childNode;
          }

          node->right = childNode->left;
          if(childNode->left != nullptr){childNode->left->parent = node;}

          childNode->parent = node->parent;
          node->parent = childNode;
          childNode->left = node;

          node->height -=1;
        }
        else{
          if(node->parent == nullptr){
            root = grandchild;
          }
          else if(comparator(node->value, node->parent->value) <= 0){
            node -> parent -> left = grandchild;
          }
          else{
            node -> parent -> right = grandchild;
          }
          
          childNode->left = grandchild->right;
          if(grandchild->right != nullptr) {grandchild->right->parent = childNode;}
          grandchild->parent = node->parent;

          node->right = grandchild->left;
          if(grandchild->left != nullptr){grandchild->left->parent = node;}
          grandchild->left = node;
          grandchild->right = childNode;
          node-> parent = grandchild;

          node->height -= 1;
          grandchild -> height += 1;
          childNode -> height -=1;
        }
      }

      else{ //height discrepancy off of two straight right children
        AVLNode<T> *childNode = node->right;
        
        //update the parent node
        if(node -> parent == nullptr){
          root = childNode;
        }
        else if(comparator(node->value, node->parent->value) <= 0){
          node -> parent -> left = childNode;
        }
        else{
          node -> parent -> right = childNode;
        }

        childNode -> parent = node ->parent;

        //update the child node's left parameter, node's child parameter, and the node's parent parameter
        childNode -> left = node;
        node -> parent = childNode;
        node -> right = nullptr;
        
        //update the heights. 
        node -> height -= 1;
      }
    }

    void rotateRight(AVLNode<T> *node){
      if(node-> left -> right != nullptr){
        AVLNode<T> *grandchild = node -> left -> right;
        AVLNode<T> *childNode = node->left;

        if(childNode->left != nullptr && childNode->left->height > grandchild->height){
          if(node->parent == nullptr){
            root = childNode;
          }
          else if(comparator(node->value, node->parent->value) <= 0){
            node -> parent -> left = childNode;
          }
          else{
            node -> parent -> right = childNode;
          }

          node->left = childNode->right;
          if(childNode->right != nullptr){childNode->right->parent = node;}

          childNode->parent = node->parent;
          node->parent = childNode;
          childNode->right = node;

          node->height -=1;
        }
        else{
          if(node->parent == nullptr){
            root = grandchild;
          }
          else if(comparator(node->value, node->parent->value) <= 0){
            node -> parent -> left = grandchild;
          }
          else{
            node -> parent -> right = grandchild;
          }
          
          grandchild -> parent = node -> parent;

          childNode-> right = grandchild->left;
          if(grandchild->left != nullptr) {grandchild->left->parent = childNode;}
          node -> left = grandchild->right;
          if(grandchild->right != nullptr){grandchild->right->parent = node;}

          grandchild -> right = node;
          grandchild -> left = childNode;

          node -> parent = grandchild;
          childNode -> parent = grandchild;

          //adjust the height
          grandchild -> height += 1;
          grandchild -> right -> height -= 1;
          grandchild -> left -> height -= 1;
        }
      }

      else{ //height discrepancy off of two straight left children
        AVLNode<T> *childNode = node->left;
        
        //update the parent node
        if(node -> parent == nullptr){
          root = childNode;
        }
        else if(comparator(node->value, node->parent->value) <= 0){
          node -> parent -> left = childNode;
        }
        else{
          node -> parent -> right = childNode;
        }

        childNode -> parent = node ->parent;

        //update the child node's right parameter, the node's parent parameter, and node's child parameter, 
        childNode -> right = node;
        node -> parent = childNode;
        node -> left = nullptr;
        
        //update the heights. 
        node -> height -= 1;
      }
    }

    
    void rotate(AVLNode<T> *node){
      if(node -> left == nullptr){
        rotateLeft(node);
      }
      else if(node -> right == nullptr){
        rotateRight(node);
      }
      else if(node->left->height > node->right->height){
        rotateRight(node);
      } 
      else{
        rotateLeft(node);
      }
    }
    

  public:
    
    AVLTree(int(*function)(T,T)){
      comparator = function;
    }

    void insert(T value){
      AVLNode<T> *node = new AVLNode(value); //VERY IMPORTANT. need to dynamically allocate, otherwise errors because stack
      
      if(root == nullptr){
        root = node;
        root->parent = nullptr; 
        root -> height = 1;
        return;
      }

      AVLNode<T> *currentNode = root;
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

      node -> height = 1;
      AVLNode <T> *rotateNode = adjustHeights(node->parent);

      if(rotateNode != nullptr){
        cout << "Executing rotation after inserting " << to_string(value) << "\n";
        rotate(rotateNode);
      }
      //have to then adust parents' height
      //height = height < counter ? counter : height;
    }


    AVLNode<T> *find(T value){
      AVLNode<T> *currentNode = root;

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

    void remove(T val){
      AVLNode<T> *node = find(val);
      if(node != nullptr){
        AVLNode<T> *replacement = node;
        
        if(node->left != nullptr){
          replacement = rightMostNode(node->left);
          AVLNode<T> *adjustStartNode = replacement -> parent;

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
          
          replacement -> height = node->height;
          AVLNode<T> *rotateNode = postRemovalAdjust(node->parent);
          
          //needed to update rest of heights
          while(rotateNode != nullptr){
            cout << "Executing rotation after removing " << to_string(val) <<"\n";
            rotate(rotateNode);
            rotateNode = postRemovalAdjust(rotateNode->parent->parent); 
          }
          
          delete node;
        }

        //done
        else if(node->right != nullptr){
          replacement = leftMostNode(node -> right);
          AVLNode<T> *adjustStartNode = replacement -> parent;

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

          replacement -> height = node->height;
          AVLNode<T> *rotateNode = postRemovalAdjust(node->parent);

          //needed to update rest of heights
          while(rotateNode != nullptr){
            cout << "Executing rotation after removing " << to_string(val) <<"\n";
            rotate(rotateNode);
            rotateNode = postRemovalAdjust(rotateNode->parent->parent); 
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

          AVLNode<T> *rotateNode = postRemovalAdjust(node->parent);
          
          //needed to update rest of heights
          while(rotateNode != nullptr){
            cout << "Executing rotation after removing " << to_string(val) <<"\n";
            rotate(rotateNode);
            rotateNode = postRemovalAdjust(rotateNode->parent->parent); 
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

//test 1
//simple one left rotation
void leftRotTest(){
  AVLTree<int> tree(compareInt);
  tree.insert(5);

  cout << "Height of 5 is: " << tree.find(5)->getHeight()<<"\n";

  tree.insert(6);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 6 is: " << tree.find(6)->getHeight()<<"\n";

  tree.insert(7);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 6 is now: " << tree.find(6)->getHeight()<<"\n";
  cout << "Height of 7 is: " << tree.find(7)->getHeight()<<"\n";
  cout << tree.inOrderTraversalString() << "\n";
}

//test 2
//simple one left rotation
void leftRotTest2(){
  AVLTree<int> tree(compareInt);
  tree.insert(5);

  cout << "Height of 5 is: " << tree.find(5)->getHeight()<<"\n";

  tree.insert(7);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 7 is: " << tree.find(7)->getHeight()<<"\n";

  tree.insert(6);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 7 is now: " << tree.find(7)->getHeight()<<"\n";
  cout << "Height of 6 is: " << tree.find(6)->getHeight()<<"\n";
  cout << tree.inOrderTraversalString() << "\n";
}

//test3
//simple one right rotation
void rightRotTest(){
  AVLTree<int> tree(compareInt);
  tree.insert(5);

  cout << "Height of 5 is: " << tree.find(5)->getHeight()<<"\n";

  tree.insert(4);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 4 is: " << tree.find(4)->getHeight()<<"\n";

  tree.insert(3);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 4 is now: " << tree.find(4)->getHeight()<<"\n";
  cout << "Height of 3 is: " << tree.find(3)->getHeight()<<"\n";
  cout << tree.inOrderTraversalString() << "\n";
}

//test4
//simple one right rotation
void rightRotTest2(){
  AVLTree<int> tree(compareInt);
  tree.insert(5);

  cout << "Height of 5 is: " << tree.find(5)->getHeight()<<"\n";

  tree.insert(3);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 3 is: " << tree.find(3)->getHeight()<<"\n";

  tree.insert(4);
  cout << "Height of 5 is now: " << tree.find(5)->getHeight()<<"\n";
  cout << "Height of 3 is now: " << tree.find(3)->getHeight()<<"\n";
  cout << "Height of 4 is: " << tree.find(4)->getHeight()<<"\n";
  cout << tree.inOrderTraversalString() << "\n";
}

//test5
//complicated left rotation
void leftRotTest3(){
  AVLTree<int> tree(compareInt);
  tree.insert(5);
  tree.insert(2);
  tree.insert(12);
  tree.insert(9);
  tree.insert(14);
  tree.insert(4);
  tree.insert(8);
  tree.insert(13);
  tree.insert(11);
  tree.insert(10);

  cout << tree.inOrderTraversalString() << "\n";
}

//test6
//another complicated left rotation
void leftRotTest4(){
  AVLTree<int> tree(compareInt);
  tree.insert(1);
  tree.insert(0);
  tree.insert(3);
  tree.insert(2);
  tree.insert(4);
  tree.insert(5);

  cout << tree.inOrderTraversalString() << "\n";
}


//test7
//complicated right rotation
void rightRotTest3(){
  AVLTree<int> tree(compareInt);
  tree.insert(20);
  tree.insert(10);
  tree.insert(22);
  tree.insert(25);
  tree.insert(5);
  tree.insert(15);
  tree.insert(13);
  tree.insert(17);
  tree.insert(3);
  tree.insert(18);

  cout << tree.inOrderTraversalString() << "\n";
}

//test7
//another complicated right rotation
void rightRotTest4(){
  AVLTree<int> tree(compareInt);
  tree.insert(20);
  tree.insert(22);
  tree.insert(10);
  tree.insert(15);
  tree.insert(5);
  tree.insert(3);

  cout << tree.inOrderTraversalString() << "\n";
}

void removalTest(){
  AVLTree<int> tree(compareInt);
  tree.insert(10);
  tree.insert(5);
  tree.insert(15);
  tree.insert(3);
  tree.insert(13);
  tree.insert(17);
  tree.insert(18);
  tree.remove(13);

  cout << tree.inOrderTraversalString() << "\n";
}

void removalTest2(){
  AVLTree<int> tree(compareInt);
  tree.insert(10);
  tree.insert(3);
  tree.insert(22);
  tree.insert(1);
  tree.remove(22);

  cout << tree.inOrderTraversalString() << "\n";
}

int main(){
  //leftRotTest();
  //leftRotTest2();
  //leftRotTest3();
  //leftRotTest4();

  //rightRotTest();
  //rightRotTest2();
  //rightRotTest3();
  //rightRotTest4();
  removalTest();
  removalTest2();
}
  