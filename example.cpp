#define DEBUG
#include "mix.hpp"
#include <iostream>
int main() {
  MixStack s;
  auto n = s.dat(0);
  auto b = s.dat(std::string(" is not 0!"));
  while (1) {
    s.in(0);
    n->output(std::cout);
    (*n)++;
    MixIf i0(s, *n);
    b->output(std::cout);
    i0.done(s, *n);
    MixIf i1(s, *n >= 100);
    s.goTo(1);
    i1.done(s,*n >= 100);
    s.goTo(0);
    s.in(1);
    if(s.target->on())return 0;
  }
}