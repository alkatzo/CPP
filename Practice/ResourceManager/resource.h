#pragma once
#include <iostream>
#include <concepts>
#include <unistd.h>
#include <utility>

template<typename T>
concept IsPointer = std::is_pointer_v<T>;

template<typename T>
class Resource
{
public:
    Resource(T resource) : resource{resource} {
    }

    Resource(const Resource&) = delete;
    Resource & operator=(const Resource&) = delete;

    Resource(Resource&& r) : resource{std::move(r.resource)} {
        r.resource = -1;
        std::cout << "Resource moved via ctor\n";
    }

    Resource(Resource&& r) requires IsPointer<T> : resource{r.resource} {
        r.resource = nullptr;
    }

    Resource & operator=(Resource&& r) {
        if(this != &r) {
            deallocate();
            resource = r.resource;
            r.resource = -1;
            std::cout << "Resource moved via assignment\n";
        }
    }

    Resource & operator=(Resource&& r) requires IsPointer<T> {
        if(this != &r) {
            deallocate();
            resource = r.resource;
            r.resource = nullptr;
            std::cout << "ptr Resource moved via assignment\n";
        }
    }

    ~Resource() {
        deallocate();
    }

    T* get() {
        return resource;
    }

private:
    void deallocate() {
        close(resource);
        resource = -1;
        std::cout << "Resource deallocated\n";
    }

    void deallocate() requires IsPointer<T> {
        delete resource;
        resource = -1;
        std::cout << "ptr Resource deallocated\n";
    }

private:
    T resource;
};

