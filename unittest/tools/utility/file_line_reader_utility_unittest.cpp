#include <catch2/catch.hpp>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <>
#include <>


const char* FILE1 = "../LICENCE";
const char* FILE2 = "../README";

std::vector<std::string> read_file(const char* filename)
{
    std::vector<std::string> result;
    std::ifstream ifs(filename);
    assert(ifs);
    Lee::istream_line_reader reader{ifs};
    std::copy(reader.begin(), reader.end(), std::back_inserter(result));
    return result;
}

std::string convert_line_content(const std::vector<std::string>& lines)
{
    std::string result;
    for (auto&& line : lines) {
        result += line;
        result += '\n';
    }
    return result;
}

const std::vector<std::string>& get_line_content()
{
    static auto result = read_file(FILE1);
    return result;
}

const std::string& get_byte_content()
{
    static auto result = convert_line_content(get_line_content());
    return result;
}

template <typename T>
void test_iterator(T& reader1, T& reader2)
{
    auto it1 = reader1.begin();
    auto it2 = reader2.begin();
    assert(it1 != it2);
    auto it3 = it1;
    assert(it1 == it3);
    ++it1;
    assert(it1 != it3);
    assert(reader1.end() == reader1.end());
}

TEST_CASE("file_line_reader_test")
{
    FILE* fp1 = fopen(FILE1, "r");
    FILE* fp2 = fopen(FILE2, "r");
    REQUIRE(fp1 && fp2);
    {
        Lee::file_line_reader reader1{fp1};
        Lee::file_line_reader reader2{fp2};
        test_iterator(reader1, reader2);
    }
    fclose(fp1);
    fclose(fp2);

    FILE* fp = fopen(FILE1, "r");
    REQUIRE(fp);
    Lee::file_line_reader reader{fp};
    std::vector<std::string> file_content;
    for (const char* line : reader) {
        file_content.emplace_back(line);
    }
    REQUIRE(file_content.size() == get_line_content().size());
    REQUIRE(std::equal(file_content.begin(), file_content.end(),
                           get_line_content().begin()));
}


