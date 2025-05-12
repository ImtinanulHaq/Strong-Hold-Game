#include "game.h"

// First: Implement King class methods

void King::applyPolicy()
{
    Visual::printInfo(name + " applies a new policy!");

    popularity += 5;

    if (popularity > 100)
        popularity = 100;
}

void King::faceAssassinationAttempt()
{
    assassinationAttempts++;
    Visual::printError("Assassination attempt on " + name + "!");
    health -= 20;
    if (health <= 0)
    {
        Visual::printError(name + " has been assassinated!");
        Visual::printError("Game Over: King has been assassinated!");
        exit(1);  // Exit the game instead of throwing
    }
}

// Second: Implement Population class methods
Population::Population()
{
    totalPeople = 1000;  // Start with 1000 people
    peasants = 600;      // 60% peasants
    merchants = 200;     // 20% merchants
    nobility = 100;      // 10% nobility
    military = 100;      // 10% military
    birthRate = 5;
    deathRate = 2;
    isPlague = false;
    foodSupply = 1000;
    Visual::printSuccess("Population initialized with " + std::to_string(totalPeople) + " people.");
}

Population::~Population()
{
    Visual::printInfo("Population cleanup done.");
}

void Population::updatePeople(int change)
{
    totalPeople += change;
    if (totalPeople < 0)
    {
        totalPeople = 0;
        Visual::printError("Warning: Population dropped to zero!");
    }
    calculateGrowth();
}

void Population::checkSocialClasses()
{
    if (peasants > totalPeople * 0.7)
    {
        Visual::printWarning("Peasants are unhappy! Risk of riots!");
    }
    if (nobility < totalPeople * 0.05)
    {
        Visual::printWarning("Nobility too small! Political unrest possible!");
    }
    Visual::printInfo("Social classes: " + std::to_string(peasants) + " peasants, " +
        std::to_string(merchants) + " merchants, " + std::to_string(nobility) + " nobles, " +
        std::to_string(military) + " military.");
}

void Population::handlePlague()
{
    if (isPlague)
    {
        int deaths = totalPeople * 0.1;  // 10% death rate
        updatePeople(-deaths);
        Visual::printError("Plague has killed " + std::to_string(deaths) + " people!");
    }
}

void Population::updateFoodSupply(int amount)
{
    foodSupply += amount;
    if (foodSupply < 0)
    {
        foodSupply = 0;
        Visual::printError("Food supply depleted! People are starving!");
        updatePeople(-totalPeople * 0.05);  // 5% population dies
    }
}

void Population::calculateGrowth()
{
    int growth = (birthRate - deathRate) * totalPeople / 100;
    if (foodSupply > totalPeople)
    {
        updatePeople(growth);
    }
    else
    {
        Visual::printWarning("Food shortage preventing population growth!");
    }
}

void Population::decreasePopulation(int amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot decrease population by negative amount!");
        return;
    }

    totalPeople -= amount;
    if (totalPeople < 0)
    {
        totalPeople = 0;
        Visual::printError("Warning: Population dropped to zero!");
    }

    // Adjust social classes proportionally
    double ratio = (double)totalPeople / (totalPeople + amount);
    peasants = (int)(peasants * ratio);
    merchants = (int)(merchants * ratio);
    nobility = (int)(nobility * ratio);
    military = (int)(military * ratio);

    Visual::printWarning("Population decreased by " + std::to_string(amount) + 
        ". New total: " + std::to_string(totalPeople));
}

void Population::increasePopulation(int amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot increase population by negative amount!");
        return;
    }

    totalPeople += amount;
    
    // Adjust social classes proportionally
    double ratio = (double)totalPeople / (totalPeople - amount);
    peasants = (int)(peasants * ratio);
    merchants = (int)(merchants * ratio);
    nobility = (int)(nobility * ratio);
    military = (int)(military * ratio);

    Visual::printSuccess("Population increased by " + std::to_string(amount) + 
        ". New total: " + std::to_string(totalPeople));
}

