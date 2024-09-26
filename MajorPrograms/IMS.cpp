#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip> // For std::setprecision

using namespace std;

// Class to represent an Item in the inventory
class Item {
public:
    string name;
    int quantity;
    double price;

    Item(string name, int quantity, double price)
        : name(name), quantity(quantity), price(price) {}

    void display() const {
        cout << "Name: " << name << ", Quantity: " << quantity << ", Price: $" << fixed << setprecision(2) << price << endl;
    }
};

// Class to manage the Inventory
class Inventory {
private:
    vector<Item> items;

public:
    void addItem(const Item& item) {
        items.push_back(item);
    }

    void displayItems() const {
        if (items.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }
        cout << "Current Inventory:" << endl;
        for (const auto& item : items) {
            item.display();
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& item : items) {
                file << item.name << "," << item.quantity << "," << item.price << endl;
            }
            file.close();
            cout << "Inventory saved to " << filename << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string name;
            int quantity;
            double price;
            while (getline(file, name, ',')) {
                file >> quantity;
                file.ignore(); // Ignore the comma
                file >> price;
                file.ignore(); // Ignore the newline
                items.emplace_back(name, quantity, price);
            }
            file.close();
            cout << "Inventory loaded from " << filename << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void removeItem(const string& itemName) {
        auto it = remove_if(items.begin(), items.end(), [&](const Item& item) {
            return item.name == itemName;
        });
        if (it != items.end()) {
            items.erase(it, items.end());
            cout << "Item \"" << itemName << "\" removed from inventory." << endl;
        } else {
            cout << "Item \"" << itemName << "\" not found." << endl;
        }
    }

    void updateItem(const string& itemName, int quantity, double price) {
        for (auto& item : items) {
            if (item.name == itemName) {
                item.quantity = quantity;
                item.price = price;
                cout << "Item \"" << itemName << "\" updated." << endl;
                return;
            }
        }
        cout << "Item \"" << itemName << "\" not found." << endl;
    }

    void searchItem(const string& itemName) const {
        for (const auto& item : items) {
            if (item.name == itemName) {
                cout << "Found: ";
                item.display();
                return;
            }
        }
        cout << "Item \"" << itemName << "\" not found." << endl;
    }

    void sortItems() {
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.name < b.name;
        });
        cout << "Inventory sorted by item name." << endl;
    }

    void displayStatistics() const {
        int totalItems = items.size();
        double totalValue = 0.0;

        for (const auto& item : items) {
            totalValue += item.price * item.quantity;
        }

        cout << "Total number of items: " << totalItems << endl;
        cout << "Total value of inventory: $" << fixed << setprecision(2) << totalValue << endl;
    }

    void restockItem(const string& itemName, int additionalQuantity) {
        for (auto& item : items) {
            if (item.name == itemName) {
                item.quantity += additionalQuantity;
                cout << "Restocked \"" << itemName << "\" by " << additionalQuantity << " units." << endl;
                return;
            }
        }
        cout << "Item \"" << itemName << "\" not found." << endl;
    }

    void batchAddItems() {
        int numItems;
        cout << "Enter number of items to add: ";
        cin >> numItems;
        for (int i = 0; i < numItems; ++i) {
            string name;
            int quantity;
            double price;
            cout << "Enter item " << i + 1 << " name: ";
            cin >> name;
            cout << "Enter item " << i + 1 << " quantity: ";
            cin >> quantity;
            cout << "Enter item " << i + 1 << " price: ";
            cin >> price;
            addItem(Item(name, quantity, price));
        }
    }

    void listItemsBelowThreshold(int threshold) const {
        cout << "Items below threshold of " << threshold << ":" << endl;
        for (const auto& item : items) {
            if (item.quantity < threshold) {
                item.display();
            }
        }
    }

    void exportToCSV(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& item : items) {
                file << item.name << "," << item.quantity << "," << item.price << endl;
            }
            file.close();
            cout << "Inventory exported to " << filename << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void importFromCSV(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string name;
            int quantity;
            double price;
            while (getline(file, name, ',')) {
                file >> quantity;
                file.ignore(); // Ignore the comma
                file >> price;
                file.ignore(); // Ignore the newline
                items.emplace_back(name, quantity, price);
            }
            file.close();
            cout << "Inventory imported from " << filename << endl;
        } else {
            cout << "Unable to open file." << endl;
        }
    }

    void checkItemAvailability(const string& itemName) const {
        for (const auto& item : items) {
            if (item.name == itemName) {
                if (item.quantity > 0) {
                    cout << "Item \"" << itemName << "\" is available with quantity: " << item.quantity << endl;
                } else {
                    cout << "Item \"" << itemName << "\" is out of stock." << endl;
                }
                return;
            }
        }
        cout << "Item \"" << itemName << "\" not found." << endl;
    }

    // New method to get the most expensive item
    void getMostExpensiveItem() const {
        if (items.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }
        auto maxItem = max_element(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price < b.price;
        });
        cout << "Most expensive item: ";
        maxItem->display();
    }

    // New method to get the least expensive item
    void getLeastExpensiveItem() const {
        if (items.empty()) {
            cout << "Inventory is empty." << endl;
            return;
        }
        auto minItem = min_element(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price < b.price;
        });
        cout << "Least expensive item: ";
        minItem->display();
    }

    // New method to sort items by price
    void sortItemsByPrice() {
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return a.price < b.price;
        });
        cout << "Inventory sorted by item price." << endl;
    }

    // New method to filter items by price range
    void filterItemsByPriceRange(double minPrice, double maxPrice) const {
        cout << "Items in the price range $" << minPrice << " to $" << maxPrice << ":" << endl;
        for (const auto& item : items) {
            if (item.price >= minPrice && item.price <= maxPrice) {
                item.display();
            }
        }
    }
};

