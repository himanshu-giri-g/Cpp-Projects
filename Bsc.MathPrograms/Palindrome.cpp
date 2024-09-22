#include<iostream>
#include<string>
using namespace std;

int main() {
    string str_n;
    cout << "Enter a word: ";
    cin >> str_n;

    string reversed_str_n = str_n;
    for (int i = 0; i < str_n.length() / 2; i++) {
        swap(reversed_str_n[i], reversed_str_n[str_n.length() - i - 1]);
    }

    if (str_n == reversed_str_n) {
        cout << "The word " << str_n << " is a Palindrome." << endl;
    } else {
        cout << "The word " << str_n << " is not a Palindrome." << endl;
    }

    return 0;
}