#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstring>
#include <cerrno>

#include <unistd.h>     // fork, exec*
#include <sys/types.h>
#include <sys/wait.h>   // waitpid


static const char* PROMPT = "cmd> ";

// -------- helpers --------

// Trim leading/trailing whitespace IN-PLACE and return a view as std::string&
static std::string& trim(std::string& s) {
    // TODO:
    // 1) find first non-space index (std::isspace)
    // 2) find last non-space index
    // 3) assign s = s.substr(first, last-first+1) if there are non-space chars
    // 4) else set s.clear()
    return s; // TODO: replace with trimmed reference
}

// Split on ASCII whitespace into tokens (no quotes/escapes).
// Produces a vector of std::string, and a second vector<char*> for execvp.
// NOTE: execvp requires argv to be null-terminated; push_back(nullptr) at end.
static void tokenize(const std::string& line,
                     std::vector<std::string>& words,
                     std::vector<char*>& argv_buf) {
    // TODO:
    // 1) Walk 'line', split on std::isspace.
    // 2) For each token, push_back into 'words'.
    // 3) Build argv_buf by taking words[i].data() (C++17: use const_cast<char*> or &words[i][0]).
    // 4) argv_buf.push_back(nullptr) at the end.
}


int main() {
    // TODO (optional): install_parent_signal_handlers();

    std::string line;

    for (;;) {
        // 1) prompt
        std::cout << PROMPT;
        std::cout.flush();

        // 2) read line
        if (!std::getline(std::cin, line)) {
            if (std::cin.eof()) {
                // EOF (Ctrl-D)
                std::cout << "\n";
                break;
            } else {
                std::perror("getline/std::cin");
                return EXIT_FAILURE;
            }
        }

        // 3) trim
        trim(line); // TODO: implement
        if (line.empty()) continue;

        // 4) built-in exit
        if (line == "exit") break;

        // 5) tokenize -> argv for execvp
        std::vector<std::string> words;
        std::vector<char*> argv;
        tokenize(line, words, argv);  // TODO: implement

        if (words.empty()) continue;  // defensive

        // 6) fork
        pid_t pid = fork();
        if (pid < 0) {
            std::perror("fork");
            // continue shell loop instead of hard exit
            continue;
        }

        if (pid == 0) {
            // ---- child ----


            // 7) exec (search PATH)
            // Prefer execvp for argv[] support. If your assignment demands execlp,
            // you can call execlp(words[0].c_str(), words[0].c_str(), (char*)nullptr);
            if (execvp(argv[0], argv.data()) == -1) {
                std::perror(argv[0]);
                _exit(127);
            }
        } else {
            // ---- parent ----
            int status = 0;
            if (waitpid(pid, &status, 0) == -1) {
                std::perror("waitpid");
                continue;
            }

        
        }
    }

    return 0;
}
