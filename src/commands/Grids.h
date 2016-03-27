#pragma once

#include "silverbullet/math/Vectors.h"

template <class T> class GridnD  {
public:
  GridnD<T>() : m_pData(NULL) {}
  virtual ~GridnD<T>() {delete [] this->m_pData;}
  
  T* getDataPointer() const {return this->m_pData;}
  T getLinear(size_t i) const {return this->m_pData[i];}
  void setLinear(size_t i, T tValue) {this->m_pData[i] = tValue;}
  
protected:
  T* m_pData;
};

template <class T> class Grid1D : public GridnD<T> {
public:
  Grid1D<T>() : GridnD<T>(), m_iSize(0) {}
  Grid1D<T>(const size_t& iSize) : m_iSize(iSize) { this->m_pData = new T[m_iSize];}
  Grid1D<T>(const Grid1D<T> &other): m_iSize(other.m_iSize) {
    this->m_pData = new T[m_iSize];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize);
  }
  virtual ~Grid1D<T>() {}
  
  size_t getFilledSize() const {
    size_t iSize = 0;
    for (size_t i = 0;i<m_iSize;i++) {
      if (this->m_pData[i] != 0) iSize = i+1;
    }
    return iSize;
  }
  
  size_t getSize() const {return m_iSize;}
  void Resize(const size_t& iSize) {
    delete [] this->m_pData;
    m_iSize = iSize;
    this->m_pData = new T[m_iSize];
  }
  
  T get(size_t i) const {return this->m_pData[i];}
  void set(size_t i, T tValue) {this->m_pData[i] = tValue;}
  
  Grid1D<T>& operator=(const Grid1D<T>& other)  {
    delete [] this->m_pData;
    m_iSize = other.m_iSize;
    this->m_pData = new T[m_iSize];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize);
    return *this;
  }
  
private:
  size_t m_iSize;
};

template <class T> class Grid2D : public GridnD<T> {
public:
  Grid2D<T>() : GridnD<T>(), m_iSize(0,0) {}
  Grid2D<T>(const Core::Math::VECTOR2<size_t>& iSize) : m_iSize(iSize) { this->m_pData = new T[m_iSize.area()];}
  Grid2D<T>(const Grid2D<T> &other): m_iSize(other.m_iSize) {
    this->m_pData = new T[m_iSize.area()];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize.area());
  }
  virtual ~Grid2D<T>() {}
  
  Core::Math::VECTOR2<size_t> getFilledSize() const {
    Core::Math::VECTOR2<size_t> vSize(0,0);
    for (size_t y = 0;y<m_iSize.y;y++) {
      for (size_t x = 0;x<m_iSize.x;x++) {
        if (this->Get(x,y) != 0) {
          if ((x+1) > vSize.x) vSize.x = x+1;
          vSize.y = y+1;
        }
      }
    }
    return vSize;
  }
  
  
  Core::Math::VECTOR2<size_t> getSize() const {return m_iSize;}
  void Resize(const Core::Math::VECTOR2<size_t>& iSize) {
    delete [] this->m_pData;
    m_iSize = iSize;
    this->m_pData = new T[m_iSize.area()];
  }
  
  T get(const Core::Math::Vec2ui& pos) const {return this->m_pData[pos.x+pos.y*m_iSize.x];}
  void set(const Core::Math::Vec2ui& pos, T tValue) {this->m_pData[pos.x+pos.y*m_iSize.x] = tValue;}
  T get(size_t x, size_t y) const {return this->m_pData[x+m_iSize.x*y];}
  void set(size_t x, size_t y, T tValue) {this->m_pData[x+m_iSize.x*y] = tValue;}
  
  Grid2D<T>& operator=(const Grid2D<T>& other)  {
    delete [] this->m_pData;
    m_iSize = other.m_iSize;
    this->m_pData = new T[m_iSize.area()];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize.area());
    return *this;
  }
  
private:
  Core::Math::VECTOR2<size_t> m_iSize;
};


template <class T> class Grid3D : public GridnD<T> {
public:
  Grid3D<T>() : GridnD<T>(), m_iSize(0,0,0) {}
  Grid3D<T>(const Core::Math::VECTOR3<size_t>& iSize) : m_iSize(iSize) { this->m_pData = new T[m_iSize.volume()];}
  Grid3D<T>(const Grid3D<T> &other): m_iSize(other.m_iSize) {
    this->m_pData = new T[m_iSize.volume()];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize.volume());
  }
  virtual ~Grid3D<T>() {}
  
  Core::Math::VECTOR3<size_t> getSize() const {return m_iSize;}
  void Resize(const Core::Math::VECTOR3<size_t>& iSize) {
    delete [] this->m_pData;
    m_iSize = iSize;
    this->m_pData = new T[m_iSize.volume()];
  }
  
  T get(const Core::Math::Vec3ui& pos) const {return this->m_pData[pos.x+pos.y*m_iSize.x+pos.z*m_iSize.x*m_iSize.y];}
  void set(const Core::Math::Vec3ui& pos, T tValue) {this->m_pData[pos.x+pos.y*m_iSize.x+pos.z*m_iSize.x*m_iSize.y] = tValue;}
  T get(size_t x, size_t y, size_t z) const {return this->m_pData[x+m_iSize.x*y+z*m_iSize.x*m_iSize.y];}
  void set(size_t x, size_t y, size_t z, T tValue) {this->m_pData[x+m_iSize.x*y+z*m_iSize.x*m_iSize.y] = tValue;}
  
  Grid3D<T>& operator=(const Grid3D<T>& other)  {
    delete [] this->m_pData;
    m_iSize = other.m_iSize;
    this->m_pData = new T[m_iSize.volume()];
    memcpy(this->m_pData, other->m_pData, sizeof(T)*m_iSize.volume());
    return *this;
  }
  
private:
  Core::Math::VECTOR3<size_t> m_iSize;
};
