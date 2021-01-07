#include <iostream>
#include <string.h>

/*
 * Morse code conversion program
 *
 * John Chajecki
 * version 0.01.01
 * 07/01/2021
 *
 */


const uint8_t alphaArrayLength = 26;
const uint8_t digitArrayLength = 10;
const uint8_t punctuationArrayLength = 18;
const uint8_t prosignArrayLength = 4;

const uint8_t morsech[alphaArrayLength] = {0x02,0x01,0x05,0x01,0x00,0x04,0x03,0x00,0x00,0x0E,
                                           0x05,0x02,0x03,0x01,0x07,0x06,0x0B,0x02,0x00,0x01,
                                           0x04,0x08,0x06,0x09,0x0D,0x03};
const uint8_t morsecm[alphaArrayLength] = {0x02,0x04,0x04,0x03,0x01,0x04,0x03,0x04,0x02,0x04,
                                           0x03,0x04,0x02,0x02,0x03,0x04,0x04,0x03,0x03,0x01,
                                           0x03,0x04,0x03,0x04,0x04,0x04};
const uint8_t morsenu[digitArrayLength] = {0x1F,0x1E,0x1C,0x18,0x10,0x00,0x01,0x03,0x07,0x0F};
const uint8_t morsenm[digitArrayLength] = {0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05,0x05};
const uint8_t morsepu[punctuationArrayLength] = {0x35,0x12,0x48,0x02,0x1E,0x0D,0x2D,0x0A,0x33,0x21,
                                                 0x2A,0x09,0x07,0x15,0x11,0x0C,0x16,0x2C};
const uint8_t morsepm[punctuationArrayLength] = {0x06,0x06,0x07,0x05,0x06,0x05,0x06,0x05,0x06,0x06,
                                                 0x06,0x05,0x06,0x06,0x05,0x06,0x06,0x06};
const uint8_t asciipc[punctuationArrayLength] = {0x21,0x22,0x24,0x26,0x27,0x28,0x29,0x2B,0x2C,0x2D,
                                                 0x2E,0x2F,0x3A,0x3B,0x3D,0x3F,0x40,0x5F};

//const uint8_t morsepr[prosignArrayLength] = {0x28,0x00,0x15,0x0A,0x08};
//const uint8_t morserm[prosignArrayLength] = {0x06,0x08,0x05,0x05,0x05};
const uint8_t morsepr[prosignArrayLength] = {0x28,0x00,0x15,0x08};
const uint8_t morserm[prosignArrayLength] = {0x06,0x08,0x05,0x05};
const char morseProsigns[prosignArrayLength][7] = {
    {'E','N','D','\0'},
    {'E','R','R','\0'},
    {'S','T','A','R','T','\0'},
//    {'P','A','G','E','\0'},
    {'O','K','\0'}
};


static char endWord = ' ';
static char endLetter = '/';
static char * convertData = nullptr;
static uint8_t convertMode = 0;
static char * inFile = nullptr;
static char * outFile = nullptr;
static FILE * outFilePtr;


void getArgs(int argc, char *argv[]){
    char arg = 0x00;
    char * argdata = nullptr;
    size_t arglen = 0;
    bool err = false;

    if (argc>1){
        for (int i=1; i<argc; i++){
            arglen = strlen(argv[i]);
            // If we have an argument staring '-'...
            if ((argv[i][0] == '-') && (arglen > 1)) {
                // Extract the command letter
                arg = argv[i][1];
                // If argument contains more data then treat it as a parameter
                if (arglen > 2) {
                    argdata = &argv[i][2];
                }else{
                    // Get the parameter from the next argument (if one is available)
                    if (i<argc-1) {
                        argdata = argv[i+1];
                        i++;    // Bypass the next arg
                    }
                }
            }
            // Once we have an argument an parameter then process
            if (arg){
                switch(arg) {
                    case 'd' :  // Decode morse string to text
                        convertMode = 1;
                        if (argdata != nullptr) convertData = argdata;
                        break;
                    // Encode text string
                    case 'e' :  // Encode text string to morse
                        convertMode = 2;
                        if (argdata != nullptr) convertData = argdata;
                        break;
                    case 't' :  // Read and convert a file containing morse to text
                        convertMode = 1;
                        if (argdata != nullptr) inFile = argdata;
                        break;
                    case 'm' :  // Read and convert a file containing text to morse
                        convertMode = 2;
                        if (argdata != nullptr) inFile = argdata;
                        break;
                    case 'o' :  // Output results to a file
                        if (argdata != nullptr) outFile = argdata;
                        break;
                    case 's' :  // Use custom separators
                        if (argdata != nullptr) {
                            endLetter = argdata[0];
                            endWord = argdata[1];
                        }
                    break;
                    default:
                        err = true;
                }
            }
            // Unknown command
            if (err) std::cout << "-" << arg << " not recognized!" << std::endl;
            // Clear arg and parameter pointer ready for next one
            arg = 0x00;
            if (argdata != nullptr) argdata = nullptr;
        }
    }

/*
    if (convertData != nullptr) std::cout << "Data: " << convertData << std::endl;
    if (inFile != nullptr) std::cout << "Input file:  " << inFile << std::endl;
    if (outFile != nullptr) std::cout << "Output file: " << outFile << std::endl;
*/
}


