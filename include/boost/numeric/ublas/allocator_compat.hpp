#ifndef BOOST_UBLAS_ALLOCATOR_COMPAT_HPP
#define BOOST_UBLAS_ALLOCATOR_COMPAT_HPP

#include <memory>
#include <type_traits>

namespace boost { namespace numeric { namespace ublas {

// Compatibility layer for C++20 allocators
template<typename Alloc>
struct allocator_traits {
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename Alloc::pointer;
    using const_pointer = typename Alloc::const_pointer;
    using size_type = typename Alloc::size_type;

    static void construct(allocator_type& a, pointer p) {
        if constexpr (std::is_default_constructible_v<value_type>) {
            ::new(static_cast<void*>(p)) value_type();
        }
    }

    static void construct(allocator_type& a, pointer p, const value_type& val) {
        ::new(static_cast<void*>(p)) value_type(val);
    }

    static void destroy(allocator_type& a, pointer p) {
        p->~value_type();
    }
};

// Specialization for std::allocator
template<typename T>
struct allocator_traits<std::allocator<T>> {
    using allocator_type = std::allocator<T>;
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

    static void construct(allocator_type& a, pointer p) {
        if constexpr (std::is_default_constructible_v<value_type>) {
            ::new(static_cast<void*>(p)) value_type();
        }
    }

    static void construct(allocator_type& a, pointer p, const value_type& val) {
        ::new(static_cast<void*>(p)) value_type(val);
    }

    static void destroy(allocator_type& a, pointer p) {
        p->~value_type();
    }
};

}}} // namespace boost::numeric::ublas

#endif // BOOST_UBLAS_ALLOCATOR_COMPAT_HPP
