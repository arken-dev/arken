// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <arken/net/HttpClient>
#include <arken/digest/md5.h>
#include <arken/digest/sha1.h>
#include <regex>
#include <filesystem>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <set>
#include <vector>

namespace fs = std::filesystem;
using path   = std::filesystem::path;
using string = arken::string;
using List   = arken::string::List;

static
std::time_t to_time_t(std::filesystem::file_time_type tp)
{
  auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>
    (tp - std::filesystem::file_time_type::clock::now()
      + std::chrono::system_clock::now());
  return std::chrono::system_clock::to_time_t(sctp);
}


namespace arken {

bool os::compare(const char * path1, const char * path2)
{
  string buffer1;
  string buffer2;

  buffer1 = os::read(path1);
  buffer2 = os::read(path2);

  if( buffer1.size() != buffer2.size() ) {
    return false;
  }

  if( memcmp(buffer1.data(), buffer2.data(), buffer1.size()) == 0 ) {
    return true;
  } else {
    return false;
  }
}

void os::sleep(double msecs)
{
  auto value = int(msecs * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(value));
}

long os::size(const char * path)
{
  // opening the file in read mode
  FILE* fp = fopen(path, "r");

  // checking if the file exist or not
  if (fp == nullptr) {
    printf("File Not Found!\n");
    return -1;
  }

  fseek(fp, 0L, SEEK_END);

  // calculating the size of the file
  long res = ftell(fp);

  // closing the file
  fclose(fp);

  return res;
}

unsigned int os::cores()
{
  return std::thread::hardware_concurrency();
}

// Divide o padrão em componentes de diretório, um por nível (separador "/").
std::vector<std::string> glob_split(const std::string& pattern) {
  std::vector<std::string> segments;
  std::string current;

  for (char c : pattern) {
    if (c == '/') {
      segments.push_back(current);
      current.clear();
    } else {
      current += c;
    }
  }
  segments.push_back(current);

  return segments;
}

bool glob_has_wildcard(const std::string& segment) {
  return segment.find_first_of("*?[") != std::string::npos;
}

// Converte um único componente do glob (nunca contém "/") em regex,
// já que cada nível de diretório é resolvido separadamente.
std::regex glob_segment_to_regex(const std::string& segment) {
  std::string regex_str = "^";

  for (size_t i = 0; i < segment.size(); ++i) {
    char c = segment[i];
    switch (c) {
      case '*': regex_str += "[^/]*"; break;
      case '?': regex_str += "[^/]"; break;
      case '.': case '+': case '^': case '$':
      case '(': case ')': case '|': case '\\':
        regex_str += '\\';
        regex_str += c;
        break;
      case '[': {
        size_t close = segment.find(']', i + 1);
        if (close == std::string::npos) {
          regex_str += "\\[";
        } else {
          regex_str += segment.substr(i, close - i + 1);
          i = close;
        }
        break;
      }
      default: regex_str += c; break;
    }
  }

  regex_str += "$";
  return std::regex(regex_str);
}

// Casa recursivamente os componentes do padrão a partir de "base", um nível
// de diretório por vez. "**" é tratado à parte pois pode representar zero ou
// mais níveis de diretório, diferente de "*" que representa exatamente um.
void glob_collect(const fs::path& base, const std::vector<std::string>& segments,
                   size_t idx, List& results) {
  if (idx == segments.size()) {
    std::error_code ec;
    if (fs::exists(base, ec)) {
      results.append(base.string().c_str());
    }
    return;
  }

  const std::string& segment = segments[idx];

  // Componente vazio (ex.: "//" no meio do padrão) apenas avança um nível.
  if (segment.empty()) {
    glob_collect(base, segments, idx + 1, results);
    return;
  }

  std::error_code ec;

  if (segment == "**") {
    // "**" casando com zero diretórios: aplica o restante do padrão aqui mesmo.
    glob_collect(base, segments, idx + 1, results);

    fs::path dir = base.empty() ? fs::path(".") : base;
    if (!fs::is_directory(dir, ec)) {
      return;
    }

    for (auto& entry : fs::directory_iterator(dir, ec)) {
      fs::path child = base / entry.path().filename();

      if (entry.is_directory()) {
        // Diretório: continua expandindo "**" mais fundo (que por sua vez
        // volta a tentar casar o restante do padrão a partir daqui).
        glob_collect(child, segments, idx, results);
      } else {
        // Arquivo: só pode ser o fim da expansão de "**", não há como descer mais.
        glob_collect(child, segments, idx + 1, results);
      }
    }
    return;
  }

  fs::path dir = base.empty() ? fs::path(".") : base;
  bool last = (idx + 1 == segments.size());

  if (!glob_has_wildcard(segment)) {
    fs::path next = base / segment;
    if (last) {
      if (fs::exists(next, ec)) {
        results.append(next.string().c_str());
      }
    } else if (fs::is_directory(next, ec)) {
      glob_collect(next, segments, idx + 1, results);
    }
    return;
  }

  if (!fs::is_directory(dir, ec)) {
    return;
  }

  std::regex exp = glob_segment_to_regex(segment);

  for (auto& entry : fs::directory_iterator(dir, ec)) {
    std::string name = entry.path().filename().string();
    if (!std::regex_match(name, exp)) {
      continue;
    }

    fs::path child = base / entry.path().filename();

    if (last) {
      results.append(child.string().c_str());
    } else if (entry.is_directory()) {
      glob_collect(child, segments, idx + 1, results);
    }
  }
}

List os::glob(string full_path_pattern)
{
  List list;

  std::string pattern(full_path_pattern.data());
  if (pattern.empty()) {
    return list;
  }

  fs::path start;
  std::vector<std::string> segments;

  if (pattern.front() == '/') {
    start = "/";
    segments = glob_split(pattern.substr(1));
  } else {
    segments = glob_split(pattern);
  }

  glob_collect(start, segments, 0, list);

  return list;
}

List os::find(const char * dir, const char * rgx, bool recursive)
{

  List list;
  std::regex exp(rgx);

  if( ! os::exists(dir) ) {
    return list;
  }

  if( recursive ) {
    for(auto& p: fs::recursive_directory_iterator(dir)) {
       std::smatch matches;
       std::string path(p.path().string());
       if( std::regex_search(path, matches, exp) ) {
         list.append( std::string(path).c_str() );
       }
    }
  } else {
    for(auto& p: fs::directory_iterator(dir)) {
       std::smatch matches;
       std::string path(p.path().string());
       if( std::regex_search(path, matches, exp) ) {
         list.append( std::string(path).c_str() );
       }
    }
  }

  return list;
}

double os::microtime()
{
  uint64_t value = std::chrono::system_clock::now().time_since_epoch() / std::chrono::microseconds(1);
  return (double) value / 1.0e6;
}

bool os::touch(const char * path)
{
  bool flag;
  std::fstream fs;

  flag = false;
  fs.open(path, std::ios::out);
  if( fs.is_open() ) {
    flag = true;
  }

  fs.close();
  return flag;
}

string os::read(const char * path)
{
  using HttpClient = arken::net::HttpClient;

  if (string::startsWith(path, "http://") || string::startsWith(path, "https://") ) {
    HttpClient client(path);
    return client.performGet();
  } else {
    char * buffer;
    std::ifstream file;
    size_t length;
    file.open(path);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new char[length+1];
    file.read(buffer, length);
    file.close();
    buffer[length] = '\0';
    return string( std::move(buffer), length );
  }

}

string os::abspath(const char * p)
{
  path path = std::filesystem::absolute(p);
  return string(path.string());
}

string os::basename(const char * p)
{
  path path = std::filesystem::absolute(p).filename();
  return string(path.string());
}

size_t os::atime(const char * p)
{
  auto tp = std::filesystem::last_write_time(p);
  return to_time_t(tp);
}

bool os::copy(const char * source, const char * destination, bool force = false)
{

  if( os::isdir(source) ) {
    std::filesystem::copy(source, destination,
      std::filesystem::copy_options::overwrite_existing |
      std::filesystem::copy_options::recursive);
  } else {
    std::filesystem::copy(source, destination);
  }

  return true;
}

bool os::chdir(const char * dirpath)
{
  std::filesystem::current_path(dirpath);
  path p = std::filesystem::current_path();
  return p.string().compare(os::pwd());
}

size_t os::ctime(const char * p)
{
  auto t = std::filesystem::last_write_time(p);
  return to_time_t(t);
}

string os::dirpath(const char * p)
{
  path path = std::filesystem::absolute(p).parent_path();
  return string(path.string());
}

size_t os::du(const char * path)
{
  uintmax_t blocks512 = 0;
  std::set<std::pair<dev_t, ino_t>> seen;

  auto add = [&](const char * p) {
    struct stat st;
    if (lstat(p, &st) != 0) return;
    if (st.st_nlink > 1) {
      if (!seen.emplace(st.st_dev, st.st_ino).second) return;
    }
    blocks512 += st.st_blocks;
  };

  std::error_code ec;
  add(path);

  if (fs::is_directory(path)) {
    fs::recursive_directory_iterator it(path, fs::directory_options::skip_permission_denied, ec);
    for (; it != fs::recursive_directory_iterator(); it.increment(ec)) {
      if (ec) break;
      add(it->path().c_str());
    }
  }

  return (blocks512 * 512) / 1024;
}

bool os::exists(const char * path)
{
  return std::filesystem::exists(path);
}

string os::home()
{
  const char * p = getenv("HOME");
  if( p == nullptr ) {
    return {};
  } else {
    path path = std::filesystem::absolute(p);
    return string(path.string());
  }
}

bool os::isdir(const char * path)
{
  return std::filesystem::is_directory(path);
}

bool os::isfile(const char * path)
{
  return std::filesystem::is_regular_file(path);
}

bool os::islink(const char * path)
{
  return std::filesystem::is_symlink(path);
}

bool os::link(const char * source, const char * destination, bool force = false)
{
  if( force and os::exists(destination) ) {
    std::filesystem::remove(destination);
  }
  std::filesystem::create_symlink(source, destination);

  return os::exists(destination);
}

bool os::mkdir(const char * dirname)
{
  return std::filesystem::create_directory(dirname);
}

bool os::mkpath(const char * dirpath)
{
  return std::filesystem::create_directories(dirpath);
}

string os::pwd()
{
  path cwd = std::filesystem::current_path();
  return string(cwd.string());
}

bool os::rmdir(const char * dirpath)
{
  if (std::filesystem::is_directory(dirpath) && std::filesystem::is_empty(dirpath)) {
    uintmax_t result = std::filesystem::remove_all(dirpath);
    return result > 0;
  }

  return false;
}

bool os::rmpath(const char * dirpath)
{
  uintmax_t result = std::filesystem::remove_all(dirpath);
  return result > 0;
}

string os::target(const char * p)
{
  path target = std::filesystem::read_symlink(p);
  return string(target.string());
}

string os::tmp()
{
  path tmp = std::filesystem::temp_directory_path();
  return string(tmp.string());
}

string os::root()
{
  path root = std::filesystem::current_path().root_directory();
  return string(root.string());
}

string os::md5(const char * path)
{
  return arken::digest::md5::file(path);
}

string os::sha1(const char * path)
{
  return arken::digest::sha1::file(path);
}

} // namespace arken
