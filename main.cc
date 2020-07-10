#include <cstring>
#include <iterator>
#include <filesystem>

static auto print_help() -> void
{
    printf("Usage: swap [file1] [file2]\n");
    printf("swaps the contents of file1 and file2\n");
    printf("use swap --help to print this message\n");
}

static auto validate_files(const std::filesystem::path& file1_path, const std::filesystem::path& file2_path) -> void
{
    {
        /* check if file exists */
        const auto file1_exists = std::filesystem::exists(file1_path);
        const auto file2_exists = std::filesystem::exists(file2_path);

        const auto exists = file1_exists && file2_exists;


        if (!exists) {
            if (!file1_exists) fprintf(stderr, "cannot find file %s\n", file1_path.c_str());
            if (!file2_exists) fprintf(stderr, "cannot find file %s\n", file2_path.c_str());
            exit(EXIT_FAILURE);
        }
    }

    {
        if(file1_path == file2_path) {
            fprintf(stderr,"swaping the same two files does nothing\n");
            exit(EXIT_SUCCESS);
        }
    }
}



int main(int argc, char** argv)
{
    if(argc == 2) {
        /* convert the second arg to upper case */
        std::transform(argv[1],
                       argv[1] + strlen(argv[1]),
                       argv[1],
                       ::toupper);
        if(!strcmp(argv[1],"--HELP")) {
            print_help();
            return 0;
        }
    } else if (argc != 3) {
        fprintf(stderr, "Invalid args see --help for usage\n");
        return EXIT_FAILURE;
    }
    const auto& file1_path = std::filesystem::path(argv[1]);
    const auto& file2_path = std::filesystem::path(argv[2]);

    validate_files(file1_path, file2_path);
    char temp_filename[L_tmpnam];
    const auto& temp_filepath = std::filesystem::current_path().append(std::tmpnam(temp_filename));
    std::filesystem::rename(file1_path, temp_filepath);
    std::filesystem::rename(file2_path, file1_path);
    std::filesystem::rename(temp_filepath, file2_path);
    std::filesystem::remove(temp_filepath);
}
