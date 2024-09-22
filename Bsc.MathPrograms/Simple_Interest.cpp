#include<iostream>
using namespace std;

int main() {
    int P;
    cout << "Enter Principal Amount (in ₹): ";
    cin >> P;

    float R;
    cout << "Enter Rate of Interest (in %): ";
    cin >> R;

    int T;
    cout << "Enter Time Period (in years): ";
    cin >> T;

    float SI = (P * R * T) / 100;  // Simple Interest
    cout << "Simple Interest for the given amount is: Rs." << SI << endl;

    return 0;
}