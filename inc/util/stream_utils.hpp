#ifndef UTIL_STREAM_UTILS_HPP
#define UTIL_STREAM_UTILS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>

namespace util
{
  /**
   * Reads until the end of line ('\n') or end of file ('\0'), and returns the string read.
   * This includes any line that doesn't end with newline before EOF.
   *
   * To detect if the last line in the file does not end with a newline just before the end of file, the function
   * will return a non-empty string, but the stream state will be failed
   *  ```
   *  while (readline(stream, line).size() != 0) {
   *      if (not stream) {
   *          std::cout << "no newline at end of file: " << line << std::endl;
   *      }
   *  }
   *  ```
   * @param stream input stream that will be consumed
   * @param container will be cleared and filled with the string from the current stream position until
   * the next newline or eof
   * @return the string read. this is another reference to the second parameter.
   */
  template <typename Container>
  Container &readline(std::istream & stream, Container & container)
  {
      char c;
      container.clear();

      // using operator bool: http://www.cplusplus.com/reference/ios/ios/fail/
      // `stream.get()` sets failbit as well on eof
      while (stream && stream.get(c)) {
          container.push_back(c);
          if (c == '\n') {
              break;
          }
      }

      return container;
  }

  /**
   * Read n characters from the input stream and returns the string read.
   *
   * @tparam Container any container type that implements clear() and push_back(char) methods
   * @param stream input stream that will be consumed
   * @param container will be cleared and filled with the string from the offset stream position from the beginning
   * until n characters are read or the end of stream is reached
   * @param n the number of characters to be read. Note the returned container may contain less than n characters
   * @param off the offset value, relative to the 'way' parameter
   * @param way the position the offset is relative to
   * @return
   */
  template <typename Container>
  Container &read_n(std::istream & stream, Container & container, const std::streamsize n,
          const std::streamoff off = 0, std::ios_base::seekdir way = std::ios::beg)
  {
      char c;
      container.clear();

      stream.seekg(off, way);
      std::streamsize count = 0;

      // using operator bool: http://www.cplusplus.com/reference/ios/ios/fail/
      // `stream.get()` sets failbit as well on eof
      while (count < n && stream && stream.get(c)) {
          container.push_back(c);
          ++count;
      }

      return container;
  }

  template <typename Container>
  std::ostream & writeline(std::ostream & stream, const Container & container)
  {
      for (auto c : container) {
          stream << c;
      }
      return stream;
  }

  template <typename F, typename S>
  std::ostream &operator<<(std::ostream &os, const std::pair<F, S> &container)
  {
      return os << "{" << container.first << ", " << container.second << "}";
  }

  /**
   * This is a generic method that prints a collection if its inner type is printable with "operator<<"
   *
   * @param os ostream to write into
   * @param container must have he operations ".begin()" and ".end()"
   * defined and they provide iterators, and the inner type has an overloaded "operator<<"
   * @param open_tag will be written before the first element (e.g. "[", if you want something like "[2,4]"), can be an empty string
   * @param separator_tag will be written between elements (e.g. "," in "[2,4]"), can be empty string
   * @param close_tag will be written after the last element (e.g. "]" in "[2,4]"), can be emtpy string
   * @return the ostream passed, in the questionable case you want to do `print_containter(cout, ...) << "more messages"`;
   */
  template <typename T>
  std::ostream &print_container(std::ostream &os, const T &container,
                                std::string open_tag, std::string separator_tag, std::string close_tag) {
      os << open_tag;
      auto it = container.begin();
      auto end = container.end();
      if (it != end) {
          os << *it;
          for (++it; it != container.end(); ++it) {
              os << separator_tag << *it;
          }
      }
      os << close_tag;
      return os;
  }

  /**
   * This is a generic method that prints a collection if it has the operations ".begin()" and ".end()"
   * defined and they provide iterators, and the inner type has an overloaded "operator<<"
   *
   * provides the default open/close/separator tags to print collections like this: [2, 4]
   */
  template <typename T>
  std::ostream &print_container(std::ostream &os, const T &container) {
      return print_container(os, container, "[", ", ", "]");
  }

  // the next functions allow using the previous print_container in a easier syntax
  template <typename T>
  std::ostream &operator<<(std::ostream &os, const std::vector<T> &container)
  {
      return print_container<std::vector<T>>(os, container);
  }
  template <typename K, typename V>
  std::ostream &operator<<(std::ostream &os, const std::map<K, V> &container)
  {
      return print_container<std::map<K, V>>(os, container);
  }
  template <typename K, typename V>
  std::ostream &operator<<(std::ostream &os, const std::multimap<K, V> &container)
  {
      return print_container<std::multimap<K, V>>(os, container);
  }
}


#endif // UTIL_STREAM_UTILS_HPP

