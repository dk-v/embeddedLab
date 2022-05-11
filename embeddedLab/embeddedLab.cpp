#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

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

/// <summary>
/// Shifts all bits 1 postion to the right
/// </summary>
/// <param name="bits">The content of the register as an array of bits</param>
void shiftBitsByOne(int bits[10]) {
	int temp = bits[9];
	for (int i = 9; i > 0; i--) {
		bits[i] = bits[i - 1];
	}
	bits[0] = temp;
}

void calculateGoldSequence(int goldCode[1023], int registerSummandOne, int registerSummandTwo) {
	int register1[10] = { 1,1,1,1,1,1,1,1,1,1 };
	int register2[10] = { 1,1,1,1,1,1,1,1,1,1 };

	int index = 0;
	while (index < 1023) {
		int temp = register2[registerSummandOne - 1] ^ register2[registerSummandTwo - 1];
		int motherSequenceTwo = register2[9] ^ register2[8] ^ register2[7] ^ register2[5] ^ register2[2] ^ register2[1];
		int motherSequenceOne = register1[9] ^ register1[2];

		goldCode[index] = (temp ^ register1[9]) == 0 ? -1 : 1;

		shiftBitsByOne(register1);
		shiftBitsByOne(register2);
		register2[0] = motherSequenceTwo;
		register1[0] = motherSequenceOne;
		index++;
	}
}

void generateGoldCodes(int goldCodes[24][1023], int xorTable[24][2]) {
	for (int i = 0; i < 24; i++)
	{
		calculateGoldSequence(goldCodes[i], xorTable[i][0], xorTable[i][1]);
	}
}

int calculateScalar(int* signal, int goldCode[1023], int delta) {
	int scalar = 0;

	for (int i = 0; i < 1023; i++) {
		scalar += signal[i] * goldCode[(i + delta) % 1023];
	}

	return scalar;
}

