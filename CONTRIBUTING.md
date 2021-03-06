# Contributing

### Naming
* Namespaces, Classes, Functions: `PascalCase`
* Variables: `camelCase`

### Braces
Opening brace is placed on the same line, that is, there is _no_ line break
before it. If a control flow body consists of a single statement, braces are
omitted.

### Indentation
Use 4 spaces for indentation, not tabs.

### Structure
Following `else`- and `else if`-statements begin on the same line as the
previous closing brace, or a new line if the previous branch contained a single
statement.

### Access specifiers
A class should only declare one specifier per specifier type.

### Documentation
All non-private members need to be documented using Doxygen-style comments.
For functions and methods, parameters are documented first and return type next.

### Clang
We use clang-format (v 3.9) to enforce our coding standard.
To format a specific file, use  ``clang-format -i PATH_TO_FILE/FILENAME`` when in the /src directory.

Clang-format for windows can be grabbed from here: 
http://releases.llvm.org/

The options we use are specified in src/.clang-format. Explanations of the options can be found here:
http://releases.llvm.org/3.9.0/tools/clang/docs/ClangFormatStyleOptions.html

### Example
```cpp
#include <string>
#include <iostream>
#include "Example.hpp"

/// A happy little fish.
class Fish : public Example {
    public:
    /// Create new fish.
    /**
     * @param name The name to give the fish.
     */
    Fish(const std::string& name);

    /// Display the name of the fish.
    /**
     * @param prefix Whether to prefix the output with 'A fish called '.
     */
    void DisplayName(bool prefix = false) const;

    /// Get whether the fish is happy.
    /**
     * @return Whether the fish is happy.
     */
    bool GetHappy() const;

    private:
    std::string name;
    Fish* nextFish;
}

Fish::Fish(const std::string& name) {
    this->name = name;
}

void Fish::DisplayName(bool prefix) const {
    if (prefix)
        std::cout << "A fish called " << name << "." << std::endl;
    else {
        std::cout << name << std::endl;
        std::cout << "A second statement." << std::endl;
    }
}

bool Fish::GetHappy() const {
    return true;
}
```
