#pragma once

#include <memory>
#include <functional>
#include <cassert>
#include <string>

#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#else
#include <pthread.h>
#endif

static const uint32_t INFINITE_TIMEOUT = std::numeric_limits<uint32_t>::max();
typedef std::function<bool ()> Predicate;

class CriticalSection {
  friend class WaitCondition;
public:
  CriticalSection();
  ~CriticalSection();
  
  void lock();
  bool lock(uint32_t secs);
  bool tryLock();
  void unlock();
  
protected:
#ifdef _MSC_VER
  CRITICAL_SECTION m_csIDGuard;
#else
  pthread_mutex_t m_csIDGuard;
#endif
  
private:
  // object is non copyable, declare private c'tor and
  // assignment operator to prevent compiler warnings
  CriticalSection(CriticalSection const&);
  CriticalSection& operator=(CriticalSection const&);
};

class ScopedLock {
public:
  ScopedLock(CriticalSection& guard) : m_Guard(guard) { m_Guard.lock(); }
  ScopedLock(CriticalSection* pGuard) : m_Guard(*pGuard) {
    assert(pGuard != nullptr); m_Guard.lock();
  }
  ScopedLock(std::shared_ptr<CriticalSection> const& pGuard) :
  m_Guard(*pGuard.get())
  {
    assert(pGuard.get() != nullptr); m_Guard.lock();
  }
  ~ScopedLock() { m_Guard.unlock(); }
  
private:
  // object is non copyable, declare private c'tor and
  // assignment operator to prevent compiler warnings
  ScopedLock(ScopedLock const& other);
  ScopedLock& operator=(ScopedLock const& other);
  CriticalSection& m_Guard;
};

#define SCOPEDLOCK(guard) ScopedLock scopedLock(guard);

class WaitCondition {
public:
  WaitCondition();
  ~WaitCondition();
  
  // returns false if timeout occurred
  bool wait(CriticalSection& criticalSection,
            uint32_t timeoutInMilliseconds = INFINITE_TIMEOUT);
  void wakeOne();
  void wakeAll();
  
protected:
#ifdef _MSC_VER
  CONDITION_VARIABLE m_cvWaitCondition;
#else
  pthread_cond_t m_cvWaitCondition;
#endif
  
private:
  // object is non copyable, declare private c'tor
  //and assignment operator to prevent compiler warnings
  WaitCondition(WaitCondition const&);
  WaitCondition& operator=(WaitCondition const&);
};

class ThreadClass {
public:
  ThreadClass(std::string const& strName = "ThreadClass");
  // will kill thread if not joined in derived class d'tor
  virtual ~ThreadClass();
  
  virtual bool startThread(void* pData = nullptr);
  bool joinThread(uint32_t timeoutInMilliseconds = INFINITE_TIMEOUT);
  bool killThread(); // only use in extreme cases at own risk
  
  void requestThreadStop() { m_bContinue = false; resume(); }
  // indicates if thread should leave its run loop
  bool isThreadStopRequested() const { return !m_bContinue; }
  // query thread state at OS level
  bool isRunning();
  bool resume();
  
  // returns true if thread should proceed with its job
  bool shouldContinue() const { return m_bContinue; }
  // predicate function that maps ThreadClass::Continue()
  Predicate getContinue() const { return m_pContinue; }
  
protected:
  virtual void threadMain(void* pData = nullptr) = 0;
  bool suspend(Predicate pPredicate = Predicate());
  
  CriticalSection   m_SuspendGuard;
#ifdef _MSC_VER
  HANDLE            m_hThread;
  std::string       m_strName;
#else
  pthread_t         m_hThread;
#endif
  
private:
  // object is non copyable, declare private c'tor and
  // assignment operator to prevent compiler warnings
  ThreadClass(ThreadClass const&);
  ThreadClass& operator=(ThreadClass const&);
  
  struct ThreadData {
    void* pData;
    ThreadClass* pThread;
  };
#ifdef _MSC_VER
  static DWORD WINAPI staticStartFunc(LPVOID pThreadStarterData);
#else
  static void* staticStartFunc(void* pThreadStarterData);
  bool              m_bInitialized;
  bool              m_bJoinable;
  CriticalSection   m_JoinMutex;
  WaitCondition     m_JoinWaitCondition;
#endif
  bool              m_bContinue;
  Predicate         m_pContinue;
  ThreadData*       m_pStartData;
  bool              m_bResumable;
  WaitCondition     m_SuspendWait;
};

class LambdaThread : public ThreadClass {
public:
  // interface to be able to call protected thread methods
  // from some function that gets executed by ThreadMain
  class Interface {
    friend class LambdaThread;
  public:
    bool suspend(Predicate pPredicate = Predicate()) {
      return m_Parent.suspend(pPredicate);
    }
    CriticalSection& getSuspendGuard() {
      return m_Parent.m_SuspendGuard;
    }
    
  private:
    // object is non copyable, declare private c'tor and
    // assignment operator to prevent compiler warnings
    Interface(Interface const&);
    Interface& operator=(Interface const&);
    
    Interface(LambdaThread& parent) : m_Parent(parent) {}
    LambdaThread& m_Parent;
  };
  
  typedef std::function<void (Predicate pContinue, Interface& threadInterface)> ThreadMainFunction;
  LambdaThread(ThreadMainFunction pMain,
               std::string const& strName = "LambdaThread") :
  ThreadClass(strName),
  m_pMain(pMain) {}
  
protected:
  virtual void threadMain(void*) { Interface threadInterface(*this); m_pMain(getContinue(), threadInterface); }
  
private:
  ThreadMainFunction m_pMain;
};

template<class T>
class AtomicAccess {
public:
  AtomicAccess() : m_Value(), m_pGuard(new CriticalSection()) {}
  AtomicAccess(T const& value) : m_Value(value), m_pGuard(new CriticalSection()) {}
  AtomicAccess<T>& operator=(T const& rhs) { setValue(rhs); return *this; }
  CriticalSection& getGuard() const { return *(m_pGuard.get()); }
  T getValue() const { SCOPEDLOCK(getGuard()); return m_Value; }
  void setValue(T const& value) {
    SCOPEDLOCK(getGuard());
    m_Value = value;
  }
  
private:
  T m_Value;
  std::unique_ptr<CriticalSection> m_pGuard;
};
