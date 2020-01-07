#pragma once

#include <memory>

namespace Containers {

    template <typename T, typename Allocator>
    class Vector;
    template<typename T, typename Allocator>
    class VectorIterator;

    template <typename T, typename Allocator = std::allocator<T>>
    class Vector {
    public:

        friend VectorIterator<T, Allocator>;

        struct deleter {
            deleter(Allocator* allocator, size_t size) : allocator_(allocator), size_(size) {}
            void operator() (T* ptr) {
                if (ptr != nullptr) {
                    allocator_->deallocate(ptr, size_);
                }
            }
        private:
            Allocator* allocator_;
            size_t size_;
        };

        Vector() = default;

        Vector(size_t size) : size_(size) {
            T* ptr = allocator_.allocate(size);
            data_ = std::shared_ptr<T>(ptr, deleter(&allocator_, size));
        }


        ~Vector() {
            for (size_t i = 0; i < size_; ++i) {
                std::allocator_traits<Allocator>::destroy(allocator_, (data_.get()) + i);
            }
        }

        Vector(const Vector &) = delete;

        Vector(Vector &&) = delete;

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
            if (new_size == size_) {
                return;
            }
            if (new_size == 0) {
                data_ = nullptr;
                return;
            }
            if (new_size < size_) {
                for (size_t i = new_size; i < size_; ++i) {
                    std::allocator_traits<Allocator>::destroy(allocator_,data_.get() + i);
                }
            }
            T* ptr = allocator_.allocate(new_size);
            std::shared_ptr<T> new_data = std::shared_ptr<T>(ptr, deleter(&allocator_, new_size));
            for (size_t i = 0; i < std::min(new_size, size_); ++i) {
                new_data.get()[i] = data_.get()[i];
            }
            data_ = new_data;
            size_ = new_size;
        }


        VectorIterator<T, Allocator> begin() {
            return VectorIterator<T, Allocator>(this, 0);
        }

        VectorIterator<T, Allocator> end() {
            return VectorIterator<T, Allocator>(this, size_);
        }

        void Insert(VectorIterator<T, Allocator> it, T elem) {
            if (it.container_ != this) {
                throw std::runtime_error("Wrong iterator");
            }
            size_t position = it.pos_;
            Resize(size_ + 1);
            for (size_t i = size_ - 1; i > position; --i) {
                data_.get()[i] = data_.get()[i - 1];
            }
            data_.get()[position] = elem;
        }

        void Erase(VectorIterator<T, Allocator> it) {
            if (it.container_ != this) {
                throw std::runtime_error("Wrong iterator");
            }
            T* temp = data_.get();
            size_t position = it.pos_;
            for (size_t i = position; i < size_ - 1; ++i) {
                data_.get()[i] = data_.get()[i + 1];
            }
            data_.get()[size_ - 1] = *temp;
            Resize(size_ - 1);
        }


        size_t Size() const {
            return size_;
        }

    private:
        Allocator allocator_;
        std::shared_ptr<T> data_ = nullptr;
        size_t size_ = 0;
    };


    template<typename T, typename Allocator>
    class VectorIterator {
    public:

        friend Vector<T, Allocator>;

        using value_type = T;
        using reference = T &;
        using pointer = T *;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;


        VectorIterator(Vector<T, Allocator> *cont, size_t pos)
                : container_(cont), pos_(pos) {}

        T &operator*() {
            return (*container_)[pos_];
        }

        bool operator==(const VectorIterator &other) {
            return container_ == other.container_ && pos_ == other.pos_;
        }

        bool operator!=(const VectorIterator &other) {
            return !(*this == other);
        }

        VectorIterator &operator++() {
            if (pos_ + 1 > container_->size_) {
                throw std::runtime_error("Out of bounds");
            } else {
                pos_++;
            }
            return *this;
        };

        VectorIterator operator++(int) {
            VectorIterator result = *this;
            ++(*this);
            return result;
        }

    private:
        Vector<T, Allocator> *container_;
        size_t pos_;
    };

}