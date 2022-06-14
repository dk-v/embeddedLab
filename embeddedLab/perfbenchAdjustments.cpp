// ------------------------------------------------------------------------- //
// Welcome to Perfbench: online C++ code profiling tool.                     //
// Read through the code example below to get started.                       //
// For more information and examples, click on the "help" and "api" tabs.    //
// ------------------------------------------------------------------------- //

// Click the Run button below to see the tool in action right away.
// You can open more tabs by clicking on the "+" icon and profile different
// versions of your code. You can also switch between different runs with
// vertical tabs on the right to step over the history and compare charts.

// Make sure to specify your includes (iostream is included automatically)
#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <iostream>


/// <summary>
/// Shifts all bits 1 postion to the right
/// </summary>
/// <param name="bits">The content of the register as an array of bits</param>
void shiftBitsByOne(int bits[10]) {
	PROFILE_START("shiftBitsByOne");
	int temp = bits[9];
	for (int i = 9; i > 0; i--) {
		bits[i] = bits[i - 1];
	}
	bits[0] = temp;
	PROFILE_STOP("shiftBitsByOne");
}

void calculateGoldSequence(int goldCode[1023], int registerSummandOne, int registerSummandTwo) {
	PROFILE_START("calculateGoldSequence");
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
	PROFILE_STOP("calculateGoldSequence");
}

void generateGoldCodes(int goldCodes[24][1023], int xorTable[24][2]) {
	PROFILE_START("generateGoldCodes");
	for (int i = 0; i < 24; i++)
	{
		calculateGoldSequence(goldCodes[i], xorTable[i][0], xorTable[i][1]);
	}
	PROFILE_STOP("generateGoldCodes");
}

int calculateScalar(std::vector<int> signal, int goldCode[1023], int delta) {
	PROFILE_START("calculateScalar");
	int scalar = 0;

	for (int i = 0; i < 1023; i++) {
		scalar += signal[i] * goldCode[(i + delta) % 1023];
	}

	PROFILE_STOP("calculateScalar");
	return scalar;
}

void interpretSignal(std::vector<int> signal, int goldCodes[24][1023]) {
	PROFILE_START("interpretSignal");
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
	PROFILE_STOP("interpretSignal");
}