// Third: Implement Economy class methods
Economy::Economy()
{
    gold = new Resource<double>();  // Create gold resource
    gold->setQuantity(500.0);      // Start with 500 gold
    taxRate = 0.1;                 // 10% tax rate
    inflation = 0.0;
    isRecession = false;
    publicServices = 50;
    Visual::printSuccess("Economy initialized with " + std::to_string(gold->getQuantity()) + " gold.");
}

Economy::~Economy()
{
    delete gold;  // Free gold memory
    Visual::printInfo("Economy cleanup done.");
}

void Economy::collectTaxes(const Population& pop)
{
    double taxes = pop.getTotalPeople() * taxRate;  // Taxes based on population
    gold->setQuantity(gold->getQuantity() + taxes);
    Visual::printSuccess("Collected " + std::to_string(taxes) + " gold in taxes. Total gold: " +
        std::to_string(gold->getQuantity()) + ".");
}

void Economy::spendGold(double amount)
{
    double newGold = gold->getQuantity() - amount;
    if (newGold < 0)
    {
        Visual::printError("Error: Cannot spend gold! Insufficient funds");
        return;
    }
    gold->setQuantity(newGold);
    Visual::printInfo("Spent " + std::to_string(amount) + " gold. Remaining: " +
        std::to_string(gold->getQuantity()) + ".");
}

void Economy::fundPublicServices(int amount)
{
    double newGold = gold->getQuantity() - amount;
    if (newGold < 0)
    {
        Visual::printError("Failed to fund public services: Insufficient funds");
        return;
    }
    gold->setQuantity(newGold);
    publicServices += amount / 10;
    if (publicServices > 100) publicServices = 100;
    Visual::printSuccess("Public services funding increased to " + std::to_string(publicServices));
}

void Economy::decreaseGold(double amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot decrease gold by negative amount!");
        return;
    }

    double newGold = gold->getQuantity() - amount;
    if (newGold < 0)
    {
        Visual::printError("Warning: Gold dropped below zero!");
        newGold = 0;
    }
    gold->setQuantity(newGold);
    Visual::printWarning("Gold decreased by " + std::to_string(amount) + 
        ". New total: " + std::to_string(gold->getQuantity()));
}

void Economy::increaseGold(double amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot increase gold by negative amount!");
        return;
    }

    double newGold = gold->getQuantity() + amount;
    gold->setQuantity(newGold);
    Visual::printSuccess("Gold increased by " + std::to_string(amount) + 
        ". New total: " + std::to_string(gold->getQuantity()));
}

// Fourth: Implement Army class methods
Army::Army(int s)
{
    size = s;        // Start with given soldiers
    morale = 100;    // Full morale
    isPaid = true;   // Soldiers start paid
    trainingLevel = 1;
    equipment = 50;
    casualties = 0;
    isRebelling = false;
    Visual::printSuccess("Army initialized with " + std::to_string(size) + " soldiers.");
}

Army::~Army()
{
    Visual::printInfo("Army cleanup done.");
}

void Army::train(int cycles)
{
    if (cycles < 1 || cycles > 5)
    {
        Visual::printError("Invalid training cycles!");
        return;
    }

    Visual::printInfo("Training army for " + std::to_string(cycles) + " cycles...");
    for (int i = 0; i < cycles; i++)
    {
        Sleep(1000);  // 1 second delay
        trainingLevel++;
        morale -= 5;
        if (morale < 0)
        {
            morale = 0;
            Visual::printWarning("Warning: Army morale dropped to zero!");
        }
    }
    size += 50;
    Visual::printSuccess("Training done. Army size: " + std::to_string(size) + 
        ", Training Level: " + std::to_string(trainingLevel) + 
        ", Morale: " + std::to_string(morale) + ".");
}

