#define _CRT_SECURE_NO_WARNINGS 

#include "string.hpp"
#include <cstring>
#include <stdexcept>
#include <cstddef>

string::string() {
    m_data = new char[1];
    m_data[0] = '\0';
    m_size = 0;
    m_current_capacity = 0;
}

string::string(const char* c_ptr) {
    size_t new_length{std::strlen(c_ptr) + 1};  // +1 for the null terminator
    m_data = new char[new_length];
    std::strncpy(m_data, c_ptr, new_length);
    m_size = new_length - 1;
    m_current_capacity = m_size;
}

string::string(char c, size_t count) {
    if(count >= M_CAPACITY) {
        throw std::overflow_error("Failed to intialize string, count is too large");
    }
    size_t new_length{count + 1};  // +1 for the null terminator
    m_data = new char[new_length];
    for(size_t i{}; i < count; ++i) {
        m_data[i] = c;
    }
    m_data[count] = '\0';
    m_size = new_length - 1;
    m_current_capacity = m_size;
}

string::string(const string& str) {
    size_t str_size{str.size()};
    size_t new_length{str_size + 1};
    m_data = new char[new_length];
    m_data[str_size] = '\0';
    std::strncpy(m_data, str.data(), new_length);
    m_size = str_size;
    m_current_capacity = str_size;
}

string::string(string&& str) noexcept {
    m_data = const_cast<char*>(str.data());
    m_size = str.size();
    m_current_capacity = m_size;
    str.clear();
}

