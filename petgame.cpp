include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <fstream>

class Pet {
protected:
    std::string name;
    int health, hunger, happiness, level, age;
    bool isSick;

public:
    Pet(std::string n) 
        : name(n), health(100), hunger(50), happiness(70), level(1), age(0), isSick(false) {}

    virtual void play() = 0;  // Pure virtual function for polymorphism

    void feed() {
        hunger = std::max(0, hunger - 20);
        health += 5;
        std::cout << name << " is fed. Hunger: " << hunger << ", Health: " << health << std::endl;
    }

    void rest() {
        happiness += 10;
        health += 10;
        std::cout << name << " rested. Happiness: " << happiness << ", Health: " << health << std::endl;
    }

    void train() {
        if (happiness >= 20) {
            happiness -= 20;
            level++;
            std::cout << name << " has been trained! New level: " << level << std::endl;
        } else {
            std::cout << name << " is not happy enough to train!" << std::endl;
        }
    }

    void heal() {
        if (isSick) {
            health += 20;
            isSick = false;
            std::cout << name << " has been healed! Health: " << health << std::endl;
        } else {
            std::cout << name << " is not sick!" << std::endl;
        }
    }

    void checkIllness() {
        if (health < 30) {
            isSick = true;
            std::cout << name << " is feeling unwell! Health is low." << std::endl;
        }
    }

    virtual void updateStats() {
        hunger += 10;
        happiness -= 5;
        health -= (hunger > 70) ? 10 : 0;
        age++;  // Increment age over time
        checkIllness();  // Check if pet becomes sick
        checkLevelUp();
    }

    void displayStatus() const {
        std::cout << "Pet: " << name << " | Age: " << age << " | Health: " << health
                  << " | Hunger: " << hunger << " | Happiness: " << happiness 
                  << " | Level: " << level << " | Sick: " << (isSick ? "Yes" : "No") << std::endl;
    }

    void saveStatus() const {
        std::ofstream file(name + "_status.txt");
        if (file.is_open()) {
            file << name << "\n" << health << "\n" << hunger << "\n" << happiness 
                 << "\n" << level << "\n" << age << "\n" << isSick << "\n";
            file.close();
            std::cout << "Status saved!" << std::endl;
        } else {
            std::cout << "Error saving status!" << std::endl;
        }
    }

    void loadStatus() {
        std::ifstream file(name + "_status.txt");
        if (file.is_open()) {
            file >> name >> health >> hunger >> happiness >> level >> age >> isSick;
            file.close();
            std::cout << "Status loaded!" << std::endl;
        } else {
            std::cout << "Error loading status!" << std::endl;
        }
    }

    // Getter and Setter for happiness
    int getHappiness() const { return happiness; }
    void setHappiness(int value) { happiness = std::max(0, value); }

    // Getter for isSick
    bool getIsSick() const { return isSick; }

protected:
    void checkLevelUp() {
        if (health > 90 && happiness > 80) {
            level++;
            health += 10;
            happiness += 10;
            std::cout << name << " leveled up! New level: " << level << std::endl;
        }
    }
};

class Dog : public Pet {
public:
    Dog(std::string n) : Pet(n) {}
    
    void play() override {
        happiness += 20;
        hunger += 10;
        std::cout << name << " fetches a ball! Happiness: " << happiness << ", Hunger: " << hunger << std::endl;
    }
};

class Cat : public Pet {
public:
    Cat(std::string n) : Pet(n) {}

    void play() override {
        happiness += 15;
        hunger += 5;
        std::cout << name << " chases a laser! Happiness: " << happiness << ", Hunger: " << hunger << std::endl;
    }
};

class Bird : public Pet {
public:
    Bird(std::string n) : Pet(n) {}

    void play() override {
        happiness += 10;
        hunger += 5;
        std::cout << name << " sings a song! Happiness: " << happiness << ", Hunger: " << hunger << std::endl;
    }
};

