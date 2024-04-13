#pragma once

template<class T>
struct Link {
public:
	Link():_ptr(nullptr) {}
	Link(T& value):_ptr(&value) {}
	Link(const Link& other): _ptr(other._ptr) {}

	Link& operator =(const Link& other) & {
		_ptr = other._ptr;
		return *this;
	}

	Link& operator =(T& value) & {
		_ptr = &value;
		return *this;
	}

	T& operator()() const {
		return *_ptr;
	}

	operator T&() const {
		return *_ptr;
	}

	bool empty() const {
		return _ptr == nullptr;
	}
private:
	T* _ptr;
};

template<class T>
Link(T&) -> Link<T>;