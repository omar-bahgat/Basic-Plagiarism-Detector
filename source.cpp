#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

using namespace std;

// function prototypes

long long unsigned hashing(string sentence);
bool Rabin_Karp_string_matching(string database, string sentence);

void calculate_longest_proper_prefix(string sentence, vector<int> &lpp);
bool Knuth_Morris_Pratt_string_matching(string database, string sentence);

void calculatePatrn(string str, int n, int patrn[]);
bool Boyer_Moore_string_matching(string str, string simlr);

int calculate_hamming_distance(string s1, string s2);
bool Hamming_Distance_string_matching(string database, string sentence);

string convert_file_to_string(string fileName);
vector<string> separate_file_into_sentences(string fileName);

void DisplayResults(double percentage, set<string> s);
void plagiarism_detection(string test_file, int algorithm);


// Rabin-Karp Algorithm

long long unsigned hashing(string sentence) {
    
    long long unsigned hashingCode = 0;
    
    // complex hashing equation to make sure that there won't be two words with the same ascii code
    for (int i = 0; i < sentence.length(); i++) {
        hashingCode = (hashingCode * 31 + sentence[i]) % 1000000007;
    }
    
    return hashingCode; // return the summation of the ascii code
}

bool Rabin_Karp_string_matching(string database, string sentence){
    
    int pattern_hash = hashing(sentence);
    
    for (int i = 0; i < database.length() - sentence.length() + 1; i++) {
        string substring = database.substr(i, sentence.length());
        
        if (pattern_hash == hashing(substring)) {
            for (int i = 0; i < sentence.size(); i++) {
                if (sentence[i] != substring[i])
                    return false;
            }
            return true;
        }
    }
    return false;
}

// Knuth-Morris-Pratt Algorithm

void calculate_longest_proper_prefix(string sentence, vector<int> &lpp) {
    int length = 0;
    lpp[0] = 0;
    int i = 1;
    
    while (i < sentence.length()) {
        if (sentence[i] == sentence[length]) {
            length++;
            lpp[i] = length;
            i++;
        }
        else {
            if (length != 0) {
                length = lpp[length - 1];
            }
            else {
                lpp[i] = 0;
                i++;
            }
        }
    }
}

bool Knuth_Morris_Pratt_string_matching(string database, string sentence){
    vector<int> lpp(sentence.length());
    calculate_longest_proper_prefix(sentence, lpp);
    int i = 0, j = 0;
    while (i < database.length()) {
        if (database[i] == sentence[j]) {
            i++, j++;
        }
        if (j == sentence.length()) {
            return true;
        }
        else if (i < database.length() && database[i] != sentence[j]) {
            if (j != 0) {
                j = lpp[j - 1];
            }
            else {
                i++;
            }
        }
    }
    return false;
}
    
// Boyer-Moore Algorithm

# define TotalChars 256

void calculatePatrn(string str, int n, int patrn[TotalChars]){
    
    for (int i = 0; i < TotalChars; i++){
        patrn[i] = -1;
    }

    for (int i = 0; i < n; i++){
        patrn[(int)str[i]] = i;
    }
}

bool Boyer_Moore_string_matching(string str, string simlr){
    
    int m = simlr.size();
    int n = str.size();

    int patrn[TotalChars];

    calculatePatrn(simlr, m, patrn);

    int shift = 0;

    while (shift <= (n - m)){
        
        int indicator = m - 1;

        while (indicator >= 0 && simlr[indicator] == str[shift + indicator]){
            indicator--;
        }

        if (indicator < 0){
            return true;
        }

        else{
            shift += max(1, indicator - patrn[str[shift + indicator]]);
        }
    }
    
    return false;
}

// Hamming-Distance Brute Force Algorithm

int calculate_hamming_distance(string s1, string s2){
    int distance = 0;
    for (int i = 0; i < s1.length(); i++){
        if (s1[i] != s2[i]){
            distance++;
        }
    }
    return distance;
}

