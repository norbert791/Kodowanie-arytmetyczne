#include<iostream>
#include<string>
#include<bitset>
#include<map>
#include<limits>
#include<fstream>
#include<limits>
#include "supportingClasses.h"

#define ull unsigned long long
#define uc unsigned char

using namespace std;

int main () {
    MyModel t;
    /*
    for (int i = 0; i < 256; i++) {
        t.increaseFrequency(i);
    }
    t.updateProbabilities();  
    unsigned long long* tab = t.getPartialSums();
    for (int i = 0; i < 257; i++) {
        cout<<tab[i]<<" ";
    }
    cout<<endl;
    */
    uc temp = 125;
    cout<<((uc)255 & temp)<<endl; 
}