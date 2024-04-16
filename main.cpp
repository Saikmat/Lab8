/*
* Copyright 2024 - Howard Community College All rights reserved; Unauthorized duplication prohibited.
* Name: Sai Matukumalli
* Class: CMSY-171
* Program Name: Lab 8
* Program Description: This program is a modification of the Animal Database. It uses linked lists instead of vectors
* The goal of the project is to store several animal records in a linked list.
* It features the ability to add, display all, display endangered, and search and update animals in the list, as well as
* updating the file behind the program.
*/


#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <list>

using namespace std;

const int MAX_LENGTH = 25;
const int ENDANGERED_COUNT = 100;

struct Animal {
    char name[MAX_LENGTH];
    char species[MAX_LENGTH];
    int typeCount;
    bool endangered;
    
    inline bool operator>(const Animal a){
        return strcmp(this->name, a.name) > 0;
    }
    
    inline bool operator<(const Animal a){
        return strcmp(this->name, a.name) < 0;
    }
    
    inline bool operator ==(const Animal a){
        return strcmp(this->name, a.name) == 0;
    }
    
    inline bool operator !=(const Animal a){
      return strcmp(this->name, a.name) != 0;
    }
    
};



void printCopyright();

void addAnimals(list<Animal> *database, vector<string> species);

void displayAnimals(list<Animal> database);

void displayEndangered(list<Animal> database);

void searchAnimals(list<Animal> &, const vector<string>&, fstream&);

void readAnimal(list<Animal>& animals, fstream& stream);

void readSpecies(vector<string>& species, fstream&);

void refreshFile(list<Animal>& animals, fstream &stream);

void updateRecordInVector(list<Animal> &animals, int loc, vector<string> species, fstream &stream);

void updateRecordInFile(list<Animal> animals, int location, fstream &stream);

void updateEndangered(list<Animal> *animals);

Animal* getValueInList(list<Animal> animals, int location);

Animal notTempGetValueInList(list<Animal> animals, int location);

int main() {
    printCopyright();

    int chosenOption;
    const int ADD_ANIMALS_MENU_OPTION = 1;
    const int DISPLAY_ANIMALS_MENU_OPTION = 2;
    const int DISPLAY_ENDANGERED_MENU_OPTION = 3;
    const int SEARCH_ANIMALS_MENU_OPTION = 4;
    const int QUIT_MENU_OPTION = 5;
    const string ANIMAL_RECORD_LOCATION = R"(C:\Users\SaiKM\CLionProjects\Lab8\animal.dat)";
    const string SPECIES_RECORD_LOCATION = R"(C:\Users\SaiKM\CLionProjects\Lab8\species.txt)";

    fstream animalRecords;
    animalRecords.open(ANIMAL_RECORD_LOCATION, fstream::in | fstream::out | fstream::binary);
    list<Animal> database;
    readAnimal(database, animalRecords);
    animalRecords.close();
    animalRecords.open(ANIMAL_RECORD_LOCATION, ios::binary | ios::out);
    refreshFile(database, animalRecords);
    animalRecords.close();

    fstream speciesRecords;
    speciesRecords.open(SPECIES_RECORD_LOCATION);
    vector<string> speciesList;
    readSpecies(speciesList, speciesRecords);

    const char WELCOME_TEXT[] = "Welcome to Animal Vector Database\n\n";
    const char MENU_TEXT[] = "\n1. Add Animal(s)"
                             "\n2. Display Animals"
                             "\n3. Display Endangered"
                             "\n4. Search animals"
                             "\n5. Quit";
    cout << WELCOME_TEXT;

    bool quit = true;
    while (quit) {
        chosenOption = 0;
        cout << MENU_TEXT;
        cout << "\nEnter a menu choice: ";
        cin >> chosenOption;
        while (
                !(chosenOption == ADD_ANIMALS_MENU_OPTION
               || chosenOption == DISPLAY_ANIMALS_MENU_OPTION
               || chosenOption == DISPLAY_ENDANGERED_MENU_OPTION
               || chosenOption == SEARCH_ANIMALS_MENU_OPTION
               || chosenOption == QUIT_MENU_OPTION)) {
            cout << "Invalid option";
            cout << MENU_TEXT << endl;
            cout << "Enter a menu choice: ";
            if (!(cin >> chosenOption)) {
                return 1;
            }
        }
        switch (chosenOption) {
            case ADD_ANIMALS_MENU_OPTION: {
                addAnimals(&database, speciesList);
                animalRecords.open(ANIMAL_RECORD_LOCATION, ios::out | ios::binary);
                refreshFile(database, animalRecords);
                break;
            }
            case DISPLAY_ANIMALS_MENU_OPTION: {
                displayAnimals(database);
                break;
            }
            case DISPLAY_ENDANGERED_MENU_OPTION: {
                displayEndangered(database);
                break;
            }
            case SEARCH_ANIMALS_MENU_OPTION: {
                animalRecords.open(ANIMAL_RECORD_LOCATION, ios::binary | ios::in | ios::out);
                searchAnimals(database, speciesList, animalRecords);
                break;
            }
            default: // should never be used but prevents clang warning
            case QUIT_MENU_OPTION: {
                cout << "Thank you for using animal database";
                quit = false;
                break;
            }
        }
    }

    animalRecords.clear();
    speciesRecords.clear();
    animalRecords.close();
    speciesRecords.close();
    database.clear();
    speciesList.clear();

    return 0;
}


