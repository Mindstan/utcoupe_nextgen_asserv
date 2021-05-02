#ifndef UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP
#define UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP

#include <array>
#include <functional>
#include <optional>

namespace utcoupe::asserv::utilities {
    template<typename ElementT, std::size_t Capacity>
    class StaticQueue {
    public:
        constexpr StaticQueue() noexcept = default;
        
        constexpr std::optional<std::reference_wrapper<ElementT>> back() noexcept {
            if (empty()) {
                return std::nullopt;
            }
            auto index = (m_end + Capacity - 1) % Capacity;
            return m_array[index];
        }
        
        constexpr std::size_t capacity() const noexcept {
            return Capacity;
        }
        
        constexpr bool empty() const noexcept {
            return m_empty;
        }
        
        constexpr std::optional<std::reference_wrapper<ElementT>> front() noexcept {
            if (empty()) {
                return std::nullopt;
            }
            return m_array[m_begin];
        }
        
        constexpr std::optional<ElementT> pop() noexcept {
            if (empty()) {
                return std::nullopt;
            }
            auto oldIndex = m_begin;
            m_begin = (m_begin + 1) % Capacity;
            if (m_begin == m_end) {
                m_empty = true;
            }
            return std::move(m_array.at(oldIndex));
        }
        
        constexpr bool push(const ElementT& elem) noexcept {
            return push(std::move(ElementT{elem}));
        }
        
        constexpr bool push(ElementT&& elem) noexcept {
            if (size() == Capacity) {
                return false;
            }
            m_empty = false;
            m_array.at(m_end) = std::move(elem);
            m_end = (m_end + 1) % Capacity;
            return true;
        }
        
        constexpr std::size_t size() const noexcept {
            if (!m_empty && m_end == m_begin) {
                return Capacity;
            }
            return (m_end + Capacity - m_begin) % Capacity;
        }
        
    private:
        bool m_empty {true};
        std::size_t m_begin {0};
        std::size_t m_end {0};
        std::array<ElementT, Capacity> m_array;
    };
} // namespace utcoupe::asserv::utilities

#endif // UTCOUPE_ASSERV_UTILITIES_STATIC_QUEUE_HPP
