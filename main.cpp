#include <iostream>

using namespace std;

class Node {
    private:
        string word;
        Node * next = NULL;
    public:
    Node(){
        this->next = NULL;
        this->word = "";
    }
    Node(string word, Node *next) {
        this->next = next;
        this->word = word;
    }

    string getWord() {
        return this->word;
    }

    Node  * getNext() {
        return this->next;
    }
    void setWord(string word) {
        this->word = word;
    }
    Node setNext(Node * next) {
        this->next = next;
    }

};

class Hash{
    private:
        Node ** table;
        int numOfElements;
        int tableSize;
        double loadFactor; //numOfElements/tableSize
    public:

    Hash(double loadFactor, int tableSize) {
        this->numOfElements = 0;
        this->loadFactor = loadFactor;
        this->tableSize = tableSize;
        this->table = new Node*[tableSize];
    }
    Node  * getTable(int n) {
        return this->table[n];
    }
    Node ** getTable(){
        return this->table;
    }
    void setTable(int n, Node * table) {
        this->table[n] = table;
    }
    int getNumOfElements(){
        return this->numOfElements;
    }
    void setNumOfElements(int numOfElements){
        this->numOfElements = numOfElements;
    }
    int getTableSize(){
        return this->tableSize;
    }
    void setTableSize(int tableSize){
        this->tableSize = tableSize;
    }
    double getLoadFactor(){
        return this->loadFactor;
    }
    double  setLoadFactor(double loadFactor){
        this->loadFactor = loadFactor;
    }

};

int hashF(string input, int tableSize);

void printHash(Hash * hashTable);

void freeHash(Hash * hashTable);

bool verifyWord(Hash * hashTable, string input);

int main() {
    string inputWord, lastInputWord;
    Hash *hashTable = new Hash(0.5, 5);
    while(cin >> inputWord){
        if(inputWord.compare("+") == 0){
            if (lastInputWord.empty() || verifyWord(hashTable, lastInputWord))
                cout << "fail+";
            else {
                int hashedTo = hashF(lastInputWord, hashTable->getTableSize());
                if (hashTable->getTable(hashedTo) == NULL) {
                    Node *newNode = new Node(lastInputWord, NULL);
                    hashTable->setTable(hashedTo, newNode);
                } else {
                    Node *newNode = new Node(lastInputWord, hashTable->getTable(hashedTo));
                    hashTable->setTable(hashedTo, newNode);
                }
                hashTable->setNumOfElements(hashTable->getNumOfElements()+1);
                cout << "ok";
                if(((double)hashTable->getNumOfElements()/(double)hashTable->getTableSize()) > hashTable->getLoadFactor())
                    cout<< "should rehash";
            }
        }else if(inputWord.compare("-") == 0){
            if(lastInputWord.empty())
                cout << "fail-";
            else
                cout << "-" + lastInputWord;
        }else if (inputWord.compare("*") == 0){
            freeHash(hashTable);
            break;
        }else if(inputWord.compare("?") == 0){
            printHash(hashTable);
        }else{//find word
            lastInputWord = inputWord;
            if(verifyWord(hashTable, lastInputWord))
                cout << "ok";
        }
    }
    return 0;
}
int hashF(string input, int tableSize){
    int inputInt = (int) input.size();
    return inputInt%tableSize;
}
void printHash(Hash * hashTable){
    for(int i = 0; i < hashTable->getTableSize(); i++){
        for(Node * node = hashTable->getTable(i); node != NULL; node = node->getNext()){
            cout << "\n position ";
            cout << i;
            cout << " word: ";
            cout << node->getWord();
        }
    }
}
bool verifyWord(Hash * hashTable, string input){
    int hashedto = hashF(input, hashTable->getTableSize());
    for(Node * node = hashTable->getTable(hashedto); node != NULL; node = node->getNext()){
        if(node->getWord().compare(input) == 0)
            return true;
    }
    return false;
}
void freeHash(Hash * hashTable){
    Node * aux = NULL;
    for(int i = 0; i < hashTable->getTableSize(); i++){
        for(Node * node = hashTable->getTable(i); node != NULL; node = aux){
            aux = node->getNext();
            delete node;
        }
    }
    delete[] hashTable->getTable();
    delete hashTable;
}