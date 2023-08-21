#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>

void parsingOptions(const std::vector<char *> &options, bool needOptions[]) {
    for (int j = 0; j < options.size(); j++) {
        if (strcmp(options[j], "-l") == 0 or strcmp(options[j], "--lines") == 0) {
            needOptions[0] = true;

        } else if (strcmp(options[j], "-c") == 0 or strcmp(options[j], "--bytes") == 0) {
            needOptions[2] = true;

        } else if (strcmp(options[j], "-w") == 0 or strcmp(options[j], "--words") == 0) {
            needOptions[1] = true;

        } else if (strcmp(options[j], "-m") == 0 or strcmp(options[j], "--chars") == 0) {
            needOptions[3] = true;
        }
            // считывание опций типа -wlc, -cm
        else {
            for (int q = 1; q < strlen(options[j]); q++) {
                if (options[j][q] == 'l') {
                    needOptions[0] = true;
                } else if (options[j][q] == 'w') {
                    needOptions[1] = true;
                } else if (options[j][q] == 'c') {
                    needOptions[2] = true;
                } else if (options[j][q] == 'm') {
                    needOptions[3] = true;
                }
                    // введена неверная команда
                else {
                    needOptions[4] = true;
                }
            }
        }
    }
};


struct fileInfo {
    char *path;
    size_t lines;
    size_t words;
    size_t bytes;
    size_t chars;
};

// считает количество печатных непробельных символов
size_t chars(const char *path) {
    std::ifstream fin(path);
    size_t counter = 0;
    char ch;
    while (fin.get(ch)) {
        if (ch == ' ') {
            continue;
        }
        if (isprint(ch)) {
            counter++;
        }
    }
    return counter;
}

// количество строк равно количеству "\n"
size_t lines(const char *path) {
    std::ifstream fin(path);
    char s;
    size_t counter = 0;
    while (fin.get(s)) {
        if (s == '\n') {
            counter++;
        }
    }
    return counter;
}

size_t bytes(const char *path) {
    size_t counter = 0;
    std::ifstream fin(path);
    fin.seekg(0, std::ios_base::end);
    counter = fin.tellg();
    return counter;
}

size_t words(const char *path) {
    std::ifstream fin(path);
    size_t counter = 0;
    std::string s;
    bool flag = false;

    for (fin >> s; fin.good(); fin >> s) {
        flag = true;
        counter++;
    }
    // так как в конце цикла седуюющее слово считается,но в след цикл не зайдет, так как достигнут конец файла и kounter не увеличится
    if (flag) {
        counter++;
    }
    return counter;
}


int main(int argc, char **argv) {
    std::vector<char *> files;
    std::vector<char *> options;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            options.push_back(argv[i]);
        } else {
            files.push_back(argv[i]);
        }
    }

    //needOptions[0]-?нужны ли lines,[1]-?words,[2]-?bytes,[3]-?chars,[4]-?есть ли неверные опции
    bool needOptions[5] = {false, false, false, false, false};
    parsingOptions(options, needOptions);

    if (needOptions[4]) {
        std::cout << "Ошибка при введении опций" << std::endl;
        exit(1);
    }

    for (int i = 0; i < files.size(); i++) {
        fileInfo F = {files[i], 0, 0, 0, 0};
        std::ifstream fin(F.path);

        if (!fin.is_open()) {
            std::cout << "Ошибка открытия файла " << F.path << std::endl;
            continue;
        }

        if (options.size() == 0) {
            std::cout << "lines words bytes chars " << F.path << std::endl;
            continue;
        }

        if (needOptions[0]) {
            F.lines = lines(F.path);
            std::cout << F.lines << " ";
        }
        if (needOptions[1]) {
            F.words = words(F.path);
            std::cout << F.words << " ";
        }
        if (needOptions[2]) {
            F.bytes = bytes(F.path);
            std::cout << F.bytes << " ";
        }
        if (needOptions[3]) {
            F.chars = chars(F.path);
            std::cout << F.chars << " ";
        }

        std::cout << F.path << std::endl;
    }
    return 0;
}