void Army::paySoldiers(Economy& economy)
{
    double cost = size * 0.1;
    try
    {
        economy.spendGold(cost);
        isPaid = true;
        morale += 10;
        if (morale > 100) morale = 100;
        Visual::printSuccess("Paid " + std::to_string(cost) + " gold to soldiers. Morale: " + 
            std::to_string(morale) + ".");
    }
    catch (const exception& e)
    {
        Visual::printError("Failed to pay soldiers: " + std::string(e.what()));
    }
}

void Army::updateEquipment(int quality)
{
    if (quality < 1 || quality > 10)
    {
        Visual::printError("Invalid equipment quality!");
        return;
    }
    equipment = quality * 10;
    Visual::printSuccess("Army equipment updated to level " + std::to_string(quality) + 
        " (Quality: " + std::to_string(equipment) + ")");
}

void Army::decreaseSize(int amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot decrease army size by negative amount!");
        return;
    }

    size -= amount;
    if (size < 0)
    {
        size = 0;
        Visual::printError("Warning: Army size dropped to zero!");
    }

    // Decrease morale when army size decreases
    morale -= amount / 10;
    if (morale < 0) morale = 0;

    Visual::printWarning("Army size decreased by " + std::to_string(amount) + 
        ". New size: " + std::to_string(size) + 
        ", Morale: " + std::to_string(morale));
}

void Army::increaseSize(int amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot increase army size by negative amount!");
        return;
    }

    size += amount;
    
    // Increase morale when army size increases
    morale += amount / 20;
    if (morale > 100) morale = 100;

    Visual::printSuccess("Army size increased by " + std::to_string(amount) + 
        ". New size: " + std::to_string(size) + 
        ", Morale: " + std::to_string(morale));
}

// Fifth: Implement Bank class methods
Bank::Bank()
{
    loanAmount = 0;
    interestRate = 0.1;  // 10% interest
    isCorrupt = false;
    securityLevel = 1;
    auditCost = 100;
    Visual::printSuccess("Bank initialized.");
}

Bank::~Bank()
{
    Visual::printInfo("Bank cleanup done.");
}

void Bank::takeLoan(double amount, Economy& economy)
{
    if (amount <= 0)
    {
        Visual::printError("Loan amount must be positive!");
        return;
    }
    loanAmount += amount;
    economy.spendGold(-amount);  // Add money to economy
    Visual::printSuccess("Loan of " + std::to_string(amount) + " gold taken. Total debt: " + 
        std::to_string(loanAmount) + " gold.");
}

void Bank::repayLoan(double amount, Economy& economy)
{
    if (amount <= 0)
    {
        Visual::printError("Repayment amount must be positive!");
        return;
    }
    if (amount > loanAmount)
    {
        amount = loanAmount;
    }
    economy.spendGold(amount);
    loanAmount -= amount;
    Visual::printSuccess("Repaid " + std::to_string(amount) + " gold. Remaining debt: " + 
        std::to_string(loanAmount) + " gold.");
}

// Sixth: Implement Market class methods
Market::Market() : priceMultiplier(1.0), foodStockpile(100), weaponsStockpile(50), foodConsumptionRate(1.0) {
    // Initialize all resources to 100
    for (int i = 0; i < MAX_RESOURCES; i++) {
        resourceQuantities[i] = 100;
    }
    Visual::printSuccess("Market initialized with basic resources.");
}

Market::~Market() {
    Visual::printInfo("Market cleanup done.");
}

int Market::findResourceIndex(const string& resource) const {
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (resourceNames[i] == resource) {
            return i;
        }
    }
    return -1; // Resource not found
}

