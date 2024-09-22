#include<iostream>
using namespace std;

int main() {
    int n = 10;
    int num1 = 0;
    int num2 = 1;
    int next_number = num2;
    int count = 1;

    while (count <= n) {
        cout << next_number << " ";
        count++;
        num1 = num2;
        num2 = next_number;
        next_number = num1 + num2;
    }

    cout << endl;

    return 0;
}