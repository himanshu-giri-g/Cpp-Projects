#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream> // For string stream

using namespace std;

// Class to represent an Event
class Event {
public:
    string name;
    string date;       // Expected format: YYYY-MM-DD
    string location;
    string category;
    bool reminder;     // New attribute for reminders
    string time;       // New attribute for time (HH:MM format)

    Event(string name, string date, string location, string category = "", string time = "", bool reminder = false)
        : name(name), date(date), location(location), category(category), time(time), reminder(reminder) {}

    void display() const {
        cout << "Event: " << name 
             << ", Date: " << date 
             << ", Location: " << location 
             << (category.empty() ? "" : ", Category: " + category)
             << (reminder ? ", Reminder: Set" : ", Reminder: Not Set") 
             << (time.empty() ? "" : ", Time: " + time) << endl;
    }

    string getEventDetails() const {
        stringstream ss;
        ss << name << "," << date << "," << location << "," << category << "," << time << "," << reminder;
        return ss.str();
    }
};

// Class to manage the Event List
class EventList {
private:
    vector<Event> events;

public:
    void addEvent(const Event& event) {
        events.push_back(event);
    }

    void displayEvents() const {
        if (events.empty()) {
            cout << "No events scheduled." << endl;
            return;
        }
        cout << "Scheduled Events:" << endl;
        for (const auto& event : events) {
            event.display();
        }
    }

    void displayEventsFormatted() const {
        // Additional formatting for display
        cout << setw(20) << left << "Event" 
             << setw(12) << left << "Date" 
             << setw(15) << left << "Location" 
             << setw(12) << left << "Category" 
             << setw(12) << left << "Time" 
             << setw(10) << left << "Reminder" << endl;

        for (const auto& event : events) {
            cout << setw(20) << left << event.name 
                 << setw(12) << left << event.date 
                 << setw(15) << left << event.location 
                 << setw(12) << left << event.category 
                 << setw(12) << left << event.time 
                 << setw(10) << left << (event.reminder ? "Set" : "Not Set") << endl;
        }
    }

    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& event : events) {
                file << event.getEventDetails() << endl;
            }
            file.close();
            cout << "Events saved to " << filename << endl;
        } else {
            cout << "Unable to open file for writing." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            events.clear();
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string name, date, location, category, time;
                bool reminder;

                getline(ss, name, ',');
                getline(ss, date, ',');
                getline(ss, location, ',');
                getline(ss, category, ',');
                getline(ss, time, ',');
                ss >> reminder;

                events.emplace_back(name, date, location, category, time, reminder);
            }
            file.close();
            cout << "Events loaded from " << filename << endl;
        } else {
            cout << "Unable to open file for reading." << endl;
        }
    }

    void removeEvent(const string& eventName) {
        auto it = remove_if(events.begin(), events.end(), [&](const Event& event) {
            return event.name == eventName;
        });
        if (it != events.end()) {
            events.erase(it, events.end());
            cout << "Event \"" << eventName << "\" removed." << endl;
        } else {
            cout << "Event \"" << eventName << "\" not found." << endl;
        }
    }

    void searchEvent(const string& eventName) const {
        for (const auto& event : events) {
            if (event.name == eventName) {
                cout << "Found: ";
                event.display();
                return;
            }
        }
        cout << "Event \"" << eventName << "\" not found." << endl;
    }

    void editEvent(const string& eventName) {
        for (auto& event : events) {
            if (event.name == eventName) {
                cout << "Editing event: ";
                event.display();
                string newName, newDate, newLocation, newCategory, newTime;
                bool newReminder;

                cout << "Enter new name (leave blank to keep current): ";
                cin.ignore();
                getline(cin, newName);
                cout << "Enter new date (YYYY-MM-DD, leave blank to keep current): ";
                getline(cin, newDate);
                cout << "Enter new location (leave blank to keep current): ";
                getline(cin, newLocation);
                cout << "Enter new category (leave blank to keep current): ";
                getline(cin, newCategory);
                cout << "Enter new time (HH:MM, leave blank to keep current): ";
                getline(cin, newTime);
                cout << "Set reminder? (1 for Yes, 0 for No): ";
                cin >> newReminder;

                if (!newName.empty()) event.name = newName;
                if (!newDate.empty()) event.date = newDate;
                if (!newLocation.empty()) event.location = newLocation;
                if (!newCategory.empty()) event.category = newCategory;
                if (!newTime.empty()) event.time = newTime;
                event.reminder = newReminder;

                cout << "Event updated." << endl;
                return;
            }
        }
        cout << "Event \"" << eventName << "\" not found." << endl;
    }

    void filterEventsByDate(const string& date) const {
        cout << "Events on " << date << ":" << endl;
        bool found = false;
        for (const auto& event : events) {
            if (event.date == date) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No events found on " << date << "." << endl;
        }
    }

    void filterEventsByCategory(const string& category) const {
        cout << "Events in category \"" << category << "\":" << endl;
        bool found = false;
        for (const auto& event : events) {
            if (event.category == category) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No events found in category \"" << category << "\"." << endl;
        }
    }

    void filterEventsByLocation(const string& location) const {
        cout << "Events at location \"" << location << "\":" << endl;
        bool found = false;
        for (const auto& event : events) {
            if (event.location == location) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No events found at location \"" << location << "\"." << endl;
        }
    }

    void sortEventsByDate() {
        sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
            return a.date < b.date;
        });
        cout << "Events sorted by date." << endl;
    }

    void clearAllEvents() {
        events.clear();
        cout << "All events cleared." << endl;
    }

    void displayUpcomingEvents() const {
        cout << "Upcoming Events:" << endl;
        bool found = false;
        string today = getCurrentDate();
        for (const auto& event : events) {
            if (event.date >= today) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No upcoming events." << endl;
        }
    }

    static string getCurrentDate() {
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        char buffer[11]; // YYYY-MM-DD
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
        return string(buffer);
    }

    void searchEventsByDate(const string& date) const {
        cout << "Events on " << date << ":" << endl;
        bool found = false;
        for (const auto& event : events) {
            if (event.date == date) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No events found on " << date << "." << endl;
        }
    }

    void searchEventsByLocation(const string& location) const {
        cout << "Events at \"" << location << "\":" << endl;
        bool found = false;
        for (const auto& event : events) {
            if (event.location == location) {
                event.display();
                found = true;
            }
        }
        if (!found) {
            cout << "No events found at \"" << location << "\"." << endl;
        }
    }
};

