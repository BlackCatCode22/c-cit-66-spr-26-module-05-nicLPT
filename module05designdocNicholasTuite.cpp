#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// module 5 - adapted code to better use linked lists, new animal subclasses section, new usage of pointers
// base class
class Animal {
protected:
    std::string name;
    int age;
    std::string species;
public:
    Animal(std::string n, int a, std::string s) : name(n), age(a), species(s) {}
    virtual ~Animal() {}
    std::string getName() const { return name; }
    int getAge() const { return age; }
    std::string getSpecies() const { return species; }
    virtual std::string getHabitatInfo() const = 0;
};

// animal subclasses
class Hyena : public Animal { public: Hyena(std::string n, int a) : Animal(n, a, "Hyena") {} std::string getHabitatInfo() const override { return "Hyena Habitat"; } };
class Lion : public Animal { public: Lion(std::string n, int a) : Animal(n, a, "Lion") {} std::string getHabitatInfo() const override { return "Lion Habitat"; } };
class Tiger : public Animal { public: Tiger(std::string n, int a) : Animal(n, a, "Tiger") {} std::string getHabitatInfo() const override { return "Tiger Habitat"; } };
class Bear : public Animal { public: Bear(std::string n, int a) : Animal(n, a, "Bear") {} std::string getHabitatInfo() const override { return "Bear Habitat"; } };

// first node for individual animal instances
struct AnimalNode {
    Animal* data;
    AnimalNode* next;
    AnimalNode(Animal* a) : data(a), next(nullptr) {}
};

// node for species
struct SpeciesNode {
    std::string speciesName;
    AnimalNode* animalHead;
    SpeciesNode* next;      // pointer for next species group
    SpeciesNode(std::string name) : speciesName(name), animalHead(nullptr), next(nullptr) {}
};

int main() {
    SpeciesNode* speciesListHead = nullptr;

    std::ifstream inputFile("arrivingAnimals.txt");
    if (!inputFile) {
        std::cerr << "Error: Could not open arrivingAnimals.txt" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, name, ageStr;
        std::getline(ss, id, ';');
        std::getline(ss, name, ';');
        std::getline(ss, ageStr, ';');

        int age = 0;
        try { age = std::stoi(ageStr.substr(0, ageStr.find(" "))); } catch (...) { age = 0; }

        Animal* newAnimal = nullptr;
        if (id.find("Hy") != std::string::npos) newAnimal = new Hyena(name, age);
        else if (id.find("Li") != std::string::npos) newAnimal = new Lion(name, age);
        else if (id.find("Ti") != std::string::npos) newAnimal = new Tiger(name, age);
        else if (id.find("Be") != std::string::npos) newAnimal = new Bear(name, age);

        if (newAnimal) {
            std::string sName = newAnimal->getSpecies();

            // creating species node
            SpeciesNode* currSpecies = speciesListHead;
            SpeciesNode* prevSpecies = nullptr;

            while (currSpecies != nullptr && currSpecies->speciesName != sName) {
                prevSpecies = currSpecies;
                currSpecies = currSpecies->next;
            }

            if (currSpecies == nullptr) {
                currSpecies = new SpeciesNode(sName);
                if (prevSpecies == nullptr) speciesListHead = currSpecies;
                else prevSpecies->next = currSpecies;
            }

            AnimalNode* newNode = new AnimalNode(newAnimal);
            newNode->next = currSpecies->animalHead;
            currSpecies->animalHead = newNode;
        }
    }
    inputFile.close();

    // report
    std::ofstream outputFile("newAnimals.txt");
    outputFile << "ZOO ARRIVAL REPORT (Manual Linked Lists)\n";
    outputFile << "========================================\n\n";

    SpeciesNode* sPtr = speciesListHead;
    while (sPtr) {
        outputFile << sPtr->speciesName << "s:\n";
        AnimalNode* aPtr = sPtr->animalHead;
        while (aPtr) {
            outputFile << " - Name: " << aPtr->data->getName()
                       << ", Age: " << aPtr->data->getAge() << "\n";
            aPtr = aPtr->next;
        }
        outputFile << "\n";
        sPtr = sPtr->next;
    }

    // memory cleanup
    while (speciesListHead) {
        SpeciesNode* tempS = speciesListHead;
        AnimalNode* aPtr = tempS->animalHead;
        while (aPtr) {
            AnimalNode* tempA = aPtr;
            aPtr = aPtr->next;
            delete tempA->data;
            delete tempA;
        }
        speciesListHead = speciesListHead->next;
        delete tempS;
    }

    return 0;
}