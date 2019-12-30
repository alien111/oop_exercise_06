#pragma once

#include <iostream>
#include <algorithm>
#include <list>
#include "Stack.h"

enum class MemoryNodeType {
    Hole,
    Occupied
};

struct MemoryNode {
    char* beginning;
    size_t capacity;
    MemoryNodeType type;
};

template <typename T, size_t ALLOC_SIZE>
class Allocator {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using is_always_equal = std::false_type;

    Allocator(const Allocator&) = delete;
    Allocator(Allocator&&) = delete;

    template<class V>
    struct rebind {
        using other = Allocator<V, ALLOC_SIZE>;
    };

    Allocator() {
        data = (char *) malloc(ALLOC_SIZE);
        mem_list.Push({data, ALLOC_SIZE, MemoryNodeType::Hole});
    }

    ~Allocator() {
        free(data);
    }

    T* allocate(size_t mem_size) {
        mem_size *= sizeof(T);
        auto it = std::find_if(mem_list.begin(), mem_list.end(), [&mem_size] (const MemoryNode& node) {
            return node.type == MemoryNodeType::Hole && node.capacity >= mem_size;
        });
        if (it == mem_list.end()) {
            throw std::runtime_error("No memory");
        }
        if (it->capacity == mem_size) {
            it->type = MemoryNodeType::Occupied;
        } else {
            auto next = std::next(it);
            mem_list.Insert(std::next(it), MemoryNode{it->beginning + mem_size, it->capacity - mem_size, MemoryNodeType::Hole});
            it->type = MemoryNodeType::Occupied;
            it->capacity -= mem_size;
        }
        return (T*)it->beginning;
    }


    void deallocate(T* typed_ptr, size_t) {
        auto cur_it = std::find_if(mem_list.begin(), mem_list.end(), [&typed_ptr] (const MemoryNode& node) {
            return node.type == MemoryNodeType::Occupied && node.beginning == (char*) typed_ptr;
        });
        auto prev_it = mem_list.end();
        for (auto it = mem_list.begin(); it != mem_list.end(); ++it) {
            if (std::next(it) == cur_it) {
                prev_it = it;
                break;
            }
        }
        if (cur_it == mem_list.end()) {
            throw std::runtime_error("Wrong ptr to deallocate");
        }
        if (cur_it != mem_list.begin() && prev_it->type == MemoryNodeType::Hole) {
            cur_it = prev_it;
            cur_it->capacity += std::next(cur_it)->capacity;
            mem_list.Erase(std::next(cur_it));
        }
        if (std::next(cur_it) != mem_list.end() && std::next(cur_it)->type == MemoryNodeType::Hole) {
            cur_it->capacity += std::next(cur_it)->capacity;
            mem_list.Erase(std::next(cur_it));
        }
    }

private:
    Containers::Stack<MemoryNode> mem_list;
    char* data;
};