// Function to display the menu and get user choice
int displayMenu() {
    int choice;
    cout << "\nEvent Management System" << endl;
    cout << "1. Add Event" << endl;
    cout << "2. Display Events" << endl;
    cout << "3. Display Events Formatted" << endl;
    cout << "4. Save Events" << endl;
    cout << "5. Load Events" << endl;
    cout << "6. Remove Event" << endl;
    cout << "7. Search Event" << endl;
    cout << "8. Edit Event" << endl;
    cout << "9. Filter Events by Date" << endl;
    cout << "10. Filter Events by Category" << endl;
    cout << "11. Filter Events by Location" << endl;
    cout << "12. Sort Events by Date" << endl;
    cout << "13. Clear All Events" << endl;
    cout << "14. Display Upcoming Events" << endl;
    cout << "15. Search Events by Date" << endl;
    cout << "16. Search Events by Location" << endl;
    cout << "17. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Main function
int main() {
    EventList eventList;
    int choice;

    do {
        choice = displayMenu();

        switch (choice) {
        case 1: {
            string name, date, location, category, time;
            bool reminder;
            cout << "Enter event name: ";
            cin >> ws;
            getline(cin, name);
            cout << "Enter event date (YYYY-MM-DD): ";
            getline(cin, date);
            cout << "Enter event location: ";
            getline(cin, location);
            cout << "Enter event category (optional): ";
            getline(cin, category);
            cout << "Enter event time (HH:MM, optional): ";
            getline(cin, time);
            cout << "Set reminder? (1 for Yes, 0 for No): ";
            cin >> reminder;
            eventList.addEvent(Event(name, date, location, category, time, reminder));
            break;
        }
        case 2:
            eventList.displayEvents();
            break;
        case 3:
            eventList.displayEventsFormatted();
            break;
        case 4: {
            string filename;
            cout << "Enter filename to save events: ";
            cin >> filename;
            eventList.saveToFile(filename);
            break;
        }
        case 5: {
            string filename;
            cout << "Enter filename to load events: ";
            cin >> filename;
            eventList.loadFromFile(filename);
            break;
        }
        case 6: {
            string name;
            cout << "Enter event name to remove: ";
            cin >> ws;
            getline(cin, name);
            eventList.removeEvent(name);
            break;
        }
        case 7: {
            string name;
            cout << "Enter event name to search: ";
            cin >> ws;
            getline(cin, name);
            eventList.searchEvent(name);
            break;
        }
        case 8: {
            string name;
            cout << "Enter event name to edit: ";
            cin >> ws;
            getline(cin, name);
            eventList.editEvent(name);
            break;
        }
        case 9: {
            string date;
            cout << "Enter date to filter events (YYYY-MM-DD): ";
            cin >> ws;
            getline(cin, date);
            eventList.filterEventsByDate(date);
            break;
        }
        case 10: {
            string category;
            cout << "Enter category to filter events: ";
            cin >> ws;
            getline(cin, category);
            eventList.filterEventsByCategory(category);
            break;
        }
        case 11: {
            string location;
            cout << "Enter location to filter events: ";
            cin >> ws;
            getline(cin, location);
            eventList.filterEventsByLocation(location);
            break;
        }
        case 12:
            eventList.sortEventsByDate();
            break;
        case 13:
            eventList.clearAllEvents();
            break;
        case 14:
            eventList.displayUpcomingEvents();
            break;
        case 15: {
            string date;
            cout << "Enter date to search events (YYYY-MM-DD): ";
            cin >> ws;
            getline(cin, date);
            eventList.searchEventsByDate(date);
            break;
        }
        case 16: {
            string location;
            cout << "Enter location to search events: ";
            cin >> ws;
            getline(cin, location);
            eventList.searchEventsByLocation(location);
            break;
        }
        case 17:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 17);

    return 0;
}
