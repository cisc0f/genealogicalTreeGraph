#include <string>
#include <iostream>

namespace gen{

  struct node;
  struct edge;

  typedef node* Genogram;
  typedef edge* edgeList;

  const Genogram emptyGen=nullptr;

  // Extra useful methods
  node* createEmptyGen();
  bool isEmpty(const Genogram&);
  // Methods requested by assignment
  void addPerson(std::string, std::string, std::string, std::string, Genogram&);
  void addRelMother(std::string, std::string, Genogram&);
  void addRelFather(std::string, std::string, Genogram&);
  void addRelCouple(std::string, std::string, Genogram&);
  void addRelChildToCouple(std::string, std::string, Genogram&);
  void setBirthDate(std::string, std::string, Genogram&);
  void setDeathDate(std::string, std::string, Genogram&);
  void deletePerson(std::string, Genogram&);
  void isValid(Genogram&);

} // namespace gen

// Print and Read Methods
void readGenogramFromFile(std::string, gen::Genogram&);
void printGenogram(const gen::Genogram&);