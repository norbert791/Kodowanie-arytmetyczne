#include<iostream>
#include<string>
#include<limits>
#include<fstream>
#include<map>
#include<exception>
#include<cmath>

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
        
    private:
        ull totalCount = 256;
        ull partialSums[alphabetSize + 1] = {0};
        ull finalLength = 0;
        map<uc,ull> frequencies{};
        void updateSums() {
            for (int i = 0; i < alphabetSize; i++) {
                partialSums[i + 1] = partialSums[i] + frequencies.at(i);
            }
            totalCount = ([] (map<uc,ull>x) {
                ull y = 0;
                for (int key = 0; key < 256; key++){
                        y += x.at(key);
                    }
                return y;
                })(frequencies);
            cout<<"updateSums"<<endl;
        }
        void rescale(map<uc,ull> frequencies) {
            for (int i = 0; i < alphabetSize; i++) {
                frequencies.at(i) = frequencies.at(i) / 2 > 0 ? frequencies.at(i) / 2 : 1;
            }
            totalCount = ([] (map<uc,ull>x) {
                ull y = 0;
                for (int key = 0; key < 256; key++){
                        y+= x.at(key);
                    }
                return y;
                })(frequencies);
        }

};

void Encoder::encode(string inputFileName, string outputFileName) {
    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);
    uc tempInput = 0;
    uc outputBuffer = 0;
    uc bufferCounter = 0;
    const uc bufferSize = 8;
    ull l = 0, u = numeric_limits<ull>::max();
    ull scale3 = 0;
    ull currentCount = totalCount;
    

    if (!inputFile.good() || !outputFile.good()) {
        throw exception();
    }

    frequencies.insert(make_pair(0,1));
    partialSums[0] = 0;
    
    for (int i = 1; i < alphabetSize + 1; i++) {
        frequencies.insert(make_pair(i,1));
        partialSums[i] = partialSums[i - 1] + 1;
    }
    ull prevl, prevu;
    uc bit;

    while (inputFile.peek() != EOF) {
        inputFile>>tempInput;
        cout<<"here1 "<<currentCount<<endl;
        prevl = l, prevu = u;
        frequencies.at(tempInput)++;
        l = prevl + (prevu - prevl + 1) * partialSums[tempInput] / totalCount;
        u = prevu + (prevu - prevl + 1) * partialSums[tempInput + 1] / totalCount - 1;
        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) ==( u & msbMask))) {
                bit = (l & msbMask) == 0 ? 0 : 1;
                outputBuffer = outputBuffer << 1;
                outputBuffer += 1;
                bufferCounter = (bufferCounter + 1 ) % bufferSize;
                if (bufferCounter == 0) {
                    outputFile<<outputBuffer;
                    outputBuffer = 0;
                    cout<<"here4"<<endl;
                }
                l = l<<1;
                u = (u<<1) + 1;
                while (scale3 > 0) {
                    cout<<"here3 "<<currentCount<<endl;
                    bit = bit ^ 1;
                    outputBuffer = outputBuffer << 1;
                    outputBuffer += 1;
                    bufferCounter = (bufferCounter + 1 ) % bufferSize;
                    if (bufferCounter == 0) {
                        outputFile<<outputBuffer;
                        outputBuffer = 0;
                    }
                    scale3--;
                }
                cout<<"here6"<<endl;
            }
            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = l << 1;
                u = (u << 1) + 1;
                l = l ^ msbMask;
                u = u ^ msbMask;
                scale3++;
            }
           cout<<"here2 "<<currentCount<<endl;
           cout<<l<<" "<<u<<" "<<endl;
        }
        cout<<"here 7"<<endl;
        currentCount++;
        if (currentCount % 256 == 0) {
            updateSums();
        }
        cout<<"here 8"<<endl;
        if (currentCount == numeric_limits<ull>::max() / 4) {
            updateSums();
            rescale(this->frequencies);
        
        }
    }
    if (bufferCounter > 0) {
        outputBuffer = outputBuffer << (bufferSize - bufferCounter);
        outputFile<<outputBuffer;
    }
    outputFile.close();
    inputFile.close();
    
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