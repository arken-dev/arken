// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/utils/glob.h>

namespace arken {
namespace utils {

bool glob::hasWildcard(const std::string & segment)
{
  return segment.find_first_of("*?[") != std::string::npos;
}

// Casa um caractere contra uma classe "[...]" do padrão (colchetes já
// localizados pelo chamador). Suporta faixas ("a-z") e negação ("[^...]"),
// nos mesmos moldes de uma classe de caracteres de regex.
bool glob::matchClass(const std::string & pattern, size_t start, size_t end, char c)
{
  bool negate = false;
  size_t i = start;

  if (i < end && pattern[i] == '^') {
    negate = true;
    ++i;
  }

  bool matched = false;
  while (i < end) {
    if (i + 2 < end && pattern[i + 1] == '-') {
      if (c >= pattern[i] && c <= pattern[i + 2]) {
        matched = true;
      }
      i += 3;
    } else {
      if (pattern[i] == c) {
        matched = true;
      }
      ++i;
    }
  }

  return negate ? !matched : matched;
}

// Casa "name" contra um padrão sem separador de diretório (um único
// componente de path do os::glob, ou uma string qualquer como chave de
// cache), caractere a caractere em vez de compilar/rodar uma regex —
// backtracking guloso clássico de wildcard matching, sem alocações. Suporta
// "*", "?" e classes "[...]"/"[^...]".
bool glob::match(const std::string & name, const std::string & pattern)
{
  size_t n = 0, p = 0;
  size_t star_p = std::string::npos, star_n = 0;

  while (n < name.size()) {
    if (p < pattern.size() && pattern[p] == '?') {
      ++n; ++p;
      continue;
    }

    if (p < pattern.size() && pattern[p] == '*') {
      star_p = p++;
      star_n = n;
      continue;
    }

    if (p < pattern.size() && pattern[p] == '[') {
      size_t close = pattern.find(']', p + 1);
      if (close != std::string::npos) {
        if (matchClass(pattern, p + 1, close, name[n])) {
          ++n;
          p = close + 1;
          continue;
        }
      } else if (pattern[p] == name[n]) {
        // "[" sem "]" correspondente é tratado como caractere literal.
        ++n; ++p;
        continue;
      }
    } else if (p < pattern.size() && pattern[p] == name[n]) {
      ++n; ++p;
      continue;
    }

    if (star_p != std::string::npos) {
      p = star_p + 1;
      n = ++star_n;
      continue;
    }

    return false;
  }

  while (p < pattern.size() && pattern[p] == '*') {
    ++p;
  }

  return p == pattern.size();
}

} // namespace utils
} // namespace arken
