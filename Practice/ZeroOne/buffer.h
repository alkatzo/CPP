#pragma once
#include <cstddef>
#include <utility>
#include <cstring>
#include <memory>
#include <span>

class Buffer
{
public:
    Buffer() : buf{nullptr}, n{0} {}
    Buffer(size_t n) : buf{std::make_unique<char[]>(n)}, n{n} {}
    Buffer(std::span<const char> input) : buf{std::make_unique<char[]>(input.size())}, n{input.size()} {
        std::copy(input.begin(), input.end(), buf.get());
    }

    ~Buffer() = default;

    Buffer(const Buffer &b) : buf{std::make_unique<char[]>(b.n)}, n{b.n} {
        std::memcpy(buf.get(), b.buf.get(), n);
    }

    Buffer& operator=(const Buffer& b) {
        if (this != &b) {
            auto tmp = std::make_unique<char[]>(b.n);
            std::memcpy(tmp.get(), b.buf.get(), b.n);
            buf = std::move(tmp);
            n = b.n;
        }
        return *this;
    }

    Buffer(Buffer&& b) = default;

    Buffer& operator=(Buffer&& b) noexcept = default;

private:
    std::unique_ptr<char[]> buf;
    size_t n;
};


