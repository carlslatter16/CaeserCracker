//every other file points to this hpp which includes the header guarding file
//inclusion of header guards in this file would cause issues as include is required for data typing of prototypes
#include "enc_headerGuard_cs.hpp"
#include <iostream>
#include <map>

//a decryption class for source file access
class Decryption
{
public:
    //my alternative to a truely global variables, class scoped
    char* inputFileName;
    char* outputFileName; 
    
    void fileInput();        
private:
    void fileOutput(std::string decryptedData);
    void decrypt(std::string input);
    void countChars(std::string input);
    void sortChars(std::map<std::string, int> charNum, std::string input);                              
    int shiftCalc(int letter, int rank);
    int strLen(std::string input);
    bool is_upper(int input);
    bool is_alpha(int input);
};