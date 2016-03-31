#include "common/MemBlockPool.h"

#include "mocca/log/LogManager.h"

MemBlockPool& MemBlockPool::instance() {
    static MemBlockPool pool;
    return pool;
}

MemBlockPool::MemBlock MemBlockPool::get(size_t capacity) {
    for (auto it = begin(m_pool); it != end(m_pool); ++it) {
        if ((*it)->capacity() >= capacity) {
            auto block = *it;
            m_pool.erase(it);
            return block;
        }
    }
    LDEBUG("MemBlockPool: new block with capacity " << capacity);
    MemBlock block(new std::vector<uint8_t>(capacity), Deleter());
    block->reserve(capacity);
    return block;
}

void MemBlockPool::Deleter::operator()(std::vector<uint8_t>* p) const {
    MemBlock recycled(p, Deleter());
    recycled->clear();
    MemBlockPool::instance().m_pool.push_back(recycled);
}