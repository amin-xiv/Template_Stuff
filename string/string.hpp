#pragma once
#include <cstddef>



class string {
  public:
    string();
    string(const char* c_ptr);
    string(char c, size_t count);
    string(const string& str);
    string(string&& str) noexcept;
    string& operator=(const string& str);
    string& operator=(string&& str) noexcept;
    ~string();

    const char* data() const;
    size_t size() const;
    void clear();
    explicit operator bool() const;
    char& operator[](const size_t i) const;
    char& at(const size_t i) const;
    char& front() const;
    char& back() const;
    bool empty() const;
    int length() const;
    size_t capacity() const;
    size_t get_current_capacity() const;
    void reserve(const size_t s);
    void shrink_to_fit();
    void push_back(const char c);
    void pop_back();
    void append(const string& str);
    void append(const char* c_ptr);
    void append(char* c_ptr);
    void append(const char& c);
    void append(const char c, size_t count);
    void operator+=(const string& str);
    void operator+=(const char* c_ptr);
    void operator+=(char* c_ptr);
    void insert(size_t pos, const string& str);
    void insert(size_t pos, const char* c_ptr);
    void insert(size_t pos, char* c_ptr);
    //void insert(size_t pos, char c_ptr);

  private:
    // M_CAPACITY, m_current_capacity, m_size, all NOT including null terminator
    static const size_t M_CAPACITY{1000000000};
    size_t m_current_capacity{}; 
    size_t m_size{};
    char* m_data{nullptr};

    void create_null();
};
