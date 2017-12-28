# Lines Are Beautiful

[![Build Status develop](https://img.shields.io/travis/ax3l/lines-are-beautiful/develop.svg?label=develop)](https://travis-ci.org/ax3l/lines-are-beautiful/branches)
[![Documentation Status](https://readthedocs.org/projects/rmlab/badge/?version=latest)](http://rmlab.readthedocs.io)
[![Language](https://img.shields.io/badge/language-C%2B%2B14-orange.svg)](https://isocpp.org)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

A C++ file API for the [reMarkable e-ink tablet](https://remarkable.com).

**Warning:** The libraries and tools in this project are not (yet) hardened for malicious input.
Only process files that you can trust with it!

## Dependencies

- A C++14 capable compiler such as
  - GCC 6.0+
  - Clang 3.9+ (tested)
- CMake 3.7+

## Install

![Spack Package](https://img.shields.io/badge/spack.io-rmlab-brightgreen.svg)
![Conan Package](https://img.shields.io/badge/conan.io-notyet-brightgreen.svg)
![Conda Package](https://img.shields.io/badge/conda.io-notyet-brightgreen.svg)
![Docker Image](https://img.shields.io/badge/docker-notyet-brightgreen.svg)

If one of the popular user-level package managers above is not already satisfying your needs, install from source via:

```bash
git clone https://github.com/ax3l/lines-are-beautiful.git

mkdir lines-are-beautiful/build
cd lines-are-beautiful/build

cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make -j

# optional
sudo make install
```

## Usage CLI

This is a small example implementing a renderer for PNG while changing the brush type.

```bash
# without the ".lines" file ending!
lines2png $HOME/.local/share/remarkable/xochitl/f434bd89-64ec-4071-ad75-806e5a951d93
```

## Usage API

Set environment hints:
```bash
# optional: only needed if installed outside of system paths
export CMAKE_PREFIX_PATH=/your/path/to/installed/path:$CMAKE_PREFIX_PATH
```

Add to your `CMakeLists.txt`:
```cmake
find_package(Rmlab::Rmlab 0.1.0)

target_link_libraries(YourTarget PRIVATE Rmlab::Rmlab)
```

In your C++ files:
```C++
#include <lab/lab.hpp>
#include <iostream>

// ...

rmlab::Notebook myNotebook("share/lab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc");

for( auto & page : myNotebook.pages )
    for( auto & layer : page.layers )
        for( auto & line : layer.lines )
            for( auto & point : line.points )
                std::cout << point.x << " " << point.y << std::endl;
```

## Disclaimer

This is a hobby project.

The author(s) and contributor(s) are not associated with reMarkable AS, Norway.
**reMarkable** is a registered trademark of *reMarkable AS* in some countries.
Please see https://remarkable.com for their product.
