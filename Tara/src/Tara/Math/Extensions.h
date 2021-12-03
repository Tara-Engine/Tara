#pragma once
#include <functional>
#include <glm/glm.hpp>

//hash combiner from here:
//https://stackoverflow.com/questions/2590677/how-do-i-combine-hash-values-in-c0x
//I could not think of a cleaner or better way to do this quickly!

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    hash_combine(seed, rest...);
}

#define MAKE_HASHABLE(type, ...) \
    template<> struct std::hash<type> {\
        std::size_t operator()(const type &t) const {\
            std::size_t ret = 0;\
            hash_combine(ret, __VA_ARGS__);\
            return ret;\
        }\
    }\

MAKE_HASHABLE(glm::ivec1, t.x);
MAKE_HASHABLE(glm::ivec2, t.x, t.y);
MAKE_HASHABLE(glm::ivec3, t.x, t.y, t.z);
MAKE_HASHABLE(glm::ivec4, t.x, t.y, t.z, t.w);

MAKE_HASHABLE(glm::vec1, t.x);
MAKE_HASHABLE(glm::vec2, t.x, t.y);
MAKE_HASHABLE(glm::vec3, t.x, t.y, t.z);
MAKE_HASHABLE(glm::vec4, t.x, t.y, t.z, t.w);


inline std::ostream& operator<< (std::ostream& out, const glm::vec1& v)
{
    out << "{x:" << v.x << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec2& v)
{
    out << "{x:" << v.x << ",y:" << v.y << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec3& v)
{
    out << "{x:" << v.x << ",y:" << v.y << ",z:" << v.z << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::vec4& v)
{
    out << "{x:" << v.x << ",y:" << v.y << ",z:" << v.z << ",w:" << v.w << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec1& v)
{
    out << "{x:" << v.x << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec2& v)
{
    out << "{x:" << v.x << ",y:" << v.y << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec3& v)
{
    out << "{x:" << v.x << ",y:" << v.y << ",z:" << v.z << "}";
    return out;
}

inline std::ostream& operator<< (std::ostream& out, const glm::ivec4& v)
{
    out << "{x:" << v.x << ",y:" << v.y << ",z:" << v.z << ",w:" << v.w << "}";
    return out;
}