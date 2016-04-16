#pragma once

#include <memory>
#include <vector>

class MemBlockPool {
public:
    static MemBlockPool& instance();

    using MemBlock = std::shared_ptr<std::vector<uint8_t>>;
    MemBlock get(size_t capacity = 1024 * 1024);

private:
    struct Deleter {
        void operator()(std::vector<uint8_t>* p) const;
    };

    MemBlockPool() = default;

private:
    std::vector<std::unique_ptr<std::vector<uint8_t>>> m_pool;
};