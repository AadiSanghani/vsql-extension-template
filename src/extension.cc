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

using namespace villagesql::extension_builder;
using namespace villagesql::func_builder;

void soundex_impl(vef_context_t* ctx, vef_invalue_t* input,
                  vef_vdf_result_t* result);
void levenshtein_impl(vef_context_t* ctx, vef_invalue_t* source,
                      vef_invalue_t* target, vef_vdf_result_t* result);

VEF_GENERATE_ENTRY_POINTS(
  make_extension("fuzzystrmatch", "1.0.0")
    .func(make_func<&soundex_impl>("soundex")
      .returns(STRING)
      .param(STRING)
      .buffer_size(5)
      .build())
    .func(make_func<&levenshtein_impl>("levenshtein")
      .returns(INT)
      .param(STRING)
      .param(STRING)
      .build())
)
