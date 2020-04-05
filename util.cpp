#include "util.h"

std::string trim(const std::string& str, const std::string& whitespace) {
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == std::string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
}

char digits[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
bool digits_only(const std::string& str) {
  int count = 0;
  for (int i = 0; i < str.length(); i++) {
    for (int j = 0; j < 10; j++) {
      if (str[i] == digits[j])
        count++;
    }
  }
  
  return count == str.length();
}