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
	string get_line_at(size_t a);
	string get_current_line();
	size_t num_lines();
	size_t get_addr();
	void set_addr(size_t new_addr);
	void inc_addr();
	void dec_addr();
};

bool prompt(State& state);
bool run_command(State& state, string s);

State::State(vector<string>& b){
	buffer = b;
	addr = 0;
	yank_buffer = "";
}

string State::get_line_at(size_t a){
	return buffer[a];
}

string State::get_current_line(){
	return buffer[addr];
}

size_t State::num_lines(){
	return buffer.size();
}

size_t State::get_addr(){
	return addr;
}

void State::set_addr(size_t new_addr){
	addr = new_addr;
}

void State::inc_addr(){
	addr++;
}

void State::dec_addr(){
	addr--;
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
	if(s == "."){
		cout << state.get_current_line() << endl;
	}else if (s == "r"){
		cout << state.get_addr() << endl;
	}else if (s == "^"){
		state.set_addr(0);
	}else if (s == "$"){
		state.set_addr(state.num_lines() - 1);
	}else if (s == "+"){
		if(state.get_addr() + 1 >= state.num_lines()){
			cout << "?" << endl;
		}else{
			state.inc_addr();
		}
	}else if (s == "-"){
		if(state.get_addr() == 0){
			cout << "?" << endl;
		}else{
			state.dec_addr();
		}
	}else if (s == "a"){
	}else if (s == "A"){
	}else if (s == "c"){
	}else if (s == "d"){
	}else if (s == "y"){
	}else if (s == "x"){
	}else if (s == "p"){
	}else if (s == "P"){
	}else if (s == "w"){
	}else if (s == "q"){
		return false;
	}else{

	}
		

	return true;
}
