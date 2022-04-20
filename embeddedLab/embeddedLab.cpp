#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

using namespace std;

/// <summary>
/// Reads the contents of a text file 
/// </summary>
/// <param name="fileName">The name of the file</param>
/// <returns>A vector of integers containing the integer representation of the text </returns>
std::vector<int> readGpsSequence(const std::string& fileName) {
    
    // Attach an input stream to the wanted file
    std::ifstream input_stream(fileName);

    // Check stream status
    if (!input_stream) std::cerr << "Can't open input file!";

    // File content  
    std::string content;
    std::string line;
    std::vector<int> result;

    // Extract all the text from the input file
    while (std::getline(input_stream, line)) {
        // Assign the text to a string
        content += line;
    }

    // Convert the string into a vector of integers 
    std::stringstream stream(content);
    int num;

    while (stream >> num)
        result.push_back(num);

    return result;
}

void shiftBitsByOne(int bits[10]) {
    int temp = bits[9];
    for (int i = 9; i > 0; i--) {
        bits[i] = bits[i - 1];
    }
    bits[0] = temp;
}

void createGoldCodeForSat(int goldCode[1023], int x, int y) {
    int sequence1[10] = { 1,1,1,1,1,1,1,1,1,1 };
    int sequence2[10] = { 1,1,1,1,1,1,1,1,1,1 };

    int index = 0;
    while (index < 1023) {
        int temp2 = sequence2[x - 1] ^ sequence2[y - 1];
        int new2 = sequence2[9] ^ sequence2[8] ^ sequence2[7] ^ sequence2[5] ^ sequence2[2] ^ sequence2[1];
        int new1 = sequence1[9] ^ sequence1[2];

        goldCode[index] = (temp2 ^ sequence1[9]) == 0 ? -1 : 1;
        index++;

        shiftBitsByOne(sequence1);
        shiftBitsByOne(sequence2);
        sequence2[0] = new2;
        sequence1[0] = new1;
    }
}

void createGoldCodes(int goldCodes[24][1023]) {
    createGoldCodeForSat(goldCodes[0], 2, 6);
    createGoldCodeForSat(goldCodes[1], 3, 7);
    createGoldCodeForSat(goldCodes[2], 4, 8);
    createGoldCodeForSat(goldCodes[3], 5, 9);
    createGoldCodeForSat(goldCodes[4], 1, 9);
    createGoldCodeForSat(goldCodes[5], 2, 10);
    createGoldCodeForSat(goldCodes[6], 1, 8);
    createGoldCodeForSat(goldCodes[7], 2, 9);
    createGoldCodeForSat(goldCodes[8], 3, 10);
    createGoldCodeForSat(goldCodes[9], 2, 3);
    createGoldCodeForSat(goldCodes[10], 3, 4);
    createGoldCodeForSat(goldCodes[11], 5, 6);
    createGoldCodeForSat(goldCodes[12], 6, 7);
    createGoldCodeForSat(goldCodes[13], 7, 8);
    createGoldCodeForSat(goldCodes[14], 8, 9);
    createGoldCodeForSat(goldCodes[15], 9, 10);
    createGoldCodeForSat(goldCodes[16], 1, 4);
    createGoldCodeForSat(goldCodes[17], 2, 5);
    createGoldCodeForSat(goldCodes[18], 3, 6);
    createGoldCodeForSat(goldCodes[19], 4, 7);
    createGoldCodeForSat(goldCodes[20], 5, 8);
    createGoldCodeForSat(goldCodes[21], 6, 9);
    createGoldCodeForSat(goldCodes[22], 1, 3);
    createGoldCodeForSat(goldCodes[23], 4, 6);
}

int calculateScalar(vector<int> signal, int goldCode[1023], int delta) {
    int scalar = 0;
    for (int i = 0; i < 1023; i++) {
        scalar += signal[i] * goldCode[(i + delta) % 1023];
    }
    return scalar;
}

void interpretSignal(vector<int> signal, int goldCodes[24][1023]) {
    // calculated crosscorrelation values with formular for even register length
    // Peak = Rauschwert eines anderen Satelliten
    // deshalb 3 mal den rauschwert abziehen
    int numberOfInterferingSatellites = 3;
    float maxNoise = 65.0;
    float upperPeak = 1023 - numberOfInterferingSatellites * maxNoise;
    float lowerPeak = -1023 + numberOfInterferingSatellites * maxNoise;

    for (int satCounter = 0; satCounter < 24; satCounter++) {
        for (int delta = 0; delta < 1024; delta++) {
            // Das skalarprodukt wird nicht normalisiert. D.h. für eine 1 ist das skalar gleich 1023
            // und für eine 0 ist das skalar = -1023. Allerdings nur im idealfall.
            // Die anderen Satelliten stören das signal aber nur maximal mit dem wert,
            // der über die kreuzkorrelation berechnet werden kann. Das heißt wir suchen nach
            // einem Signal, dass in dem Bereich für 0 (-1023) bzw 1 (1023) plus/minus dem Rauschen
            // der anderen Satteliten liegt.
            float scalar = calculateScalar(signal, goldCodes[satCounter], delta);
            if (scalar >= upperPeak) {
                cout << "Satellit " << satCounter + 1 << " hat folgedes Bit gesendet: " << 1 << " (Delta: " << delta << ")" << endl;
            }
            if (scalar <= lowerPeak) {
                cout << "Satellit " << satCounter + 1 << " hat folgedes Bit gesendet: " << 0 << " (Delta: " << delta << ")" << endl;
            }
        }
    }
}

int main(int argc, const char* argv[]) {
    int xorTable[24][2] = {
    {2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6},
    {6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6}
    };

    // Read the content of the file provided through the command line 
    auto sumSignal = readGpsSequence(argv[1]);
    for (auto i : sumSignal)
        std::cout << i << " ";

    int goldCodes[24][1023];
    createGoldCodes(goldCodes);
    for (int i = 0; i < 1023; i++)
        cout << goldCodes[23][i] << ", ";
    cout << endl;

    interpretSignal(sumSignal, goldCodes);

    return 0;
}