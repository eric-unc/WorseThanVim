#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class State {

};

int main(int argc, char **argv){
	if (argc != 2) {
		cerr << "Incorrect amount of arguments (should only accept one: the path)" << endl;
		return 1;
	}

	char *path = argv[1];

	fstream file;
	file.open(path, ios::in);

	vector<string> buffer;

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			cout << line << endl;
			buffer.push_back(line);
		}

		file.close();
	}

	// Blank/new file
	if (buffer.size() == 0) buffer.push_back("");

	file.open(path, ios::out);

	if (!file.is_open()) { // in case the file path was invalid, we'll find out here
		cerr << "Failed to open path!" << endl;
		return 1;
	}

	// writeback step
	for (size_t i = 0; i < buffer.size(); i++) {
		file << buffer[i] << endl;
	}
	
	return 0;
}
