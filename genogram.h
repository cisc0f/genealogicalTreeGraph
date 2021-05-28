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
  bool isEmpty();
  // Methods requested by assignment
  void addPerson();
  void addRelMother();
  void addRelFather();
  void addRelCouple();
  void addRelChildToCouple();
  void setBirthDate();
  void setDeathDate();
  void deletePerson();
  void isValid();

} // namespace gen

// Print and Read Methods
void readGenogramFromFile();
void printGenogram();