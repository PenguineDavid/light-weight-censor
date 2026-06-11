# Lightweight Censor Library

A minimal word censoring library with no third-party dependencies, available in three language targets: **C++**, **Java**, and **C#**. Each implementation relies only on its respective standard library (`<vector>`, `<unordered_map>` in C++; `java.util` collections in Java; `System.Collections.Generic` and `System.Text` in C#). The library is built around a named-filter model: you maintain one or more named word lists (filters), and pass a filter name into any censor call. Matched words are replaced with asterisks (`*`) of equal length, preserving the surrounding text structure including punctuation and whitespace exactly as-is.

---

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

---

## Table of Contents

- [Concept: Named Filters](#concept-named-filters)
- [Word Boundary Rules](#word-boundary-rules)
- [C++ Usage](#c-usage)
  - [Project Structure](#c-project-structure)
  - [API Reference](#c-api-reference)
  - [Compiling](#compiling-c)
  - [Example](#c-example)
- [Java Usage](#java-usage)
  - [Project Structure](#java-project-structure)
  - [API Reference](#java-api-reference)
  - [Compiling and Running](#compiling-and-running-java)
  - [Example](#java-example)
- [C# Usage](#c-usage-1)
  - [Project Structure](#c-project-structure-1)
  - [API Reference](#c-api-reference-1)
  - [Compiling and Running](#compiling-and-running-c)
  - [Example](#c-example-1)
- [Implementation Notes](#implementation-notes)
- [Behavioral Reference](#behavioral-reference)

---

## Concept: Named Filters

Every function takes a `key` string that identifies which filter list to operate on. This lets you maintain independent word lists for different contexts without any global state bleeding between them.

```
filter key "profanity"  ->  ["word1", "word2", ...]
filter key "spoilers"   ->  ["character", "location", ...]
filter key "badwords"   ->  ["apple", "banana", "orange"]
```

You create a filter with `set_filter` / `setFilter` / `SetFilter`, append to it with `awf` / `Awf`, clear its contents with `clear_filter` / `clearFilter` / `ClearFilter`, and remove it entirely with `delete_filter` / `deleteFilter` / `DeleteFilter`. The filter registry is static and process-scoped, so it persists for the lifetime of the application.

---

## Word Boundary Rules

The library tokenises input by splitting on two character classes, both of which are passed through verbatim to the output:

**Whitespace:** space (` `), tab (`\t`), newline (`\n`), carriage return (`\r`)

**Punctuation (ASCII ranges):**
- `!` through `/` (33-47)
- `:` through `@` (58-64)
- `[` through `` ` `` (91-96)
- `{` through `~` (123-126)

Note: the C# implementation delegates punctuation detection to `char.IsPunctuation()`, which is slightly broader than the explicit ASCII ranges used by C++ and Java. In practice this only affects non-ASCII punctuation characters and is unlikely to matter for typical use.

Everything else is treated as part of a word token. When a token matches an entry in the named filter, it is replaced by an equal-length string of `*` characters. The delimiter that ended the token is then appended unchanged.

This means `"hello,"` produces `"*****,"` (comma preserved), `"hello world"` produces `"***** *****"` (space preserved), and punctuation-surrounded words like `"(hello)"` produce `"(*****)"`.

---

## C++ Usage

### C++ Project Structure

```
cpp/
  include/
    censorer.hpp     <- header: namespace cen declarations
    censorer.cpp     <- implementation
  src/
    main.cpp         <- usage example
```

The implementation file lives alongside the header in `include/` rather than in `src/`. Keep this in mind when specifying include paths.

### C++ API Reference

All functions live in the `cen` namespace.

```cpp
void cen::set_filter(std::string key, std::vector<std::string> content);
```
Creates or replaces a filter entirely. Passing an empty vector is valid and results in a filter that censors nothing.

```cpp
void cen::awf(std::string word, std::string key);
```
Appends a single word to an existing filter. If the key does not yet exist, the entry is created. (`awf` = append word, filter.)

```cpp
void cen::clear_filter(std::string key);
```
Empties the word list for the given key. The key remains registered in the map.

```cpp
void cen::delete_filter(std::string key);
```
Removes the key and its word list from the map entirely. Calling `censor` with a deleted key will produce an empty lookup and pass all words through unchanged.

```cpp
std::string cen::censor(std::string text, std::string key);
```
Returns a copy of `text` with all exact-match words replaced by asterisks. Case-sensitive.

```cpp
std::string cen::censor_case_insensitive(std::string text, std::string key);
```
Same as `censor` but performs case-folding on both the input token and the filter list before comparing. The asterisk replacement preserves the original token length (not the filter word length).

### Compiling C++

The library has no build system. Compile it directly with any C++17-capable compiler.

**Linux / WSL / macOS (g++ or clang++):**

```bash
g++ -std=c++17 -o censor_demo cpp/src/main.cpp cpp/include/censorer.cpp -I cpp/include
./censor_demo
```

**Windows (MSVC via Developer Command Prompt):**

```cmd
cl /std:c++17 /EHsc /I cpp\include cpp\src\main.cpp cpp\include\censorer.cpp /Fe:censor_demo.exe
censor_demo.exe
```

**Windows (MinGW / MSYS2):**

```bash
g++ -std=c++17 -o censor_demo.exe cpp/src/main.cpp cpp/include/censorer.cpp -I cpp/include
./censor_demo.exe
```

To integrate into your own project, add `censorer.cpp` to your source list and `cpp/include/` to your include path. There are no external dependencies.

### C++ Example

```cpp
#include "censorer.hpp"
#include <iostream>

int main()
{
    // Create a filter named "chat" with an initial list
    cen::set_filter("chat", {"bad", "worse"});

    // Add another word later
    cen::awf("terrible", "chat");

    // Case-sensitive pass
    std::string result = cen::censor("That was bad and terrible!", "chat");
    // result == "That was *** and *********!"

    // Case-insensitive pass
    std::string result2 = cen::censor_case_insensitive("That was BAD and Terrible!", "chat");
    // result2 == "That was *** and *********!"

    std::cout << result << "\n";
    std::cout << result2 << "\n";

    // Discard the filter entirely when done
    cen::delete_filter("chat");
    return 0;
}
```

---

## Java Usage

### Java Project Structure

```
java/
  src/
    include/
      censorer.java    <- implementation (package include)
    Main.java          <- usage example
```

The implementation is in the `include` package. `Main.java` sits one level above it in `src/`.

### Java API Reference

All methods are `public static` on the `censorer` class in the `include` package.

```java
censorer.setFilter(String key, List<String> content)
```
Creates or replaces a filter. Pass any `List<String>` implementation; the reference is stored directly, so pass `new ArrayList<>(List.of(...))` if you want the library to own its own copy.

```java
censorer.awf(String word, String key)
```
Appends a word to a filter. If the key does not exist it is created automatically via `computeIfAbsent`.

```java
censorer.clearFilter(String key)
```
Clears the word list. The key remains in the map.

```java
censorer.deleteFilter(String key)
```
Removes the entry from the map entirely.

```java
String censorer.censor(String text, String key)
```
Case-sensitive censoring. Returns the processed string.

```java
String censorer.censorCaseInsensitive(String text, String key)
```
Case-insensitive censoring via `String.equalsIgnoreCase` on each filter entry. Returns the processed string.

### Compiling and Running Java

The source root is `java/src/`. Compile from there so the package hierarchy resolves correctly.

```bash
cd java/src
javac include/censorer.java Main.java
java Main
```

To integrate into a larger project, copy the `include/` directory into your source tree and import `include.censorer`.

### Java Example

```java
import include.censorer;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        censorer.setFilter("chat", new ArrayList<>(List.of("bad", "worse")));
        censorer.awf("terrible", "chat");

        String result = censorer.censor("That was bad and terrible!", "chat");
        // "That was *** and *********!"

        String result2 = censorer.censorCaseInsensitive("That was BAD and Terrible!", "chat");
        // "That was *** and *********!"

        System.out.println(result);
        System.out.println(result2);

        censorer.deleteFilter("chat");
    }
}
```

---

## C# Usage

### C# Project Structure

```
CS/
  include/
    censorer.cs       <- implementation (namespace include)
  src/
    Program.cs        <- usage example
  Censorer.csproj     <- .NET 8 project file
```

The project targets **net8.0** with nullable reference types enabled.

### C# API Reference

All methods are `public static` on the `Censorer` class in the `include` namespace.

```csharp
Censorer.SetFilter(string key, List<string> content)
```
Creates or replaces the filter. The list reference is stored directly.

```csharp
Censorer.Awf(string word, string key)
```
Appends a word. Creates the key if it does not exist.

```csharp
Censorer.ClearFilter(string key)
```
Empties the word list. The key stays registered.

```csharp
Censorer.DeleteFilter(string key)
```
Removes the key from the dictionary.

```csharp
string Censorer.Censor(string text, string key)
```
Case-sensitive censoring.

```csharp
string Censorer.CensorCaseInsensitive(string text, string key)
```
Case-insensitive censoring via `ToLowerInvariant` comparison on both the token and each filter entry.

### Compiling and Running C#

**Using the .NET CLI (recommended):**

From the `CS/` directory:

```bash
dotnet run
```

This will compile and execute `Program.cs` automatically. The project file already has the correct configuration.

To build without running:

```bash
dotnet build
```

To produce a self-contained executable:

```bash
dotnet publish -c Release -r win-x64 --self-contained true
# or for Linux:
dotnet publish -c Release -r linux-x64 --self-contained true
```

**Minimum SDK:** .NET 8

To integrate the library into an existing project, copy `censorer.cs` into your project and adjust the namespace import to `using include;` (or move the class into your own namespace).

### C# Example

```csharp
using System.Collections.Generic;
using include;

Censorer.SetFilter("chat", new List<string> { "bad", "worse" });
Censorer.Awf("terrible", "chat");

string result = Censorer.Censor("That was bad and terrible!", "chat");
// "That was *** and *********!"

string result2 = Censorer.CensorCaseInsensitive("That was BAD and Terrible!", "chat");
// "That was *** and *********!"

System.Console.WriteLine(result);
System.Console.WriteLine(result2);

Censorer.DeleteFilter("chat");
```

---

## Implementation Notes

**Filter storage is global and static.** All three implementations use a static map/dictionary scoped to the process. There is no instance-level isolation. If you need separate filter namespaces in the same process, use distinct key strings.

**Thread safety.** None of the implementations provide any synchronisation. Concurrent reads from multiple threads against a stable (non-mutating) filter map are safe in practice given the data structures used, but concurrent writes or a write racing a read are not protected.

**Filter lookup is linear.** Each filter's word list is a `vector` / `ArrayList` / `List<string>`. Membership checks during censoring are O(n) in the number of words in that filter. For small filter lists (hundreds of words) this is negligible. If you are running filters with thousands of entries at high throughput, consider replacing the underlying storage with a hash set.

**Case-insensitive path in C++.** The `censor_case_insensitive` function builds a temporary lowercased copy of the filter list on every call. For repeated calls against the same filter, you may want to cache the lowercased list yourself externally.

**`awf` on a non-existent key.** Behaviour differs slightly between implementations. In C++ and C#, indexing a non-existent key in an `unordered_map` / `Dictionary` with `[]` default-constructs an empty list, so `awf` silently creates it. The Java implementation uses `computeIfAbsent` explicitly. All three are safe to call without a prior `setFilter`.

**`clear_filter` vs `delete_filter`.** `clear_filter` zeroes the word list but keeps the key alive in the map. Calling `censor` after clearing will simply pass all words through unchanged. `delete_filter` removes the key entirely; calling `censor` with a missing key has the same observable effect but the key is no longer enumerable. Use `delete_filter` when you want to release memory associated with the list.

---

## Behavioral Reference

| Input | Filter contains | Function | Output |
|---|---|---|---|
| `"hello world"` | `["hello"]` | `censor` | `"***** world"` |
| `"Hello world"` | `["hello"]` | `censor` | `"Hello world"` |
| `"Hello world"` | `["hello"]` | `censor_case_insensitive` | `"***** world"` |
| `"say hello!"` | `["hello"]` | `censor` | `"say *****!"` |
| `"(hello)"` | `["hello"]` | `censor` | `"(*****)"` |
| `"hello"` | `["hello"]` | `censor` | `"*****"` |
| `"helloworld"` | `["hello"]` | `censor` | `"helloworld"` |
| `""` | `["hello"]` | `censor` | `""` |
| `"hello"` | `[]` | `censor` | `"hello"` |

The last two rows illustrate that an empty input or an empty filter are both no-ops. The `"helloworld"` row shows that substrings are not matched; only complete tokens delimited by whitespace or punctuation qualify.
