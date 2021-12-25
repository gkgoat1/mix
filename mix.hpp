#pragma once
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>
template <typename T> class MixData;
struct MixDataBase {
  virtual bool &on() = 0;
  virtual void swap() = 0;
  virtual ~MixDataBase() {}
  virtual void output(std::ostream &o) = 0;
  template <typename T> MixData<T> &as() {
    return dynamic_cast<MixData<T> &>(*this);
  }
};
template <typename T> class MixData : public MixDataBase {
  T vals[2];
  volatile bool on_;

public:
  MixData(T v) : vals{v, v}, on_{true} {}
  operator T &() { return vals[on_]; };
  bool &on() override { return const_cast<bool &>(on_); }
  void swap() override { on() = !on(); }
  void output(std::ostream &o) override {
    if (on())
      o << *this;
  }
};

struct MixStack {
  std::vector<std::shared_ptr<MixDataBase>> data;
  std::shared_ptr<MixData<int>> target;
  MixStack() { target = dat<int>(-1); }
  std::vector<std::shared_ptr<MixDataBase>> lp_restart;
  template <typename T> std::shared_ptr<MixData<T>> dat(T val) {
    auto t = std::make_shared<MixData<T>>(val);
    data.push_back(t);
    return t;
  }
  void in(int x) {
    if (x == *target) {
      for (auto r : lp_restart) {
        r->on() = true;
      };
      *target = -1;
      lp_restart = {};
    }
  }
  void goTo(int x) {
    if (!target->on())
      return;
    if (*target != -1)
      return;
    *target = x;
    lp_restart = of(true);
    force(false);
  }
  void swap() {
    if (!target->on())
      return;
    for (auto d : data) {
      d->swap();
    }
  }
  void force(bool x) {
    if (!target->on())
      return;
    for (auto d : data) {
      d->on() = x;
    }
  }
  std::vector<std::shared_ptr<MixDataBase>> of(bool x) {
    if (!target->on())
      return {};
    std::vector<std::shared_ptr<MixDataBase>> r;
    for (auto d : data)
      if (d->on() == x)
        r.push_back(d);
    return r;
  }
};
struct MixIf {
  std::vector<std::shared_ptr<MixDataBase>> restore;
  MixIf(MixStack &s, bool b) : restore(s.of(true)) {
    if (!b) {
      s.force(false);
    }
  }
  void done(MixStack &s, bool b) {
    if (!b) {
      for (auto r : restore) {
        r->on() = true;
      }
    }
  }
};