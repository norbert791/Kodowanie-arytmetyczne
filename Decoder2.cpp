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

class Decoder {
    public:
        void decode(string inputFileName, string outputFileName);
    private:
        Model* model = new MyModel();

};

void Decoder::decode(string inputFileName, string outputFileName) {
    ofstream writer(outputFileName);
    ull l = 0, u = numeric_limits<ull>::max();
    ull inputLength = 0;
    ull prevl, prevu;
    ull tag;
    unique_ptr<BitReader> reader;
    
    try {
        if (!writer.good()) {
            throw logic_error("Input error");
        }
        reader = make_unique<BitReader>(inputFileName);
        inputLength = reader->readTag();
    } catch(logic_error &e) {
        cerr<<e.what()<<endl;
        exit(1);
    }

    while (inputLength > 0) {
        tag = reader->readTag();
        uc k = 0;
        while ((ull) floor(((tag * 1.0 - l + 1) * model->getTotalLength() - 1) / (u - l + 1)) >= model->getPartialSums()[k]) {
            k++;
        }
        writer<<(k-1);
        
        prevu = u;
        prevl = l;
        
        l = prevl + (ull)floor(((prevu * 1.0 - prevl + 1) * model->getPartialSums()[k - 1]) / model->getTotalLength());
        u = prevl + (ull)floor(((prevu * 1.0 - prevl + 1) * model->getPartialSums()[k]) / model->getTotalLength()) - 1;

        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) == ( u & msbMask))) {
                l = (ull) (l <<1);
                u = (ull) (u <<1) + 1;
                tag = (ull) (tag <<1) + reader->getBit();
            }

            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = (ull) (l <<1);
                u = (ull) (u <<1) + 1;
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