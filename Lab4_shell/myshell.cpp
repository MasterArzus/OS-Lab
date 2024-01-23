#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <vector>

using namespace std;

#define MAX_COMMAND_NUM 10
#define MAX_SIZE 1024

vector<string> history_commands;
vector<string> commmads;
string current_path = "";


void internal_command(const string& command)
{
    if (command == "cd"){
        string new_dic = cammands[1];
        //cin >> new_dic;
        if(history_commands[history_commands.size()-1]=="cd"){
            history_commands[history_commands.size()-1]="cd " + new_dic;
        }
        chdir(new_dic.c_str());
    }
    else if(command == "exit") {
        exit(0);
    }
    else if (command == "history") {
        cout << "History commands: \n";
        for (const auto& history_command : history_commands) {
            cout << history_command << "\n";
        }
    }
    else{
        cerr << "Invalid internal command:" << command << endl;
    }
}

void external_command(const string& command) {
    pid_t pid = fork();
    if (pid == 0) {
        execlp(command.c_str(), command.c_str(), nullptr);
        cerr << "Failed to execute command: " << command << endl;
        exit(1);
    }
    else if (pid > 0) {
        wait(nullptr);
    }
    else {
        cerr << "Failed to create child process." << endl;
    }
}

void exec_command(const string& command) {
    history_commands.push_back(command);
    if (history_commands.size() > MAX_COMMAND_NUM) {
        history_commands.erase(history_commands.begin());
    }
    if (command.size() > 3){
        
    }

    if (command == "cd" || command == "exit" || command == "history") {
        internal_command(command);
    }
    else {
        external_command(command);
    }
}

int main() {
    while (true) {
        cout << current_path << "$ ";
        string command;
        getline(cin, command);
        
	    istringstream iss(command);	// instream
    	string token;			// buffer
	    while (getline(iss, token, split))	// split
    	{
            commmads.push_back(token);
	    	// cout << token << endl;
    	}

        if (!command.empty()) {
            exec_command(commands[0]);
        }
        commands.clear();

        cout << endl;
    }

    return 0;
}

