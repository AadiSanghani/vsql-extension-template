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
#include <villagesql/func_builder.h>

std::string soundex_impl(const std::string& input);
int64_t levenshtein_impl(const std::string& source, const std::string& target);

using namespace villagesql::extension_builder;
using namespace villagesql::func_builder;

VEF_GENERATE_ENTRY_POINTS(
  make_extension("fuzzystrmatch", "1.0.0")
    .func(make_func("soundex")
      .returns(STRING)
      .param(STRING)
      .wrap<&soundex_impl>())
    .func(make_func("levenshtein")
      .returns(INT)
      .param(STRING)
      .param(STRING)
      .wrap<&levenshtein_impl>())
    .build()
);
