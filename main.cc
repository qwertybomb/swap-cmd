#include <iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include <iterator>
#include <filesystem>


static auto remove_case(char str[]) -> char*
{
    for (char* i = str; *i = static_cast<char>(std::toupper(*i)), *++i;);
    return str;
}
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

        const auto exists = file1_exists || file2_exists;


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
    using vector_t = std::vector<char>;
    if(argc == 2) {
        if(!strcmp(remove_case(argv[1]),"--HELP")) {
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

    /* open files */
    std::fstream file1{file1_path},
    file2{file2_path};

    /* copy files to memory */
    vector_t file1_data{std::istreambuf_iterator<char>(file1), std::istreambuf_iterator<char>()};
    vector_t file2_data{std::istreambuf_iterator<char>(file2), std::istreambuf_iterator<char>()};

    /* swap contents of data */
    std::swap(file1_data, file2_data);

    /* set writing cursor to the beginning of the file */
    file1.seekp(0), file2.seekp(0);
    file1.write(file1_data.data(),file1_data.size());
    file2.write(file2_data.data(),file2_data.size());

    /* resize files to their new sizes */
    std::filesystem::resize_file(file1_path, file1_data.size());
    std::filesystem::resize_file(file2_path, file2_data.size());

    /* close files */
    file1.close();
    file2.close();
}
