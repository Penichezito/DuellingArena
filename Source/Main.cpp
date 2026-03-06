#include <iostream>
#include <string>
#include <cstdlib> // Necessary for std::rand and std::srand
#include <ctime>   // Necessary for std::time
#include <memory>  // Necessary for using Smart Pointers std::unique_ptr

// Class representing a character in the dueling arena
class Character {
private:
	// Attributes of the character
	std::string name;
	int health;
	int maxHealth;
	int baseAttack;
	bool isDefending;

public:
	// Constructor to initialize the character's attributes
	Character(std::string characterName, int initialHealth, int attackPower)
		: name(characterName), health(initialHealth), maxHealth(initialHealth), baseAttack(attackPower), isDefending(false) {
	}

	// Getters: methods to access private attributes
	std::string getName() {
		return name;
	}

	int getHealth() {
		return health;
	}

	int isAlive() {
		return health > 0;
	}


	// Action: Defend 
	void defend() {
		isDefending = true;
		std::cout << name << " Assuming a defensive stance! " << std::endl;
	}

	// Reseting stance in each turn
	void resetDefense() {
		isDefending = false;
	}

	// Method to calculate and apply damage received from an attack
	void takeDamage(int damage) {
		// if character is defending, reduce damage by half
		if (isDefending) {
			damage = damage / 2;
			std::cout << name << " blocks the attack partially ";
		}

		health -= damage;

		// Prevents life from going negative
		if (health < 0) {
			health = 0;
		}

		std::cout << name << " suffered" << damage << " damage. (Life: " << health << "/" << maxHealth << ")" << std::endl;
	}

	// Action: Attack(Receives a pointer to the target character)
	void attack(Character* target) {
		// Adding a random factor to the attack damage for variability (Base Damage +/- 3)
		int randomVariation = (std::rand() % 7) - 3; // Generates a random number between -3 and +3
		int finalDamage = baseAttack + randomVariation;

		if (finalDamage < 0) finalDamage = 0; // Prevents negative damage

		std::cout << name << " Attack fercefully the" << target->getName() << "!" << std::endl;

		// Calling the method to apply damage to the target
		target->takeDamage(finalDamage);
	}

};

// Main Function to display the dueling arena
int main() {
	// Initializing random seed generator for variability in attacks using current time
	srand(time_t(0));

	std::cout << "======================================" << std::endl;
	std::cout << "      WELCOME TO DUELLING ARENA       " << std::endl;
	std::cout << "======================================" << std::endl;

	// Stack allocating characters using smart pointers for automatic memory management
	// Used unique_ptr to ensure that memory is automatically released when the character goes out of scope

	Character player("Fingolfin", 100, 18);

	// ALLOCATION WITH SMART POINTERS (Safe Dynamic Memory)
	// The 'make_unique' allocates the object in Heap memory and guarantees its automatic destruction at the end.
	// Access to methods is done using the arrow (->)
	std::unique_ptr<Character> enemy = std::make_unique<Character>("Melkor", 120, 14);

	int round = 1;

	// Game loop continues until one of the characters is defeated
	while (player.isAlive() && enemy->isAlive()) {
		std::cout << " /n--- Turn " << round << "---" << std::endl;
		std::cout << player.getName() << "HP: [" << player.getHealth() << "]" << std::endl;
		std::cout << enemy->getName() << "HP: [" << enemy->getHealth() << "]" << std::endl;

		std::cout << "Choose your action -> (1)Attack or (2)Defend:" << std::endl;
		std::cout << "(1) Attack" << std::endl;
		std::cout << "(2) Defend" << std::endl;

		int choice;
		std::cin >> choice;

		// Clears up the defense from the previous round.
		player.resetDefense();
		enemy->resetDefense();

		std::cout << "/n> End turn: " << std::endl;

		// Player's logic based on user input
		if (choice == 2) {
			player.defend();
		}

		// IA logic for the enemy's action (Randomly decides to attack or defend)
		// 25% chances to defend and 75% chances to attack)
		int enemyChoice = rand() % 4; // Generates a random number between 0 and 3
		bool enemyWillDefend = (enemyChoice == 0);

		if (choice == 1) {
			// Since 'enemy' is a unique_ptr, we use .get() to temporarily extract the raw pointer.
			player.attack(enemy.get());
		}

		// Enemy's turn to attack if it's alive and not defending
		if (enemy->isAlive() && !enemyWillDefend) {
			enemy->attack(&player); // Passing the address of player to attack method

		}

		round++;
	}

	// Displaying the result of the duel
	std::cout << "/n====================================" << std::endl;
	if (player.isAlive()) {
		std::cout << "YOU WON! " << enemy->getName() << " was defeated, Middle-earth is safe from the darkness" << std::endl;
	}
	else {
		std::cout << "YOU LOST! " << player.getName() << " fell, and Middle-earth is in danger " << std::endl;
	}
	std::cout << "======================================" << std::endl;

	// This is where main() ends. The unique_ptr 'enemy' detects the end of scope
	// and automatically deallocates the memory. No memory leaks!

	return 0;
}


