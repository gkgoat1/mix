#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
template <typename T> class MixData;
struct MixStack;
inline bool o(MixStack *);
struct Bad {
  bool screw = false;
};
struct MixDataBase {
  virtual bool &on() = 0;
  virtual void swap() = 0;
  virtual ~MixDataBase() {}
  virtual void output(std::ostream &o) = 0;
  virtual void destroy() = 0;
  virtual const std::string &_os() = 0;
  template <typename T> MixData<T> &as() {
    return dynamic_cast<MixData<T> &>(*this);
  }
  template <typename T, typename Fn> void call(Fn f) {
    if (on())
      f(as<T>());
  }
  MixStack *_stack;
  std::shared_ptr<bool> bad = std::make_shared<bool>(false);
  inline bool chk_os() { return _os()[o(_stack) ? 0 : on()] == '['; }
  void report(Bad &b) {
    if (!b.screw)
      *bad = true;
    if (b.screw) {
      on() ^= 1;
    }
  }
};
template <typename T> class MixData : public MixDataBase {
  T vals[2];
  volatile bool on_;
  std::string os = std::string("[T") + typeid(T).name();

public:
  MixData(T v) : vals{v, v}, on_{true} {}
  std::string &________os() { return const_cast<std::string &>(os); }
  operator T &() {
    if (chk_os()) {
      ________os() = "[";
      ________os() += std::to_string(on_);
      ________os() += "]";
    };
    return vals[on_ + chk_os()];
  };
  T &get() {
    if (chk_os()) {
      ________os() = "[";
      ________os() += std::to_string(on_);
      ________os() += "]";
    };
    return vals[on_ + chk_os()];
  }
  void destroy() override { ________os() = "destructed"; }
  bool &on() override { return const_cast<bool &>(on_); }
  void swap() override { on() = !on(); }
  void output(std::ostream &o) override {
    call<T>([&](T &val) { o << val; });
  }
  const std::string &_os() override {
    return const_cast<const std::string &>(os);
  }
};

struct MixStack {
  std::vector<std::shared_ptr<MixDataBase>> data;
  std::shared_ptr<MixData<int>> target;
  inline MixStack() { target = dat<int>(-1); }
  inline void report(Bad &b) {
    if (!target->on())
      return;
    for (auto d : data) {
      d->report(b);
    }
  }
  std::vector<std::shared_ptr<MixDataBase>> lp_restart;
  template <typename T> std::shared_ptr<MixData<T>> dat(T val) {
    auto t = std::make_shared<MixData<T>>(val);
    t->_stack = this;
    data.push_back(t);
    return t;
  }
  inline void in(int x) {
    if (*target == -1 && lp_restart.size() != 0)
      *target = 0;
    if (x == *target) {
      for (auto r : lp_restart) {
        r->on() = true;
      };
      *target = -1;
      lp_restart = {};
      target->on() = true;
    }
  }
  inline void goTo(int x) {
    if (!target->on())
      return;
    if (*target != -1)
      return;
    *target = x;
    lp_restart = of(true);
    force(false);
    target->on() = true;
  }
  inline void swap() {
    if (!target->on())
      return;
    for (auto d : data) {
      d->swap();
    }
  }
  inline void force(bool x) {
    if (!target->on())
      return;
    for (auto d : data) {
      d->on() = x;
    }
  }
  inline std::vector<std::shared_ptr<MixDataBase>> of(bool x) {
    if (!target->on())
      return {};
    std::vector<std::shared_ptr<MixDataBase>> r;
    for (auto d : data)
      if (d->on() == x)
        r.push_back(d);
    return r;
  }
};
inline bool o(MixStack *s) {
  for (auto &d : s->data) {
    if (*d->bad)
      return false;
  }
  return true;
}
struct MixIf {
  std::vector<std::shared_ptr<MixDataBase>> restore;
  inline MixIf(MixStack &s, bool b) : restore(s.of(true)) {
    if (!b) {
      s.force(false);
    }
  }
  inline void done(MixStack &s, bool b) {
    if (!b && s.target->on()) {
      for (auto r : restore) {
        r->on() = true;
      }
    }
  }
};