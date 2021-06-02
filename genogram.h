#include <string>
#include <iostream>

namespace gen{

  enum relKind{PARENT,CHILD,PARTNER};

  struct node;
  struct edge;

  typedef node* Genogram;
  typedef edge* edgeList;

  const Genogram emptyGen=nullptr;

  // Extra useful methods
  node* createEmptyGen();
  bool isEmpty(const Genogram&);
  // Methods requested by assignment
  void addPerson(std::string, std::string, std::string, std::string, Genogram&); //X
  void addRelMother(std::string, std::string, Genogram&); //X
  void addRelFather(std::string, std::string, Genogram&); //X
  void addRelCouple(std::string, std::string, Genogram&); //X
  void addRelChildToCouple(std::string, std::string, Genogram&); //X
  void setBirthDate(std::string, std::string, Genogram&); //X
  void setDeathDate(std::string, std::string, Genogram&); //X
  void deletePerson(std::string, Genogram&); //X
  void isValid(Genogram&);

} // namespace gen

// Print and Read Methods
void readGenogramFromFile(std::string, gen::Genogram&);
void printGenogram(const gen::Genogram&);