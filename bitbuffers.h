#include<fstream>
#include<vector>
#include<string>

#define uc unsigned char

using namespace std;

class BitWriter {
    public:
    BitWriter(string filename) {
        output = ofstream(filename);
        
    }
    ~BitWriter() {
        if (bufferCounter > 0) {
            while (bufferCounter < 8) {
                buffer = (uc)(buffer << 1);
                bufferCounter++;
            }
        }
        output.close();
    }
    void writeBits(vector<bool> input) {
        for (bool b : input) {
            buffer = (unsigned char)(buffer<<1);
            buffer += b ? 1 : 0;
            bufferCounter++;
          //cout<<(int)bufferCounter<<endl;
            if (bufferCounter == 8) {
                bufferCounter = 0;
                output.put(buffer);
                buffer = 0;
            }
        }
    }
    void writeChar(char symbol) {
        for (int i = 0; i < 8; i++) {
            buffer = (buffer <<1) + ((unsigned char)(symbol & 0x80) == 0x80 ? 1 : 0);
            symbol = (uc) (symbol << 1);
            bufferCounter++;

            if (bufferCounter == 8) {
                bufferCounter = 0;
                output.put(buffer);
                buffer = 0;;
            }
        }
    }
    private:
        ofstream output;
        unsigned char bufferCounter = 0;
        unsigned char buffer = 0;
};

class BitReader {

    public:
        BitReader(string filerName) {
            input = ifstream(filerName);
            buffer = input.get();
        }
        ~BitReader(){
            input.close();
        }
        bool good() {
            return !input.eof();
        }
        uc readByte() {
            uc result = 0;
            for (int i = 0; i < 8; i++) {
                bool bit = readBit();
                result = (uc) (result<<1) + (bit ? 1 : 0);
            }
            return result;
        }
        bool readBit() {
            bool result;
            if ((buffer & msbMask) == msbMask) {
                result = true;
            }
            else {
                result = false;
            }
            buffer = (uc)(buffer << 1);
            bufferCounter++;
            if (bufferCounter == 8) {
                bufferCounter = 0;
                buffer = input.get();
            }
            return result;
        }

    private:
        ifstream input;
        uc buffer = 0;
        uc bufferCounter = 0;
        const static uc msbMask = 128; //0x80
};