#pragma once
#include <stdexcept>

struct unexpected_t { };
inline constexpr unexpected_t unexpected_v{};

template <typename T, typename E>
class expected {
  public:
    expected() = default;
    expected(const T& t);
    expected(const unexpected_t& u, const E& e);

    explicit operator bool() const {
        return isExpected;
    };
    const T& operator*() const {
        return m_expected_v;
    };

    T get_value() const;
    bool has_value() const;
    E error() const;

  private:
    T m_expected_v;
    E m_unexpected_value;
    bool isExpected{false};
};

template <typename T, typename E>
expected<T, E>::expected(const T& t) : m_expected_v(t), isExpected(true) {
}

template <typename T, typename E>
expected<T, E>::expected(const unexpected_t& u, const E& e)
    : m_unexpected_value(e){

      };

template <typename T, typename E>
T expected<T, E>::get_value() const {
    if(isExpected) {
        return m_expected_v;
    } else {
        throw std::runtime_error("Access to expected value of an unexpected/unintialised object");
    }
}

template <typename T, typename E>
bool expected<T, E>::has_value() const {
    return isExpected;
}

template <typename T, typename E>
E expected<T, E>::error() const {
    if(!isExpected) {
        return m_unexpected_value;
    } else {
        throw std::runtime_error("Access of error value of expected object");
    }
}