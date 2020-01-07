#include "utility/utility.h"

#include <algorithm>
#include <cassert>
#include <catch2/catch.hpp>
#include <fstream>
#include <string>
#include <vector>

const char* FILE1 = "../CMakeLists.txt";
const char* FILE2 = "../LICENCE.txt";

std::vector<std::string> read_file(const char* filename) {
  std::vector<std::string> result;
  std::ifstream ifs(filename);
  assert(ifs);
  Lee::istream_line_reader reader{ifs};
  std::copy(reader.begin(), reader.end(), std::back_inserter(result));
  return result;
}

std::string convert_line_content(const std::vector<std::string>& lines) {
  std::string result;
  for (auto&& line : lines) {
    result += line;
    result += '\n';
  }
  return result;
}

const std::vector<std::string>& get_line_content() {
  static auto result = read_file(FILE1);
  return result;
}

const std::string& get_byte_content() {
  static auto result = convert_line_content(get_line_content());
  return result;
}

template <typename T>
void test_iterator(T& reader1, T& reader2) {
  auto it1 = reader1.begin();
  auto it2 = reader2.begin();
  assert(it1 != it2);
  auto it3 = it1;
  assert(it1 == it3);
  ++it1;
  assert(it1 != it3);
  assert(reader1.end() == reader1.end());
}

TEST_CASE("file_line_reader_test") {
  FILE* fp1 = nullptr;
  auto error_code1 = fopen_s(&fp1, FILE1, "r");
  REQUIRE(0 == error_code1);
  REQUIRE(fp1);

  FILE* fp2 = nullptr;
  auto error_code2 = fopen_s(&fp2, FILE2, "r");
  REQUIRE(0 == error_code2);
  REQUIRE(fp2);

  Lee::file_line_reader reader1{fp1};
  Lee::file_line_reader reader2{fp2};
  test_iterator(reader1, reader2);

  if (fp1 != nullptr) {
    fclose(fp1);
  }
  if (fp2 != nullptr) {
    fclose(fp2);
  }

  FILE* fp = nullptr;
  auto error_code3 = fopen_s(&fp, FILE1, "r");
  REQUIRE(0 == error_code3);
  REQUIRE(fp != nullptr);
  Lee::file_line_reader reader{fp};
  std::vector<std::string> file_content;
  for (auto line : reader) {
    std::cout << line << std::endl;
    file_content.emplace_back(line);
  }
  REQUIRE(file_content.size() == get_line_content().size());
  REQUIRE(std::equal(file_content.begin(), file_content.end(),
                     get_line_content().begin()));
}