void Market::tradeResource(string resource, int amount, Economy& economy) {
    if (!tradeRoute.getIsSecure()) {
        Visual::printWarning("Warning: Trade route is not secure!");
        if (rand() % 100 < tradeRoute.getAttackProbability()) {
            Visual::printError("Trade caravan was attacked!");
            return;
        }
    }

    int resourceIndex = findResourceIndex(resource);
    if (resourceIndex == -1) {
        Visual::printError("Invalid resource type!");
        return;
    }

    double basePrice = 10.0;
    if (resource == "wood") basePrice = 5.0;
    else if (resource == "stone") basePrice = 8.0;
    else if (resource == "iron") basePrice = 15.0;
    else if (resource == "food") basePrice = 12.0;
    else if (resource == "weapons") basePrice = 25.0;

    double totalCost = basePrice * amount * priceMultiplier;
    
    if (amount > 0) { // Buying
        if (economy.getGold() < totalCost) {
            Visual::printError("Not enough gold!");
            return;
        }
        economy.decreaseGold(totalCost);
        resourceQuantities[resourceIndex] += amount;
        Visual::printSuccess("Successfully bought " + to_string(amount) + " " + resource);
    }
    else { // Selling
        if (resourceQuantities[resourceIndex] < -amount) {
            Visual::printError("Not enough " + resource + "!");
            return;
        }
        economy.increaseGold(totalCost);
        resourceQuantities[resourceIndex] += amount;
        Visual::printSuccess("Successfully sold " + to_string(-amount) + " " + resource);
    }
}

int Market::getResource(string resource) const {
    int resourceIndex = findResourceIndex(resource);
    if (resourceIndex == -1) {
        return 0;
    }
    return resourceQuantities[resourceIndex];
}

void Market::decreaseResource(string resource, int amount) {
    if (amount < 0) {
        Visual::printError("Cannot decrease resource by negative amount!");
        return;
    }

    int resourceIndex = findResourceIndex(resource);
    if (resourceIndex == -1) {
        Visual::printError("Invalid resource type!");
        return;
    }

    int newQuantity = resourceQuantities[resourceIndex] - amount;
    if (newQuantity < 0) {
        Visual::printError("Warning: " + resource + " dropped below zero!");
        newQuantity = 0;
    }
    resourceQuantities[resourceIndex] = newQuantity;
    
    Visual::printWarning(resource + " decreased by " + to_string(amount) + 
        ". New quantity: " + to_string(newQuantity));
}

void Market::updateFoodStockpile(int population, const Weather& weather) {
    // Calculate food production (base amount)
    int foodProduction = 50;
    
    // Apply weather effects to food production
    foodProduction = static_cast<int>(foodProduction * weather.getFoodProductionMultiplier());
    
    // Update food stockpile
    foodStockpile += foodProduction;
    
    // Consume food based on population
    consumeFood(population);
    
    // Check for food shortage
    if (checkFoodShortage()) {
        Visual::printWarning("Food shortage detected! Population may be affected.");
    }
}

void Market::consumeFood(int population) {
    int foodNeeded = population * foodConsumptionRate;
    if (foodStockpile >= foodNeeded) {
        foodStockpile -= foodNeeded;
    }
    else {
        foodStockpile = 0;
        Visual::printError("Food shortage! Population is starving!");
    }
}

bool Market::checkFoodShortage() const {
    return foodStockpile < 50; // Consider it a shortage if below 50 units
}

void Market::updateWeaponsStockpile(int armySize) {
    // Calculate weapons needed based on army size
    int weaponsNeeded = armySize * 2; // Each soldier needs 2 weapons
    
    if (weaponsStockpile < weaponsNeeded) {
        Visual::printWarning("Weapons shortage! Army effectiveness may be reduced.");
    }
}

int Market::getFoodStockpile() const {
    return foodStockpile;
}

int Market::getWeaponsStockpile() const {
    return weaponsStockpile;
}

TradeRoute& Market::getTradeRoute() {
    return tradeRoute;
}

double Market::getPriceMultiplier() const {
    return priceMultiplier;
}

// Seventh: Implement Politics class methods
Politics::Politics()
{
    currentKing = new King("No King", 50);
    electionTimer = 0;
    stability = 50;
    isCoup = false;
    corruptionLevel = 0;
    Visual::printSuccess("Politics initialized.");
}

Politics::~Politics()
{
    delete currentKing;
    Visual::printInfo("Politics cleanup done.");
}