void test_func() {
	int xorTable[24][2] = {
	  {2, 6}, {3, 7}, {4, 8}, {5, 9}, {1, 9}, {2, 10}, {1, 8}, {2, 9}, {3, 10}, {2, 3}, {3, 4}, {5, 6},
	  {6, 7}, {7, 8}, {8, 9}, {9, 10}, {1, 4}, {2, 5}, {3, 6}, {4, 7}, {5, 8}, {6, 9}, {1, 3}, {4, 6}
	};

	std::vector<int> sumSignal{ -2,0,0,-2,4,-2,-2,-4,2,0,0,0,-2,-2,2,0,0,-2,0,-2,-2,-2,0,-2,0,2,0,0,0,-2,-2,0,-4,-2,0,2,0,0,2,2,0,2,0,0,0,-2,-2,0,0,0,0,0,-2,0,2,2,0,-2,-2,2,2,0,2,2,-2,2,-2,-4,4,-2,0,2,4,-2,-2,0,2,0,-2,0,0,0,-2,-2,-2,0,0,-2,-2,-2,2,2,2,0,2,-2,0,-2,-2,-4,-2,0,0,2,2,-2,2,0,-2,0,2,2,2,-2,-2,-4,0,2,2,-2,-2,0,0,4,-2,-4,-4,0,0,0,0,2,2,2,-4,-2,2,0,0,-4,2,2,-4,-2,0,-2,2,0,2,0,-2,0,0,-2,-2,2,0,0,4,0,0,0,-4,0,2,0,4,0,-2,-4,0,-2,0,0,2,-2,0,0,2,4,0,-2,0,-2,2,-2,0,0,-2,2,2,2,2,0,-2,0,0,2,-2,2,0,-4,0,0,2,-2,-2,0,-2,0,0,0,0,-2,0,-2,2,-4,2,2,-4,-2,2,-4,2,2,-2,0,-2,2,-2,-2,0,-2,0,-2,0,2,4,0,4,0,0,0,0,-2,2,2,0,0,0,-2,-2,0,0,0,4,0,0,0,4,0,-2,0,0,0,-2,-4,2,0,-2,0,-2,2,2,2,-2,0,2,-2,-2,0,-2,0,2,0,2,-2,0,0,4,0,2,-2,2,-2,2,0,0,-4,0,0,-2,-2,0,-2,0,2,2,-2,0,0,4,-2,0,-2,0,-2,0,0,0,-2,0,0,4,-2,-2,-2,2,-2,2,-2,2,-2,-2,0,0,0,0,-2,0,0,0,0,0,4,-2,0,0,2,4,0,0,2,0,2,-2,-2,0,2,2,0,0,-4,2,2,0,-2,0,2,-2,2,4,0,0,0,0,0,-2,0,2,2,0,-4,-2,2,0,-2,0,0,0,2,0,-2,0,0,-2,2,0,2,0,0,-4,0,0,0,0,2,0,0,2,-2,-2,0,-2,0,-2,0,2,0,-2,2,0,-2,0,-2,2,2,0,0,0,-4,0,-2,0,0,2,0,0,0,-2,0,0,0,2,2,4,0,0,2,0,-4,0,2,2,0,-2,0,4,2,0,2,2,0,-2,0,-2,-2,-2,0,-2,2,0,0,0,0,0,0,-2,-2,-2,2,4,2,-2,-2,0,0,-2,4,-4,0,2,-2,0,0,2,-2,0,2,2,2,2,4,0,0,-2,2,0,0,-4,-2,0,-2,2,4,0,-2,-4,2,0,-2,0,-2,4,0,0,0,-2,0,2,4,2,2,0,2,2,0,2,-2,-2,2,-2,0,0,0,0,-2,2,0,0,-2,-4,0,0,2,4,2,0,2,-2,0,-2,2,0,-2,-4,2,2,2,-4,-4,4,0,0,-2,-2,-2,-4,0,2,-2,-2,0,-2,-4,-4,0,-2,-4,-2,-2,2,4,0,0,0,4,0,-2,0,-2,0,-4,4,-2,0,2,-4,-2,-4,0,0,0,0,-2,-4,2,2,4,-2,-2,2,0,2,4,0,0,0,2,0,0,2,0,-2,0,2,0,-2,4,0,0,0,0,2,-2,-2,0,0,0,0,-2,2,-4,4,2,2,-2,2,-2,-2,4,0,0,0,2,-4,2,0,-4,-2,2,2,-2,2,0,0,-2,-2,-4,0,0,0,2,2,0,-2,-2,0,2,2,-2,2,2,2,0,-2,0,0,0,0,2,0,-4,2,2,0,4,0,-2,4,2,2,2,2,-4,2,0,-2,4,0,2,0,0,2,0,0,0,2,2,-2,2,0,2,2,-2,0,2,0,0,-2,0,4,-2,0,0,0,0,2,2,-2,2,0,-2,-2,2,0,4,0,0,0,0,0,2,2,0,2,4,0,0,0,0,0,0,-2,2,0,-2,2,0,0,0,0,2,2,-2,2,0,0,2,2,2,0,0,2,2,0,-4,-2,0,-2,0,2,0,-2,0,0,-4,0,0,-2,0,0,0,0,0,2,4,0,0,0,0,-2,0,4,-2,0,0,0,0,0,2,0,0,0,0,0,2,0,2,0,0,-4,-4,2,2,4,4,-4,-2,0,-2,0,4,0,-4,0,0,-4,-2,2,2,0,0,0,0,0,0,-2,0,0,2,0,2,-2,-2,-2,0,-2,-2,-2,-2,0,0,2,-2,-4,0,-2,0,0,-2,0,2,-2,0,4,4,-2,-2,-2,-2,2,2,2,2,-2,4,2,0,-2,0,-4,0,2,0,0,-2,0,0,-2,-4,0,0,2,0,-2,0,0,2,2,0,0,2,0,2,-2,2,-2,2,0,2,0,-2,-4,-2,0,-2,2,-2,2,-2,0,-2,4,0,2,0,2,0,0,2,2,0,0,0,0,-2,0,0,0,2,2,0,0,-2,-4,-2,0,0,2,-2,2,-2,0,2,0,2,2,0,0,0,0,0,2,0,0,2,0,2,4,0,2,4,2,2,0,0,2,0,4,-2,-2,-2,2,2,4,2,0,4,-2,4,-2 };

	// auto old_count = sumSignal.size();
	// sumSignal.reserve(2 * old_count);
	// std::copy_n(sumSignal.begin(), old_count, std::back_inserter(sumSignal));

	PROFILE_START("cdma");
	int goldCodes[24][1023];

	generateGoldCodes(goldCodes, xorTable);

	interpretSignal(sumSignal, goldCodes);
	PROFILE_STOP("cdma");
}

// The main function should be provided. The code will be compiled on the 
// Perfbench server to a binary and executed.
int main() {
	// The PROFILE_START + PROFILE_STOP combo (or PROFILE_ONCE with an 
	// expression) is all you need to have latency charts plotted. If we call 
	// the above function (test_map_latency) here, we will get charts based 
	// on a single measurement.

	// It is a good idea though to warm things up and repeat measurements in 
	// a loop to get reliable latency distributions.

	// The PROFILE_RUN_ALL function is a wrapper that executes an expression 
	// for the specified number of warmup loops, then clears collected stats 
	// and runs actual test loops. It doesn't internally call PROFILE_START / 
	// PROFILE_STOP, so your expression should have them. The "api" tab has 
	// details on the exact logic.
	PROFILE_RUN_ALL(1000, 100,
		test_func();
	);

	// There are also some useful low-level functions available. They don't 
	// affect charts, but you can print results directly to the stdout. 
	// See the "api" tab for more details.
	const auto [min, avg, max] = profile::get_latency(20, 10, sqrt, 3.14);
	std::cout << "Sqrt latency usec:" << std::endl;
	std::cout << "Min: " << min / 1000 << std::endl;
	std::cout << "Avg: " << avg / 1000 << std::endl;
	std::cout << "Max: " << max / 1000 << std::endl;

	return 0;
}
