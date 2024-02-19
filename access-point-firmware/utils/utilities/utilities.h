// =============================================================================
// Utils | MathUtils
// =============================================================================

#ifndef UTILS_MATHUTILS_
#define UTILS_MATHUTILS_

#define ARR_ELEMENT_COUNT(a) (sizeof(a) / sizeof(*a))

template <typename E>
constexpr auto to_underlying(E e)
{
   static_assert(std::is_enum<E>::value);
   return static_cast<typename std::underlying_type<E>::type>(e);
}

#endif