/*
 * Prints Howard CC Copyright statement
 */
void printCopyright() {
    cout << "Copyright 2024 - Howard Community College All rights reserved; Unauthorized duplication prohibited\n\n\n\n";
}

/*
 * This function takes in the list where animals are stored as well as the species list
 * It adds an animal to the list given parameters name, species, and population, and checks if it is endangered
 */
void addAnimals(list<Animal> *database, vector<string> species) {
    int animalCount;
    string speciesName;
    char *inputCstring;
    const string NONE = "none";
    string type = "Please enter the animal type (none to stop): ";
    string speciesText = "Please enter the animal species from the menu";
    string number = "Enter the number of animals: ";
    string error = "Negative Count, enter a positive value: ";
    string alreadyFound = "This animal is already in the database, please enter another animal";
    while (true) {
        cout << type;
        cin.ignore();
        string originalCaps;
        string input;
        bool flag;
        do {
            flag = false;
            getline(cin, input);
            originalCaps = input;
            for (char &c: input) {
                c = tolower(c);
            }
            if (input == NONE) {
                return;
            }
            inputCstring = new char[originalCaps.length() + 1];
            for (int i = 0; i < originalCaps.length(); i++) {
                inputCstring[i] = originalCaps.at(i);
            }
            inputCstring[input.length()] = '\0';\

            for(Animal animal : *database){
                if((animal).name == originalCaps){
                    cout << alreadyFound;
                    flag = true;
                }
            }
        } while (flag);
        cout << speciesText << "\n";
        for (int i = 0; i < species.size(); ++i) {
            cout << i+1 << ". " + species.at(i) << endl;
        }
        int speciesValue = 0;
        cin >> speciesValue;
        while(speciesValue < 1 || speciesValue > species.size()) {
            cout << "Invalid entry, enter a value between 1 and " << species.size()-1;
            cin >> speciesValue;
        }
        speciesName = species.at(speciesValue - 1);
        cout << number;
        cin >> animalCount;
        while (animalCount < 0) {
            cout << error;
            cin >> animalCount;
        }
        cout << "\n";
        Animal animal = *new Animal;
        animal.typeCount = animalCount;
        strncpy_s(animal.name, MAX_LENGTH, inputCstring, MAX_LENGTH);
        strncpy_s(animal.species, MAX_LENGTH, speciesName.c_str(), MAX_LENGTH);
        animal.endangered = animalCount < ENDANGERED_COUNT;
        
        for (auto iterator = database->begin(); iterator != database->end(); ++iterator) {
            if(*iterator > animal){
                database->insert(iterator, animal);
                return;
            }
        }
        database->emplace_back(animal);
    }
}

/*
 * This function refreshes the file to ensure that the file is in the same order as the list
 * Modifies entire file, so takes a long time, avoid using if possible
 */
void refreshFile(list<Animal>& animals, fstream &stream) {
    stream.seekp(ios::beg);
    for (Animal animal : animals) {
        stream.write(reinterpret_cast<char*>(&animal), sizeof(Animal));
    }
}

/*
 * Displays the list of animals with their name, species, population, and whether they are endangered or not
 * Takes parameter of the list of animals
 */
void displayAnimals(list<Animal> database) {
    const char ENDANGERED_STRING[] = "\nThis animal is endangered";
    const char NOT_ENDANGERED_STRING[] = "\nThis animal is not endangered";
    for (const Animal animal: database) {
        cout << "\nAnimal: " << (animal).name <<
             "\nOf species " << (animal).species <<
             "\nHas a count of: " << (animal).typeCount <<
             ((animal).endangered ? ENDANGERED_STRING : NOT_ENDANGERED_STRING);
        cout << "\n\n";
    }
    if(database.empty()){
        cout << "\nThere are no animals to display\n";
    }
}

/*
 * Displays the names of the animals that are endangered
 * Takes in the list of animals and checks if the animal is endangered, and prints
 */
void displayEndangered(list<Animal> database) {
    const char ENDANGERED_STRING[] = " is endangered\n";
    for(const Animal animal : database){
        if((animal).endangered){
            cout << (animal).name << ENDANGERED_STRING;
        }
    }
    if(database.empty()){
        cout << "\nThere are no endangered animals to display\n";
    }
}

/*
 * Uses linear search to find the animals in the list given a user specified name
 * Asks the user if they want to update the record as well
 * Takes in the list of animals, the list of species, and a filestream that is passed through
 */
