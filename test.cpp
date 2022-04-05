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

const uint32_t msbMask = 0x80000000; //binary 1000.....
const uint32_t secMsbMask = 0x040000000; //second msb mask 


using namespace std;

int main () {
    
    {
        BitWriter writer("encoded.txt",22);
        writer.writeBit(true);
    }

    MyModel model;
    unsigned char tempInput = 'd';
    uint32_t l = 0, prevl = 0, prevu = numeric_limits<uint32_t>::max(), u = prevu = numeric_limits<uint32_t>::max();

    cout<<((u & msbMask) == msbMask)<<endl;

   // l = prevl + (uint32_t)((((uint64_t)prevu - prevl + 1) * model.getPartialSums()[tempInput]) / model.getTotalLength());
 //   u = prevl + (uint32_t)((( (uint64_t)prevu - prevl + 1) * model.getPartialSums()[tempInput + 1]) / model.getTotalLength()) - 1;

    cout<<l<<" "<<u<<endl;
    
    /*
    BitReader reader("encoded.txt");
    auto t = reader.readTag();
    bool b = reader.getBit();
    cout<<b<<endl;
    cout<<t<<endl;*/


    

}