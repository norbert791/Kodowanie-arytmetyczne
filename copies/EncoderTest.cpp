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

#define ull unsigned long long 
#define uint unsigned int
#define uc unsigned char 

const int alphabetSize = 256;
const uint8_t msbMask =  128;//0x80000000; //binary 1000.....
const uint8_t secMsbMask = 64;//0x040000000; //second msb mask 

class Encoder {
    public:
        void encode(string inputFileName, string outputFileName);
        virtual ~Encoder() {
            delete model;
        }
    private: 
        Model* model = new TestingModel();

};

void Encoder::encode(string inputFileName, string outputFileName) {
    ifstream inputFile(inputFileName);
    uc tempInput = 0;
    uint8_t l = 0, u = numeric_limits<uint8_t>::max();
    uint8_t scale3 = 0;
    uint8_t currentCount = model->getTotalLength();
    uint8_t inputLength = 0;
    uint8_t prevl, prevu;
    bool bit;
    unique_ptr<BitWriter> writer;
    
    try {
        if (!inputFile.good()) {
            throw logic_error("Input error");
        }
        while (inputFile.peek() != EOF) {
        inputLength++;
        inputFile>>tempInput;
    }
    inputFile.seekg (0, ios::beg);
        writer = make_unique<BitWriter>(outputFileName);
    } catch(logic_error &e) {
        cerr<<e.what()<<endl;
        exit(1);
    }

    while (inputLength > 0) {
        inputFile>>tempInput;
        cout<<tempInput<<endl;
        prevl = l, prevu = u;
      //  cout<<(int)l<<" "<<(int)u<<endl;
        model->increaseFrequency(tempInput);
        l = prevl + (uint8_t)((((uint64_t) prevu - prevl + 1) * model->getPartialSums()[tempInput]) / model->getTotalLength());
        u = prevl + (uint8_t)((((uint64_t) prevu - prevl + 1) * model->getPartialSums()[tempInput + 1]) / model->getTotalLength()) - 1;
      //  cout<<(int)l<<" "<<(int)u<<endl;
        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
         //   cout<<"here"<<endl;
            if (((l & msbMask) == ( u & msbMask))) {
                bit = ((l & msbMask) == msbMask ? true : false);
                writer->writeBit(bit);
                l = (uint8_t) (l<<1);
                u = (uint8_t) (u<<1) + 1;
                while (scale3 > 0) {
                    writer->writeBit(!bit);
                    scale3--;
                }
            }
            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = (uint8_t) (l << 1);
                u = (uint8_t) (u << 1) + 1;
                l = l ^ msbMask;
                u = u ^ msbMask;
                scale3++;
            }
        //    cout<<(int)l<<" "<<(int)u<<endl;
        }
        currentCount++;
        if (currentCount % 256 == 0) {
            model->updateProbabilities();
        }
        if (currentCount == numeric_limits<uint8_t>::max() / 4) {
            model->updateProbabilities();
            model->rescale();
            currentCount = model->getTotalLength();
        }
        inputLength--;
   }
 //  cout<<(int)scale3<<endl;
   writer->writeTag(l, scale3);
   
}

int main(int N, char** args) {
    try {
        Encoder temp;
        temp.encode(args[1], args[2]);
    }
    catch (exception& e) {
        cerr<<e.what()<<endl;
    }
}