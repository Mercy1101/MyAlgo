#define CATCH_CONFIG_RUNNER

#include <iostream>

#include "catch/catch.hpp"  // include after defining CATCH_CONFIG_RUNNER
#include <unordered_map>
#include <vector>
#include <algorithm>
using namespace std;
    bool uniqueOccurrences(vector<int>& arr) {
      std::unordered_map<int,int> map;
      for(auto & it : arr) {
        map[it]++;
      }
      return std::all_of(map.begin(), map.end(), [](const std::pair<int, int>& p){
        return p.second <= 1;
      });
    }
int main(int argc, char** argv) {
  Catch::Session().run(argc, argv);
  std::vector<int> vec{1,2,2,1,1,3};
  auto b = uniqueOccurrences(vec);
  std::cout << boolalpha << b << std::endl;

  return 0;
}