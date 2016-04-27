#pragma once

#include <memory>
#include <vector>
#include <mutex>

class MemBlockPool {
public:
    static MemBlockPool& instance();

    using MemBlock = std::shared_ptr<std::vector<uint8_t>>;
    MemBlock get(size_t capacity = 1024 * 1024);

private:
    struct Deleter {
        void operator()(std::vector<uint8_t>* p) const;
    };

    MemBlockPool();

private:
    std::vector<size_t> m_capacities;
    std::vector<std::unique_ptr<std::vector<uint8_t>>> m_pool;
    std::mutex m_mutex;
};