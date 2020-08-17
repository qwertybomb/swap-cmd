#include <iostream>
#include <filesystem>
#include <cstring>
#include <cassert>

static auto print_help() -> void {
    std::cerr << "Usage: swap [file1] [file2]\n";
    std::cerr << "Swaps the contents of file1 and file2\n";
    std::cerr << "Use swap to print this message\n";
}

static auto validate_files(const std::filesystem::path& file1_path, const std::filesystem::path& file2_path) -> void {
    {
        /* check if the files exists */
        const auto file1_exists = std::filesystem::exists(file1_path);
        const auto file2_exists = std::filesystem::exists(file2_path);
        const auto exists = file1_exists && file2_exists;


        if (!exists) {
            if (!file1_exists) std::cerr << "cannot find file " << file1_path << '\n';
            if (!file2_exists) std::cerr << "cannot find file " << file2_path << '\n';
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

static auto get_temp_filename(char* template_name) -> void {
    /* tmpnam_s does not work on linux */
#if defined(WIN32) || defined(_WIN32)
    errno_t err = tmpnam_s(template_name, L_tmpnam);
    assert(!err);
#else
    int err = mkstemp(template_name);
    assert(err != -1);
#endif
}

int main(int argc, char** argv) {
    std::ios::sync_with_stdio(false);
    if (argc != 3) {
        print_help();
        return EXIT_FAILURE;
    }
    const auto file1_path = std::filesystem::path{ argv[1] };
    const auto file2_path = std::filesystem::path{ argv[2] };


    validate_files(file1_path, file2_path);
    char temp_filename[L_tmpnam] = "XXXXXX";
    get_temp_filename(temp_filename);
    const auto temp_filepath = std::filesystem::path{ temp_filename };
    /* move-swap the files instead of copy-swaping */
    /* renaming a file is the same as moving it */
    const auto rename_file = [](const auto& old_file, const auto& new_file) {
        std::error_code error;
        std::filesystem::rename(old_file, new_file, error);
        if (error) {
            std::cerr << error.message() << '\n';
            exit(EXIT_FAILURE);
        }
    };
    rename_file(file1_path, temp_filepath);
    rename_file(file2_path, file1_path);
    rename_file(temp_filepath, file2_path);
}