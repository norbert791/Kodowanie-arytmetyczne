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
const uint8_t msbMask = 128; //binary 1000.....
const uint8_t secMsbMask = 64; //second msb mask 

class Decoder {
    public:
        void decode(string inputFileName, string outputFileName);
    private:
        Model* model = new TestingModel();

};

void Decoder::decode(string inputFileName, string outputFileName) {
    ofstream writer(outputFileName);
    uint8_t l = 0, u = numeric_limits<uint8_t>::max();
    uint8_t inputLength = 0;
    uint8_t prevl, prevu;
    uint8_t tag;
    unique_ptr<BitReader> reader;
    
    try {
        if (!writer.good()) {
            throw logic_error("Input error");
        }
        reader = make_unique<BitReader>(inputFileName);
       // inputLength = reader->readTag();
       inputLength = 4;
    } catch(logic_error &e) {
        cerr<<e.what()<<endl;
        exit(1);
    }
    tag = reader->readTag();
    while (inputLength > 0) {
        
        uc k = 0;
        cout<<"tag: "<<(int)tag<<endl;
        while ((uint8_t)((((uint64_t) tag - l + 1) *  model->getTotalLength() - 1) / ((uint64_t)u - l + 1)) >= model->getPartialSums()[k]) {
            k++;
        }
        cout<<"while: "<<(int)(uint8_t)((((uint64_t) tag - l + 1) *  model->getTotalLength() - 1) / ((uint64_t)u - l + 1))<<endl;
        k -= 1;
        writer<<(k);
        prevu = u;
        prevl = l;

        l = prevl + (uint8_t)((((uint64_t)prevu - prevl + 1) * model->getPartialSums()[k]) / model->getTotalLength());
        u = prevl + (uint8_t)((((uint64_t)prevu - prevl + 1) * model->getPartialSums()[k + 1]) / model->getTotalLength()) - 1;
        cout<<(int)l<<" "<<(int)u<<endl;
        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) == ( u & msbMask))) {
                l = (uint8_t) (l <<1);
                u = (uint8_t) (u <<1) + 1;
                tag = (uint8_t) (tag <<1) + reader->getBit();
            }

            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = (uint8_t) (l <<1);
                u = (uint8_t) (u <<1) + 1;
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