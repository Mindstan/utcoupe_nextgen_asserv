#ifndef UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP
#define UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP

#include <array>
#include <optional>

namespace utcoupe::asserv::utilities {
    template<typename ElementT, std::size_t Capacity>
    class StaticQueue {
    public:
        constexpr StaticQueue() noexcept = default;
        
        constexpr ElementT* back() noexcept {
            if (size() == 0) {
                return nullptr;
            }
            auto index = (m_end + Capacity - 1) % Capacity;
            return &m_array[index];
        }
        
        constexpr std::size_t capacity() const noexcept {
            return Capacity;
        }
        
        constexpr bool empty() const noexcept {
            return size() == 0;
        }
        
        constexpr ElementT* front() noexcept {
            if (empty()) {
                return nullptr;
            }
            return &m_array[m_begin];
        }
        
        constexpr std::optional<ElementT> pop() noexcept {
            if (empty()) {
                return std::nullopt;
            }
            auto oldIndex = m_begin;
            m_begin = (m_begin + 1) % Capacity;
            return std::move(m_array.at(oldIndex));
        }
        
        constexpr bool push(const ElementT& elem) noexcept {
            return push(std::move(ElementT{elem}));
        }
        
        constexpr bool push(ElementT&& elem) noexcept {
            if (size() + 1 == Capacity) {
                return false;
            }
            m_array.at(m_end) = std::move(elem);
            m_end = (m_end + 1) % Capacity;
            return true;
        }
        
        constexpr std::size_t size() const noexcept {
            return (m_end + Capacity - m_begin) % Capacity;
        }
        
    private:
        std::size_t m_begin {0};
        std::size_t m_end {0};
        std::array<ElementT, Capacity> m_array;
    };
} // namespace utcoupe::asserv::utilities

#endif // UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP
