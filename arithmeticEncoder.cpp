#include<iostream>
#include<fstream>
#include<exception>
#include<map>
#include<limits>
#include<cmath>

using namespace std;

#define ull unsigned long long 
#define uint unsigned int
#define uc unsigned char 

const int alphabetSize = 256;
const ull msbMask = 9223372036854775808U; //binary 1000.....
const ull secMsbMask = 4611686018427387904; //second msb mask

inline void updateSums(ull* partialSums, map<uc,ull> frequencies) {
    for (int i = 0; i < alphabetSize; i++) {
        partialSums[i + 1] = partialSums[i] + frequencies.at(i);
    }
}

inline void rescale(map<uc,ull> frequencies) {
    for (int i = 0; i < alphabetSize; i++) {
        double temp = ceil(frequencies.at(i) / 2.0);
        frequencies.at(i) = (ull) temp;
    }
}


int main (int N, char** args) {
    ifstream inputFile(args[1]);
    ofstream outputFile(args[2]);
    uc tempInput = 0;
    uc outputBuffer = 0;
    uc bufferCounter = 0;
    const uc bufferSize = 8;
    ull l = 0, u = numeric_limits<ull>::max();
    ull scale3 = 0;
    ull tracker = 0;
    ull totalCount = 256;
    ull partialSums[alphabetSize + 1] = {0};
    map<uc,ull> frequencies{};
    

    if (!inputFile.good() || !outputFile.good()) {
        cerr<<"IOerror"<<endl;
        return 0;
    }

    frequencies.insert(make_pair(0,1));
    partialSums[0] = 0;
    
    for (int i = 1; i < alphabetSize + 1; i++) {
        frequencies.insert(make_pair(i,1));
        partialSums[i] = partialSums[i - 1] + 1;
    }
    try {
        ull prevl, prevu;
        uc bit;

        while (inputFile.peek() != EOF) {
            inputFile>>tempInput;
           // cout<<"here"<<endl;
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
                    }
                    l = l<<1;
                    u = (u<<1) + 1;
                    while (scale3 > 0) {
                        //cout<<"here3"<<endl;
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
                }
                if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                    l = l << 1;
                    u = (u << 1) + 1;
                    l = l ^ msbMask;
                    u = u ^ msbMask;
                    scale3++;
                }
               // cout<<"here2"<<endl;
            }
            tracker = (tracker + 1) % 256;
            if (tracker % 256 == 0) {
                totalCount += 256;
                updateSums(partialSums, frequencies);
            }
        }
        if (bufferCounter > 0) {
            outputBuffer = outputBuffer << (bufferSize - bufferCounter);
            outputFile<<outputBuffer;
        }
        outputFile.close();
        inputFile.close();
    }
    catch (exception &e) {
        cerr<<e.what()<<endl;
        return 0;
    }
    
    return 0;
}