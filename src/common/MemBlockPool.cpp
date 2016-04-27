#include "common/MemBlockPool.h"

#include "mocca/log/LogManager.h"
#include "mocca/base/Memory.h"

#include <algorithm>

MemBlockPool::MemBlockPool() {
    std::vector<std::pair<size_t, size_t>> initialSizes{
        { 16, 512 * 1024 },
        { 64, 1024 * 1024 },
        { 64, 2 * 1024 * 1024 },
        { 32, 4 * 1024 * 1024 },
        { 16, 8 * 1024 * 1024 },
        { 4, 16 * 1024 * 1024 }
    };
    for (const auto& s : initialSizes) {
        for (size_t i = 0; i < s.first; ++i) {
            m_capacities.push_back(s.second);
            auto block = mocca::make_unique<std::vector<uint8_t>>();
            block->reserve(s.second);
            m_pool.push_back(std::move(block));
        }
    }
}

MemBlockPool& MemBlockPool::instance() {
    static MemBlockPool pool;
    return pool;
}

MemBlockPool::MemBlock MemBlockPool::get(size_t capacity) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto capIt = std::lower_bound(begin(m_capacities), end(m_capacities), capacity);
        if (capIt != end(m_capacities)) {
            auto poolIt = begin(m_pool);
            std::advance(poolIt, std::distance(begin(m_capacities), capIt));
            m_capacities.erase(capIt);
            auto block = std::move(*poolIt);
            m_pool.erase(poolIt);
            return MemBlock(block.release(), Deleter());
        }
    }
    LDEBUG("MemBlockPool: Could not satisfy request for block of size " << capacity);
    MemBlock block = std::make_shared<std::vector<uint8_t>>();
    block->reserve(capacity);
    return block;
}

void MemBlockPool::Deleter::operator()(std::vector<uint8_t>* p) const {
    p->clear();
    std::unique_ptr<std::vector<uint8_t>> ptr(p);
    auto& inst = MemBlockPool::instance();
    {
        std::lock_guard<std::mutex> lock(inst.m_mutex);
        auto capIt = std::lower_bound(begin(inst.m_capacities), end(inst.m_capacities), p->capacity());
        auto poolIt = begin(inst.m_pool);
        std::advance(poolIt, std::distance(begin(inst.m_capacities), capIt));
        inst.m_capacities.insert(capIt, p->capacity());
        inst.m_pool.insert(poolIt, std::move(ptr));
    }
}