void interpretSignal(int* signal, int goldCodes[24][1023]) {
	int numberOfInterferingSatellites = 3;
	// Result of the cross correlation formula with even register length and a length of 10
	// R = -2^((n+2)/2)-1 = -2^((10+2)/2)-1 = -65
	float maxNoise = 65.0;
	float upperPeak = 1023 - numberOfInterferingSatellites * maxNoise;
	float lowerPeak = -1023 + numberOfInterferingSatellites * maxNoise;

	for (int i = 0; i < 24; i++) {
		for (int delta = 0; delta < 1024; delta++) {
			int scalar = calculateScalar(signal, goldCodes[i], delta);
			if (scalar >= upperPeak) {
				cout << "Satellit " << i + 1 << " hat folgedes Bit gesendet: " << 1 << " (Delta: " << delta << ")" << endl;
			}
			if (scalar <= lowerPeak) {
				cout << "Satellit " << i + 1 << " hat folgedes Bit gesendet: " << 0 << " (Delta: " << delta << ")" << endl;
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
	/*auto sumSignal = readGpsSequence(argv[1]);
	for (auto i : sumSignal)
		std::cout << i << " ";*/

	int a[1023] = { -2,0,0,-2,4,-2,-2,-4,2,0,0,0,-2,-2,2,0,0,-2,0,-2,-2,-2,0,-2,0,2,0,0,0,-2,-2,0,-4,-2,0,2,0,0,2,2,0,2,0,0,0,-2,-2,0,0,0,0,0,-2,0,2,2,0,-2,-2,2,2,0,2,2,-2,2,-2,-4,4,-2,0,2,4,-2,-2,0,2,0,-2,0,0,0,-2,-2,-2,0,0,-2,-2,-2,2,2,2,0,2,-2,0,-2,-2,-4,-2,0,0,2,2,-2,2,0,-2,0,2,2,2,-2,-2,-4,0,2,2,-2,-2,0,0,4,-2,-4,-4,0,0,0,0,2,2,2,-4,-2,2,0,0,-4,2,2,-4,-2,0,-2,2,0,2,0,-2,0,0,-2,-2,2,0,0,4,0,0,0,-4,0,2,0,4,0,-2,-4,0,-2,0,0,2,-2,0,0,2,4,0,-2,0,-2,2,-2,0,0,-2,2,2,2,2,0,-2,0,0,2,-2,2,0,-4,0,0,2,-2,-2,0,-2,0,0,0,0,-2,0,-2,2,-4,2,2,-4,-2,2,-4,2,2,-2,0,-2,2,-2,-2,0,-2,0,-2,0,2,4,0,4,0,0,0,0,-2,2,2,0,0,0,-2,-2,0,0,0,4,0,0,0,4,0,-2,0,0,0,-2,-4,2,0,-2,0,-2,2,2,2,-2,0,2,-2,-2,0,-2,0,2,0,2,-2,0,0,4,0,2,-2,2,-2,2,0,0,-4,0,0,-2,-2,0,-2,0,2,2,-2,0,0,4,-2,0,-2,0,-2,0,0,0,-2,0,0,4,-2,-2,-2,2,-2,2,-2,2,-2,-2,0,0,0,0,-2,0,0,0,0,0,4,-2,0,0,2,4,0,0,2,0,2,-2,-2,0,2,2,0,0,-4,2,2,0,-2,0,2,-2,2,4,0,0,0,0,0,-2,0,2,2,0,-4,-2,2,0,-2,0,0,0,2,0,-2,0,0,-2,2,0,2,0,0,-4,0,0,0,0,2,0,0,2,-2,-2,0,-2,0,-2,0,2,0,-2,2,0,-2,0,-2,2,2,0,0,0,-4,0,-2,0,0,2,0,0,0,-2,0,0,0,2,2,4,0,0,2,0,-4,0,2,2,0,-2,0,4,2,0,2,2,0,-2,0,-2,-2,-2,0,-2,2,0,0,0,0,0,0,-2,-2,-2,2,4,2,-2,-2,0,0,-2,4,-4,0,2,-2,0,0,2,-2,0,2,2,2,2,4,0,0,-2,2,0,0,-4,-2,0,-2,2,4,0,-2,-4,2,0,-2,0,-2,4,0,0,0,-2,0,2,4,2,2,0,2,2,0,2,-2,-2,2,-2,0,0,0,0,-2,2,0,0,-2,-4,0,0,2,4,2,0,2,-2,0,-2,2,0,-2,-4,2,2,2,-4,-4,4,0,0,-2,-2,-2,-4,0,2,-2,-2,0,-2,-4,-4,0,-2,-4,-2,-2,2,4,0,0,0,4,0,-2,0,-2,0,-4,4,-2,0,2,-4,-2,-4,0,0,0,0,-2,-4,2,2,4,-2,-2,2,0,2,4,0,0,0,2,0,0,2,0,-2,0,2,0,-2,4,0,0,0,0,2,-2,-2,0,0,0,0,-2,2,-4,4,2,2,-2,2,-2,-2,4,0,0,0,2,-4,2,0,-4,-2,2,2,-2,2,0,0,-2,-2,-4,0,0,0,2,2,0,-2,-2,0,2,2,-2,2,2,2,0,-2,0,0,0,0,2,0,-4,2,2,0,4,0,-2,4,2,2,2,2,-4,2,0,-2,4,0,2,0,0,2,0,0,0,2,2,-2,2,0,2,2,-2,0,2,0,0,-2,0,4,-2,0,0,0,0,2,2,-2,2,0,-2,-2,2,0,4,0,0,0,0,0,2,2,0,2,4,0,0,0,0,0,0,-2,2,0,-2,2,0,0,0,0,2,2,-2,2,0,0,2,2,2,0,0,2,2,0,-4,-2,0,-2,0,2,0,-2,0,0,-4,0,0,-2,0,0,0,0,0,2,4,0,0,0,0,-2,0,4,-2,0,0,0,0,0,2,0,0,0,0,0,2,0,2,0,0,-4,-4,2,2,4,4,-4,-2,0,-2,0,4,0,-4,0,0,-4,-2,2,2,0,0,0,0,0,0,-2,0,0,2,0,2,-2,-2,-2,0,-2,-2,-2,-2,0,0,2,-2,-4,0,-2,0,0,-2,0,2,-2,0,4,4,-2,-2,-2,-2,2,2,2,2,-2,4,2,0,-2,0,-4,0,2,0,0,-2,0,0,-2,-4,0,0,2,0,-2,0,0,2,2,0,0,2,0,2,-2,2,-2,2,0,2,0,-2,-4,-2,0,-2,2,-2,2,-2,0,-2,4,0,2,0,2,0,0,2,2,0,0,0,0,-2,0,0,0,2,2,0,0,-2,-4,-2,0,0,2,-2,2,-2,0,2,0,2,2,0,0,0,0,0,2,0,0,2,0,2,4,0,2,4,2,2,0,0,2,0,4,-2,-2,-2,2,2,4,2,0,4,-2,4,-2 };

	//using std::chrono::high_resolution_clock;
	//using std::chrono::duration_cast;
	//using std::chrono::duration;
	//using std::chrono::milliseconds;

	//std::chrono::milliseconds time_ms = std::chrono::milliseconds(0);
	//std::chrono::duration<double, std::milli> time_ms_d = std::chrono::duration<double, std::milli>(0.);

	//const int numberOfRuns = 100;

	//for (auto i = 0; i < numberOfRuns; i++)
	//{
	//	auto t1 = high_resolution_clock::now();

		int goldCodes[24][1023];
		generateGoldCodes(goldCodes, xorTable);

	//std:cout << endl;

		interpretSignal(a, goldCodes);
	//	auto t2 = high_resolution_clock::now();
	//	/* Getting number of milliseconds as an integer. */
	//	auto ms_int = duration_cast<milliseconds>(t2 - t1);

	//	/* Getting number of milliseconds as a double. */
	//	duration<double, std::milli> ms_double = t2 - t1;
	//	time_ms += ms_int;
	//	time_ms_d += ms_double;

	//	std::cout << ms_int.count() << "ms\n";
	//	std::cout << ms_double.count() << "ms\n";

	//}

	//auto test = time_ms /= numberOfRuns;
	//std::cout << numberOfRuns << " run average: " << test.count() << std::endl;
	//auto test2 = time_ms_d /= numberOfRuns;
	//std::cout << numberOfRuns << " run average: " << test2.count() << std::endl;
	return 0;
}