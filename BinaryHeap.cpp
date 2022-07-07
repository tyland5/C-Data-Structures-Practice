#include <iostream>
#include <string> 
#include <vector>
#include <exception>
using namespace std;

//VERY IMPORTANT NOTE FOR SELF, runtime doesn't detect vector properly for me, so need to include this flag on compilation. -static-libstdc++
//https://stackoverflow.com/questions/70994977/c-code-wont-run-if-a-vector-contains-value


template<class T>
class BinaryHeap{
    private:
        //reserves more space when there is more than specifically 32 bytes in the vector. For instance, 33 bytes would make the total size 64 bytes. 65 -> 128. 
        //However, 1 or 2 bytes -> 1 or 2 bytes, not 16 or 32. I have tested this

        vector<T> vect{NULL}; //first index a dummy to help with functionality and swaps.
        int (*comparator)(T, T);

        void fixDown(int size, int index){
            while(size >= index * 2 + 1){
               if(comparator(vect[index], vect[index * 2]) < 0){
                    if(comparator(vect[index * 2], vect[index * 2 + 1]) >= 0){
                        vect[0] = vect[index * 2];
                        vect[index * 2] = vect[index];
                        vect[index] = vect[0];
                        index *= 2;
                    }
                    else{
                        vect[0] = vect[index * 2 + 1];
                        vect[index * 2 + 1] = vect[index];
                        vect[index] = vect[0];
                        index = index * 2 + 1;
                    }
               }
               else if(comparator(vect[index], vect[index * 2 + 1]) < 0){
                    vect[0] = vect[index * 2 + 1];
                    vect[index * 2 + 1] = vect[index];
                    vect[index] = vect[0];
                    index = index * 2 + 1;
               }
            }

            if(size == index * 2 && comparator(vect[index], vect[index * 2]) < 0){
                vect[0] = vect[index * 2];
                vect[index * 2] = vect[index];
                vect[index] = vect[0];
            }
        }

        void fixUp(int index, T object){
            while(index > 1){
                int parentIndex = index >> 1; //shifting right faster than division

                if(comparator(object, vect[parentIndex]) > 0){ 
                    vect[0] = vect[parentIndex];
                    vect[parentIndex] = vect[index];
                    vect[index] = vect[0];
                    index = parentIndex;
                }
                else{
                    break;
                }
            }
        }

        int find(T value){
            int size = getSize();

            for(int i = 1; i < size; i ++){
                if(comparator(vect[i], value) == 0){
                    return i;
                }
            }
            
            return 0;
        }

    public:

        BinaryHeap(int(*func)(T, T)){
            comparator = func;
        }

        void insert(T object){
            vect.push_back(object);
            int index = vect.size() -1;

            fixUp(index, object);
        }

        T pop(){
            if(getSize() == 1){
                T obj = vect[1];
                vect.pop_back();
                return obj;
            }

            if(getSize() == 0){
                cout << "The Binary Heap is currently empty. Nothing to pop. NULL will be returned\n"; 
                //throw exception(); 
                return NULL;//doing this way might make application more flexible instead of returning -1 in main
            }

            T returnedObj = vect[1];
            vect[1] = vect.back();
            vect.pop_back();
            int index = 1;
            int size = getSize();//already has -1 attached to it so

            fixDown(size, index);

            return returnedObj;
        }

        //so you can use delete in main if dynamically allocated
        T remove(T object){
            int index = find(object);
            if(index == 0){
                cout << "This object does not exist and could not be removed. Null will be returned\n";
                //throw exception();
                return NULL;
            }

            if(index == getSize()){
                T replacedObj = vect[index];
                vect.pop_back();

                return replacedObj;
            }
            else{
                T replacedObj = vect[index];
                vect[index] = vect.back();
                vect.pop_back();

                if(comparator(vect[index], replacedObj) > 0){
                    fixUp(index, vect[index]);
                }
                else{
                    fixDown(getSize(), index);
                }

                return replacedObj;
            }
        }

        bool exists(T value){
            return find(value);
        }

        //for testing 
        string toString(){
            int size = vect.size();
            string output = "";
            for(int i = 1; i < size ; i ++){
                output += to_string(vect[i]) + " - ";
            }
            output += "end\n";
            return output;
        }

        int getSize(){
            return vect.size() - 1; //disregard index 0
        } 
};


int compareInt(int x, int y){
    return x - y;
}

void test1(){
    int (*comparator)(int x, int y) = compareInt;
    

    BinaryHeap<int> bh(comparator);


    bh.pop();
    cout << "size is starting off at: " << bh.getSize() << "\n";

    int x = 5;
    int y = 12;
    int z  = 24;

    bh.insert(x);
    bh.insert(y);
    bh.insert(z);
    bh.insert(1);
    bh.insert(3);

    cout << bh.toString() << "\n";

    cout << "size before is: " << bh.getSize() << "\n";
    cout << to_string(bh.pop()) << " was popped. Size is now " << bh.getSize() << "\n";
    cout << bh.toString() << "\n";

    bh.insert(48);
    cout << bh.toString() << "\n";

    cout << "Does 3 exist? " << bh.exists(3) << "\n";
    cout << "Does 24 exist? " << bh.exists(24) << "\n";
}

void test2(){
    BinaryHeap<int> bh(compareInt);

    for(int i = 1; i < 20; i ++){
        bh.insert(i);
    }

    cout << bh.toString() << "\n";

    bh.remove(8); //tests fix down

    cout << bh.toString() << "\n";
    
    bh.remove(18); //tests fix up

    cout << bh.toString() << "\n";
    
    bh.remove(20);
}

int main(){
    test2();
    return 0;
}