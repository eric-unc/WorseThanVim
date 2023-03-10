#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

class State {
	fstream *file;
	vector<string> buffer;
	size_t addr;
	string yank_buffer;

public:
	State(fstream *file, vector<string>& buffer);
	string get_line_at(size_t a);
	void set_line_at(size_t a, string s);
	void remove_line_at(size_t a);
	void add_line_at(size_t a, string s);
	string get_current_line();
	size_t num_lines();
	size_t get_addr();
	void set_addr(size_t new_addr);
	void inc_addr();
	void dec_addr();
	string get_yank_buffer();
	void set_yank_buffer(string s);
	void save();
};

bool prompt(State& state);
bool run_command(State& state, string s);

State::State(fstream *f, vector<string>& b){
	file = f;
	buffer = b;
	addr = 0;
	yank_buffer = "";
}

string State::get_line_at(size_t a){
	return buffer[a];
}

void State::set_line_at(size_t a, string s){
	buffer[a] = s;
}

void State::remove_line_at(size_t a){
	buffer.erase(buffer.begin() + a);
}


void State::add_line_at(size_t a, string s){
	buffer.insert(buffer.begin() + a, s);
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

string State::get_yank_buffer(){
	return yank_buffer;
}

void State::set_yank_buffer(string s){
	yank_buffer = s;
}

void State::save(){
	for(size_t i = 0; i < buffer.size(); i++){
		*file << buffer[i] << endl;
	}
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

	State state(&file, buffer);

	while (prompt(state));
	
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
		while(true){
			string line;
			cin >> line;

			if(line == ".") break;

			state.add_line_at(state.get_addr() + 1, line);
			state.inc_addr();
		}
	}else if (s == "A"){
		while(true){
			string line;
			cin >> line;

			if(line == ".") break;

			state.add_line_at(state.get_addr(), line);
			state.inc_addr();
		}
	}else if (s == "c"){
		string line;
		cin >> line;

		if(line != ".") state.set_line_at(state.get_addr(), line);
	}else if (s == "d"){
		if(state.num_lines() != 1){
			state.remove_line_at(state.get_addr());

			if(state.get_addr() == state.num_lines()) state.dec_addr();
		}else{
			if(state.get_current_line() != ""){
				state.set_line_at(0, "");
			}else{
				cout << "?" << endl;
			}
		}
	}else if (s == "y"){
		state.set_yank_buffer(state.get_current_line());
	}else if (s == "x"){
		if(state.num_lines() != 1){
			state.set_yank_buffer(state.get_current_line());
			state.remove_line_at(state.get_addr());

			if(state.get_addr() == state.num_lines()) state.dec_addr();
		}else{
			if(state.get_current_line() != ""){
				state.set_yank_buffer(state.get_current_line());
				state.set_line_at(0, "");
			}else{
				cout << "?" << endl;
			}
		}
	}else if (s == "p"){
		if(state.get_yank_buffer() != ""){
			state.add_line_at(state.get_addr() + 1, state.get_yank_buffer());
			state.inc_addr();
		}else{
			cout << "?" << endl;
		}
	}else if (s == "P"){
		if(state.get_yank_buffer() != ""){
			state.add_line_at(state.get_addr(), state.get_yank_buffer());
			state.inc_addr();
		}else{
			cout << "?" << endl;
		}
	}else if (s == "w"){
		state.save();
	}else if (s == "q"){
		return false;
	}else{
		regex num("^(\\d+)$");
		regex dot_num("^.(\\d+)$");

		smatch m;

		if(regex_match(s, m, num)){
			size_t line_num = stoul(s);
			if(line_num >= state.num_lines()){
				cout << "?" << endl;
			}else{
				state.set_addr(line_num);
			}
		}else if(regex_match(s, m, dot_num)){
			size_t line_num = stoul(&s[1]);
			if(line_num >= state.num_lines()){
				cout << "?" << endl;
			}else{
				cout << state.get_line_at(line_num) << endl;
			}
		}else{
			cout << "?" << endl;
		}
	}
		

	return true;
}
