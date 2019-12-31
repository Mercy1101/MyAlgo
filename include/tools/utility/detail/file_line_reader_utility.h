///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////
/// Copyright (c) 2019 Lijiancong. All rights reserved.
///
/// Use of this source code is governed by a MIT license
/// that can be found in the License file.
///
/// @file   file_line_reader.h
/// @brief  实现了一个按行读取文件的class
///
/// @author lijiancong, pipinstall@163.com
///         Token From https://github.com/adah1972/nvwa/nvwa/file_line_reader.h
/// @date   2019-12-31 14:57:58
///////// ///////// ///////// ///////// ///////// ///////// ///////// /////////

#ifndef MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_FILE_LINE_READER_UTILITY_H_
#define MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_FILE_LINE_READER_UTILITY_H_

#include <assert.h>  // assert
#include <stddef.h>  // ptrdiff_t
#include <stdio.h>   // file streams
#include <string.h>  // memcpy

#include <istream>    // std::istream
#include <iterator>   // std::input_iterator_tag
#include <stdexcept>  // std::runtime_error
#include <string>     // std::string
#include <utility>    // std::swap

#include "utility/detail/marco_utility.h"

namespace Lee {
inline namespace utility {
inline namespace file_line_reader {
const size_t BUFFER_SIZE = 256;

static inline char* expand(char* data, size_t size, size_t capacity) {
  char* new_ptr = new char[capacity];
  memcpy(new_ptr, data, size);
  delete[] data;
  return new_ptr;
}

/** Class to allow iteration over all lines of a text file. */
class file_line_reader {
 public:
  /**
   * Iterator that contains the line content.
   *
   * The iterator \e owns the content.
   */
  class iterator {  // implements InputIterator
   public:
    typedef ptrdiff_t difference_type;
    typedef char* value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::input_iterator_tag iterator_category;

    iterator() noexcept
        : _M_reader(nullptr),
          _M_offset(0),
          _M_line(nullptr),
          _M_size(0),
          _M_capacity(0) {}
    /**
     * Constructs the beginning iterator.
     *
     * @param reader  pointer to the file_line_reader object
     */
    explicit iterator(file_line_reader* reader)
        : _M_reader(reader), _M_offset(0), _M_size(0) {
      _M_line = new char[BUFFER_SIZE];
      _M_capacity = BUFFER_SIZE;
      ++*this;
    }
    ~iterator() { delete[] _M_line; }

    /**
     * Copy-constructor.  The line content will be copied to the newly
     * constructed iterator.
     *
     * @param rhs  the iterator to copy from
     */
    iterator(const iterator& rhs)
        : _M_reader(rhs._M_reader),
          _M_offset(rhs._M_offset),
          _M_size(rhs._M_size),
          _M_capacity(rhs._M_capacity) {
      if (rhs._M_line) {
        _M_line = new char[rhs._M_capacity];
        memcpy(_M_line, rhs._M_line, _M_size + 1);
      }
    }
    /**
     * Assignment.  The line content will be copied to the newly constructed
     * iterator.
     *
     * @param rhs  the iterator to copy from
     */
    file_line_reader::iterator& operator=(const iterator& rhs) {
      iterator temp(rhs);
      swap(temp);
      return *this;
    }

    /**
     * Move constructor. The line content will be moved to the newly
     * constructed iterator.
     *
     * @param rhs  the iterator to move from
     */
    iterator(iterator&& rhs) noexcept
        : _M_reader(rhs._M_reader),
          _M_offset(rhs._M_offset),
          _M_line(rhs._M_line),
          _M_size(rhs._M_size),
          _M_capacity(rhs._M_capacity) {
      rhs._M_reader = nullptr;
      rhs._M_offset = 0;
      rhs._M_line = nullptr;
      rhs._M_size = 0;
      rhs._M_capacity = 0;
    }
    /**
     * Move assignment. The line content will be moved to the newly
     * constructed iterator.
     *
     * @param rhs  the iterator to move from
     */
    file_line_reader::iterator& operator=(iterator&& rhs) noexcept {
      iterator temp(std::move(rhs));
      swap(temp);
      return *this;
    }

    /**
     * Swaps the iterator with another.
     *
     * @param rhs  the iterator to swap with
     */
    void swap(file_line_reader::iterator& rhs) noexcept {
      std::swap(_M_reader, rhs._M_reader);
      std::swap(_M_offset, rhs._M_offset);
      std::swap(_M_line, rhs._M_line);
      std::swap(_M_size, rhs._M_size);
      std::swap(_M_capacity, rhs._M_capacity);
    }

    reference operator*() {
      assert(_M_reader != nullptr);
      return _M_line;
    }
    pointer operator->() {
      assert(_M_reader != nullptr);
      return &_M_line;
    }
    iterator& operator++() {
      if (!_M_reader->read(_M_line, _M_size, _M_capacity)) {
        _M_reader = nullptr;
        _M_offset = 0;
      } else {
        _M_offset = _M_reader->_M_offset;
      }
      return *this;
    }
    iterator operator++(int) {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    bool operator==(const iterator& rhs) const noexcept {
      return _M_reader == rhs._M_reader && _M_offset == rhs._M_offset;
    }
    bool operator!=(const iterator& rhs) const noexcept {
      return !operator==(rhs);
    }

    size_t size() const { return _M_size; }

   private:
    file_line_reader* _M_reader;
    size_t _M_offset;
    char* _M_line;
    size_t _M_size;
    size_t _M_capacity;
  };

  /** Enumeration of whether the delimiter should be stripped. */
  enum strip_type {
    strip_delimiter,     ///< The delimiter should be stripped
    no_strip_delimiter,  ///< The delimiter should be retained
  };

  /**
   * Constructor.
   *
   * @param stream     the file stream to read from
   * @param delimiter  the delimiter between text `lines' (default to LF)
   * @param strip      enumerator about whether to strip the delimiter
   */
  file_line_reader(FILE* stream, char delimiter, strip_type strip)
      : _M_stream(stream),
        _M_delimiter(delimiter),
        _M_strip_delimiter(strip == strip_delimiter),
        _M_offset(0),
        _M_read_pos(0),
        _M_size(0) {
    if (delimiter == '\n') {
      _M_buffer = nullptr;
    } else {
      _M_buffer = new char[BUFFER_SIZE];
    }
  }

  /** Destructor. */
  ~file_line_reader() { delete[] _M_buffer; }

  iterator begin() { return iterator(this); }
  iterator end() const noexcept { return iterator(); }

  /**
   * Reads content from the file stream.  If necessary, the receiving
   * buffer will be expanded so that it is big enough to contain all the
   * line content.
   *
   * @param[in,out] output    initial receiving buffer
   * @param[out]    size      size of the line
   * @param[in,out] capacity  capacity of the initial receiving buffer on
   *                          entering the function; it can be increased
   *                          when necessary
   * @return                  \c true if line content is returned;
   *                          \c false otherwise
   */
  bool read(char*& output, size_t& size, size_t& capacity) {
    bool found_delimiter = false;
    size_t write_pos = 0;

    if (_M_delimiter == '\n') {
      for (;;) {
        if (!fgets(output + write_pos, capacity - write_pos, _M_stream)) {
          break;
        }
        size_t len = strlen(output + write_pos);
        write_pos += len;
        if (output[write_pos - 1] == '\n') {
          found_delimiter = true;
          break;
        }
        if (write_pos + 1 == capacity) {
          output = expand(output, write_pos, capacity * 2);
          capacity *= 2;
        }
      }
    } else {
      for (;;) {
        if (_M_read_pos == _M_size) {
          _M_read_pos = 0;
          _M_size = fread(_M_buffer, 1, BUFFER_SIZE, _M_stream);
          if (_M_size == 0) {
            break;
          }
        }
        char ch = _M_buffer[_M_read_pos++];
        if (write_pos + 1 == capacity) {
          output = expand(output, write_pos, capacity * 2);
          capacity *= 2;
        }
        output[write_pos++] = ch;
        if (ch == _M_delimiter) {
          found_delimiter = true;
          break;
        }
      }
    }
    _M_offset += write_pos;

    if (write_pos != 0) {
      if (found_delimiter && _M_strip_delimiter) {
        --write_pos;
      }
      output[write_pos] = '\0';
      size = write_pos;
      return true;
    } else {
      return false;
    }
  }

 private:
  MYALGO_DISALLOW_COPY_AND_ASSIGN_(file_line_reader);

  FILE* _M_stream;
  char _M_delimiter;
  bool _M_strip_delimiter;
  char* _M_buffer;
  size_t _M_offset;
  size_t _M_read_pos;
  size_t _M_size;
  inline void swap(Lee::file_line_reader::iterator& lhs,
                   Lee::file_line_reader::iterator& rhs) noexcept
  {
    lhs.swap(rhs);
  }
};



class istream_line_reader {
 public:
  /**
   * Iterator that contains the line content.
   *
   * The iterator \e owns the content.
   */
  class iterator {  // implements InputIterator
   public:
    typedef ptrdiff_t difference_type;
    typedef std::string value_type;
    typedef const value_type* pointer;
    typedef const value_type& reference;
    typedef std::input_iterator_tag iterator_category;

    iterator() noexcept : _M_stream(nullptr) {}
    explicit iterator(std::istream& is) : _M_stream(&is) { ++*this; }

    reference operator*() const noexcept {
      assert(_M_stream != nullptr);
      return _M_line;
    }
    pointer operator->() const noexcept {
      assert(_M_stream != nullptr);
      return &_M_line;
    }
    iterator& operator++() {
      assert(_M_stream != nullptr);
      getline(*_M_stream, _M_line);
      if (!*_M_stream) {
        _M_stream = nullptr;
      }
      return *this;
    }
    iterator operator++(int) {
      iterator temp(*this);
      ++*this;
      return temp;
    }

    bool operator==(const iterator& rhs) const noexcept {
      // This implementation basically says, any iterators
      // pointing to the same stream are equal.  This behaviour
      // may seem a little surprising in the beginning, but, in
      // reality, it hardly has any consequences, as people
      // usually compare an input iterator only to the sentinel
      // object.  The alternative, using _M_stream->tellg() to
      // get the exact position, harms the performance too dearly.
      // I do not really have a better choice.
      //
      // If you do need to compare valid iterators, consider using
      // file_line_reader or mmap_line_reader.
      return _M_stream == rhs._M_stream;
    }
    bool operator!=(const iterator& rhs) const noexcept {
      return !operator==(rhs);
    }

   private:
    std::istream* _M_stream;
    std::string _M_line;
  };

  istream_line_reader() noexcept : _M_stream(nullptr) {}
  explicit istream_line_reader(std::istream& is) noexcept : _M_stream(&is) {}
  iterator begin() {
    if (!_M_stream) {
      throw std::logic_error("input stream is null");
    }
    if (_M_stream->fail()) {
      throw std::runtime_error("input stream error");
    }
    return iterator(*_M_stream);
  }
  iterator end() const noexcept { return iterator(); }

 private:
  std::istream* _M_stream;
};

}  // namespace file_line_reader
}  // namespace utility
}  // namespace Lee

#endif  // MYALGO_INCLUDE_TOOLS_UTILITY_DETAIL_FILE_LINE_READER_UTILITY_H_