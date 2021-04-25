#ifndef HASH_HPP
#define HASH_HPP

#include <cstddef>
#include <cstdint>

namespace JamJar {
template <uint32_t N, uint32_t I = 0> struct hash_calc {
    static constexpr uint32_t apply(const char (&s)[N]) { return (hash_calc<N, I + 1>::apply(s) ^ s[I]) * 16777619u; };
};

template <uint32_t N> struct hash_calc<N, N> {
    static constexpr uint32_t apply(const char (&s)[N]) { return 2166136261u; };
};

template <uint32_t N> constexpr uint32_t hash(const char (&s)[N]) { return hash_calc<N>::apply(s); }
}; // namespace JamJar

#endif
