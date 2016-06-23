#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace  std;

void split(const string& str, vector<string> &tokens,
           const string &delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

inline string trim(string &str)
{
    const string whitespaces(" \t\f\v\n\r");

    string::size_type pos = str.find_first_not_of(whitespaces);
    if(pos != string::npos)
        str.erase(0, pos);   // prefixing spaces

    pos = str.find_last_not_of(whitespaces);
    if(pos != string::npos)
        str.erase(pos + 1);    // surfixing spaces

    return str;
}

void parse_command(string &command, string &name, string &argc)
{
    command = trim(command);

    string::size_type pos = command.find_first_of(' ');
    if(pos != string::npos) {
        name = command.substr(0, pos);
        argc = command.substr(pos + 1, command.length() - pos - 1);
    } else {
        name = command;
        argc = "";
    }
}

void exec_command(int n, vector<string> &commands)
{
    string name, args;
    parse_command(commands[n], name, args);
    execlp(name.c_str(), name.c_str(), args.c_str(), NULL);
}

// who ----(stdout)---> pfd[1] --- pfd[0] ----(stdin)---> wc -l
void execute_line(vector<string> &commands, int n)
{
    if(n > 2)
        execute_line(commands, n-1);

    int pfd[2];
    pipe(pfd);
    if(n >= 1 && !fork()) {
        // Child
        close(STDOUT_FILENO);
        dup2(pfd[1], STDOUT_FILENO);    // Copy pipe out to STDOUT
        close(pfd[1]);  // Close as was redirected
        close(pfd[0]);  // Close as not necessary here

        exec_command(n - 1, commands);
    } else if(n != commands.size() - 1) {
        // Parent
        close(STDIN_FILENO);
        dup2(pfd[0], STDIN_FILENO); // Copy pipe in to STDIN
        close(pfd[0]);  // Close as was redirected
        close(pfd[1]);  // Close as not necessary here

        exec_command(n, commands);
    } else {
        int fd = open("result.out", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        dup2(fd, STDOUT_FILENO);            // Copy stdout to file
        dup2(pfd[0], STDIN_FILENO);         // Copy pipe in to STDIN
        close(pfd[0]);  // Close as was redirected
        close(pfd[1]);  // Close as not necessary here
        close(fd);

        exec_command(n, commands);
    }
}

int main()
{
    char buffer[1024];
    ssize_t size = read(STDIN_FILENO, buffer, 1024);
    if(size > 0) {
        buffer[size] = '\0';
        string command = buffer;
        vector<string> commands;
        split(command, commands, "|");
        execute_line(commands, commands.size() - 1);
    }
    return 0;
}
