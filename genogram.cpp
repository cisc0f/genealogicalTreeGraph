#include "genogram.h"

using namespace gen;

struct gen::node{
  // Payload
  std::string name;
  std::string gender;
  std::string birth;
  std::string death;
  // Structure
  edge* edges;
  node* next;
};

struct gen::edge{
  node* name;
  edge* next;
};