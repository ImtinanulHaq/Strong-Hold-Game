#include "game.h"

// First: Function to clear input buffer
void clearInputBuffer()
{
    cin.clear();               // Clear error flags
    cin.ignore(1000, '\n');    // Ignore up to 1000 characters or until newline
}

// Function to wait for user input
void waitForUser()
{
    std::cout << "\nPress Enter to continue...";
    clearInputBuffer();
}

// Second: Function to display the game menu
void displayMenu()
{
    Visual::clearScreen();
    Visual::printLine();
    cout << "Stronghold Game Menu" << endl;
    Visual::printLine();
    Visual::printMenuItem(1, "View Kingdom Status");
    Visual::printMenuItem(2, "Collect Taxes");
    Visual::printMenuItem(3, "Train Army");
    Visual::printMenuItem(4, "Pay Soldiers");
    Visual::printMenuItem(5, "Take Loan");
    Visual::printMenuItem(6, "Repay Loan");
    Visual::printMenuItem(7, "Trade Resources");
    Visual::printMenuItem(8, "Hold Election");
    Visual::printMenuItem(9, "Make Treaty");
    Visual::printMenuItem(10, "Break Treaty");
    Visual::printMenuItem(11, "Save Game");
    Visual::printMenuItem(12, "Load Game");
    Visual::printMenuItem(13, "Fund Public Services");
    Visual::printMenuItem(14, "Update Army Equipment");
    Visual::printMenuItem(15, "Send Message");
    Visual::printMenuItem(16, "View Messages");
    Visual::printMenuItem(17, "Exit");
    Visual::printLine();
    std::cout << "Enter choice (1-17): ";
}

// Third: Function to view kingdom status
void viewStatus(Kingdom& kingdom)
{
    Visual::clearScreen();
    Visual::printLine();
    cout << "Kingdom Status" << endl;
    Visual::printLine();

    Visual::printSection("Basic Information");
    std::cout << "Name: " << kingdom.getName() << std::endl;
    std::cout << "Turn: " << kingdom.getTurn() << std::endl;
    std::cout << "Population: " << kingdom.getPeople().getTotalPeople() << std::endl;

    Visual::printSection("Social Classes");
    std::cout << "- Peasants: " << kingdom.getPeople().getPeasants() << std::endl;
    std::cout << "- Merchants: " << kingdom.getPeople().getMerchants() << std::endl;
    std::cout << "- Nobility: " << kingdom.getPeople().getNobility() << std::endl;
    std::cout << "- Military: " << kingdom.getPeople().getMilitary() << std::endl;
    std::cout << "- Food Supply: " << kingdom.getPeople().getFoodSupply() << std::endl;

    Visual::printSection("Economy & Military");
    std::cout << "Gold: " << kingdom.getEconomy().getGold() << std::endl;
    std::cout << "Inflation: " << kingdom.getEconomy().getInflation() * 100 << "%" << std::endl;
    std::cout << "Public Services: " << kingdom.getEconomy().getPublicServices() << std::endl;
    std::cout << "Army Size: " << kingdom.getArmy().getSize() << std::endl;
    std::cout << "Army Morale: " << kingdom.getArmy().getMorale() << std::endl;
    std::cout << "Training Level: " << kingdom.getArmy().getTrainingLevel() << std::endl;
    std::cout << "Equipment Quality: " << kingdom.getArmy().getEquipment() << std::endl;
    std::cout << "Soldiers Paid: " << (kingdom.getArmy().getIsPaid() ? "Yes" : "No") << std::endl;
    std::cout << "Debt: " << kingdom.getBank().getLoanAmount() << std::endl;

    Visual::printSection("Resources");
    std::cout << "- Wood: " << kingdom.getMarket().getResource("wood") << std::endl;
    std::cout << "- Stone: " << kingdom.getMarket().getResource("stone") << std::endl;
    std::cout << "- Iron: " << kingdom.getMarket().getResource("iron") << std::endl;
    std::cout << "- Food: " << kingdom.getMarket().getResource("food") << std::endl;
    std::cout << "- Price Multiplier: " << kingdom.getMarket().getPriceMultiplier() << std::endl;

    Visual::printSection("Politics & Diplomacy");
    std::cout << "King: " << kingdom.getPolitics().getKingName() << std::endl;
    std::cout << "Stability: " << kingdom.getPolitics().getStability() << std::endl;
    std::cout << "Corruption Level: " << kingdom.getPolitics().getCorruptionLevel() << std::endl;
    std::cout << "Treaty: " << kingdom.getDiplomacy().getTreaty() << std::endl;
    std::cout << "Relations: " << kingdom.getDiplomacy().getRelations() << std::endl;

    Visual::printLine();
    waitForUser();
}

