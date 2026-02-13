# fuzzystrmatch — Fuzzy String Matching for VillageSQL

A VillageSQL extension that provides functions for fuzzy string matching and phonetic comparison. Inspired by PostgreSQL's [fuzzystrmatch](https://www.postgresql.org/docs/current/fuzzystrmatch.html) module, this extension brings Soundex, Levenshtein distance, and Metaphone algorithms to VillageSQL.

## Functions

### `soundex(string)`

Returns the [Soundex](https://en.wikipedia.org/wiki/Soundex) code for a given string — a four-character phonetic encoding that represents how a word sounds in English. Useful for matching names that are spelled differently but sound alike.

```sql
SELECT soundex('Robert');
-- Returns: R163

SELECT soundex('Rupert');
-- Returns: R163
```

### `levenshtein(string, string)`

Returns the [Levenshtein edit distance](https://en.wikipedia.org/wiki/Levenshtein_distance) between two strings — the minimum number of single-character insertions, deletions, or substitutions needed to transform one string into the other.

```sql
SELECT levenshtein('kitten', 'sitting');
-- Returns: 3

SELECT levenshtein('hello', 'hello');
-- Returns: 0
```

### `metaphone(string, max_phonemes)`

Returns the [Metaphone](https://en.wikipedia.org/wiki/Metaphone) phonetic code of a string, truncated to `max_phonemes` characters. Metaphone improves on Soundex by using a larger set of rules for English pronunciation.

```sql
SELECT metaphone('Thompson', 4);
-- Returns: TMPS

SELECT metaphone('Smith', 4);
-- Returns: SM0
```

## Project Structure

```
fuzzystrmatch/
├── manifest.json              # Extension metadata (name, version, description)
├── CMakeLists.txt             # CMake build configuration
├── cmake/
│   └── FindVillageSQL.cmake   # CMake module for finding VillageSQL
├── src/
│   ├── extension.cc           # Entry point — registers all functions via VEF API
│   ├── soundex.cc             # Soundex implementation
│   ├── levenshtein.cc         # Levenshtein distance implementation
│   └── metaphone.cc           # Metaphone implementation
└── test/
    ├── t/                     # Test files (.test)
    └── r/                     # Expected results (.result)
```

## Prerequisites

- VillageSQL build directory (with completed build)
- CMake 3.16 or higher
- C++ compiler with C++17 support

## Building the Extension

1. Create a build directory and configure:

   **Linux:**
   ```bash
   mkdir build
   cd build
   cmake .. -DVillageSQL_BUILD_DIR=$HOME/build/villagesql
   ```

   **macOS:**
   ```bash
   mkdir build
   cd build
   cmake .. -DVillageSQL_BUILD_DIR=~/build/villagesql
   ```

   **Note**: `VillageSQL_BUILD_DIR` should point to your VillageSQL build directory. The VEB install directory is automatically set to `${VillageSQL_BUILD_DIR}/veb_output_directory`.

2. Build the extension:

   ```bash
   make -j $(($(getconf _NPROCESSORS_ONLN) - 2))
   ```

   This creates the `fuzzystrmatch.veb` package in the build directory.

3. Install the VEB (optional):

   ```bash
   make install
   ```

   This copies the VEB to the directory specified by `VEB_INSTALL_DIR`. You can also manually copy the VEB file to your desired location.

## Using the Extension

Install the extension in VillageSQL:

```sql
INSTALL EXTENSION fuzzystrmatch;
```

Then use the functions:

```sql
-- Phonetic matching with Soundex
SELECT soundex('Robert'), soundex('Rupert');

-- Edit distance between strings
SELECT levenshtein('kitten', 'sitting');

-- Metaphone phonetic encoding
SELECT metaphone('Thompson', 4);
```

## Testing

The extension uses the MySQL Test Runner (MTR) framework.

### Running Tests

**Option 1: Using installed VEB**

Assumes you have run `make install` to install the VEB to your veb_dir.

```bash
cd ~/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/fuzzystrmatch/test
```

**Option 2: Using a specific VEB file**

Test a specific VEB build without installing it first:

```bash
cd ~/build/villagesql/mysql-test
FUZZYSTRMATCH_VEB=/path/to/build/fuzzystrmatch.veb \
  perl mysql-test-run.pl --suite=/path/to/fuzzystrmatch/test
```

### Creating/Updating Test Results

```bash
cd ~/build/villagesql/mysql-test
perl mysql-test-run.pl --suite=/path/to/fuzzystrmatch/test --record
```

## Resources

- [VillageSQL Documentation](https://villagesql.com/docs)
- [Soundex algorithm](https://en.wikipedia.org/wiki/Soundex)
- [Levenshtein distance](https://en.wikipedia.org/wiki/Levenshtein_distance)
- [Metaphone algorithm](https://en.wikipedia.org/wiki/Metaphone)

## License

This extension is released under the GPL-2.0 license. See the license header in source files for details.
