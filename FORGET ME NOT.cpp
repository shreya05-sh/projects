#include <iostream>
#include <fstream>
#include<string>
#include<ctime>
#include<cstdlib>

using namespace std;

class Pet {
protected:
    string name, fav_food, allergic_food;
    int hunger, energy, happiness, points;

public:
    Pet(string n = "", string ff = "", string af = "")
        : name(n), fav_food(ff), allergic_food(af), hunger(100), energy(100), happiness(100), points(0) {}

    void show() {
        cout << "\nPet Name: " << name;
        cout << "\nFavorite Food: " << fav_food;
        cout << "\nAllergic To: " << allergic_food;
        cout << "\nHunger: " << hunger;
        cout << "\nEnergy: " << energy;
        cout << "\nHappiness: " << happiness;
        cout << "\nPoints: " << points;
    }

    void play() {
        if (energy >= 10) {
            cout << "\nYou played with " << name << "! Happiness increased!";
            happiness += 10;
            energy -= 10;
            points += 5;
        } else {
            cout << "\n" << name << " is too tired to play.";
        }
    }

    void feed(string food) {
        if (food == allergic_food) {
            cout << "\nOh no! " << name << " is allergic to " << food << "! Lost happiness.";
            happiness -= 20;
        } else if (food == fav_food) {
            cout << "\n" << name << " loves " << food << "! Gained energy and happiness.";
            happiness += 15;
            energy += 10;
            points += 10;
        } else {
            cout << "\n" << name << " ate " << food << ".";
            hunger -= 10;
            energy += 5;
        }
    }
    
    void interact(const string& weather) {
        cout << "\nHi, today the weather seems quite " << weather << ".\n";
        if (weather == "sunny") {
            cout << "How about a walk in the park? Your pet seems super energetic!\n";
            happiness += 20;
        } else if (weather == "rainy") {
            cout << "It's pouring outside! Your pet seems a bit lazy. Maybe some indoor games?\n";
            happiness += 10;
        } else if (weather == "cold") {
            cout << "Brr... it's chilly! Your pet is curled up and sleepy.\n";
            happiness += 5;
        } else {
            cout << "The weather seems calm. Your pet is feeling neutral today.\n";
            happiness += 10;
        }
        cout << "Current Happiness Level: " << happiness << "\n";
    }

     int getHappiness() const { return happiness; }

    int getPoints() { return points; }//returns the current points
    void setPoints(int p) { points = p; }//helps in setting the point to the value a player accumulated when he played the game again(i.e. loads the game again)
    virtual string getType() { return "Pet"; }//virtual function that'll help the pet to have different types....(no idea what it does)
    //below is a function to store the details of pettype,name and points accumulated so far in a file.
    /*
    this is also a virtual function...and we have created a reference object for file
    Virtual function= it is written in the base class and is overriden by the derived class. 
    It uses a concept of Runtime Polymorphism.
    This function will perform differently depending on the object that calls this function.
    Why is it necessary?? Because if we look at the saveProgress function, we are saving the type of the pet as well.\
    so if it was not A virtual function...everytime we would have stored the pet type as Pet...and we would never know if our pet is a cat,dog,or dragon.
    
    beause of this function...on reloading the game again, our function loadgame can easily create another pet of it's respective type,
    depending upon the type retrieved.
    
    this is a great example of dynamic polymorphism	
    */
    virtual void saveProgress(ofstream &file) { file << getType() << " " << name << " " << points << endl; }
};

class Dog : public Pet {
public:
    Dog(string n = "") : Pet(n, "Bone", "Chocolate") {}
    string getType() { return "Dog"; }//will override the above virtual function in base class
};

class Cat : public Pet {
public:
    Cat(string n = "") : Pet(n, "Fish", "Milk") {}
    string getType() { return "Cat"; }//will override the above virtual function in base class
};

class Rabbit : public Pet {
public:
    Rabbit(string n = "") : Pet(n, "Carrot", "Tomato") {}
    string getType() { return "Rabbit"; }//will override the above virtual function in base class
};

class Dragon : public Pet {
public:
    Dragon(string n = "") : Pet(n, "Gems", "Silver") {}
    string getType() { return "Dragon"; }//will override the above virtual function in base class
};



// Weather System Function
string getRandomWeather() {
    string weatherOptions[] = {"sunny", "rainy", "cloudy", "cold"};
    return weatherOptions[rand() % 4];
}

// Time-Tracking Functions
void saveExitTime() {
    ofstream timeFile("time.txt");
    time_t now = time(0);
    timeFile << now;
    timeFile.close();
}

time_t getLastExitTime() {
    ifstream timeFile("time.txt");
    time_t lastTime = 0;
    if (timeFile.is_open()) {
        timeFile >> lastTime;
        timeFile.close();
    }
    return lastTime;
}

