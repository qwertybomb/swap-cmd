#include <cstring>
#include <iterator>
#include <filesystem>
#include <iostream>

static auto print_help() -> void {
    std::cout << "Usage: swap [file1] [file2]\n";
    std::cout << "swaps the contents of file1 and file2\n";
    std::cout << "use swap --help to print this message\n";
}

static auto validate_files(const std::filesystem::path &file1_path, const std::filesystem::path &file2_path) -> void {
    {
        /* check if file exists */
        const auto file1_exists = std::filesystem::exists(file1_path);
        const auto file2_exists = std::filesystem::exists(file2_path);
        const auto exists = file1_exists && file2_exists;


        if (!exists) {
            if (!file1_exists) std::cerr << "cannot find file " << file1_path.c_str() << '\n';
            if (!file2_exists) std::cerr << "cannot find file " << file2_path.c_str() << '\n';
            exit(EXIT_FAILURE);
        }
    }

    {
        if (file1_path == file2_path) {
            std::cerr << "swaping the same two files does nothing\n";
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    switch (argc) {
        case 2: {
            /* convert the second arg to upper case */
            std::transform(argv[1],
                           argv[1] + strlen(argv[1]),
                           argv[1],
                           ::toupper);
            if (!strcmp(argv[1], "--HELP")) {
                print_help();
                return EXIT_SUCCESS;
            }
        }
        case 3:
            break;
        default: {
            std::cerr << "Invalid args see --help for usage\n";
            return EXIT_FAILURE;
        }
    }
    const auto &file1_path = std::filesystem::path{argv[1]};
    const auto &file2_path = std::filesystem::path{argv[2]};


    validate_files(file1_path, file2_path);
    char temp_filename[L_tmpnam];
    const auto &temp_filepath = std::filesystem::current_path().append(std::tmpnam(temp_filename));
    std::filesystem::rename(file1_path, temp_filepath);
    std::filesystem::rename(file2_path, file1_path);
    std::filesystem::rename(temp_filepath, file2_path);
}
