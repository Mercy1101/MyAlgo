///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019,2020 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   two_dimensional_vector_serialize.h
/// @brief  二维vector的序列化与反序列化
///
/// @author lijiancong, pipinstall@163.com
/// @date   2020-10-13 20:45:08
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef INCLUDE_DETAIL_TWO_DIMENSIONAL_VECTOR_SERIALIZE_HPP_
#define INCLUDE_DETAIL_TWO_DIMENSIONAL_VECTOR_SERIALIZE_HPP_

#include <vector>
#include <string>

namespace lee {
inline namespace serialize {
class two_dimensional_vector_serialize {
 public:
  std::string serialize(std::vector<std::vector<int>> vec) {
    std::string str = "[";
    for(auto& it : vec) {
      std::string temp = "[";
      for(auto& i : it) {
        temp += std::to_string(i) + ",";
      }
      temp += "],";
    }
    str.erase(std::prev(str.end()));
    str += "]";
    return str;
  }

  std::vector<std::vector<int>> deserialize(std::string str) {
    str.erase(str.begin());
    str.erase(std::prev(str.end()));
    
  }
};
}  // namespace serialize
}  // namespace lee

#endif  ///< end of INCLUDE_DETAIL_TWO_DIMENSIONAL_VECTOR_SERIALIZE_HPP_