void Politics::holdElection()
{
    if (electionTimer > 0)
    {
        Visual::printError("Cannot hold election yet! Wait " + std::to_string(electionTimer) + " more turns.");
        return;
    }
    
    delete currentKing;
    currentKing = new King("New King", rand() % 50 + 50);  // Random skill 50-100
    electionTimer = 10;  // 10 turns until next election
    stability += 10;
    if (stability > 100) stability = 100;
    
    Visual::printSuccess("New king elected: " + currentKing->getName() + 
        " (Skill: " + std::to_string(currentKing->getSkill()) + ")");
}

void Politics::decreaseStability(int amount)
{
    if (amount < 0)
    {
        Visual::printError("Cannot decrease stability by negative amount!");
        return;
    }

    stability -= amount;
    if (stability < 0)
    {
        stability = 0;
        Visual::printError("Warning: Kingdom stability dropped to zero!");
        isCoup = true;  // Trigger coup if stability reaches zero
    }
    else if (stability < 20)
    {
        Visual::printWarning("Warning: Kingdom stability critically low! Risk of rebellion!");
    }

    Visual::printWarning("Kingdom stability decreased by " + std::to_string(amount) + 
        ". New stability: " + std::to_string(stability));
}

string Politics::getKingName() const
{
    return currentKing->getName();
}

// Eighth: Implement Diplomacy class methods
Diplomacy::Diplomacy()
{
    treaty = "No active treaty";
    relations = 50;
    isAlliance = false;
    tradeSanctions = 0;
    Visual::printSuccess("Diplomacy initialized.");
}

Diplomacy::~Diplomacy()
{
    Visual::printInfo("Diplomacy cleanup done.");
}

void Diplomacy::makeTreaty(string t)
{
    treaty = t;
    relations += 10;
    if (relations > 100) relations = 100;
    isAlliance = true;
    Visual::printSuccess("Treaty established: " + treaty);
}

void Diplomacy::breakTreaty()
{
    if (treaty == "No active treaty")
    {
        Visual::printError("No active treaty to break!");
        return;
    }
    treaty = "No active treaty";
    relations -= 20;
    if (relations < 0) relations = 0;
    isAlliance = false;
    Visual::printWarning("Treaty broken! Relations decreased.");
}

// Ninth: Implement Communication class methods
Communication::Communication()
{
    messageCount = 0;  // Start with no messages
    for (int i = 0; i < 5; i++)
    {
        messages[i] = "";  // Clear message array
    }
    cout << "Communication initialized with no messages." << endl;
}

Communication::~Communication()
{
    // No dynamic memory to free
    cout << "Communication cleanup done." << endl;
}

void Communication::sendMessage(const string& msg)
{
    if (messageCount < 5)
    {
        messages[messageCount] = msg;
        messageCount++;
        cout << "Sent message: " << msg << "." << endl;
    }
    else
    {
        cout << "Error: Message limit reached!" << endl;
    }
}

void Communication::showMessages() const
{
    if (messageCount == 0)
    {
        cout << "No messages." << endl;
        return;
    }
    cout << "Messages:" << endl;
    for (int i = 0; i < messageCount; i++)
    {
        cout << "- " << messages[i] << endl;
    }
}

string Communication::getMessage(int index) const
{
    if (index >= 0 && index < messageCount)
    {
        return messages[index];
    }
    return "Invalid message index";
}

// Update Kingdom constructor
Kingdom::Kingdom(string n)
{
    name = n;
    people = new Population();
    economy = new Economy();
    army = new Army(100);
    bank = new Bank();
    market = new Market();
    politics = new Politics();
    diplomacy = new Diplomacy();
    communication = new Communication();
    turn = 1;
    isGameOver = false;
    turnsSinceLastWeatherUpdate = 0;
    Visual::printSuccess("Kingdom " + name + " initialized!");
}

