#include <iostream>
#include <string> 
using namespace std;

class Node{
    public:
        char character; //default 0 cause in BSS or false
        bool complete; //specifies if node marks end of a word
        int numOfChildren = 0;
        Node *parent;
        Node *children[26] = {nullptr}; 
        Node(char c){
            character = c;
        }
};

class Trie{
    public:
        Node *root = new Node('\0'); 
        
        void insert(string word){
            Node *currentNode = root;

            for(int i = 0; word[i] != '\0'; i++){
                Node **children = currentNode -> children;
                char letter = word[i];
                int index = letter - 'a';
                if(children[index] == nullptr){
                    children[index] = new Node(letter);
                    cout << "new letter: " << letter << " added\n";
                    currentNode -> numOfChildren +=1;
                    children[index]-> parent = currentNode;
                }
                currentNode = children[index];
            }
            currentNode -> complete = true;
        }

        Node *find(string word){
            Node *currentNode = root;

            for(int i =0; word[i] != '\0'; i++){
                Node **children = currentNode -> children;
                char letter = word[i];
                int index = letter - 'a';

                if(children[index] == nullptr){
                    cout << word + " was not found\n";
                    return nullptr;
                }
                currentNode = children[index];
            }
            
            return currentNode;
        }

        bool exists(string word){
            Node *node = find(word);
            if(node == nullptr || !(node -> complete)){
                return false;
            }
            return true;
        }

        void remove(string word){
            if(exists(word)){
                Node *currentNode = find(word);
                if(currentNode -> numOfChildren > 0){ //means other words contain this word
                    currentNode -> complete = false;
                }
                else{
                    Node *deletedNode = currentNode;
                    currentNode = currentNode->parent;
                    currentNode -> children[deletedNode -> character - 'a'] = nullptr;
                    currentNode -> numOfChildren -=1;
                    delete deletedNode;
                    
                    while(currentNode -> character != '\0'){
                        if(currentNode -> complete == true){ //we don't want to interfere with other words
                            break;
                        }
                        else{
                            deletedNode = currentNode;
                            currentNode = currentNode -> parent;
                            currentNode -> children[deletedNode -> character - 'a'] = nullptr;
                            currentNode -> numOfChildren -= 1;
                            delete deletedNode;
                        }
                    }
                }
            }
            else{cout<< word + " doesn't exist so it can't be removed\n";}
        }
};

int main(){
    Trie test;
    test.insert("test");
    cout << "result from exists function: " << test.exists("tes") << "\n";
    cout << "result from exists function: " << test.exists("test") << "\n";
    test.insert("tests");
    cout << "result from exists function: " << test.exists("tests") << "\n";

    /*
    test.remove("test");
    cout << "does test exist? " << test.exists("test") << "\n";
    cout << "does tests exist? " << test.exists("tests") << "\n";
    */

    test.remove("tests");
    cout << "does tests exist? " << test.exists("tests") << "\n";
    cout << "does test exist? " << test.exists("test") << "\n";
    cout << "num of children that t in test has: " << test.find("test")->numOfChildren <<"\n";
    return 0;
}