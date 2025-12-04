#pragma once

#include <cstddef>
#include <type_traits>

namespace stack_string {

/**
 * A stateful allocator that uses an external fixed buffer for allocations.
 * Designed for a single std::basic_string instance per buffer.
 * 
 * This allocator can be used with std::basic_string to create stack-based strings
 * while maintaining compatibility with the standard library.
 * 
 * @tparam T The type to allocate
 */
template <typename T>
class FixedBufAllocator {
private:
    char* m_buffer;
    std::size_t m_capacity;
    std::size_t m_used;
    
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
    /**
     * Construct allocator with external buffer
     * @param buffer Pointer to buffer
     * @param capacity Size of buffer in bytes
     */
    FixedBufAllocator(void* buffer, std::size_t capacity) noexcept
        : m_buffer(static_cast<char*>(buffer))
        , m_capacity(capacity)
        , m_used(0) {}
    
    /**
     * Copy constructor for rebinding
     */
    template <typename U>
    FixedBufAllocator(const FixedBufAllocator<U>& other) noexcept
        : m_buffer(other.m_buffer)
        , m_capacity(other.m_capacity)
        , m_used(other.m_used) {}
    
    /**
     * Allocate n objects of type T
     * Uses buffer if space available, returns nullptr if exhausted
     */
    T* allocate(std::size_t n) {
        std::size_t bytes = n * sizeof(T);
        
        // Try to allocate from buffer
        if (m_buffer && m_used + bytes <= m_capacity) {
            T* result = reinterpret_cast<T*>(m_buffer + m_used);
            m_used += bytes;
            return result;
        }
        
        // Buffer exhausted - return nullptr
        return nullptr;
    }
    
    /**
     * Deallocate memory
     * Does nothing for monotonic allocator
     */
    void deallocate(T*, std::size_t) noexcept {
        // Monotonic allocator - no individual deallocation
    }
    
    /**
     * Rebind allocator to different type
     */
    template <typename U>
    struct rebind {
        using other = FixedBufAllocator<U>;
    };
    
    // Allow access to members from other instantiations
    template <typename U>
    friend class FixedBufAllocator;
};

/**
 * Equality comparison for allocators
 * Two allocators are equal if they share the same buffer
 */
template <typename T, typename U>
bool operator==(const FixedBufAllocator<T>& a, const FixedBufAllocator<U>& b) noexcept {
    return a.m_buffer == b.m_buffer;
}

template <typename T, typename U>
bool operator!=(const FixedBufAllocator<T>& a, const FixedBufAllocator<U>& b) noexcept {
    return !(a == b);
}

} // namespace stack_string
