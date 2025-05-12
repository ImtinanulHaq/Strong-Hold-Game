#ifndef GAME_H
#define GAME_H

#include <iostream>  
#include <fstream>   
#include <string>   
#include <windows.h>
#include <ctime>

using namespace std;

// Forward declarations
class Economy;
class Population;
class Kingdom;

// Function declarations
void handleRandomEvent(Kingdom& kingdom);
void updateGameState(Kingdom& kingdom);

// Visual utility functions
namespace Visual {
    // Clear screen function
    inline void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    // Print a horizontal line
    inline void printLine(char ch = '=', int length = 50) {
        cout << string(length, ch) << endl;
    }

    // Print a message
    inline void printMessage(const string& message) {
        cout << message << endl;
    }

    // Print a success message
    inline void printSuccess(const string& message) {
        printMessage("SUCCESS: " + message);
    }

    // Print an error message
    inline void printError(const string& message) {
        printMessage("ERROR: " + message);
    }

    // Print a warning message
    inline void printWarning(const string& message) {
        printMessage("WARNING: " + message);
    }

    // Print an info message
    inline void printInfo(const string& message) {
        printMessage("INFO: " + message);
    }

    // Print a menu item
    inline void printMenuItem(int number, const string& text) {
        cout << number << ". " << text << endl;
    }

    // Print a section header
    inline void printSection(const string& title) {
        cout << endl;
        printLine('-', 50);
        cout << title << endl;
        printLine('-', 50);
    }
}

// Enhanced Resource template class
template <typename T>
class Resource {
    T quantity;
    T maxQuantity;  // Maximum capacity
    bool isDepleted;  // Resource depletion status

public:
    Resource() : quantity(0), maxQuantity(1000), isDepleted(false) {}

    void setQuantity(T q) {
        if (q < 0) {
            Visual::printError("Error: Cannot set negative quantity");
            return;
        }
        quantity = q;
        if (quantity >= maxQuantity) {
            quantity = maxQuantity;
            isDepleted = true;
        }
    }

    T getQuantity() const { return quantity; }
    T getMaxQuantity() const { return maxQuantity; }
    bool getIsDepleted() const { return isDepleted; }
    void setMaxQuantity(T max) { maxQuantity = max; }
};

// Enhanced Leader class with more features
class Leader {
protected:
    string name;
    int skill;
    int popularity;  // Leader's popularity
    bool isCorrupt;  // Corruption status
    int health;      // Leader's health

public:
    Leader(string n, int s) : name(n), skill(s), popularity(50), isCorrupt(false), health(100) {}

    virtual void applyPolicy() = 0;
    virtual ~Leader() {}

    string getName() const { return name; }
    int getSkill() const { return skill; }
    int getPopularity() const { return popularity; }
    bool getIsCorrupt() const { return isCorrupt; }
    int getHealth() const { return health; }

    void setPopularity(int p) { popularity = p; }
    void setCorruption(bool c) { isCorrupt = c; }
    void setHealth(int h) { health = h; }
};

// Enhanced King class
class King : public Leader {
    int reignLength;  // Years in power
    int assassinationAttempts;  // Number of assassination attempts

public:
    King(string n, int s) : Leader(n, s), reignLength(0), assassinationAttempts(0) {}

    void applyPolicy() override;
    void faceAssassinationAttempt();
    int getReignLength() const { return reignLength; }
    void incrementReign() { reignLength++; }
};

// Enhanced Population class
class Population {
    int totalPeople;
    int peasants;
    int merchants;
    int nobility;
    int military;
    int birthRate;      // Population growth rate
    int deathRate;      // Population death rate
    bool isPlague;      // Plague status
    int foodSupply;     // Food supply level

public:
    Population();
    ~Population();

    void updatePeople(int change);
    void checkSocialClasses();
    void handlePlague();  // Handle plague outbreaks
    void updateFoodSupply(int amount);  // Update food supply
    void calculateGrowth();  // Calculate population growth
    void decreasePopulation(int amount); // Decrease population by specific amount
    void increasePopulation(int amount); // Increase population by specific amount

    int getTotalPeople() const { return totalPeople; }
    int getPeasants() const { return peasants; }
    int getMerchants() const { return merchants; }
    int getNobility() const { return nobility; }
    int getMilitary() const { return military; }
    
    int getBirthRate() const { return birthRate; }
    int getDeathRate() const { return deathRate; }
    bool getIsPlague() const { return isPlague; }
    int getFoodSupply() const { return foodSupply; }
};

// Enhanced Army class
class Army {
    int size;
    int morale;
    bool isPaid;
    int trainingLevel;  // Army training level
    int equipment;      // Equipment quality
    int casualties;     // Battle casualties
    bool isRebelling;   // Rebellion status

public:
    Army(int s);
    ~Army();

    void train(int cycles);
    void paySoldiers(Economy& economy);
    void updateEquipment(int quality);  // Update equipment
    void decreaseSize(int amount);      // Decrease army size
    void increaseSize(int amount);      // Increase army size

    int getSize() const { return size; }
    int getMorale() const { return morale; }
    bool getIsPaid() const { return isPaid; }
    
    int getTrainingLevel() const { return trainingLevel; }
    int getEquipment() const { return equipment; }
    int getCasualties() const { return casualties; }
    bool getIsRebelling() const { return isRebelling; }
};

// Enhanced Economy class
class Economy {
    Resource<double>* gold;
    double taxRate;
    double inflation;  // Inflation rate
    bool isRecession;  // Recession status
    int publicServices;  // Public services funding

public:
    Economy();
    ~Economy();

