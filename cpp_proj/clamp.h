// Clamp.h
#pragma once

/**
 * @brief Clamps a value within a specified range.
 *
 * This function ensures that the value `v` falls within the range defined by `lo` (lower bound)
 * and `hi` (upper bound). If `v` is less than `lo`, the function returns `lo`. If `v` is greater than
 * `hi`, it returns `hi`. Otherwise, it returns `v` unchanged.
 *
 * @tparam T The type of the value to clamp. Must support comparison operators.
 * @param v The value to be clamped.
 * @param lo The lower bound of the range.
 * @param hi The upper bound of the range.
 * @return A constant reference to the clamped value.
 */
template <typename T>
const T& clamp(const T& v, const T& lo, const T& hi)
{
    return (v < lo) ? lo : (hi < v) ? hi : v;
}
