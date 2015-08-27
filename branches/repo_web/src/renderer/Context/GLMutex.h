#ifndef GLMutex
#define GLMutex

#include <mutex>

namespace Tuvok{
namespace Renderer{

class GLMutex{
public:
     static GLMutex& getInstance(){
         static GLMutex    instance;
         return instance;
     }

     void lockContext(){
        contextMutex.lock();
     }

     void unlockContext(){
        contextMutex.unlock();
     }

private:
    GLMutex(){};
    ~GLMutex(){};

    std::mutex contextMutex;

};
};
};

#endif //GLMutex
