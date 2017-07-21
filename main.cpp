#include <iostream>
#include <algorithm>

using namespace std;

class Node {
    protected:
        string word;
        Node * next = NULL;//or right son
        Node * previusly = NULL;// or left Son
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
    Node ** getPreviuslyPointeer(){
        return  &this->previusly;
    }
    Node * getPreviusly(){
        return  this->previusly;
    }

    string getWord() {
        return this->word;
    }
    Node ** getNextPointeer(){
        return  &this->next;
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
        //fnv=1a hash
        unsigned int hash = 0x811c9dc5;
        for(int i = 0; i < input.length(); i++){
            hash = hash xor input[i];
            hash = hash * 16777619;
        }
        return hash % this->tableSize;
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
    bool put(string input){
        int hashedTo = hashF(input);
        if (this->table[hashedTo] == NULL) {
            Node *newNode = new Node(input, NULL, NULL);
            this->table[hashedTo] = newNode;
        }else{
            Node *newNode = new Node(input, this->getTable(hashedTo), NULL);
            this->table[hashedTo] = newNode;
            this->table[hashedTo]->getNext()->setPreviusly(newNode);
        }
        this->numOfElements++;
        if(((double)this->numOfElements/(double)this->tableSize) > this->loadFactor){
           // cout<< "should rehash";
            rehash();
            //printHash();
        }
        return true;
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
class AvlTree{
private:
    Node * root;
    void rightBalance(Node ** root){
        int fb = balancingFactor((*root)->getNext());
        if(fb < 0){
            RRRotation(root);
            return;
        }else if(fb > 0){
            LLRotation(((* root)->getNextPointeer()));
            RRRotation(root);
            return;
        }
    }
    void leftBalance(Node ** root){
        int fb= balancingFactor((*root)->getPreviusly());
        if(fb > 0){
            LLRotation(root);
            return;
        }else if(fb < 0){
            RRRotation(((*root)->getPreviuslyPointeer()));
            LLRotation(root);
            return;
        }
    }
    void balance(Node ** root){
        int fb = balancingFactor(*root);
        if(fb > 1){
            leftBalance(root);
        }else if(fb < -1){
            rightBalance(root);
        }else return;
    }
    int balancingFactor(Node *root){
        if(root == NULL) return 0;
        return height(root->getPreviusly()) - height(root->getNext());
    }
    int height(Node *node){
        int leftHeight, rightHeight;
        if(node == NULL) return 0;
        leftHeight = height(node->getPreviusly());
        rightHeight = height(node->getNext());
        if(leftHeight > rightHeight){
            return leftHeight+1;
        }else return rightHeight+1;
    }
//rotação simples a direita
    void LLRotation(Node ** root){
        Node * node = (*root)->getPreviusly();
        (*root)->setPreviusly(node->getNext());
        node->setNext(*root);
        *root = node;
    }
//rotação simples a esquerda
    void RRRotation(Node ** root){
        Node * node = (*root)->getNext();
        (*root)->setNext(node->getPreviusly());
        node->setPreviusly(*root);
        *root = node;
    }
public:
    AvlTree(){
        this->root = NULL;
    }
    Node ** getRoot(){
        return &this->root;
    }
    void setRoot(Node * root){
        this->root = root;
    }
    void insert(string input, Node **root){
        if(*root == NULL )
            *root = new Node(input, NULL, NULL);
        else{
            if((*root)->getWord().compare(input) < 0){
                insert(input, (*root)->getNextPointeer());
                balance(root);
            }
            else if((*root)->getWord().compare(input) > 0){
                insert(input, (*root)->getPreviuslyPointeer());
                balance(root);
            }
        }
    }
    void printSimetricOrder(Node *root){
        if(root != NULL){
            printSimetricOrder(root->getPreviusly());
            cout << (root)->getWord() + "\n";
            printSimetricOrder(root->getNext());
        }
    }
    void freeTree(Node **root){
        if((*root) != NULL){
            freeTree((*root)->getPreviuslyPointeer());
            freeTree((*root)->getNextPointeer());
            delete *root;
        }
    }
};

bool recognizeIt(string input, Hash * hashTable);

int main() {
    string inputWord, lastInputWord;
    Hash *hashTable = new Hash(0.75, 5);
    while(cin >> inputWord){
        if(inputWord.compare("+") == 0){
            if (lastInputWord.empty() || hashTable->containsValue(lastInputWord))
                cout << "fail\n";
            else if(hashTable->put(lastInputWord))
                cout << "ok\n";
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
            transform(inputWord.begin(), inputWord.end(), inputWord.begin(), ::tolower);
            lastInputWord = inputWord;
            if(hashTable->containsValue(lastInputWord))
                cout << "ok\n";
            else{
                if(!recognizeIt(lastInputWord, hashTable))
                    cout << "not found\n";
            }
        }
    }
    return 0;
}

bool recognizeIt(string input, Hash * hashTable){
    //one more char put
    AvlTree * avlTree = new AvlTree();
    for(int i = 0; i < input.length() ;i++){
        string newInput;
        newInput.append(input);
        newInput.erase(i,1);
       // cout << newInput +"\n";
        if(hashTable->containsValue(newInput)){
            avlTree->insert(newInput, avlTree->getRoot());
           // cout<< newInput + "\n";
        }
    };
    // one less char input
    for(int i = 0; i <= input.length() ;i++){
       char letter = 'a';
       while(letter >= 'a' && letter <='z'){
           string newInput;
           newInput.append(input);
           newInput.insert(i, 1, letter);
           //cout << newInput +"\n";
           if(hashTable->containsValue(newInput))
               avlTree->insert(newInput, avlTree->getRoot());

           //cout<< newInput + "\n";
           letter++;
       }
        string newInput;
        newInput.append(input);
        newInput.insert(i, 1, '-');
        //cout << newInput +"\n";
        if(hashTable->containsValue(newInput))
            avlTree->insert(newInput, avlTree->getRoot());
        //cout<< newInput + "\n";
    }
    // changed letters
    for(int i=0; i < input.length() -1 ; i++){
        string newInput;
        newInput.append(input);
        swap(newInput[i], newInput[i+1]);
        //cout << newInput + " cl\n";
        if(hashTable->containsValue(newInput))
            avlTree->insert(newInput, avlTree->getRoot());

//            cout<< newInput + "\n";
    }
    // wrong letter
    for(int i=0; i < input.length(); i++){
        char letter = 'a';
        while(letter >= 'a' && letter <='z'){
            string newInput;
            newInput.append(input);
            newInput[i] = letter;
            //cout << newInput +"\n";
            if(hashTable->containsValue(newInput))
                avlTree->insert(newInput, avlTree->getRoot());

//                cout<< newInput + "\n";
            letter++;
        }
        string newInput;
        newInput.append(input);
        newInput[i] = '-';
        //cout << newInput +"\n";
        if(hashTable->containsValue(newInput))
            avlTree->insert(newInput, avlTree->getRoot());

    }
    avlTree->printSimetricOrder(*avlTree->getRoot());
    if(*avlTree->getRoot()!=NULL){
        avlTree->freeTree(avlTree->getRoot());
        delete avlTree;
        return true;
    }else{
        delete avlTree;
        return  false;
    }
    //cout << input;
}