// Function to display the menu and get user choice
int displayMenu() {
    int choice;
    cout << "\nInventory Management System" << endl;
    cout << "1. Add Item" << endl;
    cout << "2. Display Items" << endl;
    cout << "3. Save Inventory" << endl;
    cout << "4. Load Inventory" << endl;
    cout << "5. Remove Item" << endl;
    cout << "6. Update Item" << endl;
    cout << "7. Search Item" << endl;
    cout << "8. Sort Items" << endl;
    cout << "9. Display Statistics" << endl;
    cout << "10. Restock Item" << endl;
    cout << "11. Batch Add Items" << endl;
    cout << "12. List Items Below Threshold" << endl;
    cout << "13. Export to CSV" << endl;
    cout << "14. Import from CSV" << endl;
    cout << "15. Check Item Availability" << endl;
    cout << "16. Get Most Expensive Item" << endl;
    cout << "17. Get Least Expensive Item" << endl;
    cout << "18. Sort Items by Price" << endl;
    cout << "19. Filter Items by Price Range" << endl;
    cout << "20. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Main function
int main() {
    Inventory inventory;
    int choice;

    do {
        choice = displayMenu();

        switch (choice) {
        case 1: {
            string name;
            int quantity;
            double price;
            cout << "Enter item name: ";
            cin >> name;
            cout << "Enter item quantity: ";
            cin >> quantity;
            cout << "Enter item price: ";
            cin >> price;
            inventory.addItem(Item(name, quantity, price));
            break;
        }
        case 2:
            inventory.displayItems();
            break;
        case 3: {
            string filename;
            cout << "Enter filename to save inventory: ";
            cin >> filename;
            inventory.saveToFile(filename);
            break;
        }
        case 4: {
            string filename;
            cout << "Enter filename to load inventory: ";
            cin >> filename;
            inventory.loadFromFile(filename);
            break;
        }
        case 5: {
            string name;
            cout << "Enter item name to remove: ";
            cin >> name;
            inventory.removeItem(name);
            break;
        }
        case 6: {
            string name;
            int quantity;
            double price;
            cout << "Enter item name to update: ";
            cin >> name;
            cout << "Enter new quantity: ";
            cin >> quantity;
            cout << "Enter new price: ";
            cin >> price;
            inventory.updateItem(name, quantity, price);
            break;
        }
        case 7: {
            string name;
            cout << "Enter item name to search: ";
            cin >> name;
            inventory.searchItem(name);
            break;
        }
        case 8:
            inventory.sortItems();
            break;
        case 9:
            inventory.displayStatistics();
            break;
        case 10: {
            string name;
            int quantity;
            cout << "Enter item name to restock: ";
            cin >> name;
            cout << "Enter quantity to add: ";
            cin >> quantity;
            inventory.restockItem(name, quantity);
            break;
        }
        case 11:
            inventory.batchAddItems();
            break;
        case 12: {
            int threshold;
            cout << "Enter stock threshold: ";
            cin >> threshold;
            inventory.listItemsBelowThreshold(threshold);
            break;
        }
        case 13: {
            string filename;
            cout << "Enter filename to export inventory to CSV: ";
            cin >> filename;
            inventory.exportToCSV(filename);
            break;
        }
        case 14: {
            string filename;
            cout << "Enter filename to import inventory from CSV: ";
            cin >> filename;
            inventory.importFromCSV(filename);
            break;
        }
        case 15: {
            string name;
            cout << "Enter item name to check availability: ";
            cin >> name;
            inventory.checkItemAvailability(name);
            break;
        }
        case 16:
            inventory.getMostExpensiveItem();
            break;
        case 17:
            inventory.getLeastExpensiveItem();
            break;
        case 18:
            inventory.sortItemsByPrice();
            break;
        case 19: {
            double minPrice, maxPrice;
            cout << "Enter minimum price: ";
            cin >> minPrice;
            cout << "Enter maximum price: ";
            cin >> maxPrice;
            inventory.filterItemsByPriceRange(minPrice, maxPrice);
            break;
        }
        case 20:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 20);

    return 0;
}
