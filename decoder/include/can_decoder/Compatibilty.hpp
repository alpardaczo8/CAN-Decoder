#if (defined(_MSC_VER) && _MSC_VER >= 1920) || \
    (defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 11) || \
    (defined(__clang__) && __clang_major__ >= 12)
    #define HAS_FROM_CHARS_DOUBLE
#endif