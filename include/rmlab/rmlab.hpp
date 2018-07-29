/* Copyright 2017-2018 Axel Huebl
 *
 * This file is part of lines-are-beautiful.
 *
 * lines-are-beautiful is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * lines-are-beautiful is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with lines-are-beautiful.
 * If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @mainpage lines-are-beautiful
 *
 * A C++ file API for the [reMarkable e-ink tablet](https://remarkable.com).
 *
 * **Warning:** The libraries and tools in this project are not (yet) hardened
 * for malicious input. Only process files that you can trust with it!
 *
 * \section Overview
 *
 * The [reMarkable tablet](https://remarkable.com/) is a E Ink device that can
 * be written on. It can be used as a note-taking device, for annotating or
 * reading digital books. User notes are stored in so-called “notebooks” that
 * behave just like real-life notebooks.
 *
 * This library implements reading the `.lines` binary file format, used by
 * the device to store notebooks. The data structures are modeled after the
 * ones found into this format, namely:
 *
 * | Structure       | Role                                                  |
 * | --------------- | ----------------------------------------------------- |
 * | rmlab::Notebook | Entry point.                                          |
 * | rmlab::Page     | Each notebook is made up of several pages.            |
 * | rmlab::Layer    | Each page contains a stack of layers.                 |
 * | rmlab::Line     | A line is a stroke of the pen, stored inside a layer. |
 * | rmlab::Point    | A line contains a sequence of sampled points.         |
 *
 * For a complete overview of the file format, see
 * [this blog post by Axel Hübl.](https://plasma.ninja/blog/devices/remarkable/binary/format/2017/12/26/reMarkable-lines-file-format.html)
 *
 * \section Disclaimer
 *
 * This is a hobby project.
 *
 * The author(s) and contributor(s) are not associated with reMarkable AS,
 * Norway. **reMarkable** is a registered trademark of *reMarkable AS* in
 * some countries. Please see <https://remarkable.com> for their product.
 */

#pragma once

// objects for file hierarchy
#include "Notebook.hpp"
#include "Page.hpp"
#include "Layer.hpp"
#include "Line.hpp"
#include "Point.hpp"


namespace rmlab
{

}
