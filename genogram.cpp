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

// Auxiliary Functions
node* findNode(std::string name, Genogram& g){
  Genogram aux=g;
  while(aux){
    if(aux->name==name){
      return aux;
    }
    aux=aux->next;
  }
  return emptyGen;
}
// End Auxiliary Functions

// Functions Implementation
node* gen::createEmptyGen(){
  return emptyGen;
}

bool gen::isEmpty(const Genogram& g){
  return (g==emptyGen);
}

void gen::addPerson(std::string name, std::string gender, std::string birth, std::string death, Genogram& g){
  if(findNode(name,g))throw "La persona inserita esiste!";
  node* aux = new node;
  aux->name = name;
  aux->gender = gender;
  aux->birth = birth;
  aux->death = death;
  aux->next = g;
  g=aux;
  return;
}
// End Functions Implementation

// Print and Read Functions
void printGenogram(const gen::Genogram& g){
  Genogram aux=g;
  while(aux){
    std::cout << aux->name << std::endl;
    aux=aux->next;
  }
  return;
}
// End Print and Read Functions