#include<fstream>
#include<string>
#include<exception>
#include<limits>

#define uc unsigned char


class BitWriter {
    public:
        [[deprecated]]
        BitWriter(const std::string& outputName, uint32_t fileLength) {
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
        void writeTag(uint32_t tag, uint32_t scale3) {
            bool bit = ((tag & msbMask) == msbMask);
            writeBit(bit);
            while(scale3 > 0) {
                writeBit(!bit);
                scale3--;
            }
            for (int i = 0; i < 31; i++) {
                tag =  (uint32_t)(tag<<1);
                bit = ((tag & msbMask) == msbMask);
                writeBit(bit);
            }
        }
        void writeBit(const bool input) {
      //      std::cout<<"bool: "<<input<<std::endl;
            buffer = (unsigned char)(buffer<<1);
            buffer += input ? 1 : 0;
       //     std::cout<<"buffer "<<(int)buffer<<std::endl;
            bufferCounter++;
            if (bufferCounter == 8) {
                bufferCounter = 0;
                output.put(buffer);
                buffer = 0;
            }
        }
        void writeLength(int32_t input) {
            
            for (int i = 0; i < 32; i++) {
                bool bit = ( (msbMask & input) ==  msbMask);
                writeBit(bit);
                input = (input << 1);
            }
        }
        ~BitWriter() {
            if (bufferCounter > 0) {
            while (bufferCounter < 8) {
                buffer = (uc)(buffer << 1);
                bufferCounter++;
            }
            output.put(buffer);
        }
        output.close();
        }
    private:
        unsigned char buffer = 0;
        unsigned char bufferCounter = 0;
        const static uint32_t msbMask = 0x80000000;//0x80000000;
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
        ~BitReader() {
            input.close();
        }
        bool good() {
            return !input.eof();
        }
        bool getBit() {
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
      //      std::cout<<result<<std::endl;
            return result;
        }
        uint32_t readTag() {
            uint32_t result = 0;
            for (int i = 0; i < 4; i++) {
                buffer = input.get();
                result = result << 8;
                result += buffer;
            }
            return result;
        }
        uint32_t readLength() {
            uint32_t result = 0;
            for (int i = 0; i < 4; i++) {
                buffer = input.get();
                result = result << 8;
                result += buffer;
            }
            return result;
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
        virtual uint32_t* getPartialSums() = 0;
        virtual uint32_t getTotalLength() = 0;
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

        void increaseFrequency(unsigned char symbol) override{
            frequencies[symbol]++;
        }

        void updateProbabilities() override{
            for (int i = 1; i < 257; i++) {
                partialSums[i] = partialSums[i - 1] + frequencies[i - 1];
            }
        }

        void rescale() override{
            totalLength = 0;
            for (int i = 0; i < 256; i++) {
                frequencies[i] = frequencies[i] / 2 > 0 ? frequencies[i] / 2 : 1;
                totalLength += frequencies[i];
            }
        }

        uint32_t* getPartialSums() override{
            return partialSums;
        }

        uint32_t getTotalLength() override{
            return totalLength;
        }
    protected:
       uint32_t totalLength = 256;
       uint32_t partialSums[257] = {0};
       uint32_t frequencies[256] = {0};
       
};

class TestingModel : public MyModel {
    public:
    TestingModel() : MyModel(){
        for (int i = 0; i < 257; i++) {
            partialSums[i] = 0;
        }
        partialSums[1 + 'a'] = 40;
        partialSums[1 + 'b'] = 41;
        partialSums[1 + 'c'] = 50;
        totalLength = 50;
    }
};