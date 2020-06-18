/**
 * @file enc_library_cs.cpp
 * @author Carl Slatter w18004969 
 * @date 1st April 2020
 * @version 1.0
 * @brief A Caeser Cipher decrypter class based on patterns in the english langauge
 */

#include <iostream>
#include <map>
#include <set>
#include <functional>
#include <fstream>
#include "enc_library_cs.hpp"

/**
 * @brief Calculates the length of a given string
 * 
 * @param input - The array of characters that make up the input string
 * @return The length of the input string
 */
int Decryption::strLen(std::string input)
{
    int i = 0;
    
    while(input[i] != '\0') {
        i++;
    }
    return i;
}

/**
 * @brief Takes input and returns whether it is an upper case letter
 * 
 * @param input - The letter ascii value to be checked
 * @return true if upper
 * @return false if anything else (only lower given that is_alpha is used in conjunction)
 */
bool Decryption::is_upper(int input)
{       
    //I use ascii characters a lot, they are better than litural values, as per style guide
    if(input >= 'A' && input <= 'Z') {
        return true;
    }
    return false;
}

/**
 * @brief Takes input and returns whether it is an alpha numeric letter
 * 
 * @param input - The ascii value to be checked
 * @return true - if alpha
 * @return false - if not alpha
 */
bool Decryption::is_alpha(int input)
{
    if((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
        return true;
    }
    return false;
}

/**
 * @brief Outputs decrypted data to a user specified file
 * 
 * @param decryptedData - The new decrypted string
 */
void Decryption::fileOutput(std::string decryptedData)
{
    std::ofstream outputFile(outputFileName);

    if(outputFile.is_open() == true) {
        std::cout << "Writing...";
        //write new decrypted string to the output file
        outputFile << decryptedData; 
        outputFile.close();
        std::cout << "Done!" << '\n';
    }
    else
    {
        std::cout << "Error Creating Decrypted File..." << '\n';
    }
}

/**
 * @brief Decrypts a given string using a user defined shift based off the system's best guess
 * 
 * @param input - The encrypted string to be decrypted
 */
void Decryption::decrypt(std::string input)
{
    //temporary loop variables
    int i;
    int x;

    int shift;
    //used for the start of the character set, whether upper or lower
    int asciiStart; 
    std::string output;
    //requried to loop the function if needed
    std::string restartStatus; 

    std::cout << "Please Type The Shift You Would Like to Try Decryption With?" << '\n';
    //user input
    std::cin >> shift; 

    std::cout << '\n';
    
    for (i=0; i < strLen(input); i++) {
        if(is_alpha(input[i]) == true) {
            if(is_upper(input[i]) == true) {
                asciiStart = 'A';
            } else {
                asciiStart = 'a';
            }
            //will shift and loop around using the 26 character set
            output += (((input[i] - shift) - asciiStart + 26) %26 ) + asciiStart;
        } else {
            //if a symbol or number, no shifting is required. += used for appending
            output += input[i];
        }
    }

    //shows a 1/4 preview
    for(x=0; x < (strLen(input) /4); x++) { 
        std::cout << output[x];
    }

    std::cout << ".... (1/4 Preview)"<< '\n' << '\n'; 
    std::cout << "Use A Different Shift? (Any answer other than 'y' or 'yes' will write the specified file)?" << '\n';
    std::cin >> restartStatus;

    if(restartStatus == "yes" || restartStatus == "y") {
        //rerun of function to respecify shift
        decrypt(input); 
    }

    fileOutput(output);
}

/**
 * @brief Calculates a shift based of where the given letter is, in relation to the top 5 most common letters of the alphabet in turn.
 * 
 * @param letter - The letter to base the shift calculation on
 * @param rank - The variable to hold where in the top 5 occurances we are working at
 * @return The shift guess of the given letter 
 */
int Decryption::shiftCalc(int letter, int rank)
{
    int i;
    int asciiStart;
    int shift;

    //our top 5 most used characters in english
    //both lower and upper case
    int lowerCorpusOrder[] = {'e', 't', 'a', 'o', 'i'}; 
    int upperCorpusOrder[] = {'E', 'T', 'A', 'O', 'I'}; 

    //pointer to start of lower array by default
    int requiredCorpus = *lowerCorpusOrder; 
    
    //lower character set wrapping for bounding
    int lowerBound = 'a' - 1; 
    int upperBound = 'z' + 1;

    //upper character set wrapping for bounding
    if(is_upper(letter) == true) {
        lowerBound = 'A' - 1; 
        upperBound = 'Z' + 1;
        //point to upper corpus array instead
        requiredCorpus = *upperCorpusOrder; 
    }

    //subtraction order matters based on which number is smaller, to get a positive number
    if(letter > (requiredCorpus + rank) && letter < upperBound) {
        //needed to see relation of character to our corpus order
        shift = letter - (requiredCorpus + rank); 
        //subtraction order matters based on which number is smaller, to get a positive number
    } else if (letter < (requiredCorpus + rank) && letter > lowerBound) { 
        //needed to see relation of character to our corpus order
        shift = (requiredCorpus + rank) - letter;
    } else {
        //if not a letter, used as a fallback
        shift = 0; 
    }
    
    return shift;
}

/**
 * @brief A map sorter using set logic to form a 'top 5 map' that we can use in shift calculations
 * 
 * @param charNum - The original map of all occurances
 * @param input - Given simply to pass to a later function that requires the encrypted input
 */
void Decryption::sortChars(std::map<std::string, int> charNum, std::string input)
{
    //create map of letter and occurance 
    int itr = 0;
    int shift;

    //create comparator prototype
    typedef std::function<bool(std::pair<std::string, int>, std::pair<std::string, int>)> Comparator; 

    //a comparator function to compare results and sort into descending order
    Comparator compFunctor = 
		[](std::pair<std::string, int> current ,std::pair<std::string, int> previous)
		{
			return current.second > previous.second;
		};
        
    //function call to comparator and declare set to store data in.
    std::set<std::pair<std::string, int>, Comparator> letterSet(
        //fill set with newly sorted map
		charNum.begin(), charNum.end(), compFunctor); 

    std::cout << '\n';
    std::cout << "Top Potential Encrypted Shift Keys - Not Guaranteed!" << '\n';
    std::cout << '\n';

    //prints newly sorted set out, in c++, a set is a pair
    for (std::pair<std::string, int> charNumPair : letterSet) {
        //needed to limit to top 5, all occurances are listed elsewhere
        if(itr < 5)
        {   
            //element is stored as a string, we need to parse it into an ascii interger
            int asciiValue = std::stoi(charNumPair.first);
            //a letter from parsed int first element. Useful for display
            char letter = asciiValue;

            //ascii value from first element being used as an index
            if(charNumPair.second != 0 && is_alpha(asciiValue) == true) 
            {
                std::cout << letter << " = " << charNumPair.second << " Times " << "Suggested Shift - " << shiftCalc(letter, itr) << '\n';
            }
            itr++; 
        }        
    }
    std::cout << '\n';
    decrypt(input);
}

/**
 * @brief Creates a map of letter values to occurances, that increments based on the input. This is displayed to the user
 * 
 * @param input - The input to be split apart and counted
 */
void Decryption::countChars(std::string input)
{
    int i;
    //I could have also used temporary variables and kept it short
    //in my opinion, reuse of the same variable of short scope as long as it makes more sense to the code, is better
    int asciiLetter; 

    //a map was chosen because it is a perfect value to value data structure
    std::map<std::string, int> charNum; 

    //for each upper case letter
    for(asciiLetter='A'; asciiLetter<='Z'; asciiLetter++) { 
        //create letter string from interger value - 97 to '97'
        std::string actualLetter = std::to_string(asciiLetter);
        //creates [97 : 0] map - for example
        charNum[actualLetter] = 0; 
    }

    //for each lower case letter
    for(asciiLetter='a'; asciiLetter<='z'; asciiLetter++) { 
        //create letter string from interger value - 65 to '65'
	    std::string actualLetter = std::to_string(asciiLetter);
        //creates [65 : 0] map - for example
        charNum[actualLetter] = 0; 
    }

    //for each letter of input
    for(i=0; i < strLen(input); i++) { 
        if(is_alpha(input[i]) == true) {
            //gets string value from input - string is the required type, not char or int
            std::string actualLetter = std::to_string(input[i]);
            //increments mapped value
            charNum[actualLetter] = charNum[actualLetter] + 1; 
        }
    }

    std::cout << '\n' << "Overall Character Count" << '\n' <<  '\n';

    //prints every upper case Map
    for(asciiLetter='A'; asciiLetter <= 'Z'; asciiLetter++) 
    {
        //letter manipulation for display
        std::string index = std::to_string(asciiLetter); 
        //suitible formatting
        std::cout << "[" << char(asciiLetter) << " = " << charNum[index] << "]"  << '\t'; 
    }

    std::cout << '\n' << '\n';

    //prints every lower case Map
    for(asciiLetter='a'; asciiLetter <= 'z'; asciiLetter++) 
    {
        //letter manipulation for display
        std::string index = std::to_string(asciiLetter);
        //suitible formatting
        std::cout << "[" << char(asciiLetter) << " = " << charNum[index] << "]" << '\t'; 
    }

    std::cout << '\n';
    sortChars(charNum, input);
}

/**
 * @brief Reads the input file and forms a string to be parsed later
 * 
 * @param inputFileName - The encrypted file that is passed by the first program argument
 */
void Decryption::fileInput()
{
    std::string cipherText;
    std::string line;
    //the module used for file read
    std::ifstream inputFile; 

    inputFile.open(inputFileName);

    if(inputFile.is_open() == true) {
        while (std::getline (inputFile, line))
        {
            cipherText.append(line);
        }
        countChars(cipherText);
    } else {
        std::cout << "Error Opening Encrypted File! - Please Run Again And Try A Different File Name" << '\n';
    }   
}