// Function to simulate random events affecting pet
void randomEvent(Pet &pet) {
    int event = rand() % 5;  // Increased to 5 for more variety
    if (event == 0) {
        std::cout << "Random Event: Your pet found a toy! Happiness increased." << std::endl;
        pet.play();
    } else if (event == 1) {
        std::cout << "Random Event: Your pet is in a grumpy mood. Happiness decreased." << std::endl;
        pet.rest();
    } else if (event == 2) {
        std::cout << "Random Event: Your pet went on a walk. Hunger slightly increased." << std::endl;
        pet.feed();
    } else if (event == 3) {
        std::cout << "Random Event: A surprise visitor brings treats! Happiness increased." << std::endl;
        pet.rest();
        pet.feed();
    } else {
        std::cout << "Random Event: Your pet caught a cold! Health decreased." << std::endl;
        pet.updateStats();  // Trigger health decrease
    }
}

// Function to simulate a shopping experience
void shop(Pet &pet) {
    int choice;
    std::cout << "Welcome to the pet shop! What would you like to buy?\n";
    std::cout << "1. Treats (+20 Happiness) - $10\n";
    std::cout << "2. Toy (+30 Happiness) - $20\n";
    std::cout << "3. Medicine (Cures Illness) - $15\n";
    std::cout << "4. Exit Shop\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            if (pet.getHappiness() <= 80) {
                pet.setHappiness(pet.getHappiness() + 20);
                std::cout << "You bought treats! Happiness: " << pet.getHappiness() << std::endl;
            } else {
                std::cout << "Happiness is already high!" << std::endl;
            }
            break;
        case 2:
            if (pet.getHappiness() <= 70) {
                pet.setHappiness(pet.getHappiness() + 30);
                std::cout << "You bought a toy! Happiness: " << pet.getHappiness() << std::endl;
            } else {
                std::cout << "Happiness is already high!" << std::endl;
            }
            break;
        case 3:
            if (pet.getIsSick()) {
                pet.heal();
            } else {
                std::cout << "Your pet is not sick!" << std::endl;
            }
            break;
        case 4:
            std::cout << "Exiting shop." << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Seed for random events

    std::cout << "Choose your pet type:\n1. Dog\n2. Cat\n3. Bird\n";
    int choice;
    std::cin >> choice;

    Pet *myPet = nullptr;
    std::string petName;
    std::cout << "Enter your pet's name: ";
    std::cin >> petName;

    switch (choice) {
        case 1: myPet = new Dog(petName); break;
        case 2: myPet = new Cat(petName); break;
        case 3: myPet = new Bird(petName); break;
        default: std::cout << "Invalid choice!" << std::endl; return 1;
    }

    // Game loop
    char action;
    while (true) {
        myPet->displayStatus();
        std::cout << "Choose an action:\nF: Feed\nP: Play\nR: Rest\nT: Train\nS: Shop\nH: Heal\nL: Load Status\nQ: Quit\n";
        std::cin >> action;

        if (action == 'F' || action == 'f') {
            myPet->feed();
        } else if (action == 'P' || action == 'p') {
            myPet->play();
        } else if (action == 'R' || action == 'r') {
            myPet->rest();
        } else if (action == 'T' || action == 't') {
            myPet->train();
        } else if (action == 'S' || action == 's') {
            shop(*myPet);
        } else if (action == 'H' || action == 'h') {
            myPet->heal();
        } else if (action == 'L' || action == 'l') {
            myPet->loadStatus();
        } else if (action == 'Q' || action == 'q') {
            myPet->saveStatus();
            std::cout << "Goodbye!" << std::endl;
            break;
        } else {
            std::cout << "Invalid action!" << std::endl;
        }

        // Update pet's stats and trigger random event occasionally
        myPet->updateStats();
        if (rand() % 4 == 0) {
            randomEvent(*myPet);
        }
    }

    delete myPet;
    return 0;
}