    void collectTaxes(const Population& pop);
    void spendGold(double amount);
    void fundPublicServices(int amount);  // Fund public services
    void decreaseGold(double amount);     // Decrease gold by specific amount
    void increaseGold(double amount);     // Increase gold by specific amount

    double getGold() const { return gold->getQuantity(); }
    void setTaxRate(double rate);
    
    double getInflation() const { return inflation; }
    bool getIsRecession() const { return isRecession; }
    int getPublicServices() const { return publicServices; }
};

// Enhanced Bank class
class Bank {
    double loanAmount;
    double interestRate;
    bool isCorrupt;  // Bank corruption status
    int securityLevel;  // Bank security level
    int auditCost;  // Cost of auditing

public:
    Bank();
    ~Bank();

    void takeLoan(double amount, Economy& economy);
    void repayLoan(double amount, Economy& economy);

    double getLoanAmount() const { return loanAmount; }
    
    bool getIsCorrupt() const { return isCorrupt; }
    int getSecurityLevel() const { return securityLevel; }
    int getAuditCost() const { return auditCost; }
};

class Weather {
private:
    string currentCondition;
    int duration;
    double foodProductionMultiplier;
    bool isHarsh;

public:
    Weather() : currentCondition("Normal"), duration(0), foodProductionMultiplier(1.0), isHarsh(false) {}
    
    void updateWeather();
    string getCurrentCondition() const { return currentCondition; }
    double getFoodProductionMultiplier() const { return foodProductionMultiplier; }
    bool isHarshWeather() const { return isHarsh; }
    int getDuration() const { return duration; }
};

class TradeRoute {
private:
    bool isSecure;
    double riskLevel;
    int attackProbability;

public:
    TradeRoute() : isSecure(true), riskLevel(0.0), attackProbability(0) {}
    
    void updateSecurity();
    bool getIsSecure() const { return isSecure; }
    double getRiskLevel() const { return riskLevel; }
    int getAttackProbability() const { return attackProbability; }
    void setRiskLevel(double level) { riskLevel = level; }
};

class Market {
private:
    static const int MAX_RESOURCES = 5;
    string resourceNames[MAX_RESOURCES] = {"wood", "stone", "iron", "food", "weapons"};
    int resourceQuantities[MAX_RESOURCES];
    double priceMultiplier;
    TradeRoute tradeRoute;
    int foodStockpile;
    int weaponsStockpile;
    double foodConsumptionRate;

    // Helper function declaration
    int findResourceIndex(const string& resource) const;

public:
    // Constructor and destructor
    Market();
    ~Market();

    // Resource management
    void tradeResource(string resource, int amount, Economy& economy);
    int getResource(string resource) const;
    void decreaseResource(string resource, int amount);
    void updateFoodStockpile(int population, const Weather& weather);
    void consumeFood(int population);
    bool checkFoodShortage() const;
    int getFoodStockpile() const;
    void updateWeaponsStockpile(int armySize);
    int getWeaponsStockpile() const;
    TradeRoute& getTradeRoute();
    
    // Price management
    double getPriceMultiplier() const;
};

// Enhanced Politics class
class Politics {
    King* currentKing;
    int electionTimer;
    int stability;  // Kingdom stability
    bool isCoup;  // Coup status
    int corruptionLevel;  // Political corruption

public:
    Politics();
    ~Politics();

    void holdElection();
    void decreaseStability(int amount);  // Decrease kingdom stability
    string getKingName() const;
    
    int getStability() const { return stability; }
    bool getIsCoup() const { return isCoup; }
    int getCorruptionLevel() const { return corruptionLevel; }
};

// Enhanced Diplomacy class
class Diplomacy {
    string treaty;
    int relations;  // Diplomatic relations
    bool isAlliance;  // Alliance status
    int tradeSanctions;  // Trade sanctions

public:
    Diplomacy();
    ~Diplomacy();

    void makeTreaty(string t);
    void breakTreaty();

    string getTreaty() const { return treaty; }
    
    int getRelations() const { return relations; }
    bool getIsAlliance() const { return isAlliance; }
    int getTradeSanctions() const { return tradeSanctions; }
};

// Enhanced Communication class
class Communication {
    string messages[5];  // Store last 5 messages
    int messageCount;    // Current number of messages

public:
    Communication();
    ~Communication();

    void sendMessage(const string& msg);
    void showMessages() const;
    int getMessageCount() const { return messageCount; }
    string getMessage(int index) const;
};

// Enhanced Kingdom class
class Kingdom {
private:
    string name;
    Population* people;
    Economy* economy;
    Army* army;
    Bank* bank;
    Market* market;
    Politics* politics;
    Diplomacy* diplomacy;
    Communication* communication;
    bool isGameOver;
    int turn;

public:
    Weather weather;  // Make weather public
    int turnsSinceLastWeatherUpdate;  // Make turnsSinceLastWeatherUpdate public

    Kingdom(string n);
    ~Kingdom();

    void saveGame() const;
    void loadGame();
    void updateWeather();  // Weather update method

    string getName() const { return name; }
    Population& getPeople() { return *people; }
    Economy& getEconomy() { return *economy; }
    Army& getArmy() { return *army; }
    Bank& getBank() { return *bank; }
    Market& getMarket() { return *market; }
    Politics& getPolitics() { return *politics; }
    Diplomacy& getDiplomacy() { return *diplomacy; }
    Communication& getCommunication() { return *communication; }
    
    int getTurn() const { return turn; }
    bool getIsGameOver() const { return isGameOver; }
    void incrementTurnsSinceLastWeatherUpdate() { turnsSinceLastWeatherUpdate++; }
    void resetTurnsSinceLastWeatherUpdate() { turnsSinceLastWeatherUpdate = 0; }
};

#endif