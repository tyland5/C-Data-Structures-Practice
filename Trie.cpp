#include <iostream>
#include <string> 
using namespace std;

class Node{
    public:
        char character; //default 0 cause in BSS or false
        bool complete; //specifies if node marks end of a word
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
};

int main(){
    Trie test;
    test.insert("test");
    cout << "result from exists function: " << test.exists("tes") << "\n";
    cout << "result from exists function: " << test.exists("test") << "\n";
    test.insert("tests");
    cout << "result from exists function: " << test.exists("tests") << "\n";
    return 0;
}