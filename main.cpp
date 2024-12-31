#include <iostream>
#include <string>
#include <vector>
#include <conio.h>

// Base class
class Character
{
protected:
    std::string name;
    int health;
    int attackPower;

public:
    Character(std::string name, int health, int attackPower)
        : name(name), health(health), attackPower(attackPower) {}

    virtual void displayStats() {}

    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;
    }

    bool isAlive() const
    {
        return health > 0;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttackPower() const { return attackPower; }
    void setHealth(int health)
    {
        this->health = health;
    }
};

class Item
{
protected:
    std::string name;
    int effectValue;

public:
    Item(std::string name, int effectValue) : name(name), effectValue(effectValue) {}

    virtual void onUse(Character &character) = 0;
    std::string getName() const { return name; }
    virtual ~Item() = default;
};

// HealItem derived class
class HealItem : public Item
{
public:
    HealItem(std::string name, int effectValue) : Item(name, effectValue) {}

    void onUse(Character &character) override
    {
        int health = character.getHealth();
        character.setHealth(health + effectValue);
        std::cout << character.getName() << " used " << name << " and healed for " << effectValue << " points!\n";
    }
};

// Player class
class Player : public Character
{
    std::vector<Item *> inventory;
    int level;
    int gold;

public:
    Player(std::string name, int health, int attackPower)
        : Character(name, health, attackPower), level(1) {}

    void levelUp()
    {
        level++;
        health += 30;
        attackPower += 10;
    }

    void addItem(Item *item)
    {
        inventory.push_back(item);
    }
    void useItem(const std::string &itemName)
    {
        bool itemFound = false;

        try
        {
            for (auto it = inventory.begin(); it != inventory.end(); ++it)
            {
                if ((*it)->getName() == itemName)
                {
                    (*it)->onUse(*this);
                    inventory.erase(it);
                    itemFound = true;
                    return;
                }
            }

            if (!itemFound)
            {
                std::cerr << "Item not found: " << itemName << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void showInventory()
    {
        if (inventory.empty())
        {
            std::cout << "Inventory is empty.\n";
            return;
        }
        std::cout << "Inventory:\n";
        for (size_t i = 0; i < inventory.size(); i++)
        {
            std::cout << i + 1 << ". " << inventory[i]->getName() << "\n";
        }
    }

    void addGold(int amount)
    {
        gold += amount;
    }
    int getGold()
    {
        return gold;
    }

    int getLevel() const { return level; }

    void displayStats() const
    {
        std::cout << "Player: " << name << ", Level: " << level << "\n";
        std::cout << "Health: " << health << ", Attack Power: " << attackPower << "\n";
    }
};

// Enemy Class
class Enemy : public Character
{

public:
    void displayStats() const
    {
        std::cout << "Player: " << name << ", Type: " << EnemyType << "\n";
        std::cout << "Health: " << health << ", Attack Power: " << attackPower << "\n";
    };
    std::string EnemyType;
    Enemy(std::string name, int health, int attackPower, std::string enemyType) : Character(name, health, attackPower) {}
    void heal()
    {
        if (health + 10 < 100)
        {
            health += 30;
        }
    }
};

// Shop Class

class Shop
{

    std::vector<Item *> items;

public:
    Shop()
    {
        items.push_back(new HealItem("Healing Potion", 20));
        items.push_back(new HealItem("Large Healing Potion", 50));
    }

    void displayItems()
    {
        std::cout << "Welcome to the Shop!\n";
        for (size_t i = 0; i < items.size(); ++i)
        {
            std::cout << i + 1 << ". " << items[i]->getName() << " - " << (i + 1) * 10 << " gold\n";
        }
    }
    void buyItem(Player &player, int index)
    {
        int cost = (index + 1) * 10; // Example cost logic
        if (player.getGold() >= cost)
        {
            player.addItem(items[index]);
            player.addGold(-cost);
            std::cout << "You purchased " << items[index]->getName() << "!\n";
        }
        else
        {
            std::cout << "Not enough gold to purchase this item!\n";
        }
    }
};

void battle(Player &player, Enemy enemy)
{

    bool playerTurn = true;
    while (player.isAlive() && enemy.isAlive())
    {

        if (playerTurn)
        {
            int option;
            std::cout << "1. Attack\n";
            std::cout << "2. Show Availible items\n";
            std::cout << "Enter option: ";
            std::cin >> option;
            switch (option)
            {
            case 1:
                std::cout << player.getName() << " attacks " << enemy.getName() << " for " << player.getAttackPower() << " damage.\n";
                enemy.takeDamage(player.getAttackPower());
                if (!enemy.isAlive())
                {
                    std::cout << player.getName() << " defeated " << enemy.getName() << "!\n";
                    player.levelUp();
                }
                break;
            case 2:

                player.showInventory();
            }

            if (option != 2)
            {
                playerTurn = !playerTurn;
            }
        }
        else
        {
            int enemyOption = rand() % 2;
            switch (enemyOption)
            {
            case 0:
                std::cout << enemy.getName() << " attacks " << player.getName() << " for " << enemy.getAttackPower() << " damage.\n";
                player.takeDamage(enemy.getAttackPower());
                if (!player.isAlive())
                {
                    std::cout << enemy.getName() << " defeated " << player.getName() << "!\n";
                }
                break;
            case 1:
                std::cout << enemy.getName() << " heals himself.\n";
                enemy.heal();
            }
            playerTurn = !playerTurn;
        }
    }
}

void gameMenu()
{
    Shop shop;
    Player player("Hero", 100, 30);
    while (true)
    {
        std::cout << "1. Shop\n";
        std::cout << "2. Battle\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter option: ";
        int option;
        std::cin >> option;
        switch (option)
        {
        case 1:
            shop.displayItems();
            std::cout << "Enter item number: ";
            int itemNumber;
            std::cin >> itemNumber;
            shop.buyItem(player, itemNumber - 1);
            break;
        case 2:
            battle(player, Enemy("Goblin", 50, 30, "Goblin"));
            break;
        case 3:
            std::cout << "Goodbye!\n";
            return;
        }
    }
};

int main()
{
    gameMenu();
    return 0;
};
