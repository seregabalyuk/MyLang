#pragma once

#include <cstdint>

template<class T>
struct Link {
public:
	Link():_ptr(nullptr) {}
	Link(T& value):_ptr(&value) {}
	Link(T* value):_ptr(value) {}
	Link(const Link& other): _ptr(other._ptr) {}

	Link& operator =(const Link& other) & {
		_ptr = other._ptr;
		return *this;
	}

	Link& operator =(T& value) & {
		_ptr = &value;
		return *this;
	}

	Link& operator =(T* value) & {
		_ptr = value;
		return *this;
	}

	T& operator()() const {
		return *_ptr;
	}

	operator T&() const {
		return *_ptr;
	}
    template<class U>
    auto operator <=>(const Link<U>& other) {
        return getInt() <=> other.getInt();
    }

    template<class U>
    bool operator ==(const Link<U>& other) {
        return getInt() == other.getInt();
    }

    template<class U>
    bool operator !=(const Link<U>& other) {
        return !(*this == other);
    }

	bool empty() const {
        return _ptr == nullptr;
	}

    uint64_t getInt() const {
        return reinterpret_cast<uint64_t>(_ptr);
    }
private:
	T* _ptr;
};

template<class T>
bool operator ==(const Link<T>& link, const T& other) {
    return &link() == &other;
}

template<class T>
bool operator ==(const T& other, const Link<T>& link) {
    return &link() == &other;
}

template<class T>
Link(T&) -> Link<T>;

template<class T>
Link(T*) -> Link<T>;