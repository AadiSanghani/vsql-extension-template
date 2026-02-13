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

#include <algorithm>
#include <string>
#include <vector>

int64_t levenshtein_impl(const std::string& source, const std::string& target) {
  const size_t m = source.size();
  const size_t n = target.size();

  if (m == 0) return static_cast<int>(n);
  if (n == 0) return static_cast<int>(m);

  // Use two rows instead of full matrix for O(min(m,n)) space
  std::vector<int> prev(n + 1);
  std::vector<int> curr(n + 1);

  for (size_t j = 0; j <= n; ++j) {
    prev[j] = static_cast<int>(j);
  }

  for (size_t i = 1; i <= m; ++i) {
    curr[0] = static_cast<int>(i);

    for (size_t j = 1; j <= n; ++j) {
      int cost = (source[i - 1] == target[j - 1]) ? 0 : 1;
      curr[j] = std::min({prev[j] + 1,        
                          curr[j - 1] + 1,     
                          prev[j - 1] + cost}); 
    }

    std::swap(prev, curr);
  }

  return prev[n];
}
