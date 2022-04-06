#include <iostream>
#include <string>
#include <fstream>
#include <vector>

/// <summary>
/// Reads the contents of a text file 
/// </summary>
/// <param name="fileName">The name of the file</param>
/// <returns>A string containing the text in the file</returns>
std::string readGpsSequence(const std::string& fileName) {
	// Attach an input stream to the wanted file
	std::ifstream input_stream(fileName);

	// Check stream status
	if (!input_stream) std::cerr << "Can't open input file!";

	// File content  
	std::vector<std::string> content;
	std::string line;
	std::string result;

	// Extract all the text from the input file
	while (std::getline(input_stream, line)) {
		// store each line in the vector
		content.push_back(line);
	}

	// Assign the vector contents to a simple string
	for (const auto i : content)
		result += i;

	return result;
}

void calculateCrossCorrelation();

void findPeeks();

int main(int argc, char* argv[]) {
	
	// Read the content of the file provided through the command line 
	auto sumSignal = readGpsSequence(argv[1]);
	std::cout << sumSignal;

	return 0;
}