// Function to handle loan repayment
bool handleLoanRepayment(Kingdom& kingdom)
{
    if (kingdom.getBank().getLoanAmount() > 0)
    {
        double minRepayment = kingdom.getBank().getLoanAmount() * 0.1;
        double repay;
        bool validInput = false;

        while (!validInput)
        {
            Visual::clearScreen();
            Visual::printLine();
            cout << "Loan Repayment Required" << endl;
            Visual::printLine();
            Visual::printWarning("You have an outstanding loan of " + std::to_string(kingdom.getBank().getLoanAmount()) + " gold.");
            Visual::printInfo("You must repay at least 10% of the loan amount.");
            Visual::printInfo("Enter repayment amount (minimum " + std::to_string(minRepayment) + "): ");

            if (!(cin >> repay))
            {
                clearInputBuffer();
                Visual::clearScreen();
                Visual::printError("Please enter a valid number!");
                waitForUser();
                continue;
            }

            if (repay < minRepayment)
            {
                clearInputBuffer();
                Visual::clearScreen();
                Visual::printError("Repayment amount must be at least " + std::to_string(minRepayment) + "!");
                waitForUser();
                continue;
            }

            if (repay > kingdom.getEconomy().getGold())
            {
                clearInputBuffer();
                Visual::clearScreen();
                Visual::printError("You don't have enough gold!");
                waitForUser();
                continue;
            }

            validInput = true;
        }

        // Clear input buffer after getting valid input
        clearInputBuffer();

        // Clear screen before showing success message
        Visual::clearScreen();
        kingdom.getBank().repayLoan(repay, kingdom.getEconomy());
        Visual::printSuccess("Loan repayment successful!");
        waitForUser();
    }
    return true;
}

// Function to handle random events
void handleRandomEvent(Kingdom& kingdom)
{
    srand(time(0));
    int event = rand() % 100;
    
    Visual::clearScreen();
    Visual::printLine();
    cout << "Random Event" << endl;
    Visual::printLine();
    
    // Update weather every 3 turns
    kingdom.turnsSinceLastWeatherUpdate++;
    if (kingdom.turnsSinceLastWeatherUpdate >= 3) {
        kingdom.weather.updateWeather();
        kingdom.turnsSinceLastWeatherUpdate = 0;
        
        if (kingdom.weather.isHarshWeather()) {
            Visual::printWarning("Weather Alert: " + kingdom.weather.getCurrentCondition());
            Visual::printInfo("Food production will be affected for " + 
                            to_string(kingdom.weather.getDuration()) + " turns.");
        }
    }
    
    // Update trade route security
    kingdom.getMarket().getTradeRoute().updateSecurity();
    
    if (event < 15) // 15% chance for plague
    {
        if (kingdom.getMarket().checkFoodShortage()) {
            int populationLoss = kingdom.getPeople().getTotalPeople() * 0.15; // 15% population loss due to starvation
            kingdom.getPeople().decreasePopulation(populationLoss);
            Visual::printWarning("Food shortage has led to starvation!");
            Visual::printError("Population decreased by " + to_string(populationLoss) + " people.");
        }
        else {
            int populationLoss = kingdom.getPeople().getTotalPeople() * 0.1; // 10% population loss
            kingdom.getPeople().decreasePopulation(populationLoss);
            Visual::printWarning("A deadly plague has struck the kingdom!");
            Visual::printError("Population decreased by " + to_string(populationLoss) + " people.");
        }
    }
    else if (event < 30) // 15% chance for war
    {
        int goldLoss = kingdom.getEconomy().getGold() * 0.2; // 20% gold loss
        int armyLoss = kingdom.getArmy().getSize() * 0.15; // 15% army loss
        int resourceLoss = 30; // Fixed resource loss
        
        kingdom.getEconomy().decreaseGold(goldLoss);
        kingdom.getArmy().decreaseSize(armyLoss);
        kingdom.getMarket().decreaseResource("weapons", resourceLoss);
        kingdom.getMarket().decreaseResource("food", resourceLoss);
        
        Visual::printWarning("A neighboring kingdom has declared war!");
        Visual::printError("Lost " + to_string(goldLoss) + " gold, " + 
                         to_string(armyLoss) + " soldiers, and resources.");
    }
    else if (event < 45) // 15% chance for natural disaster
    {
        int resourceLoss = 20; // Fixed resource loss
        kingdom.getMarket().decreaseResource("food", resourceLoss);
        kingdom.getMarket().decreaseResource("wood", resourceLoss);
        Visual::printWarning("A natural disaster has struck!");
        Visual::printError("Lost " + std::to_string(resourceLoss) + " units of food and wood.");
    }
    else if (event < 60) // 15% chance for assassination attempt
    {
        if (rand() % 2 == 0) // 50% chance of success
        {
            kingdom.getPolitics().decreaseStability(20);
            Visual::printWarning("An assassination attempt on the king has failed!");
            Visual::printError("Kingdom stability decreased by 20%.");
        }
        else
        {
            kingdom.getPolitics().holdElection(); // Force new election
            Visual::printWarning("The king has been assassinated!");
            Visual::printInfo("A new election must be held.");
        }
    }
    else if (event < 75) // 15% chance for revolt
    {
        int goldLoss = kingdom.getEconomy().getGold() * 0.1; // 10% gold loss
        kingdom.getEconomy().decreaseGold(goldLoss);
        kingdom.getPolitics().decreaseStability(15);
        Visual::printWarning("The peasants are revolting!");
        Visual::printError("Lost " + std::to_string(goldLoss) + " gold and stability decreased by 15%.");
    }
    else if (event < 90) // 15% chance for good event
    {
        int goldGain = kingdom.getEconomy().getGold() * 0.1; // 10% gold gain
        kingdom.getEconomy().increaseGold(goldGain);
        Visual::printSuccess("A wealthy merchant has donated to the kingdom!");
        Visual::printInfo("Gained " + std::to_string(goldGain) + " gold.");
    }
    // 10% chance for nothing to happen
    
    waitForUser();
}

