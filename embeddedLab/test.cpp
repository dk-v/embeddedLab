//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <vector>
//
////static int const sequences = 1023;
////static int const satellites = 24;
////static int const n = 10; //Registerstufen
////int table[24][2] = {
////    {2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6},
////    {6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6} };
////// table [x][y]; x - Satellite ID
////int numbers[1023] = {};
////const double upperPeak = 63; // pow(2.0, (10 + 2) / 2) - 1;
////const double lowerPeak = -65; // (-1 * pow(2.0, (10 + 2) / 2)) - 1;
//
///*Name as parameter (?)
//Meanwhile just reads stuff from console because of reasons */
//
///// <summary>
///// Reads the contents of a text file 
///// </summary>
///// <param name="fileName">The name of the file</param>
///// <returns>A string containing the text in the file</returns>
//std::string readGpsSequence(const std::string& fileName) {
//    // Attach an input stream to the wanted file
//    std::ifstream input_stream(fileName);
//
//    // Check stream status
//    if (!input_stream) std::cerr << "Can't open input file!";
//
//    // File content  
//    std::vector<std::string> content;
//    std::string line;
//    std::string result;
//
//    // Extract all the text from the input file
//    while (std::getline(input_stream, line)) {
//        // store each line in the vector
//        content.push_back(line);
//    }
//
//    // Assign the vector contents to a simple string
//    for (const auto i : content)
//        result += i;
//
//    return result;
//}
////int* stringToIntArray(std::string s) {
////    std::stringstream ss(s);
////    std::string word;
////    int i = 0;
////    while (ss >> word) {
////        numbers[i] = std::stoi(word);
////        //std::cout << word << "/n";
////        i++;
////    }
////    return numbers;
////}
//
////int getBitValueAtPosition(int bitSequence, int position) {
////    int shifter = n - position;
////    int filter = 1 << shifter;
////
////    return (bitSequence & filter) >> shifter;
////}
////int getBitValueAtPositionForSatellite(int satelliteID, int bitSequence, int positionIdentifier) {
////    int position = table[satelliteID - 1][positionIdentifier];
////    return getBitValueAtPosition(bitSequence, position);
////}
////int calcNumbersFromBelow(int satelliteID, int bitSequence) {
////    int value1 = getBitValueAtPositionForSatellite(satelliteID, bitSequence, 0);
////    int value2 = getBitValueAtPositionForSatellite(satelliteID, bitSequence, 1);
////
////    return value1 ^ value2;
////}
////int nextChipSequenceBit(int satelliteId, int above, int below) {
////    //Only take the last bit from the above sequence
////    int fromAbove = getBitValueAtPosition(above, n);
////    int fromBelow = calcNumbersFromBelow(satelliteId, below);
////
////    return fromAbove ^ fromBelow;
////}
////int generateSequence(int bitSequence, int firstBit) {
////
////    // Move the sequence one step to the right
////    bitSequence = bitSequence >> 1;
////    // Place the calculated bit at the beginning of the sequence.
////    return bitSequence | firstBit;
////}
////int moveBitForward(int firstBit) {
////    return firstBit << 9;
////}
////int generateAboveSequence(int bitSequence) {
////    int firstBit = getBitValueAtPosition(bitSequence, 3) ^ getBitValueAtPosition(bitSequence, n);
////    int firstBitMovedForward = moveBitForward(firstBit);
////
////    return generateSequence(bitSequence, firstBitMovedForward);
////}
////int generateBelowSequence(int bitSequence) {
////    int firstBit = getBitValueAtPosition(bitSequence, 2) ^ getBitValueAtPosition(bitSequence, 3) ^
////        getBitValueAtPosition(bitSequence, 6) ^ getBitValueAtPosition(bitSequence, 8) ^
////        getBitValueAtPosition(bitSequence, 9) ^ getBitValueAtPosition(bitSequence, n);
////    int firstBitMovedForward = moveBitForward(firstBit);
////    return generateSequence(bitSequence, firstBitMovedForward);
////}
////void recursiveChipSequenceCalculation(int satelliteID, int above, int below, int cycleCount, int* resultArray) {
////    if (cycleCount >= sequences) {
////        //Abort!
////    }
////    else {
////        int returnValue = nextChipSequenceBit(satelliteID, above, below);
////        int nextAbove = generateAboveSequence(above);
////        int nextBelow = generateBelowSequence(below);
////
////        resultArray[cycleCount] = returnValue;
////        recursiveChipSequenceCalculation(satelliteID, nextAbove, nextBelow, ++cycleCount, resultArray);
////    }
////}
//
///*----------------------------------WHAT IS GOING ON HERE---------------------------------------*/
////int* shiftSequenceByDelta(int* sequence, int delta) {
////    int* shiftedSequence = 0;
////    for (int i = 0; i < n; i++) {
////        shiftedSequence[i] = sequence[(i + delta) % n];
////    }
////
////    return shiftedSequence;
////}
////void generateChipSequence(int satelliteID, int* resultArray) {
////    int defaultConfigAbove = 0b1111111111;
////    int defaultConfigBelow = 0b1111111111;
////    int cycleStart = 0;
////
////    recursiveChipSequenceCalculation(satelliteID, defaultConfigAbove, defaultConfigBelow, cycleStart, resultArray);
////}
////int checkForBit(int* sequence, int* resultSequence) {
////    int scalarProduct = 0;
////
////    for (int i = 0; i < sequences; i++) {
////        int bit = sequence[i] == 1 ? 1 : -1;
////        int resultValue = resultSequence[i];
////
////        scalarProduct += bit * resultValue;
////    }
////
////    scalarProduct = scalarProduct / n;
////
////    if (scalarProduct >= upperPeak) {
////        return 1;
////    }
////    else if (scalarProduct <= lowerPeak) {
////        return 0;
////    }
////
////    return -1;
////
////}
////void decode(int* filename) {
////
////    int satelliteChipSequences[satellites][n];
////
////    for (int i = 0; i < n; i++) {
////        generateChipSequence(i + 1, satelliteChipSequences[i]);
////    }
////
////    for (int satelliteID = 0; satelliteID < n; satelliteID++) {
////        for (int delta = 0; delta < n; delta++) {
////
////            int shiftedSequence = shiftSequenceByDelta(satelliteChipSequences[satelliteID], delta);
////            int resultBit = checkForBit(shiftedSequence, sequences);
////            if (resultBit != -1) {
////                printf("Satellite %2d has sent bit %d (delta %4d)\n", satelliteID, resultBit, delta);
////            }
////        }
////    }
////}
//
//int main(int argc, char* argv[]) {
//    //decode(stringToIntArray(readFile(argv[1])));
//   std::string test = readGpsSequence(argv[1]);
//   std::cout << test;
//    //std::cout << numbers[1];
//}
