#include "genogram.h"

#if defined(_WIN32) || defined(WIN32)
  const char* clearTerminalCmd = "cls";
  void prettyPrint(int color, std::string text){
    std::cout << text;
  }
  void prettyPrintError(std::string text){
    std::cout << text;
  }
#else 
  const char* clearTerminalCmd = "clear";
  void prettyPrint(int color, std::string text){
    std::cout << "\033[1;" << color << "m" << text << "\033[0m";
  }
  void prettyPrintError(std::string text){
    prettyPrint(41, text);
  }
#endif


int main(){

  int mainColor{32}, secondaryColor{31};
  int mainBackgroundColor{42}, secondaryBackgroundColor{41}, accentBackgroundColor{46};

  int choice{0};
  std::string name1{""},name2{""},gender{""},date1{""},date2{""};
  gen::Genogram g= gen::createEmptyGen();

  do{

    std::cout << "--------------------------------------------" << std::endl;
    prettyPrint(secondaryBackgroundColor, " MENU "); 
    std::cout << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    prettyPrint(mainColor, "1 - Inserimento genogramma da file\n");
    prettyPrint(mainColor, "2 - Inserimento persona nel genogramma\n");
    prettyPrint(mainColor, "3 - Inserimento della relazione madre\n");
    prettyPrint(mainColor, "4 - Inserimento della relazione padre\n");
    prettyPrint(mainColor, "5 - Inserimento della relazione coppia\n");
    prettyPrint(mainColor, "6 - Inserimento della relazione figlio ad una coppia\n");
    prettyPrint(mainColor, "7 - Imposta data di nascita\n");
    prettyPrint(mainColor, "8 - Imposta data di morte\n");
    prettyPrint(mainColor, "9 - Cancella persona\n");
    prettyPrint(mainColor, "10 - Controlla validità del genogramma\n");
    prettyPrint(mainColor, "11 - Stampa genogramma\n");
    prettyPrint(secondaryColor, "0 - Exit\n");
    std::cout << "--------------------------------------------" << std::endl;
    prettyPrint(mainBackgroundColor, " Inserisci opzione: ");
    std::cout << " ";
    std::cin >> choice;

    try{
      switch (choice){
        case 0:
          exit(0);
        case 1:
          system(clearTerminalCmd);
          std::cout << "Nome file: ";
          std::cin >> name1;
          readGenogramFromFile(name1,g);
          break;
        case 2:
          system(clearTerminalCmd);
          std::cout << "Nome persona: ";
          std::cin >> name1;
          std::cout << "Genere (M/F): ";
          std::cin >> gender;
          std::cin.ignore();
          std::cout << "Data di nascita gg/mm/aaaa: ";
          getline(std::cin, date1);
          std::cout << "Data di morte gg/mm/aaaa: ";
          getline(std::cin, date2);
          gen::addPerson(name1,gender,date1,date2,g);
          break;
        case 3:
          system(clearTerminalCmd);
          std::cout << "Nome della madre: ";
          std::cin >> name1;
          std::cout << "Nome del figlio/a: ";
          std::cin >> name2;
          gen::addRelMother(name1,name2,g);
          break;
        case 4:
          system(clearTerminalCmd);
          std::cout << "Nome del padre: ";
          std::cin >> name1;
          std::cout << "Nome del figlio/a: ";
          std::cin >> name2;
          gen::addRelFather(name1,name2,g);
          break;
        case 5:
          system(clearTerminalCmd);
          std::cout << "Nome prima persona: ";
          std::cin >> name1;
          std::cout << "Nome seconda persona: ";
          std::cin >> name2;
          gen::addRelCouple(name1,name2,g);
          break;
        case 6:
          system(clearTerminalCmd);
          std::cout << "Nome di uno dei genitori: ";
          std::cin >> name1;
          std::cout << "Nome del figlio/a: ";
          std::cin >> name2;
          gen::addRelChildToCouple(name1,name2,g);
          break;
        case 7:
          system(clearTerminalCmd);
          std::cout << "Nome persona: ";
          std::cin >> name1;
          std::cout << "Data di nascita: ";
          std::cin >> date1;
          gen::setBirthDate(name1,date1,g);
          break;
        case 8:
          system(clearTerminalCmd);
          std::cout << "Nome persona: ";
          std::cin >> name1;
          std::cout << "Data di morte: ";
          std::cin >> date1;
          gen::setDeathDate(name1,date1,g);
          break;
        case 9:
          system(clearTerminalCmd);
          std::cout << "Nome persona da eliminare: ";
          std::cin >> name1;
          gen::deletePerson(name1,g);
          break;
        case 10:
          system(clearTerminalCmd);
          if(gen::isValid(g)){
            prettyPrint(accentBackgroundColor, " GENOGRAMMA VALIDO \n");
          }else{
            prettyPrintError(" GENOGRAMMA NON VALIDO \n");
          }
          break;
        case 11:
          system(clearTerminalCmd);
          prettyPrint(accentBackgroundColor, " RISULTATO \n");
          printGenogram(g);
          break;
        default:
          system(clearTerminalCmd);
          prettyPrintError(" COMANDO NON RICONOSCIUTO \n");
          break;
      }
    }catch(const char* exception){
      std::string msg = " ERRORE: "+std::string(exception)+" \n";
      prettyPrintError(msg);
    }

    }while(true);

  return 0;
}