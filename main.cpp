#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;
string word;
//key value using as a hash table
std::map<std::string,int> hashTable;

class DataAnalytics{
public:


};

void countWord(string word){
    //use a try catch statment in case the value is not in the hash array meaning we will go out of bounds
    try {
        //checks to see if the value is in the hashtable
        //this is fast 0(1) to find out
        if (hashTable.at(word) > 0) {
            //checks to see if the value is already in the hash table, if it is add to the count
            std::cout << "Word already in the hash table";
            //get the current value of the item in the hash table
            int newValue =  hashTable.at(word) + 1;
            //set the new value for the hash table
            hashTable.insert (std::pair<std::string,int>(word,newValue));
        }
        //catch block used to catch the error if the input is valid
    } catch (...) {
        //fires because we went out of range in the hash table meaning that the value is a new word that can be added
        std::cout << "New Word Found" << "\n";
        //insert the invalid vins into the hash table, where the key is the word
        hashTable.insert(std::pair<std::string,int>(word,1));
    }

}

string clean(string word){

    transform(word.begin(), word.end(), word.begin(), ::tolower);
    for(int i = 0; i < word.size(); i++){
        if(word.size() != 0){
            if(!isalpha(word.at(i))){
                word.erase(i);
            }

        }

    }
    return word;
}
void ignoreWord(){
    string ignoreList[] = {"any", "in", "of", "or", "the", "to", "you", "with"};
    for(int i = 0; i < ignoreList->size(); i++){
        hashTable.insert(std::pair<std::string,int>(ignoreList[i],0));
    }

}

int main() {
    //ignoreWords();

    ifstream inFS;
    string readData;
    try{
        inFS.open("Bible");
        if(!inFS.is_open()){
            throw "File is empty";
        }
        while(!inFS.eof()){
            inFS >> readData;
            word = clean(readData);
            cout << word << endl;

        }
    }
    catch(const char* msg){
        cerr << msg << endl;
    }


}
