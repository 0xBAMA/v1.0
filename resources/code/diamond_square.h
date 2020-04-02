// Copyright (c) 2018 Martyn Afford
// Licensed under the MIT licence (below)

// Copyright (c) 2018 Martyn Afford

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef DIAMOND_SQUARE_HPP
#define DIAMOND_SQUARE_HPP

#include <cassert>

namespace heightfield {

// Generate a heightfield using random midpoint displacement and the
// diamond-square algorithm.
//
// \param size
//   Size of the desired heightfield. Valid values are powers of two plus one
//   (i.e., 2^n + 1), such as 129, 257 and 513. Must be at least five. Invalid
//   sizes will trigger an assertion failure.
// \param random
//   A callback of the form `float(float limit)` that returns a random value
//   ranging from 0 (inclusive) to limit (exclusive). This function is called
//   once for each heightfield location excluding the corner points (i.e.,
//   size^2 - 4 times).
// \param variance
//   A callback of the form `float(int level)` that returns the maximum
//   variance in one direction for the specified level. The level starts from
//   zero and increases for each recursion. For example, a heightfield of size
//   129 has seven levels, zero through six. Generally speaking, the variance
//   halves each iteration. A good start is `constant * pow(0.5f, level)`. This
//   function is called once for each level.
// \param at
//   A callback of the form `U(int x, int y)` that returns a mutable reference
//   to the heightfield location specified by the coordinates. A coordinate will
//   range from zero (inclusive) to size (exclusive). The return type U can be
//   any numeric type but it must be, or act as, a non-const reference to allow
//   the algorithm to output its results. This allows the user to decide how
//   the heightfield is stored in memory.
//
// The corner points of the heightfield should be initialised to suitable
// starting values before calling this function.
//
// The resulting heightfield is not tileable. Points along the edges are
// averaged over three neighbouring points instead of the usual four.
//
// Variance is defined in one direction but is applied in both. If we assume an
// integer type for the heightfield and an initial value of 128, then a variance
// of 100 will result in a range of 28 to 228.
//
// No clamping is performed by this algorithm so if the randomness, variance and
// initial values combine to generate a value smaller or larger than the data
// type referenced by `at`, underflow or overflow can occur.
//
// The time complexity of this algorithm is O(n^2) where n is `size`. The space
// complexity is constant (and small) as this algorithm makes no allocations
// internally, relying on the user to provide the data structure and accessing
// it via `at`.
template <typename T, typename T2, typename T3>
void
diamond_square_no_wrap(int size, T&& random, T2&& variance, T3&& at)
{
    assert(size >= 5 && ((size - 1) & (size - 2)) == 0 && "valid size");

    auto level = 0;
    auto stride = size - 1;
    auto end = size - 1;

    while (stride > 1) {
        auto range = variance(level);
        auto half = stride / 2;

        // Diamond step:
        for (auto y = half; y < end; y += stride) {
            for (auto x = half; x < end; x += stride) {
                auto tl = at(x - half, y - half);
                auto bl = at(x - half, y + half);
                auto tr = at(x + half, y - half);
                auto br = at(x + half, y + half);

                auto average = (tl + tr + br + bl) / 4.0f;
                auto displacement = random(range * 2.0f) - range;

                at(x, y) = average + displacement;
            }
        }

        // Square step:
        // Firstly there are four special cases to handle the rows and columns
        // with only three neighbours.

        // Top row - no up
        for (auto x = half; x < end; x += stride) {
            auto down = at(x, half);
            auto left = at(x - half, 0);
            auto right = at(x + half, 0);

            auto average = (down + left + right) / 3.0f;
            auto displacement = random(range * 2.0f) - range;

            at(x, 0) = average + displacement;
        }

        // Bottom row - no down
        for (auto x = half; x < end; x += stride) {
            auto up = at(x, end - half);
            auto left = at(x - half, end);
            auto right = at(x + half, end);

            auto average = (up + left + right) / 3.0f;
            auto displacement = random(range * 2.0f) - range;

            at(x, end) = average + displacement;
        }

        // Left column - no left
        for (auto y = half; y < end; y += stride) {
            auto up = at(0, y - half);
            auto down = at(0, y + half);
            auto right = at(half, y);

            auto average = (up + down + right) / 3.0f;
            auto displacement = random(range * 2.0f) - range;

            at(0, y) = average + displacement;
        }

        // Right column - no right
        for (auto y = half; y < end; y += stride) {
            auto up = at(end, y - half);
            auto down = at(end, y + half);
            auto left = at(end - half, y);

            auto average = (up + down + left) / 3.0f;
            auto displacement = random(range * 2.0f) - range;

            at(end, y) = average + displacement;
        }

        // The remaining points -- the edges all processed as special cases
        // above -- that have four neighbours.
        auto offset = true;

        for (auto y = half; y < end; y += half) {
            for (auto x = offset ? stride : half; x < end; x += stride) {
                auto up = at(x, y - half);
                auto down = at(x, y + half);
                auto left = at(x - half, y);
                auto right = at(x + half, y);

                auto average = (up + down + left + right) / 4.0f;
                auto displacement = random(range * 2.0f) - range;

                at(x, y) = average + displacement;
            }

            offset = !offset;
        }

        stride /= 2;
        ++level;
    }
}

// Generate a tileable heightfield using random midpoint displacement and the
// diamond-square algorithm.
//
// \param size
//   Size of the desired heightfield. Valid values are powers of two (i.e.,
//   2^n), such as 128, 256 and 512. Must be at least four. Invalid sizes will
//   trigger an assertion failure.
// \param random
//   A callback of the form `float(float limit)` that returns a random value
//   ranging from 0 (inclusive) to limit (exclusive). This function is called
//   once for each heightfield location excluding the initial top left point
//   (i.e., size^2 - 1 times).
// \param variance
//   A callback of the form `float(int level)` that returns the maximum
//   variance in one direction for the specified level. The level starts from
//   zero and increases for each recursion. For example, a heightfield of size
//   128 has seven levels, zero through six. Generally speaking, the variance
//   halves each iteration. A good start is `constant * pow(0.5f, level)`. This
//   function is called once for each level.
// \param at
//   A callback of the form `U(int x, int y)` that returns a mutable reference
//   to the heightfield location specified by the coordinates. A coordinate will
//   range from zero (inclusive) to size (exclusive). The return type U can be
//   any numeric type but it must be, or act as, a non-const reference to allow
//   the algorithm to output its results. This allows the user to decide how
//   the heightfield is stored in memory.
//
// The top left point of the heightfield should be initialised to a suitable
// starting value before calling this function.
//
// The resulting heightfield is tileable.
//
// Variance is defined in one direction but is applied in both. If we assume an
// integer type for the heightfield and an initial value of 128, then a variance
// of 100 will result in a range of 28 to 228.
//
// No clamping is performed by this algorithm so if the randomness, variance
// and initial values combine to generate a value smaller or larger than the
// data type referenced by `at`, underflow or overflow can occur.
//
// The time complexity of this algorithm is O(n^2) where n is `size`. The space
// complexity is constant (and small) as this algorithm makes no allocations
// internally, relying on the user to provide the data structure and accessing
// it via `at`.
template <typename T, typename T2, typename T3>
void
diamond_square_wrap(int size, T&& random, T2&& variance, T3&& at)
{
    assert(size >= 4 && (size & (size - 1)) == 0 && "valid size");

    auto level = 0;
    auto stride = size;
    auto end = size;
    auto mask = size - 1;

    // The same as 'at' but handles x or y exceeding their maximum, size - 1.
    // As the size is a power of two, simple bit masking can be used. Note that
    // this does not handle negative indices. This is why there are two special
    // cases in the square step code later.
    auto at_wrap = [&at, mask](int x, int y) { return at(x & mask, y & mask); };

    while (stride > 1) {
        auto range = variance(level);
        auto half = stride / 2;

        // Diamond step:
        for (auto y = half; y < end; y += stride) {
            for (auto x = half; x < end; x += stride) {
                auto tl = at(x - half, y - half);
                auto bl = at_wrap(x - half, y + half);
                auto tr = at_wrap(x + half, y - half);
                auto br = at_wrap(x + half, y + half);

                auto average = (tl + tr + br + bl) / 4.0f;
                auto displacement = random(range * 2.0f) - range;

                at(x, y) = average + displacement;
            }
        }

        // Square step:
        // Special cases for the top row and left column where the x or y index
        // would be negative in the main square step algorithm (e.g., x - half,
        // when x is zero, would be -half). Not adding conditionals to handle
        // negative indices in the main loop improves performance by 3-8%.
        for (auto x = half; x < end; x += stride) {
            auto up = at(x, end - half);
            auto down = at(x, half);
            auto left = at(x - half, 0);
            auto right = at_wrap(x + half, 0);

            auto average = (up + down + left + right) / 4.0f;
            auto displacement = random(range * 2.0f) - range;

            at(x, 0) = average + displacement;
        }

        // As above; special case for the left column.
        for (auto y = half; y < end; y += stride) {
            auto up = at(0, y - half);
            auto down = at_wrap(0, y + half);
            auto left = at(end - half, y);
            auto right = at(half, y);

            auto average = (up + down + left + right) / 4.0f;
            auto displacement = random(range * 2.0f) - range;

            at(0, y) = average + displacement;
        }

        // The remaining points, not including the leftmost and topmost; that
        // is, all coordinates, x or y, with a value of at least half. Thus,
        // x - half and y - half are assured to be at least zero.
        bool offset = true;

        for (auto y = half; y < end; y += half) {
            for (auto x = offset ? stride : half; x < end; x += stride) {
                auto up = at(x, y - half);
                auto down = at_wrap(x, y + half);
                auto left = at(x - half, y);
                auto right = at_wrap(x + half, y);

                auto average = (up + down + left + right) / 4.0f;
                auto displacement = random(range * 2.0f) - range;

                at(x, y) = average + displacement;
            }

            offset = !offset;
        }

        stride /= 2;
        ++level;
    }
}

} // namespace heightfield

#endif /* DIAMOND_SQUARE_HPP */
