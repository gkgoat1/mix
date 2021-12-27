#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <vector>

// typedefs
typedef char WORD;
typedef std::vector<WORD> instruction_t;
typedef instruction_t::iterator instruction_pointer;

typedef std::vector<WORD> memory_t;
typedef memory_t::iterator memory_pointer;

// structs
struct environment {
  memory_t memory;
  instruction_t instructions;

  instruction_pointer ip;
  memory_pointer mp;

  environment() : memory(3000) {
    clear();
    mp = memory.begin();
  }
  void clear() {
    instructions.clear();
    ip = instructions.begin();
  }
};

// prototypes
void compile(environment &env);
int from_line(environment &env, std::string &line, int open_brackets = 0);
int from_stream(environment &env, std::istream &stream, int open_brackets = 0);
void print_word(WORD word);

int main(int argc, char **argv) {
  environment env;

  // if no args, read from stdin. else, read from file specified as first
  // argument
  if (argc == 1) {
    //interactive_mode(env);
  } else {
    std::ifstream stream = std::ifstream(argv[1]);
    if (stream.is_open()) {
      int open_brackets = from_stream(env, stream);
      if (open_brackets == 0)
        compile(env);
      else
        std::cerr << "Unmatched brackets!" << std::endl;
    } else {
      std::cerr << "File not found : " << argv[1] << std::endl;
    }
  }

  return 0;
}

void print_word(WORD word) {
  if (isprint(word))
    std::cout << word;
  else
    std::cout << "0x" << std::hex << (int)word << std::dec;
}

int from_line(environment &env, std::string &line, int open_brackets) {
  std::istringstream stream = std::istringstream(line);
  return from_stream(env, stream, open_brackets);
}

int from_stream(environment &env, std::istream &stream, int open_brackets) {
  if (open_brackets == 0)
    env.instructions.push_back('\0');
  WORD word;
  while (true) {
    stream >> word;
    if (!stream)
      break;
    switch (word) {
    case '<':
    case '>':
    case '+':
    case '-':
    case ',':
    case '.':
      env.instructions.push_back(word);
      break;
    case '[':
      open_brackets++;
      env.instructions.push_back(word);
      break;
    case ']':
      open_brackets--;
      env.instructions.push_back(word);
      break;
    default:
      break;
    }
  }
  if (open_brackets == 0)
    env.instructions.push_back('\0');
  return open_brackets;
}



int find_closing(environment &env) {
  int balance = 1;
  auto i = env.ip;
  int j = 0;
  do {
    env.ip++;
    j++;
    if (*env.ip == '[')
      balance++;
    else if (*env.ip == ']')
      balance--;

  } while (balance != 0);
  env.ip = i;
  return j;
}

int find_opening(environment &env) {
  int balance = 0;
  auto i = env.ip;
  int j = 0;
  do {
    if (*env.ip == '[')
      balance++;
    else if (*env.ip == ']')
      balance--;
    env.ip--;
  } while (balance != 0);
  env.ip = i;
  return j;
}

void compile(environment &env) {
  std::cout << "auto mp = s.def<int>(0);auto mem = "
               "s.def<std::map<int,int>>({});MixIf i();while(1){";
  int i = -1;
  env.ip = env.instructions.begin();
  while (env.ip != env.instructions.end()) {
    i++;
    std::cout << "s.in(" << i << ")";
    switch (*env.ip) {
    case '+':
      std::cout << "*mem[*mp]++;";
      break;
    case '-':
      std::cout << "*mem[*mp]--;";
      break;
    case '>':
      std::cout << "mp++;";
      break;
    case '<':
      std::cout << "mp--;";
      break;
    case '.':
      std::cout << "if(mem->on())out<< *mem[*mp];";
      break;
    case ',':
      std::cout << "if(mem->on())in >> *mem[*mp];";
      break;
    case '[': {
      auto c = i + find_closing(env);
      std::cout << "i = MixIf(s,*mem[*mp] == 0);s.goTo(" << c
                << ");i.done(s,*mem[*mp] == 0);s.goTo(" << c - 1 << ");";
      break;
    }
    case ']':
      std::cout << "s.goTo(" << i - find_opening(env) - 1 << ");";
      break;
    case '_':
    std::cout << "if(mem->on())s.goTo(*mem[*mp]);";
    break;
    case '\0':
      break;
    }
    env.ip++;
  }
  std::cout << "};";
}