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

void soundex_impl(vef_context_t* ctx, vef_invalue_t* input,
                  vef_vdf_result_t* result) {
  if (input->is_null || input->str_len == 0) {
    result->type = VEF_RESULT_NULL;
    return;
  }

  std::string src(input->str_value, input->str_len);

  // Soundex: keep first letter, map consonants to digits
  static const char table[] = {
    //  A  B  C  D  E  F  G  H  I  J  K  L  M
       '0','1','2','3','0','1','2','0','0','2','2','4','5',
    //  N  O  P  Q  R  S  T  U  V  W  X  Y  Z
       '5','0','1','2','6','2','3','0','1','0','2','0','2'
  };

  std::string code;
  code += static_cast<char>(std::toupper(static_cast<unsigned char>(src[0])));

  char last = table[std::toupper(static_cast<unsigned char>(src[0])) - 'A'];

  for (size_t i = 1; i < src.size() && code.size() < 4; ++i) {
    char c = std::toupper(static_cast<unsigned char>(src[i]));
    if (c < 'A' || c > 'Z') continue;
    char mapped = table[c - 'A'];
    if (mapped != '0' && mapped != last) {
      code += mapped;
    }
    last = mapped;
  }

  while (code.size() < 4) code += '0';

  std::memcpy(result->str_buf, code.c_str(), code.size());
  result->actual_len = code.size();
  result->type = VEF_RESULT_VALUE;
}
