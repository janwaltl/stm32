#pragma once

#include "my_assert.hpp"
#include <array>
#include <atomic>
#include <cstddef>

/*
 * @brief Single-producter single-consumer queue of fixed capacity.
 *
 * Lockfree, interrupt-safe.
 */
template <typename T, std::size_t N>
class RingBuffer {
  public:
    /*******************************************************************************
     * @brief Whether the ring buffer is empty.
     ******************************************************************************/
    bool
    is_empty() const;
    /*******************************************************************************
     * @brief Whether the ring buffer is empty.
     ******************************************************************************/
    bool
    is_full() const;

    /*******************************************************************************
     * @brief Number of currently stored elements.
     ******************************************************************************/
    std::size_t
    size() const;

    /*******************************************************************************
     * @brief Maximum capacity, equal to N.
     ******************************************************************************/
    constexpr std::size_t
    capacity() const;

    /*******************************************************************************
     * @brief Enqueue the element
     *
     * Precondition: the buffer is not full.
     *
     * @param c Element to be inserted.
     ******************************************************************************/
    void
    push_back(T e);

    /*******************************************************************************
     * @brief Extracts the first element from the buffer.
     *
     * Precondition: the buffer is not empty.
     *
     * @return the popped element.
     ******************************************************************************/
    T
    pop_front();

  private:
    using index_t = std::atomic_uint32_t;
    static_assert(index_t::is_always_lock_free, "We assume lockfree access");

    index_t m_begin = 0;
    index_t m_end = 0;

    /*
     * @brief Buffer storage
     *
     * Plus one because we need to be able to distinguish between empty and full
     * buffer. In both cases m_begin==m_end would be true, so let's make an
     * invariant that the buffer can only ever contain N elements so the
     * equality means empty.
     *
     * Contains valid data in range [m_begin,m_end) mod N.
     */
    std::array<T, N + 1> m_buffer;
};

template <typename T, std::size_t N>
bool
RingBuffer<T, N>::is_empty() const {
    return m_begin == m_end;
}

template <typename T, std::size_t N>
bool
RingBuffer<T, N>::is_full() const {
    return size() == capacity();
}

template <typename T, std::size_t N>
std::size_t
RingBuffer<T, N>::size() const {
    return ((m_end + m_buffer.size()) - m_begin) % m_buffer.size();
}

template <typename T, std::size_t N>
constexpr std::size_t
RingBuffer<T, N>::capacity() const {
    return N;
}

template <typename T, std::size_t N>
void
RingBuffer<T, N>::push_back(T e) {
    my_assert(!is_full(), "Cannot push into full buffer");

    auto i = m_end.load(std::memory_order_acquire);
    m_buffer[i] = std::move(e);
    m_end.store((i + 1) % m_buffer.size(), std::memory_order_release);
}

template <typename T, std::size_t N>
T
RingBuffer<T, N>::pop_front() {
    my_assert(!is_empty(), "Cannot pop from empty buffer");

    auto i = m_begin.load(std::memory_order_acquire);
    T e = std::move(m_buffer[i]);
    m_begin.store((i + 1) % m_buffer.size(), std::memory_order_release);
    return e;
}
