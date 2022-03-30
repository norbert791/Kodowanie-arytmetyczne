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
const ull msbMask = 9223372036854775808U; //binary 1000.....
const ull secMsbMask = 4611686018427387904; //second msb mask 

class Encoder {
    public:
        void encode(string inputFileName, string outputFileName);
        virtual ~Encoder() {
            delete model;
        }
    private: 
        Model* model = new MyModel();

};

void Encoder::encode(string inputFileName, string outputFileName) {
    ifstream inputFile(inputFileName);
    uc tempInput = 0;
    ull l = 0, u = numeric_limits<ull>::max();
    ull scale3 = 0;
    ull currentCount = model->getTotalLength();
    ull inputLength = 0;
    ull prevl, prevu;
    uc bit;
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
        writer = make_unique<BitWriter>(outputFileName, inputLength);
    } catch(logic_error &e) {
        cerr<<e.what()<<endl;
        exit(1);
    }

    while (inputLength > 0) {
        inputFile>>tempInput;
        prevl = l, prevu = u;
        model->increaseFrequency(tempInput);
        l = prevl + (ull) floor((prevu*1.0 - prevl + 1) * model->getPartialSums()[tempInput] / model->getTotalLength());
        u = prevl + (ull) floor((prevu*1.0 - prevl + 1) * model->getPartialSums()[tempInput + 1] / model->getTotalLength()) - 1;
        cout<<l<<" "<<u<<" "<<endl;
        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) == ( u & msbMask))) {
                bit = (l & msbMask) == 0 ? 0 : 1;
                writer->writeBit(bit);
                l = (ull) l<<1;
                u = (ull) (u<<1) + 1;
                while (scale3 > 0) {
                    bit = bit ^ 1;
                    writer->writeBit(bit);
                    scale3--;
                }
            }
            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = (ull) (l << 1);
                u = (ull) (u << 1) + 1;
                l = l ^ msbMask;
                u = u ^ msbMask;
                scale3++;
            }
        }
        currentCount++;
        if (currentCount % 256 == 0) {
            model->updateProbabilities();
        }
        if (currentCount == numeric_limits<ull>::max() / 4) {
            model->updateProbabilities();
            model->rescale();
            currentCount = model->getTotalLength();
        }
        inputLength--;
   }
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