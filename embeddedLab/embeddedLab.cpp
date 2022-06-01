#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

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

int calculateScalar(std::vector<int> signal, int goldCode[1023], int delta) {
	int scalar = 0;

	for (int i = 0; i < 1023; i++) {
		scalar += signal[i+delta] * goldCode[(i)];
	}

	return scalar;
}

void interpretSignal(std::vector<int> signal, int goldCodes[24][1023]) {
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
				std::cout << "Satellit " << i + 1 << " hat folgedes Bit gesendet: " << 1 << " (Delta: " << delta << ")" << std::endl;
			}
			if (scalar <= lowerPeak) {
				std::cout << "Satellit " << i + 1 << " hat folgedes Bit gesendet: " << 0 << " (Delta: " << delta << ")" << std::endl;
			}
		}
	}
}

int main(int argc, const char* argv[]) {
	int xorTable[24][2] = {
	{2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6},
	{6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6}
	};

	std::vector<int> sumSignal = { -2,0,0,-2,4,-2,-2,-4,2,0,0,0,-2,-2,2,0,0,-2,0,-2,-2,-2,0,-2,0,2,0,0,0,-2,-2,0,-4,-2,0,2,0,0,2,2,0,2,0,0,0,-2,-2,0,0,0,0,0,-2,0,2,2,0,-2,-2,2,2,0,2,2,-2,2,-2,-4,4,-2,0,2,4,-2,-2,0,2,0,-2,0,0,0,-2,-2,-2,0,0,-2,-2,-2,2,2,2,0,2,-2,0,-2,-2,-4,-2,0,0,2,2,-2,2,0,-2,0,2,2,2,-2,-2,-4,0,2,2,-2,-2,0,0,4,-2,-4,-4,0,0,0,0,2,2,2,-4,-2,2,0,0,-4,2,2,-4,-2,0,-2,2,0,2,0,-2,0,0,-2,-2,2,0,0,4,0,0,0,-4,0,2,0,4,0,-2,-4,0,-2,0,0,2,-2,0,0,2,4,0,-2,0,-2,2,-2,0,0,-2,2,2,2,2,0,-2,0,0,2,-2,2,0,-4,0,0,2,-2,-2,0,-2,0,0,0,0,-2,0,-2,2,-4,2,2,-4,-2,2,-4,2,2,-2,0,-2,2,-2,-2,0,-2,0,-2,0,2,4,0,4,0,0,0,0,-2,2,2,0,0,0,-2,-2,0,0,0,4,0,0,0,4,0,-2,0,0,0,-2,-4,2,0,-2,0,-2,2,2,2,-2,0,2,-2,-2,0,-2,0,2,0,2,-2,0,0,4,0,2,-2,2,-2,2,0,0,-4,0,0,-2,-2,0,-2,0,2,2,-2,0,0,4,-2,0,-2,0,-2,0,0,0,-2,0,0,4,-2,-2,-2,2,-2,2,-2,2,-2,-2,0,0,0,0,-2,0,0,0,0,0,4,-2,0,0,2,4,0,0,2,0,2,-2,-2,0,2,2,0,0,-4,2,2,0,-2,0,2,-2,2,4,0,0,0,0,0,-2,0,2,2,0,-4,-2,2,0,-2,0,0,0,2,0,-2,0,0,-2,2,0,2,0,0,-4,0,0,0,0,2,0,0,2,-2,-2,0,-2,0,-2,0,2,0,-2,2,0,-2,0,-2,2,2,0,0,0,-4,0,-2,0,0,2,0,0,0,-2,0,0,0,2,2,4,0,0,2,0,-4,0,2,2,0,-2,0,4,2,0,2,2,0,-2,0,-2,-2,-2,0,-2,2,0,0,0,0,0,0,-2,-2,-2,2,4,2,-2,-2,0,0,-2,4,-4,0,2,-2,0,0,2,-2,0,2,2,2,2,4,0,0,-2,2,0,0,-4,-2,0,-2,2,4,0,-2,-4,2,0,-2,0,-2,4,0,0,0,-2,0,2,4,2,2,0,2,2,0,2,-2,-2,2,-2,0,0,0,0,-2,2,0,0,-2,-4,0,0,2,4,2,0,2,-2,0,-2,2,0,-2,-4,2,2,2,-4,-4,4,0,0,-2,-2,-2,-4,0,2,-2,-2,0,-2,-4,-4,0,-2,-4,-2,-2,2,4,0,0,0,4,0,-2,0,-2,0,-4,4,-2,0,2,-4,-2,-4,0,0,0,0,-2,-4,2,2,4,-2,-2,2,0,2,4,0,0,0,2,0,0,2,0,-2,0,2,0,-2,4,0,0,0,0,2,-2,-2,0,0,0,0,-2,2,-4,4,2,2,-2,2,-2,-2,4,0,0,0,2,-4,2,0,-4,-2,2,2,-2,2,0,0,-2,-2,-4,0,0,0,2,2,0,-2,-2,0,2,2,-2,2,2,2,0,-2,0,0,0,0,2,0,-4,2,2,0,4,0,-2,4,2,2,2,2,-4,2,0,-2,4,0,2,0,0,2,0,0,0,2,2,-2,2,0,2,2,-2,0,2,0,0,-2,0,4,-2,0,0,0,0,2,2,-2,2,0,-2,-2,2,0,4,0,0,0,0,0,2,2,0,2,4,0,0,0,0,0,0,-2,2,0,-2,2,0,0,0,0,2,2,-2,2,0,0,2,2,2,0,0,2,2,0,-4,-2,0,-2,0,2,0,-2,0,0,-4,0,0,-2,0,0,0,0,0,2,4,0,0,0,0,-2,0,4,-2,0,0,0,0,0,2,0,0,0,0,0,2,0,2,0,0,-4,-4,2,2,4,4,-4,-2,0,-2,0,4,0,-4,0,0,-4,-2,2,2,0,0,0,0,0,0,-2,0,0,2,0,2,-2,-2,-2,0,-2,-2,-2,-2,0,0,2,-2,-4,0,-2,0,0,-2,0,2,-2,0,4,4,-2,-2,-2,-2,2,2,2,2,-2,4,2,0,-2,0,-4,0,2,0,0,-2,0,0,-2,-4,0,0,2,0,-2,0,0,2,2,0,0,2,0,2,-2,2,-2,2,0,2,0,-2,-4,-2,0,-2,2,-2,2,-2,0,-2,4,0,2,0,2,0,0,2,2,0,0,0,0,-2,0,0,0,2,2,0,0,-2,-4,-2,0,0,2,-2,2,-2,0,2,0,2,2,0,0,0,0,0,2,0,0,2,0,2,4,0,2,4,2,2,0,0,2,0,4,-2,-2,-2,2,2,4,2,0,4,-2,4,-2 };

	// Append the array to itself
	auto old_count = sumSignal.size();
	sumSignal.reserve(2 * old_count);
	std::copy_n(sumSignal.begin(), old_count, std::back_inserter(sumSignal));

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	std::chrono::milliseconds time_ms = std::chrono::milliseconds(0);
	std::chrono::duration<double, std::milli> time_ms_d = std::chrono::duration<double, std::milli>(0.);

	const int numberOfRuns = 100;

	for (auto i = 0; i < numberOfRuns; i++)
	{
		auto t1 = high_resolution_clock::now();

		int goldCodes[24][1023];
		generateGoldCodes(goldCodes, xorTable);

	std::cout << std::endl;

		interpretSignal(sumSignal, goldCodes);
		auto t2 = high_resolution_clock::now();
		/* Getting number of milliseconds as an integer. */
		auto ms_int = duration_cast<milliseconds>(t2 - t1);

		/* Getting number of milliseconds as a double. */
		duration<double, std::milli> ms_double = t2 - t1;
		time_ms += ms_int;
		time_ms_d += ms_double;

		std::cout << ms_int.count() << "ms\n";
		std::cout << ms_double.count() << "ms\n";

	}

	auto test = time_ms /= numberOfRuns;
	std::cout << numberOfRuns << " run average: " << test.count() << std::endl;
	auto test2 = time_ms_d /= numberOfRuns;
	std::cout << numberOfRuns << " run average: " << test2.count() << std::endl;
	return 0;
}