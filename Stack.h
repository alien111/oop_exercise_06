#pragma once

#include <memory>
#include <exception>

namespace Containers {

    template <typename T>
    struct StackNode {
        T data;
        std::shared_ptr<StackNode> next;
        std::weak_ptr<StackNode> prev;
    };

    template <typename T>
    struct StackIterator {
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        StackIterator(std::shared_ptr<StackNode<T>> ptr)
        : ptr_(ptr){}

        T& operator * () {
            std::shared_ptr<StackNode<T>> locked = ptr_.lock();
            if (!locked) {
                throw std::runtime_error("Iterator does not exist");
            }
            return locked->data;
        }

        T* operator -> () {
            std::shared_ptr<StackNode<T>> locked = ptr_.lock();
            if (!locked) {
                throw std::runtime_error("Iterator does not exist");
            }
            return &locked->data;
        }

        StackIterator& operator++() {
            std::shared_ptr<StackNode<T>> locked = ptr_.lock();
            if (!locked || locked->next == nullptr) {
                throw std::runtime_error("Out of bounds");
            }
            ptr_ = locked->next;
            return *this;
        }

        const StackIterator operator++(int) {
            auto copy = *this;
            ++(*this);
            return copy;
        }

        bool operator == (const StackIterator& other) const {
            return ptr_.lock() == other.ptr_.lock();
        }

        bool operator != (const StackIterator& other) const {
            return !(*this == other);
        }

        std::weak_ptr<StackNode<T>> ptr_;
    };

    template <typename T, typename Allocator = std::allocator<T>>
    class Stack {
    public:
        using allocator_type = typename Allocator::template rebind<StackNode<T>>::other;

        struct deleter {
            deleter(allocator_type* allocator) : allocator_(allocator) {}

            void operator() (StackNode<T>* ptr) {
                std::allocator_traits<allocator_type >::destroy(*allocator_, ptr);
                allocator_->deallocate(ptr,1);
            }

        private:
            allocator_type* allocator_;
        };

        Stack() {
            StackNode<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type >::construct(allocator_, ptr);
            std::shared_ptr<StackNode<T>> new_elem(ptr, deleter(&allocator_));
            tail = new_elem;
            head = tail;
            tail->next = nullptr;
        }

        Stack(const Stack&) = delete;
        Stack(Stack&&) = delete;

        bool Empty() const {
            return head == tail;
        }

        void Pop() {
            if (Empty()){
                throw std::runtime_error("Pop from empty queue");
            }
            if (head->next == tail) {
                head = tail;
                return;
            }
            std::shared_ptr<StackNode<T>> prev_ptr = tail->prev.lock()->prev.lock();
            prev_ptr->next = tail;
            tail->prev = prev_ptr;
        }

        void Push(T elem) {
            StackNode<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type>::construct(allocator_, ptr);
            std::shared_ptr<StackNode<T>> new_elem(ptr, deleter(&allocator_));
            new_elem->data = std::move(elem);
            if (Empty()) {
                head = new_elem;
                tail->prev = head;
                head->next = tail;
                return;
            }
            std::shared_ptr<StackNode<T>> prev_ptr = tail->prev.lock();
            prev_ptr->next = new_elem;
            tail->prev = new_elem;
            new_elem->next = tail;
            new_elem->prev = prev_ptr;
        }

        StackIterator<T> begin() {
            return StackIterator<T>(head);
        }

        StackIterator<T> end() {
            return StackIterator<T>(tail);
        }

        void Insert(StackIterator<T> iter, T elem) {
            StackNode<T>* ptr = allocator_.allocate(1);
            std::allocator_traits<allocator_type>::construct(allocator_, ptr);
            std::shared_ptr<StackNode<T>> new_elem(ptr, deleter(&allocator_));
            new_elem->data = std::move(elem);
            if (iter == begin()) {
                new_elem->next = head;
                head->prev = new_elem;
                head = new_elem;
            } else {
                std::shared_ptr<StackNode<T>> cur_ptr = iter.ptr_.lock();
                std::shared_ptr<StackNode<T>> prev_ptr = iter.ptr_.lock()->prev.lock();
                prev_ptr->next = new_elem;
                cur_ptr->prev = new_elem;
                new_elem->next = cur_ptr;
                new_elem->prev = prev_ptr;
            }
        }

        void Erase(StackIterator<T> iter) {
            if (iter == end()) {
                throw std::runtime_error("Erasind end iterator");
            }
            std::shared_ptr<StackNode<T>> ptr = iter.ptr_.lock();
            if (iter == begin()) {
                head = head->next;
                ptr->next = nullptr;
            } else {
                std::shared_ptr<StackNode<T>> prev_ptr = ptr->prev.lock();
                std::shared_ptr<StackNode<T>> next_ptr = ptr->next;
                prev_ptr->next = next_ptr;
                next_ptr->prev = prev_ptr;
            }
        }

    private:
        allocator_type allocator_;
        std::shared_ptr<StackNode<T>> head;
        std::shared_ptr<StackNode<T>> tail;
    };

}