bool Hamming_Distance_string_matching(string database, string sentence) {
    
    for (int i = 0; i < database.length() - sentence.length() + 1; i++){
        string substring = database.substr(i, sentence.length());
        if (calculate_hamming_distance(substring, sentence) == 0){
            return true;
        }
    }
    return false;
}


// file handling

string convert_file_to_string(string fileName){
    ifstream in;
    in.open(fileName);
    string sentence;
    string file = "";
    while (getline(in, sentence,'.')){
        file += sentence;
    }
    return file;
}

vector<string> separate_file_into_sentences(string fileName){
    ifstream in;
    in.open(fileName);
    string sentence;
    vector<string> sentences;
    
    getline(in, sentence, '.');
    sentences.push_back(sentence);
    
    while (getline(in, sentence, '.')){
        sentence.erase(sentence.begin());
        sentences.push_back(sentence);
    }
    sentences.pop_back();               // pop back last element of the vector as a dummy space is read
    
    return sentences;
}

void DisplayResults(double percentage, set<string> plagiarized_docs){
    
    cout << percentage << "%\n";
    
    cout << "Documents from which the test file was plagiarized: \n";
    
    int i = 1;
    set<string> :: iterator it;
    
    for(it = plagiarized_docs.begin(); it != plagiarized_docs.end(); it++){
        cout << i << ". " << (*it) << "\n";
        i++;
    }
    
    for(int i = 0; i < 51; i++) cout << "-";
    cout << "\n";
}

void plagiarism_detection(string test_file, int algorithm){
    
    const int num_of_files = 3;
    
    string file1 = "file1.txt";
    string file2 = "file2.txt";
    string file3 = "file3.txt";
    
    // Database where we will compare our test file with
    string database[num_of_files] = {file1, file2, file3};
    
    string comparison_file; // database files which will be converted to strings when comparing
    
    vector<string> sentences = separate_file_into_sentences(test_file);
    
    vector<int> marked(sentences.size(),0); // vector indicating sentence is plagiarized or not
    
    set<string> plagiarized_documents; // will contain documents from which the potential document was plagiarized
    
    
    // for each sentence, check if it exists in any of the files or not. If yes, we mark it by 1 and file to the set
    
    for(int i = 0; i < sentences.size(); i++){
        
        for(int j = 0; j < num_of_files; j++){
            
            comparison_file = convert_file_to_string(database[j]);  // convert file to string to be able to compare
    
            switch (algorithm){
                    
                case 1: {
                    if(Rabin_Karp_string_matching(comparison_file, sentences[i]) == 1){
                        marked[i] = 1;
                        plagiarized_documents.insert(database[j]);
                    }
                }
                case 2: {
                    
                    if(Knuth_Morris_Pratt_string_matching(comparison_file, sentences[i]) == 1){
                        marked[i] = 1;
                        plagiarized_documents.insert(database[j]);
                    }
                }
                case 3: {
                    if(Boyer_Moore_string_matching(comparison_file, sentences[i])){
                        marked[i] = 1;
                        plagiarized_documents.insert(database[j]);
                    }
                }
                case 4: {
                    if( Hamming_Distance_string_matching(comparison_file, sentences[i]) == 1){
                        marked[i] = 1;
                        plagiarized_documents.insert(database[j]);
                    }
                }
            }
        }
    }

    double counter_matching = count(marked.begin(), marked.end(), 1); // if value = 1, then sentence is plagiarized
    
    double percentage = ((counter_matching / sentences.size()) * 100); // divide # of plagiarized sentences / total # of sentences
    
    DisplayResults(percentage, plagiarized_documents);
}

int main(){
    
    string test_file = "testfile.txt";
    
    cout << "Rabin-Karp: ";
    plagiarism_detection(test_file, 1);
    
    cout << "Knuth-Morris-Pratt: ";
    plagiarism_detection(test_file, 2);
    
    cout << "Boyer-Moore: ";
    plagiarism_detection(test_file, 3);
    
    cout << "Hamming Distance: ";
    plagiarism_detection(test_file, 4);
    
    return 0;
}