string& string::operator=(const string& str) {
    if(this == &str) {
        return *this;
    }

    size_t str_size{str.size()};
    size_t new_length{str_size + 1};

    if(m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    m_data = new char[new_length];
    m_data[str_size] = '\0';
    std::strncpy(m_data, str.data(), new_length);
    m_size = str_size;
    m_current_capacity = str_size;
    return *this;
}

string& string::operator=(string&& str) noexcept {
    if(m_data != nullptr) {
        delete[] m_data;
        m_data = nullptr;
    }

    if(str[0] == '\0') {
        m_data = new char[1];
        *m_data = '\0';
    } else {
        m_data = const_cast<char*>(str.data());
    }
    m_size = str.size();
    m_current_capacity = m_size;
    str.clear();
    return *this;
}

string::~string() {
    if(m_data != nullptr) {
        delete[] m_data;
    }
}

const char* string::data() const {
    return m_data;
}

size_t string::size() const {
    return m_size;
}

void string::clear() {
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
    m_current_capacity = 0;
}

string::operator bool() const {
    return !(m_data == nullptr || m_size < 1 || m_current_capacity < 1);
}

char& string::operator[](const size_t i) const {
    return *(m_data + i);
}

void string::create_null() {
    char* terminator{m_data + m_size};
    if(*terminator == '\0') {
        return;
    } else {
        *terminator = '\0';
        return;
    }
}

char& string::at(const size_t i) const {
    if(m_data == nullptr || m_size < 1 || i > (m_size - 1) ||
       i > (m_current_capacity - 1)) {  // -1 to take the null terminator into consideration
        throw std::out_of_range("Invalid string access");
    }
    return *(m_data + i);
}

char& string::front() const {
    return this->at(0);
}

char& string::back() const {
    return this->at(m_size - 1);
}

bool string::empty() const {
    return *this ? false : true;
}

int string::length() const {
    return static_cast<int>(this->size());
}

size_t string::capacity() const {
    return M_CAPACITY;
}

size_t string::get_current_capacity() const {
    if(m_current_capacity < 1 || m_data == nullptr) {
        return 0;
    }

    return m_current_capacity;
}

void string::reserve(const size_t s) {
    if(s < 1) {
        throw std::runtime_error("can't reserve a value < 1");
    }

    // Check if newely requested size < the current size
    if(s <= m_size) {
        return;
    }

    if((s + m_size) >= M_CAPACITY) {
        throw std::length_error("can't reserve memory above total capacity");
    }

    const size_t new_buffer_size{s + 1};  // +1 for the null terminator

    if(m_data == nullptr || m_size < 1) {
        m_data = new char[new_buffer_size];
        return;
    }

    // Temp buffer to copy existing data
    char* new_data = new char[new_buffer_size];
    std::strncpy(new_data, m_data, m_size + 1);
    delete[] m_data;
    m_data = new_data;
    m_current_capacity = s;
    new_data = nullptr;
}

void string::shrink_to_fit() {
    if(m_current_capacity == m_size || m_data == nullptr || m_size < 1) {
        return;
    }

    char* temp_buffer = new char[m_size + 1];
    std::strncpy(temp_buffer, m_data, m_size + 1);
    delete[] m_data;
    m_data = temp_buffer;
    m_current_capacity = m_size;
    temp_buffer = nullptr;
}

void string::push_back(const char c) {
    if((m_size + 1) >= M_CAPACITY) {
        throw std::length_error("string character limit can't be exceeded");
    }

    if(m_current_capacity == m_size) {
        char* new_data{nullptr};
        new_data = new char[m_size + 2];  // +2 => +1 for the null terminator, +1 for the new char

        if(m_data == nullptr) {
            m_data = new_data;
            m_size++;
            m_current_capacity++;
            this->at(m_size) = c;
            this->create_null();
        } else {
            std::strncpy(new_data, m_data, m_size + 1);
            delete[] m_data;
            m_data = new_data;
            m_size++;
            m_current_capacity++;
            this->at(m_size - 1) = c;  // -1 due to conversion from SIZE to INDEX, which is zero-based
            this->create_null();
        }
        new_data = nullptr;
    } else if(m_current_capacity > m_size) {
        m_size++;
        this->at(m_size - 1) = c;  // -1 due to conversion from SIZE to INDEX, which is zero-based
        this->create_null();
    } else {
        throw std::runtime_error("unkown error occured while pushing back to string");
    }
}

void string::pop_back() {
    this->back() = '\0';
    m_size--;
}

void string::append(const string& str) {
    if(str.size() < 1 || str.data() == nullptr) {
        return;
    }

    if(m_data != nullptr) {
        if((str.size() + m_size) > m_current_capacity) {
            const size_t new_length{m_size + str.size() + 1};
            char* new_data = new char[new_length];  // +1 for the null terminator

            std::strncpy(new_data, m_data, new_length - str.size());
            std::strncpy((new_data + m_size), str.data(), new_length - m_size);  // overwrite the null terminator
            delete[] m_data;
            m_data = new_data;
            m_current_capacity = new_length - 1;  // -1 to account for the null terminator

        } else {
            std::strncpy((m_data + m_size), str.data(), str.size() + 1);
        }
    } else if(m_data == nullptr) {
        m_data = new char[str.size() + 1];  // +1 for null terminator
        std::strncpy(m_data, str.data(), str.size() + 1);
        m_current_capacity = str.size();
    } else {
        throw std::runtime_error("unkown error occured while appending to string");
    }

    m_size += str.size();
}

void string::append(const char* c_ptr) {
    if(c_ptr == nullptr || *c_ptr == '\0') {
        return;
    }

    append(string{c_ptr});
}

void string::append(char* c_ptr) {
    if(c_ptr == nullptr || *c_ptr == '\0') {
        return;
    }

    this->append(string{c_ptr});
}

void string::append(const char& c) {
    this->push_back(c);
}

void string::append(const char c, size_t count) {
    if(count == 0 || c == '\0') {
        return;
    }

    for(size_t i{}; i < count; ++i) {
        this->push_back(c);
    }
}

void string::operator+=(const string& str) {
    this->append(str);
}

void string::operator+=(const char* c_ptr) {
    this->append(c_ptr);
}

void string::operator+=(char* c_ptr) {
    this->append(c_ptr);
}

void string::insert(size_t pos, const string& str) {
    if(str.data() == nullptr || str.size() < 1) {
        return;
    }

    if(pos > m_size) {
        throw std::out_of_range("bad string insertion");
    }

    if(m_size < 1 && m_data == nullptr) {
        *this = str;
        return;
    }

    if(pos == 0) {
        string temp{str};
        temp.append(m_data);
        *this = temp;
    } else if(pos == m_size) {
        this->append(str);
    } else {
        char* new_data = new char[m_size + str.size() + 1];
        // strncpy and not strcpy_s due to not copying a null pointer intitally
        strncpy(new_data, m_data, pos);                   // pos and not pos-1 in order to PUSH the char at pos
        strncpy(new_data + pos, str.data(), str.size());  // not pos+1 cuz this evaluates to new_data PLUS pos
        strncpy(new_data + pos + str.size(), m_data + pos, m_size - pos);
        delete[] m_data;
        m_data = new_data;
        m_size = str.size() + m_size;
        m_current_capacity = m_size;
        this->create_null();
        new_data = nullptr;
    }
}

void string::insert(size_t pos, const char* c_ptr) {
    if(c_ptr == nullptr) {
        return;
    }

    if(pos > m_size) {
        throw std::out_of_range("bad string insertion");
    }

    if(m_size < 1 && m_data == nullptr) {
        *this = string{c_ptr};
        return;
    }

    string temp{c_ptr};
    this->insert(pos, temp);
}

void string::insert(size_t pos, char* c_ptr) {
    if(c_ptr == nullptr) {
        return;
    }

    if(pos > m_size) {
        throw std::out_of_range("bad string insertion");
    }

    if(m_size < 1 && m_data == nullptr) {
        *this = string{c_ptr};
        return;
    }

    string temp{c_ptr};
    this->insert(pos, temp);
}