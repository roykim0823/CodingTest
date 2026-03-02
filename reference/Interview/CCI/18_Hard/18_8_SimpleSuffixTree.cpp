// Q. Given a string s and an array of smaller string T, design a method to search s for each small
//    string in T.

/* Solutions: suing suffix tree */

#include <iostream>
#include <cstring>
using namespace std;

class Trie{
public:
    static const int MAX_N = 100 * 100;	// Size of rows
    static const int CLD_NUM = 26;      // Each characters, only lower characters
    int size;
    int trie[MAX_N][CLD_NUM];

    Trie(const char* s);
    void insert(const char* s);
    bool find(const char* s);
};

Trie::Trie(const char* s){
    memset(trie[0], -1, sizeof(trie[0]));
    size = 1;
    while(*s){
        insert(s); 	// Insert each substring into the subfix tree
        ++s;
    }
}

void Trie::insert(const char* s){
    int p = 0;
    while(*s){
        int i = *s - 'a';
        if(-1 == trie[p][i]){
            memset(trie[size], -1, sizeof(trie[size]));
            trie[p][i] = size++;
        }
        p = trie[p][i];
        ++s;
    }
}

bool Trie::find(const char* s){
    int p = 0;
    while(*s){
        int i = *s - 'a';
        if(-1 == trie[p][i])
            return false;
        p = trie[p][i];
        ++s;
    }
    return true;
}

int main(){
    Trie tree("mississippi");
    string patt[] = {
        "is", "sip", "hi", "sis", "mississippa"
    };
    int n = 5;
    for(int i=0; i<n; ++i)
        cout<<tree.find((char*)&patt[i][0])<<endl;
    return 0;
}
