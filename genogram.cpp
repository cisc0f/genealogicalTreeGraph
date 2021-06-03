#include "genogram.h"

using namespace gen;

struct gen::node{
  // Payload
  std::string name;
  std::string gender;
  std::string birth;
  std::string death;
  bool visited;
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

void setDate(std::string name, std::string date, bool isBirth, Genogram& g){
  Genogram aux=g;
  while (aux){
    if(aux->name==name){
      if(isBirth){
        aux->birth=date;
      }else{
        aux->death=date;
      }
      return;
    }
    aux=aux->next;
  }
  throw "Persona non presente nel genogramma!";
}

void deleteRelation(node* nameToDelete, edge* person){
  edge* aux=person->name->edges;
  edge* prev=nullptr;
  while(aux->name!=nameToDelete){
    prev=aux;
    aux=aux->next;
  }
  if(!prev){
    person->name->edges=aux->next;
  }else{
    prev->next=aux->next;
  }
  delete(aux);
}

void startVisit(Genogram& g){
  g->visited=true;
  edge* tmp=g->edges;
  while(tmp){
    g=tmp->name;
    if(!g->visited){
      startVisit(g);
    }
    tmp=tmp->next;
  }
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
  aux->visited = false;
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

// Set Birth of a person
void gen::setBirthDate(std::string name, std::string birth, Genogram& g){
  setDate(name, birth, true, g);
}

// Set Death of a person
void gen::setDeathDate(std::string name, std::string death, Genogram& g){
  setDate(name, death, false, g);
}

// Delete person from genogram
void gen::deletePerson(std::string name, Genogram& g){
  Genogram aux=g;
  node* prev=nullptr;
  while(aux->name!=name){
    prev=aux;
    aux=aux->next;
  }
  if(!aux)throw "Persona non presente nel genogramma!";
  edge* tmp=aux->edges;
  while(tmp){
    deleteRelation(aux,tmp);
    tmp=tmp->next;
  }
  if(!prev){
    g=aux->next;
  }else{
    prev->next=aux->next;
  }
  delete(aux);
}

// Check if Genogram is Valid
bool gen::isValid(Genogram& g){
  Genogram aux=g;
  startVisit(aux);
  Genogram tmp=g;
  while(tmp){
    if(!tmp->visited)return false;
    tmp->visited=false;
    tmp=tmp->next;
  }
  return true;
}
// END IMPLEMENTATION SECTION


// PRINT AND READ FUNCTIONS
// Read genogram from file
void readGenogramFromFile(std::string fileName, gen::Genogram& g){
  std::fstream inputFile;
  inputFile.open(fileName, std::ios::in);
  std::string fileLine;
  std::string arr[5];
  std::string check;
  while(getline(inputFile,fileLine)){
    std::istringstream ss(fileLine);
    int i=0;
    while(ss >> check){
      arr[i]=check;
      i++;
    }
    if(arr[0]=="P"){
      gen::addPerson(arr[1],arr[2],arr[3],arr[4],g);
    }else if(arr[0]=="R"){
      if(arr[2]=="M"){
        gen::addRelMother(arr[1],arr[3],g);
      }else if(arr[2]=="F"){
        gen::addRelFather(arr[1],arr[3],g);
      }else{
        gen::addRelCouple(arr[1],arr[3],g);
      }
    }
  }
}

// Print elements in graph
void printGenogram(const gen::Genogram& g){
  Genogram aux=g;
  node* mother=nullptr;
  node* father=nullptr;
  while(aux){
    std::cout << "_____________________________" << std::endl;
    std::cout << aux->name << " " << aux->gender << " ";
    std::cout << "nato/a: " << aux->birth << " morto/a: " << aux->death << std::endl;

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