#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


int howManyChecks(std::vector<int> v){
    int tests = ceil(log2(v.size()));
    return tests;
}


bool isRightIndex(int pBit, int index, float size){
    std::vector<int> correctIndexes;
    
    int firstBit = pBit - 1;
    for(int i = 0; i < pBit; i++){
        correctIndexes.push_back(firstBit + i);
    }
    
    for(int i = 1; i < size/2; i++){
        firstBit = correctIndexes.rbegin()[0] + (pBit+1);
        for(int i = 0; i < pBit; i++){
            correctIndexes.push_back(firstBit + i);
        }
    }
    
    correctIndexes.erase(correctIndexes.begin());

    std::vector<int>::iterator it;
    it = std::find(correctIndexes.begin(), correctIndexes.end(), index);
    
    if(it != correctIndexes.end()) return true;
    else return false;
}


bool onesCheck(std::vector<int> v, int pBit){
    int ones = 0;
    std::vector<int> bits;

    for(int i = 0; i < v.size(); i++){
        if(isRightIndex(pBit, i ,v.size()))
            bits.push_back(v[i]);
    }
    
    for(int i = 0; i < bits.size(); i++){
        if(bits[i] == 1)
            ones++;
    }

    if(ones%2 == 0) return 0;
    else return 1;
}


bool parityCheck(std::vector<int> v, int pBit){
    int value = onesCheck(v, pBit);
    int actualValue = v[pBit-1];
    if(value == actualValue) return true;
    else return false;
}


int runChecks(std::vector<int> v){
    int error = 0;
    int numChecks = howManyChecks(v);
    for(int i = 0; i < numChecks; i++){
        if(parityCheck(v, pow(2,i)) == 0)
            error += pow(2,i);
    }
    return error;
}


std::vector<int> errorCorrection(std::vector<int> v, int error){
    int errorLocation = error - 1;
    
    if(v[errorLocation] == 1)
        v[errorLocation] = 0;
    else if(v[errorLocation] == 0)
        v[errorLocation] = 1;
    
    return v;
}


int main() {
    
    std::cout << "enter bits: ";
    std::string input;
    std::vector<int> vector;
    std::getline(std::cin, input);
    
    for(int i = 0; i < input.length(); i++){
        int num = input.at(i) - '0';
        vector.push_back(num);
    }
    
    if(pow(floor(sqrt(vector.size())), 2) == 0)
        vector.erase(vector.begin());
    
    int error = runChecks(vector);
    
    if(error > 0){
        std::cout << "error found at bit " << error << std::endl;
        vector = errorCorrection(vector, error);
        std::cout << "corrected bits: " << std::endl;
        for(int i = 0; i < vector.size(); i++)
            std::cout << vector[i];
        std::cout << std::endl;
    }else{
        std::cout << "no error found" << std:: endl;
    }
    
    return 0;
}
        
