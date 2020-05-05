#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <map>
#include <string.h>
using namespace std;

bool sortByVal(const pair<string, int> &a,
               const pair<string, int> &b)
{
    return (a.second > b.second);
}

class DataAnalytics{

private:
    //key value using as a hash table
    map<string,int> hashTable;
    //total num of chars in the file
    int totalChars = 0;
    //total number of matches in the file
    int totalMatches = 0;


public:

    //constructor
    DataAnalytics(){};



    //function used to to compare two files
    void compareTwoFiles(string string1, string string2){
        //biggest file
        long fileSize = 0;
        //get the string that is bigger to use for the loop
        if(string1.size() > string2.size()){
            //set the file size to file2 since its less
            fileSize = string2.size();
        } else {
            //set the file size to the first file since its bigge
            fileSize = string1.size();
        }
        //compare word for word in the files
        for(std::string::size_type i = 0; i < fileSize; ++i) {
            //loops thru all the values in the string
            //checks to see if the words are the same
            if (string1[i] == string2[i]){
                //if the chars in the string are the same
                totalMatches = totalMatches + 1;
            }
            //add to the total number of chars
            totalChars = totalChars + 1;
        }
    }


    void countWord(string word){
        //use a try catch statment in case the value is not in the hash array meaning we will go out of bounds
        try {
            //checks to see if the value is in the hashtable
            //this is fast 0(1) to find out
            if (hashTable.at(word) > 0) {
                //checks to see if the value is already in the hash table, if it is add to the count
                //cout << "Word already in the hash table \n";
                //cout << "Word: " << word << " Count: " << hashTable.at(word) << "\n";
                //get the current value of the item in the hash table
                int newValue =  hashTable.at(word) + 1;
                //set the new value for the hash table
                hashTable[word]++;
            }
        } catch (...) {
            //fires because we went out of range in the hash table meaning that the value is a new word that can be added
            //std::cout << "New Word Found" << "\n";
            //insert into the hash table, where the key is the word
            hashTable.insert(std::pair<std::string,int>(word,1));
        }
    }


//function used to let the user search for a word and see how many times it appeared in the document
    void searchOccurrences(string word){
        //find the value the user is looking for in the hash table
        try {
            //checks to see if the value is in the hashtable
            //this is fast 0(1) to find out
            if (hashTable.at(word) > 0) {
                cout << word << " was in the file a total of : " << hashTable.at(word) << " times \n";
            }
            //catch block used to catch the error if the input is valid
        } catch (...) {
            //fires because we went out of range in the hash table meaning that the value is a new word that can be added
            std::cout << "The word you searched wasn't in the file" << "\n";
        }
    }
    //function used to display the amount of similarity between the two files after we have finished processing
    void getSimilarity(){
        //prints the simularity between the two files
        cout << "Similarity: " << ((totalMatches/totalChars) * 100) << "%" << "\n";
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
        vector<string> ignoreList = {"a", "and", "any", "for", "he", "his", "i", "in", "of", "or", "that", "the", "they", "to", "you", "with"};
        for(int i = 0; i < ignoreList.size(); i++){
            //cout << ignoreList[i] << endl;
            hashTable.insert(pair<string,int>(ignoreList[i],0));
        }

    }


    //function used to analyze the file for similarity
    void analyzeSimilarity(string file1,string file2){
        string file1Word;
        string file2Word;
        ifstream inFS1;
        ifstream inFS2;
        string readData1;
        string readData2;
        try{
            //load up the first file
            inFS1.open(file1);
            if(!inFS1.is_open()){
                throw "File is empty";
            }
            //load up the second file
            inFS2.open(file2);
            if(!inFS2.is_open()){
                throw "File is empty";
            }
            //call the function to make sure we don't process generic words
            ignoreWord();
            cout << "Processing similarity...Please Wait..\n";
            while(!inFS1.eof() && !inFS2.eof()){
                inFS1 >> readData1;
                inFS2 >> readData2;
                file1Word = clean(readData1);
                file2Word = clean(readData2);
                //perform analysis of the similarity here
                compareTwoFiles(file1Word,file2Word);
            }
            //when we have finished processing
            cout << "Finished Processing!\n";
        }
        catch(const char* msg) {
            cerr << msg << endl;
        }
    }



    //driver function that will analyze the file for word count
    void analyzeCount(){
        //performs all operations on the input file so we can access the data from it later
        string word;
        ifstream inFS;
        string readData;
        try{
            inFS.open("Bible");
            if(!inFS.is_open()){
                throw "File is empty";
            }
            //call the function to make sure we don't process generic words
            ignoreWord();
            cout << "Processing...Please Wait..\n";
            while(!inFS.eof()){
                inFS >> readData;
                //if no alphabet letters in string, string will return empty
                word = clean(readData);
                //cout << word << endl;
                //perform the word ops here //ignores empty strings
                if(word != ""){
                    countWord(word);
                }
            }
            //when we have finished processing
            cout << "Finished Processing!\n";
        }
        catch(const char* msg) {
            cerr << msg << endl;
        }
    }

    void topWords(int topwords){
        //will eventually store all words in descending order of value in key pair
        vector<pair<string,int>> topwordsVector;
        //iterator
        map<string, int>::iterator it = hashTable.begin();

        //push all pairs into vector and ignore keys less than 0
        for (it=hashTable.begin(); it!=hashTable.end(); it++){
            if(it->second >0){
                topwordsVector.push_back(make_pair(it->first, it->second));
            }

        }
        //call helper function sortval to sort vector
        sort(topwordsVector.begin(), topwordsVector.end(), sortByVal);
        //print out # of top words passed as parameter
        cout << "top " << topwords << " words found:" << endl;
        for (int i = 0; i < topwords; i++)
        {
            cout << topwordsVector[i].first << ": " << topwordsVector[i].second << endl;
        }
    }

};





int main() {

    //instance obj to the analysis class
    DataAnalytics dataAnalytics;

    //driver function that performs all internal operations
    dataAnalytics.analyzeCount();

    //when we have finished processing the file perform analysis //
    dataAnalytics.searchOccurrences("day");


    //driver function that performs internal operations on simularity
    dataAnalytics.analyzeSimilarity("Bible","Bible");

    //get the similarity between the two files word for word
    dataAnalytics.getSimilarity();

    //find top # of words, based on number passed
    dataAnalytics.topWords(5);
}