// Check for Neglect and Handle Pet's Disappearance
bool checkNeglect() {
    time_t now = time(0);
    time_t lastTime = getLastExitTime();
    time_t t=90;

    if (lastTime != 0 && difftime(now, lastTime) > t) { // 600 seconds = 10 mins
        cout << "\nOH NO! You neglected your pet for too long, and it's gone... ??\n";
        return true;
    }
    return false;
}

void clearSaveData() {
    remove("savegame.txt");
    remove("time.txt");
}

// Quiz to Retrieve the Pet

//to be improved.....
bool recoveryQuiz(string petName) {
    string answer;
    cout << "Quick! To bring your pet back, answer this question: \n";
    cout << "What was your pet's name? ";
    cin >> answer;
    return (answer == petName); 
}


void saveGame(Pet &pet) {
    ofstream file("savegame.txt");
    pet.saveProgress(file);
    file.close();
    saveExitTime();
}
//object pointer used....recreating the pet....on loading the game again.
Pet* loadGame() {
    ifstream file("savegame.txt");
    if (file.is_open()) {
        string type, name;
        int points;
        file >> type >> name >> points;
        file.close();

        Pet* loadedPet = NULL;
        if (type == "Dog") loadedPet = new Dog(name);
        else if (type == "Cat") loadedPet = new Cat(name);
        else if (type == "Rabbit") loadedPet = new Rabbit(name);
        else if (type == "Dragon") loadedPet = new Dragon(name);

        if (loadedPet) loadedPet->setPoints(points);
        return loadedPet;
    }
    return NULL;
}
/*
An object pointer is a pointer that basically points to an object of a class.

why didn't we do Dog mydog, or something similar to cat class....well, if we would have done that then, when the time would have come for evolution,
it would've failed because we would have written something like- Dog mydog= new Dragon("drako");
this would fail immediately because a dog can't become dragon...
hence the object pointer comes in handy....it makes sure to act like the object it's pointing to.
using the arrow is just the replacement of writing- (*mydog).getType();
dynamic binding-- calls the correct function based on the object type
*/



/*
Object slicing....another benefit of using the object pointer is that it's preventing the object slicing
now object slicing occurs when, we try to assign a derived class object to a base class object...
for example- if we wrote- 
Pet mypet=Dog("Brooks"); when we write this line...it c++ copies the object and when it copies it only copies the base class part.
this would forcibly convert the dog class object to an object of the base class and force it to behave as a simple pet...an dslice off it's unique features...this is called object slicing


*/
int main() {
	srand(time(0));
	string weather = getRandomWeather();
	ifstream file("savegame.txt");
    bool hasSaveFile = file.good();
    file.close();
    
    int choice;

    if (!hasSaveFile) {
        cout << "No saved game found.\n";
        cout << "1. Start a new game\n2. Exit\n";
        cin >> choice;
        if (choice != 1) return 0;
    } else {
        cout << "Do you want to: \n1. Load Previous Game\n2. Start a New Game\n";
        cin >> choice;
        if (choice == 2) clearSaveData();
    }
    string name;
    Pet* myPet = loadGame();//mypet is an object pointer to my class Pet.. mypet can point to any object, be it cat,dog,dragon, depending on the way we allocate it the memory.
	if (!myPet) {
        int choice;
        cout << "\nChoose a pet: 1. Dog  2. Cat  3. Rabbit";
        cin >> choice;
        cout << "Enter your pet's name: ";
        cin >> name;

        switch (choice) {
            case 1: myPet = new Dog(name); break;//allocating memory to the new object that is being created of the new pet
            case 2: myPet = new Cat(name); break;//allocating memory to the new object that is being created of the new pet
            case 3: myPet = new Rabbit(name); break;//allocating memory to the new object that is being created of the new pet
            default: cout << "\nInvalid choice."; return 0;
        }
    }
     myPet->interact(weather);

    if (checkNeglect()) {
        if (recoveryQuiz(name)) {
            cout << "\nYou saved your pet! They're back and happier than ever!\n";
        } else {
            cout << "\nUnfortunately, your pet has found a new home...:(( \n";
            return 0;
        }
    }
    
    int action;
    do {
        cout << "\n\nChoose an action: 1. Show Stats  2. Play  3. Feed  4. Save & Exit";
        cin >> action;
        switch (action) {
            case 1: myPet->show(); break;
            case 2: myPet->play(); break;
            case 3: {
                string food;
                cout << "Enter food: ";
                cin >> food;
                myPet->feed(food);
                break;
            }
            case 4: saveGame(*myPet);
			  break;
            default: cout << "\nInvalid action.";
        }

        if (myPet->getPoints() >= 50 && myPet->getType() != "Dragon") {
            cout << "\nCongratulations! You've unlocked the Dragon!";
            string dragonName;
            cout << "\nEnter a new name for your Dragon: ";
            cin >> dragonName;
            delete myPet;
            myPet = new Dragon(dragonName);
        }
    } while (action != 4);
    
    saveGame(*myPet);
    delete myPet;
    return 0;
}

