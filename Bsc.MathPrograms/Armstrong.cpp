#include<iostream>
#include<string>
#include <cmath> // include cmath for pow function

using namespace std;

int main() {
    int a;
    cout << "Enter a Number: ";
    cin >> a;

    string b = to_string(a);
    int c = b.length();
    double sum = 0; // declare sum as double to avoid precision loss

    for (char i : b) {
        sum += pow((i - '0'), c);
    }

    if (sum == a) {
        cout << "The No. " << a << " is an Armstrong Number.";
    } else {
        cout << "The No. " << a << " is not an Armstrong Number.";
    }

    return 0;
}