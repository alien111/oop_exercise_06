#pragma once

#include <memory>
#include <exception>

namespace Containers {

    template <typename T, typename Allocator>
    class Vector;

    template <typename T>
    class VectorIterator;

    template<typename T, typename Allocator = std::allocator<T>>
    class Vector {
    public:
        friend VectorIterator<T>;

        struct deleter {
            deleter(Allocator* allocator) : allocator_(allocator) {}
            void operator() (T* ptr) {
                if (ptr != nullptr) {
                    std::allocator_traits<Allocator>::destroy(*allocator_,ptr);
                    allocator_->deallocate(ptr, 1);
                }
            }
        private:
            Allocator* allocator_;
        };

        Vector() = default;
        ~Vector() = default;

        Vector(const Vector&) = delete;
        Vector(Vector&&) = delete;

        T &operator[](size_t index) {
            if (index >= size_) {
                throw std::out_of_range("Out of bounds");
            }
            return data_.get()[index];
        }

        const T &operator[](size_t index) const {
            if (index >= size_) {
                throw std::out_of_range("Out of bounds");
            }
            return data_.get()[index];
        }

        void Resize(size_t new_size) {
            if (new_size == 0) {
                data_ = nullptr;
                return;
            }
            T* ptr = allocator_.allocate(new_size);
            std::allocator_traits<Allocator>::construct(allocator_, ptr);
            std::shared_ptr<T> new_elem(ptr, deleter(&allocator_));
            for (size_t i = 0; i < std::min(new_size, size_); ++i) {
                *(new_elem.get() + i) = *(data_.get() + i);
            }
            data_ = new_elem;
            size_ = new_size;
        }


        VectorIterator<T> begin() {
            return VectorIterator<T>(data_, &size_, 0);
        }

        VectorIterator<T> end() {
            return VectorIterator<T>(data_, &size_, size_);
        }


        size_t Size() const {
            return size_;
        }

    private:
        Allocator allocator_;
        std::shared_ptr<T> data_ = nullptr;
        size_t size_ = 0;
    };




template <typename T>
class VectorIterator {
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;


    VectorIterator(std::shared_ptr<T> ptr, size_t* size, size_t pos)
            : ptr_(ptr), size_(size), pos_(pos) {}

    T& operator* () {
        std::shared_ptr<T> locked = ptr_.lock();
        if (locked) {
            if (pos_ >= *size_) {
                throw std::logic_error("Wrong operation");
            }
            return locked.get()[pos_];
        } else {
            throw std::runtime_error("Broken iterator");
        }
    }

    bool operator == (const VectorIterator& other) {
        return ptr_.lock() == other.ptr_.lock() && size_ == other.size_ && pos_ == other.pos_;
    }

    bool operator != (const VectorIterator& other) {
        return !(*this == other);
    }

    VectorIterator& operator++() {
        if (pos_ + 1 > *size_) {
            throw std::runtime_error("Out of bounds");
        } else {
            pos_++;
        }
        return *this;
    };

private:
    std::weak_ptr<T> ptr_;
    size_t* size_;
    size_t pos_;
};

}