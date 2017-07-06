#include <iostream>
using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    string inputWord, lastInputWord;
    while(cin >> inputWord){
        if(inputWord.compare("+") == 0){
            if(lastInputWord.empty())
                cout << "fail+";
            else cout << "+" + lastInputWord;
        }else if(inputWord.compare("-") == 0){
            if(lastInputWord.empty())
                cout << "fail-";
            else cout<< "-" + lastInputWord;
        }else if(inputWord.compare("*") == 0)
            break;
        else{//find word
            lastInputWord = inputWord;
            cout << "find " +lastInputWord;
        }
    }
    return 0;
}