// Function to send a message
void sendMessage(Kingdom& kingdom)
{
    Visual::clearScreen();
    Visual::printLine();
    cout << "Send Message" << endl;
    Visual::printLine();
    string message;
    Visual::printInfo("Enter your message (max 100 characters): ");
    getline(cin, message);
    
    if (message.length() > 100)
    {
        Visual::printError("Message too long! Maximum 100 characters.");
        waitForUser();
        return;
    }
    
    kingdom.getCommunication().sendMessage(message);
    waitForUser();
}

// Function to view messages
void viewMessages(Kingdom& kingdom)
{
    Visual::clearScreen();
    Visual::printLine();
    cout << "View Messages" << endl;
    Visual::printLine();
    kingdom.getCommunication().showMessages();
    waitForUser();
}

// Fourth: Main game function
int main()
{
    // Initialize the kingdom
    Kingdom kingdom("Westland");
    Visual::clearScreen();
    Visual::printLine();
    cout << "Welcome to Stronghold!" << endl;
    Visual::printLine();
    Visual::printSuccess("Manage your kingdom wisely.");
    waitForUser();

    // Game loop variables
    int choice = 0;
    bool running = true;
    int turnCounter = 0; // Counter for random events

    // Main game loop
    while (running && !kingdom.getIsGameOver())
    {
        Visual::clearScreen();
        Visual::printLine();
        cout << "Turn " << std::to_string(kingdom.getTurn()) << endl;
        Visual::printLine();

        // Display menu and get user choice
        displayMenu();
        if (!(cin >> choice))
        {
            clearInputBuffer();
            Visual::clearScreen();
            Visual::printError("Please enter a number!");
            waitForUser();
            continue;
        }
        clearInputBuffer();

        // Clear screen before performing action
        Visual::clearScreen();

        // Process user choice
        switch (choice)
        {
        case 1: // View Kingdom Status
            viewStatus(kingdom);
            break;

        case 2: // Collect Taxes
            kingdom.getEconomy().collectTaxes(kingdom.getPeople());
            waitForUser();
            break;

        case 3: // Train Army
            {
                int cycles;
                Visual::printLine();
                cout << "Train Army" << endl;
                Visual::printLine();
                Visual::printInfo("Enter training cycles (1-5): ");

                if (!(cin >> cycles) || cycles < 1 || cycles > 5)
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Cycles must be 1-5!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getArmy().train(cycles);
                    Visual::printSuccess("Army training completed for " + std::to_string(cycles) + " cycles.");
                    waitForUser();
                }
            }
            break;

        case 4: // Pay Soldiers
            Visual::printLine();
            cout << "Pay Soldiers" << endl;
            Visual::printLine();
            kingdom.getArmy().paySoldiers(kingdom.getEconomy());
            waitForUser();
            break;

        case 5: // Take Loan
            {
                double loan;
                Visual::printLine();
                cout << "Take Loan" << endl;
                Visual::printLine();
                Visual::printInfo("Enter loan amount: ");

                if (!(cin >> loan) || loan <= 0)
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Loan amount must be positive!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getBank().takeLoan(loan, kingdom.getEconomy());
                    Visual::printSuccess("Loan of " + std::to_string(loan) + " gold received.");
                    waitForUser();
                }
            }
            break;

        case 6: // Repay Loan
            {
                double repay;
                Visual::printLine();
                cout << "Repay Loan" << endl;
                Visual::printLine();
                Visual::printInfo("Enter repayment amount: ");

                if (!(cin >> repay) || repay <= 0)
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Repayment amount must be positive!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getBank().repayLoan(repay, kingdom.getEconomy());
                    Visual::printSuccess("Successfully repaid " + std::to_string(repay) + " gold.");
                    waitForUser();
                }
            }
            break;

        case 7: // Trade Resources
            {
                string resource;
                int amount;
                Visual::printLine();
                cout << "Trade Resources" << endl;
                Visual::printLine();
                Visual::printInfo("Enter resource (wood, stone, iron, food): ");
                getline(cin, resource);
                Visual::printInfo("Enter amount (positive to buy, negative to sell): ");

                if (!(cin >> amount))
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Invalid amount!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getMarket().tradeResource(resource, amount, kingdom.getEconomy());
                    Visual::printSuccess("Trade completed for " + resource + ".");
                    waitForUser();
                }
            }
            break;

        case 8: // Hold Election
            Visual::printLine();
            cout << "Hold Election" << endl;
            Visual::printLine();
            kingdom.getPolitics().holdElection();
            waitForUser();
            break;

        case 9: // Make Treaty
            {
                string treaty;
                Visual::printLine();
                cout << "Make Treaty" << endl;
                Visual::printLine();
                Visual::printInfo("Enter treaty (e.g., Peace with Eastland): ");
                getline(cin, treaty);
                Visual::clearScreen();
                kingdom.getDiplomacy().makeTreaty(treaty);
                Visual::printSuccess("Treaty established: " + treaty);
                waitForUser();
            }
            break;

        case 10: // Break Treaty
            Visual::printLine();
            cout << "Break Treaty" << endl;
            Visual::printLine();
            kingdom.getDiplomacy().breakTreaty();
            waitForUser();
            break;

        case 11: // Save Game
            Visual::printLine();
            cout << "Save Game" << endl;
            Visual::printLine();
            kingdom.saveGame();
            waitForUser();
            break;

        case 12: // Load Game
            Visual::printLine();
            cout << "Load Game" << endl;
            Visual::printLine();
            kingdom.loadGame();
            waitForUser();
            break;

        case 13: // Fund Public Services
            {
                int funding;
                Visual::printLine();
                cout << "Fund Public Services" << endl;
                Visual::printLine();
                Visual::printInfo("Enter funding amount: ");

                if (!(cin >> funding) || funding < 0)
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Funding amount must be positive!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getEconomy().fundPublicServices(funding);
                    Visual::printSuccess("Successfully funded public services with " + std::to_string(funding) + " gold.");
                    waitForUser();
                }
            }
            break;

        case 14: // Update Army Equipment
            {
                int quality;
                Visual::printLine();
                cout << "Update Army Equipment" << endl;
                Visual::printLine();
                Visual::printInfo("Enter equipment quality level (1-10): ");

                if (!(cin >> quality) || quality < 1 || quality > 10)
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    Visual::printError("Quality must be between 1 and 10!");
                    waitForUser();
                }
                else
                {
                    clearInputBuffer();
                    Visual::clearScreen();
                    kingdom.getArmy().updateEquipment(quality);
                    Visual::printSuccess("Army equipment updated to quality level " + std::to_string(quality) + ".");
                    waitForUser();
                }
            }
            break;

        case 15: // Send Message
            sendMessage(kingdom);
            break;

        case 16: // View Messages
            viewMessages(kingdom);
            break;

        case 17: // Exit
            running = false;
            break;

        default:
            Visual::printError("Invalid choice! Please try again.");
            waitForUser();
            break;
        }

        // Increment turn counter and check for random events
        if (running && choice != 17)
        {
            turnCounter++;
            if (turnCounter >= 4)
            {
                handleRandomEvent(kingdom);
                turnCounter = 0; // Reset counter after event
            }
        }

        // Update resources at the end of each turn
        kingdom.getMarket().updateFoodStockpile(kingdom.getPeople().getTotalPeople(), kingdom.weather);
        kingdom.getMarket().updateWeaponsStockpile(kingdom.getArmy().getSize());
        
        // Check for food shortage effects
        if (kingdom.getMarket().checkFoodShortage()) {
            kingdom.getPolitics().decreaseStability(5); // Decrease stability due to food shortage
            Visual::printWarning("Food shortage is causing unrest among the population!");
        }
    }

    Visual::clearScreen();
    Visual::printLine();
    cout << "Game Over" << endl;
    Visual::printLine();
    Visual::printSuccess("Thank you for playing Stronghold!");
    waitForUser();
    return 0;
}