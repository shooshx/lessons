#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	cout << "Hello World!" << endl;

	ifstream ifs("../test_file.txt", ios::binary);
	if (!ifs.good()) {
		cout << "Failed opening file" << endl;
		return 1;
	}
	ifs.seekg(0, ios_base::end);
	size_t sz = ifs.tellg();
	ifs.seekg(0, ios_base::beg);

	vector<char> buf(sz + 1);
	ifs.read(&buf[0], sz);
	buf[sz] = 0;

	cout << "file: " << (char*)&buf[0] << endl;

    return 0;
}

