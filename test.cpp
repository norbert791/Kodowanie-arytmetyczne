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
    while(temp.peek() != EOF) {
        temp>>chuj;
        cout<<chuj<<endl;
    }
}