#include <iostream>

using namespace std;

class Node {
    private:
        string word;
        Node * next = NULL;
        Node * previusly = NULL;
    public:
    Node(){
        this->next = NULL;
        this->previusly = NULL;
        this->word = "";
    }
    Node(string word, Node *next, Node * previusly) {
        this->next = next;
        this->word = word;
        this->previusly = previusly;
    }
    Node * getPreviusly(){
        return  this->previusly;
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
    void setPreviusly(Node * previusly){
        this->previusly = previusly;
    }
    void setNext(Node * next) {
        this->next = next;
    }

};

class Hash{
    private:
        Node ** table;
        int numOfElements;
        int tableSize;
        double loadFactor; //numOfElements/tableSize

    int hashF(string input){
        int inputInt = (int) input.size();
        return inputInt%this->tableSize;
    }
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
    void remove(string input){
        int hashedto = this->hashF(input);
        for(Node * node = this->table[hashedto]; node != NULL; node = node->getNext()){
            if(node->getWord().compare(input) == 0){
                if(node == this->table[hashedto]){
                    Node * aux = node->getNext();
                    if(node->getNext()!= NULL)
                        node->getNext()->setPreviusly(NULL);
                    delete node;
                    this->table[hashedto] = aux;
                    this->numOfElements--;
                    cout << "ok\n";
                    return;
                }
                if(node->getPreviusly() != NULL)
                    node->getPreviusly()->setNext(node->getNext());
                if(node->getNext() != NULL)
                    node->getNext()->setPreviusly(node->getPreviusly());
                delete node;
                this->numOfElements--;
                cout << "ok\n";
                return;
            }
        }
        cout << "fail\n";
    }
    void put(string input){
        int hashedTo = hashF(input);
        if (this->table[hashedTo] == NULL) {
            Node *newNode = new Node(input, NULL, NULL);
            this->table[hashedTo] = newNode;
        }else {
            Node *newNode = new Node(input, this->getTable(hashedTo), NULL);
            this->table[hashedTo] = newNode;
            this->table[hashedTo]->getNext()->setPreviusly(newNode);
        }
        this->numOfElements++;
        cout << "ok\n";
        if(((double)this->numOfElements/(double)this->tableSize) > this->loadFactor){
            cout<< "should rehash";
            rehash();
            printHash();
        }

    }
    bool containsValue(string value){
        int hashedto = hashF(value);
        for(Node * node = this->table[hashedto]; node != NULL; node = node->getNext()){
            if(node->getWord().compare(value) == 0)
                return true;
        }
        return false;
    }
    void clear(){
        Node * aux = NULL;
        for(int i = 0; i < this->tableSize; i++){
            for(Node * node = this->table[i]; node != NULL; node = aux){
                aux = node->getNext();
                delete node;
            }
        }
        delete[] this->table;
    };
    void rehash(){
        int newSize =  this->tableSize * 2;
        Hash *newHashTable = new Hash(0.75, newSize);
        Node * aux = NULL;
        for(int i = 0; i < this->tableSize; i++){
            for(Node * node = this->table[i]; node != NULL; node = aux){
                newHashTable->put(node->getWord());
                aux = node->getNext();
                delete node;
            }
        }
        delete[] this->table;
        this->tableSize = newSize;
        this->table = newHashTable->table;
        delete newHashTable;
    }
    void printHash(){
        for(int i = 0; i < this->tableSize; i++){
            for(Node * node = this->table[i]; node != NULL; node = node->getNext()){
                cout << "\n position ";
                cout << i;
                cout << " word: ";
                cout << node->getWord();
            }
        }
        /*Node * node;
        for(int i = 0; i < this->tableSize; i++){
            for(node = this->table[i]; node != NULL; node = node->getNext())
                if(node->getNext() == NULL)
                    break;
            for(Node * node1 = node; node1 != NULL; node1 = node1->getPreviusly()){
                cout << "\n position ";
                cout << i;
                cout << " word: ";
                cout << node1->getWord();
            }
        }*/

    }
};


int main() {
    string inputWord, lastInputWord;
    Hash *hashTable = new Hash(0.75, 5);
    while(cin >> inputWord){
        if(inputWord.compare("+") == 0){
            if (lastInputWord.empty() || hashTable->containsValue(lastInputWord))
                cout << "fail\n";
            else
                hashTable->put(lastInputWord);
        }else if(inputWord.compare("-") == 0){
            if(lastInputWord.empty())
                cout << "fail\n";
            else {
                hashTable->remove(lastInputWord);
            }
        }else if (inputWord.compare("*") == 0){
            hashTable->clear();
            delete hashTable;
            break;
        }else if(inputWord.compare("?") == 0){
            hashTable->printHash();
        }else{//find word
            lastInputWord = inputWord;
            if(hashTable->containsValue(lastInputWord))
                cout << "ok\n";
        }
    }
    return 0;
}


