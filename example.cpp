#include "mix.hpp"
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
    s.goTo(0);
  }
}