// Update Kingdom destructor
Kingdom::~Kingdom()
{
    delete people;
    delete economy;
    delete army;
    delete bank;
    delete market;
    delete politics;
    delete diplomacy;
    delete communication;  // Clean up communication
    Visual::printInfo("Kingdom cleanup done.");
}

void Kingdom::saveGame() const
{
    ofstream file("score.txt");
    if (!file.is_open())
    {
        Visual::printError("Error: Cannot open score.txt for saving!");
        return;
    }
    file << name << endl;
    file << people->getTotalPeople() << endl;
    file << economy->getGold() << endl;
    file << army->getSize() << endl;
    file << army->getMorale() << endl;
    file << bank->getLoanAmount() << endl;
    file << communication->getMessageCount() << endl;
    for (int i = 0; i < communication->getMessageCount(); i++)
    {
        file << communication->getMessage(i) << endl;
    }
    file.close();
    Visual::printSuccess("Game saved to score.txt.");
}

void Kingdom::loadGame()
{
    ifstream file("score.txt");
    if (!file.is_open())
    {
        Visual::printError("Error: Cannot open score.txt for loading!");
        return;
    }
    getline(file, name);
    int pop;
    double gold;
    int armySize, armyMorale;
    double loan;
    file >> pop >> gold >> armySize >> armyMorale >> loan;
    people->updatePeople(pop - people->getTotalPeople());
    economy->spendGold(gold - economy->getGold());
    delete army;
    army = new Army(armySize);
    army->train(0);
    army->paySoldiers(*economy);
    bank->takeLoan(loan - bank->getLoanAmount(), *economy);
    
    int msgCount;
    file >> msgCount;
    string msg;
    getline(file, msg);
    for (int i = 0; i < msgCount; i++)
    {
        getline(file, msg);
        communication->sendMessage(msg);
    }
    
    file.close();
    Visual::printSuccess("Game loaded from score.txt.");
}

// Add Kingdom weather update method
void Kingdom::updateWeather() {
    weather.updateWeather();
    if (weather.isHarshWeather()) {
        Visual::printWarning("Weather Alert: " + weather.getCurrentCondition());
        Visual::printInfo("Food production will be affected for " + 
                        to_string(weather.getDuration()) + " turns.");
    }
}

// Weather class implementation
void Weather::updateWeather() {
    srand(time(0));
    int weatherChange = rand() % 100;
    
    if (weatherChange < 60) { // 60% chance of normal weather
        currentCondition = "Normal";
        foodProductionMultiplier = 1.0;
        isHarsh = false;
        duration = 0;
    }
    else if (weatherChange < 75) { // 15% chance of drought
        currentCondition = "Drought";
        foodProductionMultiplier = 0.5;
        isHarsh = true;
        duration = rand() % 3 + 1;
    }
    else if (weatherChange < 90) { // 15% chance of harsh winter
        currentCondition = "Harsh Winter";
        foodProductionMultiplier = 0.3;
        isHarsh = true;
        duration = rand() % 2 + 1;
    }
    else { // 10% chance of good weather
        currentCondition = "Good Weather";
        foodProductionMultiplier = 1.5;
        isHarsh = false;
        duration = rand() % 2 + 1;
    }
}

// TradeRoute class implementation
void TradeRoute::updateSecurity() {
    srand(time(0));
    int securityCheck = rand() % 100;
    
    if (securityCheck < 70) { // 70% chance of secure route
        isSecure = true;
        riskLevel = 0.0;
        attackProbability = 0;
    }
    else if (securityCheck < 85) { // 15% chance of moderate risk
        isSecure = true;
        riskLevel = 0.3;
        attackProbability = 20;
    }
    else { // 15% chance of high risk
        isSecure = false;
        riskLevel = 0.7;
        attackProbability = 50;
    }
}

// Global function implementations
void handleRandomEvent(Kingdom& kingdom);

void updateGameState(Kingdom& kingdom) {
    kingdom.getMarket().updateFoodStockpile(kingdom.getPeople().getTotalPeople(), kingdom.weather);
}