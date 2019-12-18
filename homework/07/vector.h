#pragma once
#include <memory>
#include <utility>
#include <exception>
#include <algorithm>
#include <iterator>

template <class T>
class Allocator
{
public:
    using pointer = T*;
    
    Allocator() noexcept {}
    
    template<class U>
    Allocator(const Allocator<U>& other) noexcept {}
    
    ~Allocator() {}
    
    pointer allocate(size_t n) {
        return reinterpret_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(pointer ptr, size_t n) {
        ::operator delete[](ptr, sizeof(T) * n);
    }
    
    template<class ... Args>
    void construct(pointer p, Args&&... args) {
        ::new(p) T(std::forward<Args>(args)...);
    }

    void destroy(pointer p) {
        p->~T();
    }
};

template <class T>
class Iterator 
: public std::iterator<std::random_access_iterator_tag, T, size_t>
{ 
public:     
    using ptr = T*;
    using reference = T&;
    
    Iterator(ptr p, bool is_rev) noexcept : memory(p), is_reversed(is_rev) {}
    Iterator(ptr p) noexcept : memory(p), is_reversed(false) {}
    ~Iterator() {}
    
    const reference operator*() const {
        return *memory;
    }
    
    reference operator*() {
        return *memory;
    }
    
    const ptr operator->() const {
        return memory;
    }
    
    ptr operator->() {
        return memory;
    }
    
    Iterator& operator=(const Iterator& other) {
        is_reversed = other.is_reversed;
        memory = other.memory;
        return *this;
    }
    
    Iterator operator+(size_t n) const {
        if (is_reversed)
            return Iterator(memory - n, is_reversed);
        else
            return Iterator(memory + n, is_reversed);
    }
    
    Iterator operator-(size_t n) const {
        if (is_reversed)
            return Iterator(memory + n, is_reversed);
        else
            return Iterator(memory - n, is_reversed);
    }
    
    friend Iterator operator+(size_t n, const Iterator& other) {
        if (other.is_reversed)
            return Iterator(other.memory - n, other.is_reversed);
        else
            return Iterator(other.memory + n, other.is_reversed);
    }
    
    friend Iterator operator-(size_t n, const Iterator& other) {
        if (other.is_reversed)
            return Iterator(other.memory + n, other.is_reversed);
        else
            return Iterator(other.memory - n, other.is_reversed);
    }
    
    size_t operator-(const Iterator& other) const {
        if (is_reversed)
            return other.memory - memory;
        else
            return memory - other.memory;
    }

    reference operator[](int index) {
        return *(memory + (is_reversed? -index: index));
    }
    
    const reference operator[](int index) const {
        return *(memory + (is_reversed? -index: index));
    }

    bool operator==(const Iterator& other) const noexcept {
        return std::tie(is_reversed, memory) ==
                std::tie(other.is_reversed, other.memory);
    }

    bool operator!=(const Iterator& other) const noexcept {
        return !(*this == other);
    }

    bool operator < (const Iterator& other) const {
        if (is_reversed != other.is_reversed)
            throw std::invalid_argument(
                    "can't compare reversed and not"
                    );
        return is_reversed ^ (memory < other.memory);
    }

    bool operator > (const Iterator& other) const {
        return other < (*this);
    }

    bool operator >= (const Iterator& other) const {
        return (*this) > other || (*this) == other;
    }

    bool operator <= (const Iterator& other) const {
        return other >= (*this);
    }

    Iterator& operator++() {
        if (is_reversed)
            --memory;
        else
            ++memory;
        return *this;
    }
    Iterator operator++(int) {
        auto res = *this;
        ++(*this);
        return res;
    }  // Postfix

    Iterator& operator--() {
        if (is_reversed)
            ++memory;
        else
            --memory;
        return *this;
    }
    Iterator operator--(int) {
        auto res = *this;
        --(*this);
        return res;
    }

    Iterator& operator+=(int n) {
        if (is_reversed)
           memory -= n;
        else
            memory += n;
        return *this;
    }

    Iterator& operator-=(int n) {
        if (is_reversed)
            memory += n;
        else
            memory -= n;
        return *this;
    }
    
private:
    bool is_reversed;
    ptr memory;
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using iterator = Iterator<T>;
    using ptr = T*;
    using reference = T&;
    
    Vector(size_t size) 
    : alloc_(Alloc()),  size_(size), allocated_size(size),
    memory(alloc_.allocate(size))
    {
        for (int i = 0; i < size; ++i)
            alloc_.construct(memory + i);
    }
    
    Vector() 
    : alloc_(Alloc()),  size_(0), allocated_size(0),
    memory(nullptr) {}
    
    ~Vector() {
        for (size_t i = 0; i < size_; ++i)
            alloc_.destroy(memory + i);
        alloc_.deallocate(memory, allocated_size);
    }
    
    void reserve(size_t sz) {
        if (sz < size_)
            throw std::invalid_argument("can't reserve < this->size");
        
        ptr new_memory = alloc_.allocate(sz);
        for (int i = 0; i < size_; ++i) {
            alloc_.construct(new_memory + i, std::move(memory[i]));
            alloc_.destroy(memory + i);
        }
        
        alloc_.deallocate(memory, allocated_size);
        allocated_size = sz;
        memory = new_memory;
    }
    
    void resize(size_t sz) {
        if (sz < 0)
            throw std::invalid_argument("size can't be < 0");
        
        if (sz > allocated_size)
            reserve(std::max(2 * size_, sz));
        
        for (size_t i = size_; i < sz; ++i)
            alloc_.construct(memory + i);
        
        size_ = sz;
    }
    
    void pop_back() { 
        resize(size_ - 1);
    }
    
    void clear() {
        resize(0);
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    size_t size() const {
        return size_;
    }
    
    void push_back(T&& element) {
        if (size_ == allocated_size)
            reserve(size_ == 0 ? 1: 2 * size_);
        alloc_.construct(memory + (size_++), std::forward<T>(element));
    }
    
    void push_back(const reference element) {
        if (size_ == allocated_size)
            reserve(size_ == 0 ? 1: 2 * size_);
        alloc_.construct(memory + (size_++), element);
    }
    
    reference operator[] (size_t ind) {
        if (size() <= ind || ind < 0)
            throw std::invalid_argument("index error");
        return memory[ind];
    }
    
    const reference operator[] (size_t ind) const {
        if (size() <= ind || ind < 0)
            throw std::invalid_argument("index error");
        return memory[ind];
    }
    
    iterator begin() {
        return iterator(memory);
    }
    
    const iterator begin() const {
        return iterator(memory);
    }
    
    iterator end() {
        return iterator(memory + size());
    }
    
    const iterator end() const {
        return iterator(memory + size());
    }
    
    iterator rbegin() {
        return iterator(memory + size() - 1, true);
    }
    
    const iterator rbegin() const {
        return iterator(memory + size() - 1, true);
    }
    
    iterator rend() {
        return iterator(memory - 1, true);
    }
    
    const iterator rend() const {
        return iterator(memory - 1, true);
    }

private:
    Alloc alloc_;
    size_t size_;
    size_t allocated_size;
    ptr memory;
};

