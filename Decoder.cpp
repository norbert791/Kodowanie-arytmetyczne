#include<iostream>
#include<fstream>
#include<exception>
#include<map>
#include<limits>

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
        const uc bytesToRead = 8;
        ull totalCount = 256;
        ull partialSums[alphabetSize + 1] = {0};
        map<uc,ull> frequencies{};
        void updateSums() {
            for (int i = 0; i < alphabetSize; i++) {
                partialSums[i + 1] = partialSums[i] + frequencies.at(i);
            }
            totalCount = ([] (map<uc,ull>x) {
                ull y = 0;
                for (int key = 0; key < 256; key++){
                        y+= x.at(key);
                    }
                return y;
                })(frequencies);
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

void Decoder::decode(string inputFileName, string outputFileName) {
    ifstream inputStream(inputFileName);
    ofstream outputStream(outputFileName);
    ull l = 0;
    ull u = numeric_limits<ull>::max();
    uc k = 0;
    ull tag;
    ull currentCount = 256;
    uc outputBuffer;
    uc inputBuffer;
    uc bufferCounter = 0;
    
    //preperation
    if (!inputStream.good() || !outputStream.good()) {
        throw exception();
    }

    frequencies.insert(make_pair(0,1));
    partialSums[0] = 0;
    
    for (int i = 1; i < alphabetSize + 1; i++) {
        frequencies.insert(make_pair(i,1));
        partialSums[i] = partialSums[i - 1] + 1;
    }

    ull prevl, prevu;

    //Decoding
    inputStream>>tag;
    do {
        if(bufferCounter % 8 == 0 && inputStream.peek() != EOF) {
            inputStream>>bufferCounter;
        }
        
        k = 0;

        prevl = l;
        prevu = u;
        

        while(((tag-l+1) * totalCount-1) / (u-l+1) >= partialSums[k]) {
            k++;
        }
        outputBuffer = (uc) k;
        frequencies.at(outputBuffer)++;
        outputStream<<outputBuffer;
        currentCount++;

        l = prevl + ((prevu - prevl + 1) * partialSums[outputBuffer - 1]) / totalCount;
        u = prevl + ((prevu - prevl + 1) * partialSums[outputBuffer]) / totalCount - 1;

        while (((l & msbMask) == (u & msbMask)) || ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0)) {
            if (((l & msbMask) == ( u & msbMask))) {
                l = l <<1;
                u = (u <<1) + 1;
                tag = (tag <<1) + (((uc)128) & inputBuffer);
                bufferCounter = (bufferCounter + 1) % 8;
                inputBuffer = inputBuffer << 1;
                if(bufferCounter == 0 && inputStream.peek() != EOF) {
                    inputStream>>bufferCounter;
                }
            }

            if ((l & secMsbMask) == secMsbMask && (u & secMsbMask) == 0) {
                l = l <<1;
                u = (u <<1) + 1;
                tag = (tag <<1) + (((uc)128) & inputBuffer);
                inputBuffer = inputBuffer << 1;
                bufferCounter = (bufferCounter + 1) % 8;
                if(bufferCounter == 0 && inputStream.peek() != EOF) {
                    inputStream>>bufferCounter;
                }
                l = l ^ msbMask;
                u = u ^ msbMask;
                tag = tag ^ msbMask;
            }
            
        if (currentCount % 256 == 0) {
            updateSums();
        }
        if (currentCount == numeric_limits<ull>::max() / 4) {
            updateSums();
            rescale(frequencies);
            currentCount = totalCount;
        }
        }
        
    } while(inputStream.peek() != EOF);
    outputStream.close();
    inputStream.close();
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