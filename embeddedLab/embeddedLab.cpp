#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {
    //string fileName = argv[1];
    string fileName = "gps_sequence_8.txt";
    /*fileName += ".txt";
    cout << fileName;*/
    // attach an input stream to the wanted file
    ifstream input_stream(fileName);

    // check stream status
    if (!input_stream) cerr << "Can't open input file!";

    // file contents  
    vector<string> text;

    // one line
    string line;

    // extract all the text from the input file
    while (getline(input_stream, line)) {

        // store each line in the vector
        text.push_back(line);
    }
    for (auto i : text)
        std::cout << i << ' ';



    return 0;
}