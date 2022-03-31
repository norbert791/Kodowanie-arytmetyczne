#include<fstream>
#include<string>
#include<exception>
#include<limits>

#define ull unsigned long long
#define uc unsigned char

class BitWriter {
    public:
        BitWriter(const std::string& outputName, ull fileLength) {
            output = std::ofstream(outputName);
            if (!output.good()) {
                throw std::logic_error("IO exception");
            }
            output<<fileLength;
        }
        BitWriter(const std::string& outputName) {
            output = std::ofstream(outputName);
            if (!output.good()) {
                throw std::logic_error("IO exception");
            }
        }
        void writeBit(const unsigned char bit) {
            std::cout<<(int)bit<<std::endl;
            if (bit == 1) {
                buffer = (unsigned char)(buffer<<1) + 1;
                bufferCounter++;
            }
            else if (bit == 0) {
                buffer = (unsigned char)(buffer<<1);
                bufferCounter++;
            }
            else {
                throw std::invalid_argument("Argument must be either 1 or 0");
            }
            if (bufferCounter == 8) {
                bufferCounter = 0;
                output<<buffer;
                buffer = 0;
            }
        }
        ~BitWriter() {
            output.close();
        }   
    private:
        unsigned char buffer = 0;
        unsigned char bufferCounter = 0;
        std::ofstream output;
};

class BitReader {
    public:
        BitReader(const std::string& inputName) {
            input = std::ifstream(inputName);
            if (!input.good()) {
                throw std::logic_error("IO exception");
            }
        }
        uc getBit () {
            if ( (msbMask & buffer) == msbMask) {
                buffer = (uc) (buffer <<1);
                return 1;
            }
            else {
                buffer = (uc) (buffer <<1);
                return 0;
            }
            bufferCounter++;
            if (bufferCounter == 8) {
                bufferCounter = 0;
                input>>buffer;
            }
        }
        ull readTag() {
            ull temp;
            input>>temp;
            return temp;
        }
    private:
        uc buffer;
        uc bufferCounter = 0;
        const static uc msbMask = 128; 
        std::ifstream input;
};

class Model {
    public:
        virtual void increaseFrequency(unsigned char symbol) = 0;
        virtual void updateProbabilities() = 0;
        virtual void rescale() = 0;
        virtual ull* getPartialSums() = 0;
        virtual ull getTotalLength() = 0;
        virtual ~Model() = default;
};

class MyModel : public Model {
    public:
        MyModel() {
            for (int i = 0; i < 256; i++) {
            frequencies[i] = 1;
            partialSums[i + 1] = i + 1;
            }
        }

        void increaseFrequency(unsigned char symbol) {
            frequencies[symbol]++;
        }

        void updateProbabilities() {
            for (int i = 1; i < 257; i++) {
                partialSums[i] = partialSums[i - 1] + frequencies[i - 1];
            }
        }

        void rescale() {
            totalLength = 0;
            for (int i = 0; i < 256; i++) {
                frequencies[i] = frequencies[i] / 2 > 0 ? frequencies[i] / 2 : 1;
                totalLength += frequencies[i];
            }
        }

        ull* getPartialSums() {
            return partialSums;
        }

        ull getTotalLength() {
            return totalLength;
        }
    private:
       ull totalLength = 256;
       ull partialSums[257] = {0};
       ull frequencies[256] = {0};
       
};