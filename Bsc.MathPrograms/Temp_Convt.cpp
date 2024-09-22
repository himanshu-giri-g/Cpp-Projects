#include<iostream>
using namespace std;

float celsius_K(float temp) {  // Converting Temperature from Celsius to Kelvin
    float conv_t = temp + 273;
    return conv_t;
}

float kelvin_C(float temp) {  // Converting Temperature from Kelvin to Celsius
    float conv_t = temp - 273;
    return conv_t;
}

int main() {
    cout << "---WELCOME TO TEMPERATURE CONVERTER---" << endl;
    cout << "1. Celsius to Kelvin\n2. Kelvin to Celsius" << endl;

    int a;
    cout << "Choose any one option: ";
    cin >> a;

    if (a == 1) {
        float temp;
        cout << "Enter Temperature to be converted (in Celsius): ";
        cin >> temp;
        cout << "The Temperature in Kelvin for the given temperature is: " << celsius_K(temp) << " K" << endl;
    } else if (a == 2) {
        float temp;
        cout << "Enter Temperature to be converted (in Kelvin): ";
        cin >> temp;
        cout << "The Temperature in Celsius for the given temperature is: " << kelvin_C(temp) << " C" << endl;
    } else {
        cout << "ERROR ! Please Choose a valid option." << endl;
    }

    return 0;
}