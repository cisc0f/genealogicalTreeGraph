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
  relKind kind;
  edge* next;
};

// AUXILIARY SECTION
// Find if a node exists in the Genogram and returns the pointer
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

// Check if the type of relation exists in the edges list (useful for partner existence check)
bool checkRelKindExists(node* nodeToCheck, relKind kind){
  edge* aux=nodeToCheck->edges;
  while(aux){
    if(aux->kind==kind){
      return true;
    }
    aux=aux->next;
  }
  return false;
}

// Check if a person already has two parents assigned (used by addRel)
bool checkBothParentsExists(node* son){
  edge* parent=son->edges;
  int count{0};
  while(parent){
    if(parent->kind==PARENT){
      count++;
    }
    parent=parent->next;
  }
  return (count>1);
}

// Add in front of the edges list of a node
void addInFront(node* label, node* dstNode, relKind kind){
  edge* aux=new edge;
  aux->name=label;
  aux->kind=kind;
  aux->next=dstNode->edges;
  dstNode->edges=aux;
  return;
}

// Add a relation (aux function used for addRelMother, addRelFather, addRelChildToCouple)
void addRel(std::string parentName, std::string sonName, bool areCouple, Genogram& g){
  if(parentName==sonName)throw "Il cappio non è ammesso in questo grafo!";
  Genogram aux=g;
  node* partner=nullptr;
  node* parent=nullptr;
  node* son=nullptr;
  while(aux){
    if(aux->name==parentName){
      parent=aux;
      if(areCouple){
        edge* tmp=aux->edges;
        while(tmp){
          if(tmp->kind==PARTNER){
            partner=tmp->name;
          }
          tmp=tmp->next;
        }
      }
    }else if(aux->name==sonName){
      son=aux;
    }
    aux=aux->next;
  }
  if(checkBothParentsExists(son))throw "La modifica dei genitori non è possibile!";
  if(!son||!parent)throw "Una o entrambe queste persone non sono nel Genogramma!";
  if(!partner&&areCouple)throw "Questa persona non ha un partner associato!";
  if(areCouple){
    addInFront(son,partner,CHILD);
    addInFront(partner,son,PARENT); 
  }
  addInFront(son,parent,CHILD);
  addInFront(parent,son,PARENT);
}
// END AUXILIARY SECTION


// IMPLEMENTATION SECTION
// Init Genogram
node* gen::createEmptyGen(){
  return emptyGen;
}

// Check if Genogram is empty
bool gen::isEmpty(const Genogram& g){
  return (g==emptyGen);
}

// Add person to node list
void gen::addPerson(std::string name, std::string gender, std::string birth, std::string death, Genogram& g){
  if(findNode(name,g))throw "La persona inserita esiste!";
  node* aux = new node;
  aux->name = name;
  aux->gender = gender;
  aux->birth = birth;
  aux->death = death;
  aux->edges = nullptr;
  aux->next = g;
  g=aux;
  return;
}

void gen::addRelMother(std::string motherName, std::string sonName, Genogram& g){
  addRel(motherName, sonName, false, g);
}

void gen::addRelFather(std::string fatherName, std::string sonName, Genogram& g){
  addRel(fatherName, sonName, false, g);
}

// Add couple relation to two people if they are not already related with anyone
void gen::addRelCouple(std::string name1, std::string name2, Genogram& g){
  if(name1==name2)throw "Il cappio non è ammesso in questo grafo!";
  Genogram aux=g;
  node* firstNode=nullptr;
  node* secondNode=nullptr;
  while(aux){
    if(aux->name==name1){
      firstNode=aux;
    }else if(aux->name==name2){
      secondNode=aux;
    }
    aux=aux->next;
  }
  if(!firstNode||!secondNode)throw "Una delle due persone non esiste nel Genogramma!";
  if(checkRelKindExists(firstNode,PARTNER)||checkRelKindExists(secondNode,PARTNER)){
    throw "Una delle due persone ha già un partner associato!";
  }
  addInFront(firstNode,secondNode,PARTNER);
  addInFront(secondNode,firstNode,PARTNER);
}

// Add child to a couple
void gen::addRelChildToCouple(std::string parentName, std::string sonName, Genogram& g){
  addRel(parentName, sonName, true, g);
}
// END IMPLEMENTATION SECTION


// PRINT AND READ FUNCTIONS
// Print elements in graph
void printGenogram(const gen::Genogram& g){
  Genogram aux=g;
  node* mother=nullptr;
  node* father=nullptr;
  while(aux){
    std::cout << "_____________________________" << std::endl;
    std::cout << aux->name << " " << aux->gender << std::endl;

    edge* parent=aux->edges;
    while(parent){
      if(parent->kind==PARENT){
        if(parent->name->gender == "M"){
          father=parent->name;
        }else{
          mother=parent->name;
        }
      }
      parent=parent->next;
    }
    std::cout << "madre: ";
    if(mother){
      std::cout << mother->name;
    } 
    std::cout << std::endl;
    std::cout << "padre: ";
    if(father){
      std::cout << father->name;
    }
    std::cout << std::endl;

    std::cout << "in coppia con: ";
    edge* tmp=aux->edges;
    while(tmp){
      if(tmp->kind==PARTNER){
        std::cout << tmp->name->name;
      }
      tmp=tmp->next;
    }
    std::cout << std::endl;

    std::cout << "figli: ";
    edge* child=aux->edges;
    while(child){
      if(child->kind==CHILD){
        std::cout << child->name->name << " ";
      }
      child=child->next;
    }
    std::cout << std::endl;

    father=nullptr;
    mother=nullptr;

    aux=aux->next;
  }
  return;
}
// END PRINT AND READ FUNCTIONS