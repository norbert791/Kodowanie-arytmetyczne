#include<iostream>
#include<string>
#include<bitset>
#include<map>
#include<limits>
#include<fstream>

using namespace std;

int main () {
    char chuj;
    ifstream temp("testOutput2.txt");
    unsigned char temp2 = 255;
    cout<<((unsigned char) ((temp2 << 1) + 2 + 65) )<<endl;
}