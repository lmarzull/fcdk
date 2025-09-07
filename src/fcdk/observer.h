#pragma once


#include <vector>


namespace fcdk {


//------------------------------------------------------------------------------
template <typename T>
class observer
{
public:
  virtual       ~observer() = default;
  virtual void  updated(const T&) = 0;
};


//------------------------------------------------------------------------------
template <typename T>
class subject
{
public:
  int               register_observer(observer<T>&);
  void              unregister_observer(int i);
  void              notify_observers() const;
  virtual const T&  subject_data() const = 0;

private:
  std::vector<observer<T>*> observers_;
};


//------------------------------------------------------------------------------
template <typename T>
int
subject<T>::register_observer(observer<T>& observer)
{
  observers_.push_back(&observer);
  return observers_.size()-1;
}


//------------------------------------------------------------------------------
template <typename T>
void
subject<T>::unregister_observer(int i)
{
  observers_[i] = nullptr;
}


//------------------------------------------------------------------------------
template <typename T>
void
subject<T>::notify_observers() const
{
  for(auto obs: observers_)
    if(obs)
      obs->updated(subject_data());
}


} // fcdk
