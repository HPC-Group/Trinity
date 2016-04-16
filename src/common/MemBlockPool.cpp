#include "common/MemBlockPool.h"

#include "mocca/log/LogManager.h"

MemBlockPool& MemBlockPool::instance() {
    static MemBlockPool pool;
    return pool;
}

MemBlockPool::MemBlock MemBlockPool::get(size_t capacity) {
    for (auto it = begin(m_pool); it != end(m_pool); ++it) {
        if ((*it)->capacity() >= capacity) {
            auto block = std::move(*it);
            m_pool.erase(it);
            return MemBlock(block.release(), Deleter());
        }
    }
    LDEBUG("MemBlockPool: new block with capacity " << capacity);
    MemBlock block(new std::vector<uint8_t>(capacity), Deleter());
    block->reserve(capacity);
    return block;
}

void MemBlockPool::Deleter::operator()(std::vector<uint8_t>* p) const {
    p->clear();
    std::unique_ptr<std::vector<uint8_t>> ptr(p);
    MemBlockPool::instance().m_pool.push_back(std::move(ptr));
}