void writeOutputChar(char c) {
    if (outFilePtr == nullptr) {
        std::cout << c;
    }else{
        putc(c, outFilePtr);
    }
}


void morseOutNum(int num){
    int idx = num - 48;
    for (int i=0; i<morsenm[idx]; i++){
        if (morsenu[idx] & (1<<i)){
            writeOutputChar('-');
        }else{
            writeOutputChar('.');
        }
    }
}


void morseOutLet(int num){
    int idx = 255;
    // Uppdercase
    if (num>96 && num<123) idx = num - 97;
    // Lowercase
    if (num>64 && num<91) idx = num - 65;
    // If idx non zero (i.e letter identified)
    if (idx<127) {
        for (int i=0; i<morsecm[idx]; i++){
            if (morsech[idx] & (1<<i)){
                writeOutputChar('-');
            }else{
                writeOutputChar('.');
            }
        }
    }
}


void morseOutPunct(int num){
    int idx = 0;
    for (idx=0; idx<punctuationArrayLength; idx++){
        if (asciipc[idx]==num){ // Found a match
            for (int i=0; i<morsepm[idx]; i++){
                if (morsepu[idx] & (1<<i)){
                    writeOutputChar('-');
                }else{
                    writeOutputChar('.');
                }
            }
            break;
        }
    }
}


void asciiToMorse (int c){
    if (c>47 && c<58){
        // Morse numaric charaters
        morseOutNum(c);
    }else if ((c>64 && c<91)||(c>96 && c<123)){
        // Morse alphabetic characters
        morseOutLet(c);
    }else if ((c>32 && c<48)||(c>57 && c<65)||c==95){
        // Morse punctuation
        morseOutPunct(c);
    }
}


void asciiStrToMorse(const char *asciistr){
    size_t chlen = strlen(asciistr);
    size_t idx = 0;

    for (idx=0; idx<chlen; idx++) {
        // After first character and if current or previous charater was not a space
        // print a letter terminator
        if ((idx>0) && (asciistr[idx]>0x20) && (asciistr[idx-1]>0x20)) {
            writeOutputChar(endLetter);
        }
        // For a space, print word terminator
        if (asciistr[idx]==0x20){
            writeOutputChar(endWord);
        }
        // Encode other characters
        asciiToMorse(int(asciistr[idx]));
    }
}


/***** Return a binary value for a morse character *****/
//uint8_t morseToCode(char *morsechar, uint8_t csize){
uint8_t morseToCode(int morsechar[8], uint8_t csize){
    uint8_t idx = 0;
    uint8_t morsecode = 0;
    if (csize<8) {
        for (idx=0; idx<csize; idx++) {
            if (morsechar[idx] == 0x2D) morsecode |= 1 << idx;
        }
        return morsecode;
    }else{
        return 0x00;
    }
}


/***** Output an ASCII character for a morse symbol *****/
//void asciiOutMorse(char *morsechar, uint8_t csize){
void asciiOutMorse(int morsechar[8], uint8_t csize){
    uint8_t morsecharvalue = morseToCode(morsechar, csize);
    uint8_t idx = 0;
    bool found = false;

    // Is it a letter?
    for (idx=0; idx<alphaArrayLength; idx++){
        if (morsecharvalue == morsech[idx]){
            if (csize == morsecm[idx]) {
                writeOutputChar(static_cast<char>(idx+65));
                found = true;
            }
        }
    }

    // Is it a number?
    if (!found) {
        for (idx=0; idx<digitArrayLength; idx++){
            if (morsecharvalue == morsenu[idx]){
                if (csize == morsenm[idx]) {
                    writeOutputChar(static_cast<char>(idx+48));
                    found = true;
                }
            }
        }
    }


    // Is it a punctuation character?
    if (!found) {
        for (idx=0; idx<punctuationArrayLength; idx++){
            if (morsecharvalue == morsepu[idx]){
                if (csize == morsepm[idx]) {
                    writeOutputChar(static_cast<char>(asciipc[idx]));
                    found = true;
                }
            }
        }

    }

    // Is it a prosign?
    if (!found) {
        for (idx=0; idx<prosignArrayLength; idx++){
            if (morsecharvalue == morsepr[idx]){
                if (csize == morserm[idx]) {
                    writeOutputChar('[');
                    for (uint8_t i=0; i<strlen(morseProsigns[idx]); i++){
                        writeOutputChar(morseProsigns[idx][i]);
                    }
                    writeOutputChar(']');
                    found = true;
                }
            }
        }
    }
}


