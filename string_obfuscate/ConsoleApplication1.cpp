
#include <iostream>
#include <sstream>

using namespace std;


string deobfuscate(const char* in) {
    string d(in);
    d = d.substr(5);
    int p = d.find("__E__");
    d = d.substr(0, p);
    for(size_t i = 0; i < d.size(); ++i)
        d[i] = d[i] ^ 0xff;
    return d;
}

#define DEOB(s) deobfuscate("__S__" s "__E__")


int main(int argc, char* argv[])
{
    cout << DEOB("Please enter the serial number:") << flush;
    string input;
    getline(cin, input);
    cout << "You entered: " << input << endl;

    return 0;
}

