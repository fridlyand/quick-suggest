# autocomplete-engine

Search auto-completion and spelling correction in C++14, written from scratch —
no dependencies beyond the standard library, everything in memory.

The program loads a plain-text log of historic queries (one per line) and
serves prefix suggestions in real time, ranked by how often each query appears
in the log. Misspelled input falls back to a spell-corrected match.

## Demo

```
QUERY: go
8 suggestions:
google
google maps
google translate
google docs
google drive
google calendar
google scholar
google flights

QUERY: fasebook          <- misspelled, corrected against the unigram index
3 suggestions:
facebook
facebook login
facebook marketplace
```

## How it works

- **Ternary search tree** ([TST.cpp](TST.cpp)) indexes full queries with their
  frequencies; `keysWithPrefix` walks the matching subtree and collects results
  into a priority queue ordered by popularity.
- **Startup cache** ([Query.cpp](Query.cpp)) — empty and single-letter
  prefixes match the largest subtrees, so they are served from 27 precomputed
  popularity-ranked queues built once at load time.
- **Spell correction** ([SpellCorrector.cpp](SpellCorrector.cpp)) — Norvig
  style: generate every candidate within edit distance 1-2 of each word, keep
  the ones present in a unigram index built from the same log, pick the most
  frequent. Used as a fallback when the raw prefix has no matches.
- An array-based radix-28 **trie** ([Trie.cpp](Trie.cpp)) is included as an
  alternative index implementation; the engine itself uses the TST.

## Build and run

Requires CMake 3.10+ and any C++14 compiler (tested with MSVC 2022 and GCC).

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run from the repository root — the binary expects `query_logs.txt` in the
working directory:

```sh
./build/Release/query_completion.exe   # Windows (MSVC)
./build/query_completion               # Linux / macOS
```

Type a prefix and press Enter for suggestions; type `...` to exit. To use your
own data, replace `query_logs.txt` with any list of queries, one per line.

## Scope and limitations

This is a demonstration project, not a packaged library:

- input is restricted to letters, spaces, and hyphens (lowercased on load)
- ranking is raw frequency — no recency, context, or personalization
- the whole index lives in memory; the bundled sample log is small

## License

[MIT](LICENSE)
