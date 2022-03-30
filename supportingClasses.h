#include<fstream>
#include<string>
#include<exception>
#include<limits>

#define ull unsigned long long

class BitWriter {
    public:
        BitWriter(const std::string& outputName) {
            output = std::ofstream(outputName);
            if (!output.good()) {
                throw std::logic_error("IO exception");
            }
        }
        void writeBit(const unsigned char bit) {
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
    private:
        unsigned char buffer = 0;
        unsigned char bufferCounter = 0;
        std::ofstream output;
    ~BitWriter() {
        output.close();
    }
};

class Model {
    public:
        virtual void increaseFrequency(unsigned char symbol) = 0;
        virtual void updateProbabilities() = 0;
        virtual void rescale() = 0;
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
    private:
       ull totalLength = 256;
       ull partialSums[257] = {0};
       ull frequencies[256] = {0};
       
};