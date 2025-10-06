#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cstring>
#include <cerrno>
#include <cstdio>       // std::perror

#include <unistd.h>     // fork, exec*
#include <sys/types.h>
#include <sys/wait.h>   // waitpid

static const char* PROMPT = "cmd> ";

// -------- helpers --------

// Trim leading/trailing whitespace IN-PLACE and return s.
static std::string& trim(std::string& s) {
    size_t n = s.size();
    size_t i = 0;
    while (i < n && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    if (i == n) { s.clear(); return s; }
    size_t j = n - 1;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j]))) --j;
    s.assign(s, i, j - i + 1);
    return s;
}

// Split on ASCII whitespace into tokens (no quotes/escapes).
// Build both 'words' and an argv buffer for execvp (null-terminated).
static void tokenize(const std::string& line,
                     std::vector<std::string>& words,
                     std::vector<char*>& argv_buf) {
    words.clear();
    argv_buf.clear();

    const char* p = line.c_str();
    const char* end = p + line.size();

    while (p < end) {
        // skip leading spaces
        while (p < end && std::isspace(static_cast<unsigned char>(*p))) ++p;
        if (p >= end) break;

        // find token end
        const char* q = p;
        while (q < end && !std::isspace(static_cast<unsigned char>(*q))) ++q;

        words.emplace_back(p, q - p);
        p = q;
    }

    // Build argv (null-terminated)
    argv_buf.reserve(words.size() + 1);
    for (auto& w : words) {
        argv_buf.push_back(const_cast<char*>(w.c_str()));
    }
    argv_buf.push_back(nullptr);
}

int main() {
    std::string line;

    for (;;) {
        // 1) prompt
        std::cout << PROMPT;
        std::cout.flush();

        // 2) read line
        if (!std::getline(std::cin, line)) {
            if (std::cin.eof()) {
                std::cout << "\n";
                break;
            } else {
                std::perror("getline/std::cin");
                return EXIT_FAILURE;
            }
        }

        // 3) trim
        trim(line);
        if (line.empty()) continue;

        // 4) built-in exit
        if (line == "exit") break;

        // 5) tokenize -> argv for execvp
        std::vector<std::string> words;
        std::vector<char*> argv;
        tokenize(line, words, argv);

        if (words.empty()) continue;  // defensive

        // 6) fork
        pid_t pid = fork();
        if (pid < 0) {
            std::perror("fork");
            continue;
        }

        if (pid == 0) {
            // ---- child ----
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
