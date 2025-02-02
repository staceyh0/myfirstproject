// Stacey Huynh
// Program that detects errors in user given hamming code.
// Works with any size hamming code, but if hamming code includes
// the zero bit it can determine if there are multiple errors.
// Can only correct in cases with a single error.

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// determines the necessary number of checks for given number of bits
int howManyChecks(const std::vector<int>& v){
    int tests = ceil(log2(v.size()));
    return tests;
}

// returns true if the index given is a relevant bit for the given parity check
bool isRightIndex(const int& pBit, const int& index, const float& size){
    
    // creates a vector (correctIndexes) that contains the bit
    // locations of significant bits for each parity check
    std::vector<int> correctIndexes;
    int firstBit = pBit - 1;
    for(int i = 0; i < pBit; i++){
        correctIndexes.push_back(firstBit + i);     // *** the indexes that need to be checked
    }                                               // are in groups of "pBit size" and
                                                    // "pBit + 1" distance from each other
    for(int i = 1; i < size/2; i++){
        firstBit = correctIndexes.rbegin()[0] + (pBit+1);
        for(int i = 0; i < pBit; i++){
            correctIndexes.push_back(firstBit + i);
        }
    }
    
    // removes the value of the actual parity bit for the check
    correctIndexes.erase(correctIndexes.begin());

    std::vector<int>::iterator it;
    it = std::find(correctIndexes.begin(), correctIndexes.end(), index);
    
    if(it != correctIndexes.end()) return true;
    else return false;
}

// returns what the value of the parity bit should be
bool onesCheck(const std::vector<int>& v, const int& pBit){
    int ones = 0;
    std::vector<int> bits;
    
    for(int i = 0; i < v.size(); i++){          // if the bit is a revlevant bit
        if(isRightIndex(pBit, i ,v.size()))     // for the parity check, it gets
            bits.push_back(v[i]);               // added to vector bits
    }
    
    for(int i = 0; i < bits.size(); i++){       // counts the number of 1s in the
        if(bits[i] == 1)                        // vetcr of relevnat bits
            ones++;
    }

    if(ones%2 == 0) return 0;
    else return 1;
}

// checks if the value at the parity bit location is what it needs to be
bool parityCheck(const std::vector<int>& v, const int& pBit){
    int value = onesCheck(v, pBit);
    int actualValue = v[pBit-1];
    if(value == actualValue) return true;
    else return false;
}

// given a vector of bits, runs the necessary parity checks based on size
int runChecks(const std::vector<int>& v){
    int error = 0;
    int numChecks = howManyChecks(v);
    for(int i = 0; i < numChecks; i++){
        if(parityCheck(v, pow(2,i)) == 0)
            error += pow(2,i);
    }
    return error;
}

// flips the bit at the location of the error found
std::vector<int> errorCorrection(std::vector<int>& v, const int& error){
    int errorLocation;
    
    if(v.size() == pow(sqrt(v.size()), 2)) {    //  accounting for if the inputted
        errorLocation = error;                  //  bits included the "zero bit"
    } else {
        errorLocation = error - 1;
    }
       
    if(v[errorLocation] == 1)
        v[errorLocation] = 0;
    else if(v[errorLocation] == 0)
        v[errorLocation] = 1;
    
    return v;
}

// used if bit zero is included for an overall parity check
bool overallParityCheck(const std::vector<int>& v) {
    int ones = 0;
    for(int i = 0; i < v.size(); i++) {
        if(v[i] == 1)
            ones++;
    }
    
    if(ones%2 == 0) return true;
    else return false;
}

int main() {
    std::cout << "this program can detect and correct single bit errors in hamming codes. \nif an overall parity bit is present, it can detect if there is more than one error, but cannot locate or correct it.\nit will assume there is no bit for overall parity, unless the size of the code is 2^n" << std::endl;
    
    std::cout << "please enter bits: ";
    
    std::string code;
    std::vector<int> bits;
    std::getline(std::cin, code);
    
    for(int i = 0; i < code.length(); i++){
        int num = code.at(i) - '0';
        bits.push_back(num);
    }
    
    // case where code has a zero bit for overall parity
    if(pow(sqrt(bits.size()), 2) == bits.size()) {
        bool overallParity = overallParityCheck(bits);
        bits.erase(bits.begin());
        int error = runChecks(bits);
        
        if (overallParity == false && error > 0) {
            std::cout << "one error has been detected." << std::endl;
            std::cout << "corrupted bit found at index " << error << std::endl;
            bits = errorCorrection(bits, error);
            std::cout << "corrected bits: ";
            for(int i = 0; i < bits.size(); i++)
                std::cout << bits[i];
            std::cout << std::endl;
        }
         else if(overallParity == true && error > 0) {
            std::cout << "more than one error has been detected. can only correct single bit errors." << std::endl;
        }
         else {
             std::cout << "no error has been detected." << std::endl;
         }
    
    }
    // case where zero bit is not used
    else {
        int error = runChecks(bits);
        if(error > 0 && error <= bits.size()){
            std::cout << "corrupted bit found at index " << error << std::endl;
            bits = errorCorrection(bits, error);
            std::cout << "corrected bits: ";
            for(int i = 0; i < bits.size(); i++)
                std::cout << bits[i];
            std::cout << std::endl;
        } else if(error > 0 && error > bits.size()) {
            std::cout << "there seems to be more than one error found, but cannot be certain since there is no overall parity bit." << std::endl;
        } else{
            std::cout << "no error has been detected." << std:: endl;
        }
    }
}
        
