#include "genogram.h"

using namespace gen;

struct gen::node{
  // Payload
  std::string name;
  std::string gender;
  time_t birth;
  time_t death;
  bool visited;
  // Structure
  edge* edges;
  node* prev;
  node* next;
};

struct gen::edge{
  node* name;
  relKind kind;
  edge* next;
};

//***************************************************************************** AUXILIARY SECTION
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
  edge* edgeNode=nodeToCheck->edges;
  while(edgeNode){
    if(edgeNode->kind==kind){
      return true;
    }
    edgeNode=edgeNode->next;
  }
  return false;
}

// Check if a person already has two parents assigned (used by addRel)
bool checkParentAlreadySetted(node* son, std::string genderOfParentToCheck){
  edge* edgeNode=son->edges;
  while(edgeNode){
    if(edgeNode->kind==PARENT){
      if(edgeNode->name->gender==genderOfParentToCheck){
        return true;
      }
    }
    edgeNode=edgeNode->next;
  }
  return false;
}

// Create a string from a timestamp
std::string getStringFromTimestamp(time_t date){
  tm* parsedTime={0};
  char buffer[11]=""; // 11 is hardcoded as I expect date to be (dd/mm/yyyy\0) -> 11 chars
  parsedTime = localtime(&date);
  strftime(buffer,sizeof(buffer),"%d/%m/%Y",parsedTime);
  std::string resultDateAsString(buffer);
  return resultDateAsString;
}

// Create a timestamp from a string
time_t getTimestampFromString(std::string date){
  tm parsedTime={0};
  strptime(date.c_str(),"%d/%m/%Y",&parsedTime);
  time_t resultDateAsTimestamp = mktime(&parsedTime);
  return resultDateAsTimestamp;
}

// Add in front of the edges list of a node
void addOrderedByKindAndBirth(node* label, node* dstNode, relKind kind){
  edge* newEdgeNode=new edge;
  newEdgeNode->name=label;
  newEdgeNode->kind=kind;
  newEdgeNode->next=nullptr;

  if(newEdgeNode->kind!=CHILD||!dstNode->edges){
    newEdgeNode->next=dstNode->edges;
    dstNode->edges=newEdgeNode;
    return;
  }

  edge* edgeNode=dstNode->edges;
  edge* prev=edgeNode;
  while(edgeNode){
    if(edgeNode->kind==CHILD){
      if(edgeNode->name->birth > newEdgeNode->name->birth){
        if(prev==edgeNode){
          prev=newEdgeNode;
        }else{
          prev->next=newEdgeNode;
        }
        newEdgeNode->next=edgeNode;
        return;
      }
    }
    prev=edgeNode;
    edgeNode=edgeNode->next;
  }
  prev->next=newEdgeNode;
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
  if(!son||!parent)throw "Una o entrambe queste persone non sono nel Genogramma!";
  if(checkParentAlreadySetted(son, parent->gender))throw "La modifica dei genitori non è possibile!";
  if(!partner&&areCouple)throw "Questa persona non ha un partner associato!";
  if(areCouple){
    addOrderedByKindAndBirth(son,partner,CHILD);
    addOrderedByKindAndBirth(partner,son,PARENT); 
  }
  addOrderedByKindAndBirth(son,parent,CHILD);
  addOrderedByKindAndBirth(parent,son,PARENT);
}

void setDate(std::string name, std::string date, bool isBirth, Genogram& g){
  node* person=findNode(name,g);
  if(!person)throw "Persona non presente nel genogramma!";
  time_t actualDate=getTimestampFromString(date);
  if(isBirth){
    person->birth=actualDate;
    return;
  }
  person->death=actualDate;
}

void subDelRel(node* person, node* nameToDelete){
  edge* aux=person->edges;
  edge* prev=nullptr;
  while(aux->name->name!=nameToDelete->name){
    prev=aux;
    aux=aux->next;
  }
  if(!prev){
    person->edges=aux->next;
  }else{
    prev->next=aux->next;
  }
  delete(aux);
}

void deleteNode(node* nodeToDelete, Genogram& g){
  edge* aux=nodeToDelete->edges;
  edge* nextNode=aux;
  while(aux){
    nextNode=aux->next;
    if(aux->kind==CHILD){
      deleteNode(aux->name,g);
    }else{
      subDelRel(aux->name,nodeToDelete);
    }
    aux=nextNode;
  }
  nodeToDelete->next->prev=nodeToDelete->prev;
  if(nodeToDelete->prev){
    nodeToDelete->prev->next=nodeToDelete->next;
  }else{
    g=nodeToDelete->next;
  }
  delete(nodeToDelete);
}

void isConnected(Genogram& g){
  g->visited=true;
  edge* tmp=g->edges;
  while(tmp){
    g=tmp->name;
    if(!g->visited){
      isConnected(g);
    }
    tmp=tmp->next;
  }
}
//***************************************************************************** END AUXILIARY SECTION


//***************************************************************************** IMPLEMENTATION SECTION
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
  aux->birth = getTimestampFromString(birth);
  aux->death = getTimestampFromString(death);
  aux->visited = false;
  aux->edges = nullptr;
  aux->prev = nullptr;
  aux->next = g;
  if(!isEmpty(g)){
    g->prev=aux;
  }
  g=aux;
  return;
}

// Add mother relation to a child and vice versa
void gen::addRelMother(std::string motherName, std::string sonName, Genogram& g){
  addRel(motherName, sonName, false, g);
}

// Add father relation to a child and vice versa
void gen::addRelFather(std::string fatherName, std::string sonName, Genogram& g){
  addRel(fatherName, sonName, false, g);
}

// Add child to a couple
void gen::addRelChildToCouple(std::string parentName, std::string sonName, Genogram& g){
  addRel(parentName, sonName, true, g);
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
  addOrderedByKindAndBirth(firstNode,secondNode,PARTNER);
  addOrderedByKindAndBirth(secondNode,firstNode,PARTNER);
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
  node* nodeToDelete=findNode(name,aux);
  if(!nodeToDelete)throw "Persona non presente nel genogramma!";
  deleteNode(nodeToDelete,aux);
  g=aux;
}

// Check if Genogram is Valid
bool gen::isValid(Genogram& g){
  if(isEmpty(g))throw "Il genogramma è vuoto";
  Genogram aux=g;
  isConnected(aux);
  Genogram tmp=g;
  while(tmp){
    if(!tmp->visited)return false;
    // Re-init all nodes to false
    tmp->visited=false;
    tmp=tmp->next;
  }
  return true;
}
//***************************************************************************** END IMPLEMENTATION SECTION


//***************************************************************************** PRINT AND READ FUNCTIONS
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
    std::cout << "nato/a: ";
    if(aux->birth != -1){
      std::cout << getStringFromTimestamp(aux->birth);
    }
    std::cout << " morto/a: ";
    if(aux->death != -1){
      std::cout << getStringFromTimestamp(aux->death);
    }
    std::cout << std::endl;

    edge* edgeNode=aux->edges;
    while(edgeNode){
      if(edgeNode->kind==PARENT){
        if(edgeNode->name->gender == "M"){
          father=edgeNode->name;
        }else{
          mother=edgeNode->name;
        }
      }
      edgeNode=edgeNode->next;
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
//***************************************************************************** END PRINT AND READ FUNCTIONS