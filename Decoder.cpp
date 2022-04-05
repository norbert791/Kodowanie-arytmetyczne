#include<iostream>
#include<string>
#include<limits>
#include<fstream>
#include<map>
#include<exception>
#include<cmath>
#include "supportingClasses.h"
#include<memory>

using namespace std;

#define uc unsigned char 

const int alphabetSize = 256;
const uint32_t msbMask = 0x80000000; //binary 1000.....
const uint32_t secMsbMask = 0x040000000; //second msb mask 

class Decoder {
    public:
        void decode(string inputFileName, string outputFileName);
    private:
        Model* model = new MyModel();

};

void Decoder::decode(string inputFileName, string outputFileName) {
    ofstream writer(outputFileName);
    uint32_t l = 0, u = numeric_limits<uint32_t>::max();
    uint32_t inputLength = 0;
    uint32_t prevl, prevu;
    uint32_t tag;
    unique_ptr<BitReader> reader;
    
    try {
        if (!writer.good()) {
            throw logic_error("Input error");
        }
        reader = make_unique<BitReader>(inputFileName);
        inputLength = reader->readLength();
       
    } catch(logic_error &e) {
        cerr<<e.what()<<endl;
        exit(1);
    }
   // cout<<inputLength<<endl;
    tag = reader->readTag();
  //  cout<<tag<<endl;
    while (inputLength > 0) {
        
        uc k = 0;
      //  cout<<"tag: "<<(int)tag<<endl;
        while ((uint32_t)((((uint64_t) tag - l + 1) *  model->getTotalLength() - 1) / ((uint64_t)u - l + 1)) >= model->getPartialSums()[k]) {
            k++;
        }
       // cout<<"while: "<<(int)(uint32_t)((((uint64_t) tag - l + 1) *  model->getTotalLength() - 1) / ((uint64_t)u - l + 1))<<endl;
        k -= 1;
        writer<<(k);
        prevu = u;
        prevl = l;

        l = prevl + (uint32_t)((((uint64_t)prevu - prevl + 1) * model->getPartialSums()[k]) / model->getTotalLength());
        u = prevl + (uint32_t)((((uint64_t)prevu - prevl + 1) * model->getPartialSums()[k + 1]) / model->getTotalLength()) - 1;
   //     cout<<(int)l<<" "<<(int)u<<endl;
        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) == ( u & msbMask))) {
                l = (uint32_t) (l <<1);
                u = (uint32_t) (u <<1) + 1;
                tag = (uint32_t) (tag <<1) + reader->getBit();
            }

            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = (uint32_t) (l <<1);
                u = (uint32_t) (u <<1) + 1;
                tag = (tag <<1) + reader->getBit();
                l = l ^ msbMask;
                u = u ^ msbMask;
                tag = tag ^ msbMask;
            }
        }
        inputLength--;
    }
}

int main(int N, char** args) {
    try {
        Decoder temp;
        temp.decode(args[1], args[2]);
    }
    catch (exception& e) {
        cout<<e.what()<<endl;
    }
}