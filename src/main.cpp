#include <iostream>
#include <vector>
#include <sys/stat.h>

using namespace std;

class State {

};

int main(int argc, char **argv){
	if (argc != 2) {
		cerr << "Incorrect amount of arguments (should only accept one: the path)" << endl;
		return 1;
	}

	char *path = argv[1];

	struct stat sb;

	FILE *file = nullptr;
	vector<string> buffer;

	if (stat(path, &sb) == 0) {
		if (sb.st_mode & S_IFDIR) {
			cerr << "Cannot edit directory!" << endl;
			return 1;
		}

		FILE *file = fopen(path, "r");
		if (file == nullptr) {
			cerr << "Failed to open path!" << endl;
			return 1;
		}

		// write to buffer
	} else {
		//cout << "The Path is invalid!" << endl;
		//file = fopen(path, "a+");
		// initiate buffer
	}

	cout << "Hello world!" << endl;
	return 0;
}
