# Lines Are Beautiful

[![Build Status develop](https://img.shields.io/travis/ax3l/lines-are-beautiful/develop.svg?label=develop)](https://travis-ci.org/ax3l/lines-are-beautiful/branches)
[![Doxygen Docs](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://ax3l.github.io/lines-are-beautiful/)
[![Manual Status](https://readthedocs.org/projects/rmlab/badge/?version=latest)](http://rmlab.readthedocs.io)
[![Language](https://img.shields.io/badge/language-C%2B%2B17-orange.svg)](https://isocpp.org)
[![License](https://img.shields.io/badge/license-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)

A C++ file API for the [reMarkable e-ink tablet](https://remarkable.com).

**Warning:** The libraries and tools in this project are not (yet) hardened for malicious input.
Only process files that you can trust with it!

## Dependencies

- A C++17 capable compiler such as
  - GCC 8+ (tested)
  - Clang 7+ (tested)
- CMake 3.8+
- [PNGwriter 0.7.0+](https://github.com/pngwriter/pngwriter)
  (optional for png converts; extend environment variable `CMAKE_PREFIX_PATH` with its install location)

## Install

[![Spack Package](https://img.shields.io/badge/spack-rmlab-brightgreen.svg)](https://spack.io)
[![Conan Package](https://img.shields.io/badge/conan-notyet-yellow.svg)](https://conan.io)
[![Conda Package](https://img.shields.io/badge/conda-notyet-yellow.svg)](https://conda.io)
[![Docker Image](https://img.shields.io/badge/docker-notyet-yellow.svg)](https://docker.io)

### Spack

```bash
spack install rmlab
spack load rmlab
```

### From Source

If one of the popular user-level package managers above is not already satisfying your needs, install from source via:

```bash
git clone https://github.com/ax3l/lines-are-beautiful.git

mkdir lines-are-beautiful/build
cd lines-are-beautiful/build

# for own install prefix append: -DCMAKE_INSTALL_PREFIX=$HOME/somepath
cmake ..

make -j

# optional
make test

# sudo is only required for system paths
sudo make install
```

## Usage CLI

_Lines Are Beautiful_ comes with several tools to handle files produced by the tablet.
Try them on your own files inside `$HOME/.local/share/remarkable/xochitl/` :-)

### PNG renderer

This is a small example implementing a renderer for PNG while changing the brush type.

```bash
# without the ".rm" file ending!
lines2png share/rmlab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc
#   creates file "test.png" in the current directory
```

Note: this tool depends on an installed [PNGwriter](https://github.com/pngwriter/pngwriter) dependency.

### SVG renderer

This notebook renderer creates a set of SVG files, one for each page. Implementation status:

* [x] Basic strokes.
* [x] Initial brush size.
* [x] Brush color.
* [x] Highlighter.
* [x] Normal eraser.
* [x] Region eraser.
* [x] Layers.
* [ ] Brush size variation based on pressure/tilt.
* [ ] Brush texture.

```bash
# without the ".rm" file ending!
lines2svg share/rmlab/examples/e09e6bd4-3647-41e7-98be-b9c3b53d80c8
#   creates files "test-0.svg", "test-1.svg", ... in the current directory
```

## Usage API

Set environment hints:
```bash
# optional: only needed if installed outside of system paths
export CMAKE_PREFIX_PATH=/your/path/to/installed/path:$CMAKE_PREFIX_PATH
```

Add to your `CMakeLists.txt`:
```cmake
# supports:                     COMPONENTS PNG
find_package(Rmlab 0.1.0 CONFIG)

target_link_libraries(YourTarget PRIVATE Rmlab::Rmlab)
```

*Alternatively*, add whole repository directly to your project and add it via:
```cmake
add_subdirectory("path/to/source/of/lines-are-beautiful")

target_link_libraries(YourTarget PRIVATE Rmlab::Rmlab)
```

In your C++ files (see [Doxygen](https://ax3l.github.io/lines-are-beautiful/)):
```C++
#include <rmlab/rmlab.hpp>
#include <iostream>

// ...

rmlab::Notebook myNotebook("share/rmlab/examples/aa90b0e7-5c1a-42fe-930f-dad9cf3363cc");

for( auto & page : myNotebook.pages )
    for( auto & layer : page.layers )
        for( auto & line : layer.lines )
            for( auto & point : line.points )
                std::cout << point.x << " " << point.y << std::endl;
```

## Resources

Blog articles

- [general](https://plasma.ninja/blog/devices/remarkable/2017/12/18/reMarkable-exporation.html)
- [file format](https://plasma.ninja/blog/devices/remarkable/binary/format/2017/12/26/reMarkable-lines-file-format.html)
- 5' talk at 34C3:
  - [slides](https://plasma.ninja/34c3/reMarkable_binary_format.pdf)
  - [video](https://media.ccc.de/v/34c3-9257-lightning_talks_day_3#t=1405) (around minute 23+)

## Disclaimer

This is a hobby project.

The author(s) and contributor(s) are not associated with reMarkable AS, Norway.
**reMarkable** is a registered trademark of *reMarkable AS* in some countries.
Please see https://remarkable.com for their product.
