#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class State {
	vector<string> buffer;
	size_t addr;
	string yank_buffer;

public:
	State(vector<string>& buffer);
};

bool prompt(State& state);
bool run_command(State& state, string s);

State::State(vector<string>& b){
	buffer = b;
	addr = 0;
	yank_buffer = "";
}

int main(int argc, char **argv){
	if(argc != 2){
		cerr << "Incorrect amount of arguments (should only accept one: the path)" << endl;
		return 1;
	}

	char *path = argv[1];

	fstream file;
	file.open(path, ios::in);

	vector<string> buffer;

	if(file.is_open()){
		string line;
		while(getline(file, line)){
			cout << line << endl;
			buffer.push_back(line);
		}

		file.close();
	}

	// Blank/new file
	if(buffer.size() == 0) buffer.push_back("");

	file.open(path, ios::out);

	if(!file.is_open()){ // in case the file path was invalid, we'll find out here
		cerr << "Failed to open path!" << endl;
		return 1;
	}

	State state(buffer);

	while (prompt(state));

	// writeback step
	for(size_t i = 0; i < buffer.size(); i++){
		file << buffer[i] << endl;
	}
	
	return 0;
}

bool prompt(State& state){
	string line;
	cin >> line;
	return run_command(state, line);
}

bool run_command(State& state, string s){
	if (s == "w\n") {
		cout << "w + nl" << endl;
		return false;
	} else if (s == "w") {
		cout << "w" << endl;
		return false;
	} else {
		return true;
	}
}
