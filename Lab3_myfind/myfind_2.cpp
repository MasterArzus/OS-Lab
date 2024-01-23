#include <iostream>
#include <sys/stat.h>  // find the status of file
#include <sys/types.h>  // find the status of file
#include <dirent.h> // find files in certain dir
#include <vector>
using namespace std;

char match(char* first, char* second)
{
    if (*first == NULL && *second == NULL)
        return 1;
    if (*first == '*' && *(first + 1) != NULL && *second == NULL)
        return 0;
    if (*first == '?' || *first == *second)
        return match(first + 1, second + 1);
    if (*first == '*')
        return match(first + 1, second) || match(first, second + 1);
    return 0;
}

DIR* opendir()
{
    DIR* d; // directory here
    struct dirent* dir;

}

string myfind()

int main(int argc, char* argv[]) // needs inputs
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [PATH] [-option] [para]" << endl;
        return 1;
    }
    string path = argv[1];
    
    vector<string> options;
    // Parse options
    for (int i = 2; i < argc; i += 2) {
        options.push_back(argv[i]);
        options.push_back(argv[i + 1]);
    }

}


