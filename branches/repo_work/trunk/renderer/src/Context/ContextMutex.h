#ifndef GLMutex
#define GLMutex

#include <mutex>

namespace Tuvok{
namespace Renderer{
namespace Context{

    class ContextMutex{
    public:
         static ContextMutex& getInstance(){
             static ContextMutex    instance;
             return instance;
         }

         void lockContext(){
            contextMutex.lock();
         }

         void unlockContext(){
            contextMutex.unlock();
         }

    private:
        ContextMutex(){};
        ~ContextMutex(){};

        std::mutex contextMutex;

    };
};
};
};

#endif //GLMutex
