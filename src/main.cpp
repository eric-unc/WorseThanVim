#include <iostream>

using namespace std;

class State {

};

int main(int argc, char **argv){
	if (argc != 2) {
		cerr << "Incorrect amount of arguments (should only accept one: the path)" << endl;
		return 1;
	}

	char *path = argv[1];

	FILE *file = fopen(path, "rw");

	cout << "Hello world!" << endl;
	return 0;
}
