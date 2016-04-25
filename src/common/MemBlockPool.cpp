#include "common/MemBlockPool.h"

#include "mocca/log/LogManager.h"

#include <algorithm>

MemBlockPool& MemBlockPool::instance() {
    static MemBlockPool pool;
    return pool;
}

MemBlockPool::MemBlock MemBlockPool::get(size_t capacity) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = std::lower_bound(begin(m_capacities), end(m_capacities), capacity);
        if (it != end(m_capacities)) {
            auto ptrIt = begin(m_pool);
            std::advance(ptrIt, std::distance(begin(m_capacities), it));
            m_capacities.erase(it);
            auto block = std::move(*ptrIt);
            m_pool.erase(ptrIt);
            return MemBlock(block.release(), Deleter());
        }
    }
    LDEBUG("MemBlockPool: new block with capacity " << capacity);
    MemBlock block(new std::vector<uint8_t>(), Deleter());
    return block;
}

void MemBlockPool::Deleter::operator()(std::vector<uint8_t>* p) const {
    p->clear();
    std::unique_ptr<std::vector<uint8_t>> ptr(p);
    auto& inst = MemBlockPool::instance();
    {
        std::lock_guard<std::mutex> lock(inst.m_mutex);
        auto it = std::lower_bound(begin(inst.m_capacities), end(inst.m_capacities), p->capacity());
        auto ptrIt = begin(inst.m_pool);
        std::advance(ptrIt, std::distance(begin(inst.m_capacities), it));
        inst.m_capacities.insert(it, p->capacity());
        inst.m_pool.insert(ptrIt, std::move(ptr));
    }
}