#include <iostream>
#include <sys/types.h>
#include <filesystem>
#include <vector>
#include <string>
#include <chrono>
using namespace std;
namespace fs = filesystem; // use file sys

// Convert file_time_type to time_t
time_t convertFileTimeToTimeT(const fs::file_time_type& fileTime) {
    using namespace chrono;
    auto duration = fileTime.time_since_epoch();
    return duration_cast<seconds>(duration).count();
}

void findFiles(const fs::path& currentPath, const vector<string>& options) {
    for (const auto& entry : fs::directory_iterator(currentPath)) {
        if (fs::is_directory(entry.path())) {
            // Recursively call findFiles for subdirectories
            findFiles(entry.path(), options);
        }
        else {
            // Check Condition based on options
            bool meetsCondition = true;

            for (size_t i = 0; i < options.size(); i += 2) {
                const string& option = options[i];
                const string& value = options[i + 1];

                if (option == "-name") {
                    // Check if the file name matches the specified pattern
                    meetsCondition &= (entry.path().filename() == fs::path(value).filename());
                }
                else if (option == "-ctime") {
                    // Check if ctime (creation time) is within the specified time frame
                    time_t now = time(nullptr);
                    time_t fileTime = convertFileTimeToTimeT(fs::last_write_time(entry.path()));
                    time_t timeDiff = now - fileTime;
                    meetsCondition &= (timeDiff <= stoi(value));
                }
                else if (option == "-mtime") {
                    // Check if mtime (modification time) is within the specified time frame
                    time_t now = time(nullptr);
                    time_t fileTime = convertFileTimeToTimeT(fs::last_write_time(entry.path()));
                    time_t timeDiff = now - fileTime;
                    meetsCondition &= (timeDiff <= stoi(value));
                }
                // Add more options as needed
            }

            // Print the file path if it meets the Condition
            if (meetsCondition) {
                cout << entry.path().string() << endl;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [PATH] [-option] [para]" << endl;
        return 1;
    }

    const fs::path directoryPath(argv[1]);

    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath)) {
        cerr << "Invalid directory path." << endl;
        return 1;
    }

    vector<string> options;

    // Additional options
    for (int i = 2; i < argc; i += 2) {
        options.push_back(argv[i]);
        options.push_back(argv[i + 1]);
    }

    findFiles(directoryPath, options);

    return 0;
}
