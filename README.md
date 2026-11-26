# STILO-Q
STILO-Q is an extension of the **[STILO](https://github.com/umutcalikyilmaz/STILO)**, which is a metaheuristic framework for strict time-limited optimization. STILO-Q includes different variants of the quantum-inspired evolutionary algorithm (QEA), in addition to the metaheuristics and analysis tools provided in STILO.

## Installation
STILO is designed for Debian-based systems and is currently not compatible with Windows or macOS. STILO requires C++20. For standard installation, the following bash instructions should be executed in the project's root folder.

```bash
mkdir build
cd build
cmake ..
sudo make install
```

## Usage
### Importing the Library
After installation, STILO-Q can be imported to a project by adding the following lines in the `CMakeLists.txt` file.

```cmake
find_package(STILOQ REQUIRED)
target_link_libraries(my_project STILOQ::stiloq)
```

The framework headers can be included using:

```c++
#include <STILOQ/STILOQ.h>
```
