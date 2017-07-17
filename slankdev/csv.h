
/*
 * MIT License
 *
 * Copyright (c) 2017 Hiroki SHIROKURA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * @file   slankdev/csv.h
 * @brief  csv file operation
 * @author Hiroki SHIROKURA
 * @date   2017.7.18
 */

#pragma once
#include <string>
#include <vector>
#include <string.h>
#include <slankdev/filefd.h>

namespace slankdev {

static inline std::vector<std::string> tokenize_by_comma(const char* str);

class csv {
  std::vector<std::vector<std::string>> raw_;
  size_t row;
  size_t col;
 public:

  void read(const char* filename);
  void dump(FILE* fp) const;
  size_t get_col() const { return col; }
  size_t get_row() const { return row; }
  std::string& get(size_t r, size_t c) { return raw_[r][c]; }
};


} /* namespace slankdev */




/*
 * Function Implementation
 */
namespace slankdev {

inline void csv::dump(FILE* fp) const
{
  for (size_t r=0; r<row; r++) {
    for (size_t c=0; c<col; c++) {
      fprintf(fp, "%s,", raw_[r][c].c_str());
    }
    printf("\n");
  }
}

inline void csv::read(const char* filename)
{
  slankdev::filefd file;
  file.fopen(filename, "r");

  char str[1000];
  row = 0;
  while (file.fgets(str, sizeof(str))) {
    str[strlen(str)-2] = '\0';
    raw_.push_back(tokenize_by_comma(str));
    row ++;

#if 0 // for debug
    if (row > 10) break;
#endif
  }

  size_t max_col = 0;
  size_t nb_row = raw_.size();
  for (size_t i=0; i<nb_row; i++) {
    if (raw_[i].size() > max_col) max_col = raw_[i].size();
  }
  for (size_t i=0; i<nb_row; i++) {
    raw_[i].resize(max_col);
  }
  col = max_col;
}


static inline std::vector<std::string> tokenize_by_comma(const char* str)
{
  std::vector<std::string> vec;
  std::string s;
  for (size_t i=0; i<strlen(str); i++) {
    switch (str[i]) {
      case '\'':
      case '\"':
      case '\t':
        break;
      case ',':
        vec.push_back(s);
        s = "";
        break;
      case '\n':
        goto RET;
        break;
      default:
        s += str[i];
        break;
    }
  }
RET:
  vec.push_back(s);
  return vec;
}

} /* namespace slankdev */