void searchAnimals(list<Animal> &animals, const vector<string>& species, fstream &stream) {
    cout << "Enter the name of the animal you are looking for: ";
    string input;
    cin.ignore();
    getline(cin, input);
    int i = 0;
    for (Animal animal : animals){
        if(animal.name == input){
            cout << "Animal found" << endl
                 << "Animal: " << animal.name << endl
                 << "Species: " << animal.species << endl
                 << "Population: " << animal.typeCount
                 << ". This animal is "
                 << (animal.endangered ? "Endangered!" : "Not Endangered!") << endl;
            cout << "Do you want to update the record?<y/n>";
            cin >> input;
            while(input != "y" || input != "n") {
                if (input == "y") {
                    updateRecordInVector(animals, i, species, stream);
                    return;
                } else if (input == "n") {
                    return;
                } else{
                    cout << "Invalid input, enter y or n: ";
                    cin >> input;
                }
            }
            return;
        }
        i++;
    }

    cout << "\nThere is no animal entry corresponding to \"" << input << "\"" << endl;
}

/*
 * Finds the location of the record in the file and puts the new animal record into the file, overwriting the old record
 */
void updateRecordInFile(list<Animal> animals, int location, fstream &stream) {
    stream.seekg(location * sizeof(Animal), ios::beg);
    stream.seekp(ios::beg + location * sizeof(Animal));
    stream.write(reinterpret_cast <char*> (getValueInList(animals, location)), sizeof(Animal));
    stream.close();
}

/*
 * Asks the user to update the values in the animal, and then pushes the value into the vector
 * Also calls updateRecordInFile to update the file
 * Takes in the list of animals, the location that needs to be updated, the list of species and the filestream passes through
 */
void updateRecordInVector(list<Animal> &animals, int loc, vector<string> species, fstream &stream) {
    const string animalUpdate = "Enter the value you want to change animal to(! for no change)";
    const string speciesUpdate = "Enter the new species of the animal";
    const string countUpdate = "Enter the new count of animals";

    string temp;
    cout << animalUpdate;
    cin.ignore();
    getline(cin, temp);
    Animal animal = notTempGetValueInList(animals, loc);
    
    if(temp != "!"){
        strncpy_s(animal.name, MAX_LENGTH, temp.c_str(), MAX_LENGTH);
    }

    cout << speciesUpdate << endl;
    for (int i = 0; i < species.size(); ++i) {
        cout << i+1 << ". " << species.at(i) << endl;
    }
    
    int numberInput;
    cin >> numberInput;
    while(numberInput < 1 || numberInput > species.size()){
        cout << "\nThat is an invalid option, enter a value between 1 and " << species.size();
        cin >> numberInput;
    }
    
    strncpy_s(animal.species, MAX_LENGTH, species.at(numberInput-1).c_str(), MAX_LENGTH);

    cout << countUpdate;
    cin >> numberInput;
    while(numberInput < 0){
        cout << "Negative Number, please enter a positive number Input for count";
        cin >> numberInput;
    }
    
    animal.typeCount = numberInput;
    animal.endangered = animal.typeCount < ENDANGERED_COUNT;
    animals.remove(notTempGetValueInList(animals, loc));
    animals.insert(next(animals.begin(), loc), animal);
    updateRecordInFile(animals, loc, stream);
}

/*
 * Reads the animals from the filesteream and pushes them onto the vector
 * Takes in parameters of the list of animals and the filestream
 */
void readAnimal(list<Animal> &animals, fstream &stream) {
    while (!stream.eof()) {
        if(!stream.good() || stream.eof()) {
            cout << "read failed";
            return;
        }
        Animal animal{};
        stream.read(reinterpret_cast<char*>(&animal), sizeof(Animal));
        if(strcmp(animal.name, "") == 0){
            return;
        }
        auto iter = animals.begin();
        
        bool pushed = false;
        for (int i = 0, SIZE = animals.size(); i < SIZE && !pushed; ++i, iter++) {
            if(*iter > animal) {
                animals.insert(iter, animal);
                pushed = true;
            }
        }
        if(animals.empty() || !pushed){
            animals.push_back(animal);
        }
    }
    animals.pop_back();
    updateEndangered(&animals);
}

/*
 * Updates the endangered values for all animal records, good if the file data is incorrect or just needs to be refreshed
 * Takes in the list of animals
 */
void updateEndangered(list<Animal> *animals) {
    for (Animal animal : *animals) {
        animal.endangered = animal.typeCount < ENDANGERED_COUNT;
    }
}

/*
 * Reads the species file and pushes it into a vector
 * Takes in the list of species and a filestream that it reads from
 */
void readSpecies(vector<string> &species, fstream &stream) {
    while(!stream.eof()){
        char *str = new char[25];
        stream.getline(str, 25);
        species.emplace_back(str);
    }
}

/*
 * A function to replicate the behavior of .at(int loc) from vectors for linked lists
 * Used if random access is needed
 * Takes in the list of animals and the location that is needed
 * Returns the animal at that location
 */
Animal* getValueInList(list<Animal> animals, int location){
    auto iter = animals.begin();
    for (int i = 0; i < location; ++i) {
        iter++;
    }
    return &*iter;
}

Animal notTempGetValueInList(list<Animal> animals, int location){
    auto iter = animals.begin();
    for (int i = 0; i < location; ++i) {
        iter++;
    }
    return *iter;
}