void morseStrToAscii(char *morsestr){
    size_t chlen = strlen(morsestr);
    size_t idx = 0;
    char c;
    int morsechar[8] = {};
    uint8_t mwidx = 0;

    std::fill_n(morsechar, 8, 0);

    while (idx<chlen) {
        c = morsestr[idx];
        if (c == endLetter) {
            asciiOutMorse(morsechar, mwidx);
            std::fill_n(morsechar, 8, 0);
            mwidx = 0;
        }else if (c == endWord) {
            asciiOutMorse(morsechar, mwidx);
            std::cout << ' ';
            std::fill_n(morsechar, 8, 0);
            mwidx = 0;
        }else{
            if (mwidx<8) {
                morsechar[mwidx] = c;
                mwidx++;
            }else{
                // Shouldn't get here!
                std::cout << "\nerror!" << std::endl;
                // Exit loop
                idx = chlen;
            }
        }
        idx++;
    }
    // Last character
    asciiOutMorse(morsechar, mwidx);
//    std::cout << "\nDone morseToAscii.";
}


void asciiFileToMorse(char *filename) {
    FILE * fileptr;
    int currentChar = 0x00;
    int lastChar = 0x00;

    fileptr = fopen(filename, "r");
    if (fileptr == nullptr){
        std::cout << "Text file not found!" << std::endl;
    }else{
        do {
            currentChar = fgetc(fileptr);
            // Don't put a letter delimeter before or after a space
            if ((currentChar>0x20) && (lastChar>0x20)) {
                std::cout << endLetter;
            }
            // A space or LF is a word delimeter
            if ((currentChar == 0x20) || (currentChar == 0x0A)) {
                std::cout << endWord;
            }else{
                asciiToMorse(currentChar);
            }
            lastChar = currentChar;
        } while (currentChar != EOF);
    }
}


void morseFileToAscii(char *filename) {
    FILE * fileptr;
    int c = 0x00;
    int morsechar[8] = {0};
    uint8_t mwidx = 0;

    fileptr = fopen(filename, "r");
    if (fileptr == nullptr){
        std::cout << "Morse code file not found!" << std::endl;
    }else{
        while (c != EOF) {
            c = fgetc(fileptr);
            if (c == endLetter) {
                asciiOutMorse(morsechar, mwidx);
                std::fill_n(morsechar, 8, 0);
                mwidx = 0;
            }else if ((c == endWord) || (c == 0x0A) || (c == 0x0D)) {
                asciiOutMorse(morsechar, mwidx);
                std::cout << ' ';
                std::fill_n(morsechar, 8, 0);
                mwidx = 0;
            }else{
                if (mwidx<8) {
                    morsechar[mwidx] = c;
                    mwidx++;
                }else{
                    // Shouldn't get here!
                    std::cout << "\nCharacter read error!" << std::endl;
                    // Exit loop
                    break;
                }
            }
        }
        fclose(fileptr);
    }
}


int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);

    getArgs(argc, argv);

    if ((convertData==nullptr)&&(inFile==nullptr)) {
        std::cout << "Nothing to convert!" << std::endl;
    }else{
        if (outFile != nullptr) {
            outFilePtr = fopen(outFile, "w");
            if (outFilePtr == nullptr) {
                std::cout << "Error opening output file!" << std::endl;
                exit(-1);
            }
        }
        switch (convertMode) {
            case 0 :
                std::cout << "Convert mode [-d|-e] not specified!" << std::endl;
                break;
            case 1 :
//                std::cout << "\nDecoding....." << std::endl;
                if (inFile == nullptr){
                    morseStrToAscii(convertData);
                }else{
                    morseFileToAscii(inFile);
                }
                break;
            case 2 :
//                std::cout << "\nEncoding....." << std::endl;
                if (inFile == nullptr){
                    asciiStrToMorse(convertData);
                }else{
                    asciiFileToMorse(inFile);
                }
                break;
            default :
                std::cout << "Default." << std::endl;
        }
        if (outFilePtr != nullptr) fclose(outFilePtr);
//        std::cout << "\nDone." << std::endl;
        std::cout << std::endl;
    }
}

