/* Copyright (c) 2025 VillageSQL Contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <https://www.gnu.org/licenses/>.
 */

#include <villagesql/extension.h>

#include <cctype>
#include <cstring>
#include <string>

void metaphone_impl(vef_context_t* ctx, vef_invalue_t* input,
                    vef_invalue_t* max_phonemes, vef_vdf_result_t* result) {
  if (input->is_null || input->str_len == 0) {
    result->type = VEF_RESULT_NULL;
    return;
  }

  int max_len = static_cast<int>(max_phonemes->int_value);
  if (max_len <= 0) {
    result->type = VEF_RESULT_NULL;
    return;
  }

  // Uppercase and strip non-alpha
  std::string word;
  for (size_t i = 0; i < input->str_len; ++i) {
    char c = input->str_value[i];
    if (std::isalpha(static_cast<unsigned char>(c)))
      word += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
  }

  if (word.empty()) {
    result->type = VEF_RESULT_NULL;
    return;
  }

  auto is_vowel = [](char c) {
    return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
  };

  size_t len = word.size();
  auto at = [&](size_t i) -> char { return i < len ? word[i] : '\0'; };

  // Handle initial letter special cases
  size_t pos = 0;
  if ((at(0) == 'A' && at(1) == 'E') || (at(0) == 'G' && at(1) == 'N') ||
      (at(0) == 'K' && at(1) == 'N') || (at(0) == 'P' && at(1) == 'N') ||
      (at(0) == 'W' && at(1) == 'R')) {
    pos = 1;
  } else if (at(0) == 'X') {
    word[0] = 'S';
  }

  std::string code;

  for (; pos < len && static_cast<int>(code.size()) < max_len; ++pos) {
    char c = at(pos);

    if (c != 'C' && pos > 0 && c == at(pos - 1)) continue;

    switch (c) {
      case 'A': case 'E': case 'I': case 'O': case 'U':
        if (pos == 0) code += c;
        break;

      case 'B':
        if (!(pos > 0 && at(pos - 1) == 'M' && pos == len - 1))
          code += 'B';
        break;

      case 'C':
        if (at(pos + 1) == 'I' && at(pos + 2) == 'A') {
          code += 'X';  // CIA -> X (sh)
        } else if (at(pos + 1) == 'H') {
          code += 'X';  // CH -> X (sh)
          ++pos;
        } else if (at(pos + 1) == 'I' || at(pos + 1) == 'E' ||
                   at(pos + 1) == 'Y') {
          code += 'S';  // soft C
        } else {
          code += 'K';  // hard C
        }
        break;

      case 'D':
        if (at(pos + 1) == 'G' &&
            (at(pos + 2) == 'I' || at(pos + 2) == 'E' ||
             at(pos + 2) == 'Y')) {
          code += 'J';
        } else {
          code += 'T';
        }
        break;

      case 'F':
        code += 'F';
        break;

      case 'G':
        if (pos + 1 < len && at(pos + 1) == 'H' && pos + 2 < len &&
            !is_vowel(at(pos + 2))) {
          break;
        }
        if (pos > 0 && at(pos + 1) == 'H' && pos + 2 >= len) {
          break;
        }
        if (pos > 0 && pos + 1 >= len && at(pos - 1) == 'G') {
          break;
        }
        if (at(pos + 1) == 'I' || at(pos + 1) == 'E' ||
            at(pos + 1) == 'Y') {
          if (pos > 0 && at(pos - 1) == 'G') {
            code += 'K';
          } else {
            code += 'J';
          }
        } else {
          code += 'K';
        }
        break;

      case 'H':
        if (is_vowel(at(pos + 1)) &&
            (pos == 0 || !is_vowel(at(pos - 1))))
          code += 'H';
        break;

      case 'J':
        code += 'J';
        break;

      case 'K':
        if (pos == 0 || at(pos - 1) != 'C')
          code += 'K';
        break;

      case 'L':
        code += 'L';
        break;

      case 'M':
        code += 'M';
        break;

      case 'N':
        code += 'N';
        break;

      case 'P':
        if (at(pos + 1) == 'H') {
          code += 'F';
          ++pos;
        } else {
          code += 'P';
        }
        break;

      case 'Q':
        code += 'K';
        break;

      case 'R':
        code += 'R';
        break;

      case 'S':
        if (at(pos + 1) == 'H' ||
            (at(pos + 1) == 'I' &&
             (at(pos + 2) == 'O' || at(pos + 2) == 'A'))) {
          code += 'X';
          if (at(pos + 1) != 'H') ++pos;  // skip extra char for SIA/SIO
          ++pos;
        } else {
          code += 'S';
        }
        break;

      case 'T':
        if (at(pos + 1) == 'I' &&
            (at(pos + 2) == 'A' || at(pos + 2) == 'O')) {
          code += 'X';
        } else if (at(pos + 1) == 'H') {
          code += '0';
          ++pos;
        } else {
          code += 'T';
        }
        break;

      case 'V':
        code += 'F';
        break;

      case 'W':
        if (is_vowel(at(pos + 1)))
          code += 'W';
        break;

      case 'X':
        code += 'K';
        code += 'S';
        break;

      case 'Y':
        if (is_vowel(at(pos + 1)))
          code += 'Y';
        break;

      case 'Z':
        code += 'S';
        break;

      default:
        break;
    }
  }

  if (code.empty()) {
    result->type = VEF_RESULT_NULL;
    return;
  }

  if (static_cast<int>(code.size()) > max_len) {
    code.resize(max_len);
  }

  std::memcpy(result->str_buf, code.c_str(), code.size());
  result->actual_len = code.size();
  result->type = VEF_RESULT_VALUE;
}
