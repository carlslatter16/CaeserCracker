//my header file containing the Decryption class  
#include "enc_library.hpp" 

int main(int argc, char *argv[]) 
{   
    //named class object
    Decryption caeser;
    //access the object methods  
    caeser.inputFileName = argv[1];
    caeser.outputFileName = argv[2];

    caeser.fileInput(); 
}
