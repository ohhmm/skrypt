#ifndef BOOST_UBLAS_STORAGE_WRAPPER_HPP
#define BOOST_UBLAS_STORAGE_WRAPPER_HPP

#include <memory>
#include <type_traits>

namespace boost { namespace numeric { namespace ublas {

// C++20 allocator compatibility wrapper
template<typename Alloc>
struct allocator_wrapper : public Alloc {
    using allocator_type = Alloc;

    template<typename T>
    struct rebind {
        using other = allocator_wrapper<typename std::allocator_traits<Alloc>::template rebind_alloc<T>>;
    };

    allocator_wrapper() = default;
    allocator_wrapper(const Alloc& a) : Alloc(a) {}

    template<typename T>
    void construct(T* p) {
        std::allocator_traits<Alloc>::construct(*this, p);
    }

    template<typename T>
    void construct(T* p, const T& val) {
        std::allocator_traits<Alloc>::construct(*this, p, val);
    }

    template<typename T>
    void destroy(T* p) {
        std::allocator_traits<Alloc>::destroy(*this, p);
    }
};

}}}

#endif // BOOST_UBLAS_STORAGE_WRAPPER_HPP
