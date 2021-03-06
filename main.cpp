#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <map>



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
    vector<string> ignoreList = {"a", "all", "and", "any", "after", "among", "again", "about",
                                 "be", "because", "before", "bring",
                                 "every",
                                 "for",
                                 "he", "his", "him",
                                 "i", "is", "in", "it",
                                 "not", "neither",
                                 "of", "or",
                                 "saying", "should",
                                 "that", "the", "their", "them", "there", "they", "to", "these", "therefore", "things",
                                 "you",
                                 "what", "with", "which"};

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
    void analyzeCount(string fileName){
        //performs all operations on the input file so we can access the data from it later
        string word;
        ifstream inFS;
        string readData;
        try{
            inFS.open(fileName);
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
                if(word != "" && word.size() > 4){
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

    void addtoignore(string ignore){
        ignoreList.push_back(ignore);
    }
    void updateignore(string ignore){
        try {
            //checks to see if the value is in the hashtable
            //this is fast 0(1) to find out
            if (hashTable.at(ignore) != 0) {
                hashTable.erase(ignore);
                hashTable[ignore] = 0;
            }
            else{
                cout << "word already ignored" << endl;
            }
        }catch (...) {
            cout << "word " << ignore << " not in list" << endl;
        }

    }

    void printword(string word){
        cout << "word " << word << " count " <<  hashTable.at(word) << endl;
    }

    //function used to find and replace a word in the file and output a new file
    void findAndReplace(string findWord, string replaceWord, string newFileName){
        string word;
        ifstream inFS;
        string readData;
        // Create and open a text file
        ofstream newFile(newFileName);
        try{
            inFS.open("import.txt");
            if(!inFS.is_open()){
                throw "File is empty";
            }
            cout << "Processing...Please Wait..\n";
            while(!inFS.eof()){
                inFS >> readData;
                //cout << word << endl;
                //perform the word ops here //ignores empty strings
                if(readData != ""){
                    //if no alphabet letters in string, string will return empty
                    word = clean(readData);
                    //check to see if this is the word we want to replace
                    if(word == findWord){
                        //change the word
                        word = replaceWord;
                    }
                    // Write to the file
                    newFile << word;
                    newFile << " ";
                }
            }
            //when we have finished processing
            cout << "Finished Processing!\n";
        }
        catch(const char* msg) {
            cerr << msg << endl;
        }
        // Close the file
        newFile.close();
    }


    //function used to get input from the user
    void getInput() {

        //input values
        string file1name;
        string file2name;
        string searchWord;
        string replacedWord;
        bool getInput = true;
        //the input
        int value;
        //values that may be needed based on inputs
        while(getInput == true) {
            cout << "Select an operation\n";
            cout << "1 : Analyze word count\n";
            cout << "2 : Search Occurrences of a word\n";
            cout << "3 : Analyze Similarity\n";
            cout << "4 : Top Word\n";
            cout << "5 : Find and Replace\n";
            cout << "6 : Quit Program\n";
            cin >> value;
            //switch based on the input
            switch (value) {

                case 1:
                    //this is the case for analyze word count
                    cout << "Enter the file name\n";
                    cin >> file1name;
                    analyzeCount(file1name);
                    break;
                case 2:
                    //this is the case for search occurrences of a word
                    cout << "Enter the word you want to search\n";
                    //input the word
                    cin >> searchWord;
                    //search for the word
                    searchOccurrences(searchWord);
                    break;
                case 3:
                    //this is the case for analyze similarity
                    cout << "Enter the first file name\n";
                    cin >> file1name;
                    cout << "Enter the second file name\n";
                    cin >> file2name;
                    //call the function to analyze the similarity
                    analyzeSimilarity("import.txt", "import.txt");
                    break;
                case 4:
                    cout << "The top 5 words are : \n";
                    //this is the top word
                    topWords(5);
                    break;
                case 5:
                    cout << "Enter the file name\n";
                    cin >> file1name;
                    cout << "Enter the search word\n";
                    cin >> searchWord;
                    cout << "Enter the replace word\n";
                    cin >> replacedWord;
                    //this is the find and replace case
                    findAndReplace(searchWord, replacedWord, file1name);
                case 6:
                    getInput = false;
                    exit(0);
                default:
                    cout << "This is the default case\n";
            }

        }

    }
};





int main() {




    //instance obj to the analysis class
    DataAnalytics dataAnalytics;
    //used to get input from the user
    dataAnalytics.getInput();

    //driver function that performs all internal operations
    dataAnalytics.analyzeCount("import.txt");

    //when we have finished processing the file perform analysis //
    dataAnalytics.searchOccurrences("day");


    //driver function that performs internal operations on simularity
    dataAnalytics.analyzeSimilarity("import.txt", "import.txt");

    //get the similarity between the two files word for word
    dataAnalytics.getSimilarity();

    //find top # of words, based on number passed
    dataAnalytics.topWords(30);


    //for GUI to update and print word counts
    dataAnalytics.updateignore("called");
    dataAnalytics.printword("called");

    //function used to find a replace a new file
    dataAnalytics.findAndReplace("day", "night", "newfile.txt");


}
