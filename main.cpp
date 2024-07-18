#include <iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <ctime>

#define SEATS 4
#define ADMIN_PASS "Kavy07"

using namespace std;

struct date{
    unsigned short year;
    unsigned char month;
    unsigned char date;
};
struct coordinates{
    float x;
    float y;
};

typedef struct date date;
typedef struct coordinates coordinates;

bool compareDate(date d1, date d2) {
    if( d1.year != d2.year || d1.month != d2.month || d1.date != d2.date )
        return 0;
    return 1;
}
float calculateDistance(coordinates c1, coordinates c2) {
    return pow(pow(c1.x-c2.x, 2)+pow(c1.y-c2.y, 2), 0.5);
}

//Custom Exceptions
class TicketNotFound: public exception {
    public:
        string what() {
            return "Ticket Not Found!";
        }
};
class TicketNotAvailable: public exception {
    public:
        string what() {
            return "Ticket Not Available";
        }
};

pair<bool, unsigned char> checkTicket(unsigned long long ID, unsigned long long departure, unsigned long long destination, date schedule);
date getCurrentDate();
bool checkDate(date Date);

class Station {
    private:
        string name;
        coordinates position;
        unsigned long long id;
    public:
        coordinates getCoordinates(bool display=false) {
            if(display) {
                cout << "Station Name : " << name << endl;
                cout << "Station ID : " << id << endl;
                cout << "X - Coordinate : " << position.x << endl;
                cout << "Y - Coordinate : " << position.y << endl;
            }
            return position;
        }
        string getName() {
            return name;
        }
        unsigned long long getID() {
            return id;
        }
        void setProperties(string NAME, float x, float y, unsigned long long ID) {
            name = NAME;
            position.x = x;
            position.y = y;
            id = ID;
        }
};
vector<Station> allStations;
class Ticket {
    private:
        float ticket_constant = 0.5f;
    protected:
        Station departure, destination;
        date schedule;
        bool coach;                                // 0 for 'A' Coach, 1 for 'B' Coach
        unsigned char seat_number;
        float cost;
    public:
        void viewTicket() {
            cout << "Departure Station = " << departure.getName() << endl;
            cout << "Destination Station = " << destination.getName() << endl;
            cout << "Date" << endl;
            cout << "\tDate: " << ((int)schedule.date) << endl;
            cout << "\tMonth: " << ((int)schedule.month) << endl;
            cout << "\tYear: " << schedule.year << endl;
            cout << "Coach " << (coach ? 'B' : 'A') << endl;
            cout << "Seat Number " << ((int)seat_number) << endl;
            cout << "Cost: Rs." << cost << endl;
        }
        pair<pair<unsigned long long, unsigned long long>, date> getJourney() {
            pair<pair<unsigned long long, unsigned long long>, date> journey;
            journey.first.first = departure.getID();
            journey.first.second = destination.getID();
            journey.second = schedule;
            return journey;
        }
        pair<bool, unsigned char> coachSeatNumber() {
            pair<bool, unsigned char> position;
            position.first = coach;
            position.second = seat_number;
            return position;
        }
        Ticket(Station DEPARTURE, Station DESTINATION, date SCHEDULE, bool COACH, unsigned char SEAT_NUMBER) {
            departure = DEPARTURE;
            destination = DESTINATION;
            schedule = SCHEDULE;
            coach = COACH;
            seat_number = SEAT_NUMBER;
            cost = (float) calculateDistance(departure.getCoordinates(), destination.getCoordinates()) * ticket_constant;
        }
};
class User {
    private:
        string name, password;
    protected:
        string username;
        unsigned long long id;
        vector<Ticket> ticket_history;
    public:
        string getUsername() { 
            return username;
        }
        string getPassword() {
            return password;
        }
        string getName() {
            return name;
        }
        pair<bool, unsigned char> searchTicket(unsigned long long DEPARTURE, unsigned long long DESTINATION, date SCHEDULE) {
            for(vector<Ticket>::iterator ticket = ticket_history.begin(); ticket != ticket_history.end(); ticket++){
                pair<pair<unsigned long long, unsigned long long>, date> journey = ticket->getJourney();
                if( journey.first.first == DEPARTURE && journey.first.second == DESTINATION && compareDate(journey.second, SCHEDULE) == 1 )
                    return ticket->coachSeatNumber();
            }
            throw TicketNotFound();
        }
        void viewTickets() {
            int numberOfTicket = 1;
            for(vector<Ticket>::iterator ticket = ticket_history.begin(); ticket != ticket_history.end(); ticket++, numberOfTicket++) {
                cout << "Ticket Number : " << numberOfTicket << endl;
                ticket->viewTicket();
                cout << endl;
            }
        }
        bool verifyPassword(string PASSWORD) {
            if( PASSWORD != password )
                return 0;
            return 1;
        }
        User(string NAME, string USERNAME, string PASSWORD, unsigned long long ID) {
            name = NAME;
            username = USERNAME;
            password = PASSWORD;
            id = ID;
        }

};
class Passenger: public User{
    public:
        unsigned long long getPassengerID() {
            return User::id;
        }
        void bookTicket(unsigned long long departure, unsigned long long destination, date schedule) {
            pair<bool, unsigned char> coachSeatNumber = checkTicket(User::id, departure, destination, schedule);
            int found = 0;
            Station DEPARTURE, DESTINATION;
            for(vector<Station>::iterator station = allStations.begin(); station != allStations.end() && found < 2; station++)
                if( station->getID() == departure ) {
                    DEPARTURE = *station;
                    found++;
                } else if( station->getID() == destination ) {
                    DESTINATION = *station;
                    found++;
                }
            Ticket ticket(DEPARTURE, DESTINATION, schedule, coachSeatNumber.first, coachSeatNumber.second);
            User::ticket_history.push_back(ticket);
        }
        void displayLatestTicket() {
            vector<Ticket>::iterator ticket = User::ticket_history.end()-1;
            ticket->viewTicket();
        }
        using User::User;
};
vector<Passenger> allPassengers;
class TicketCollector: public User{
    private:
        unsigned long long employeeID;
    public:
        unsigned long long getEmployeeID() {
            return employeeID;
        }
        void setEmployeeID(unsigned long long ID) {
            employeeID = ID;
        }
        using User::User;
};
vector<TicketCollector> allEmployees;
class Travel {
    protected:
        unsigned long long departure, destination;
        date schedule;
    public:
        void displayJourney() {
            cout << "Departure Station = " << departure << endl;
            cout << "Destination Station = " << destination << endl;
            cout << "Date" << endl;
            cout << "\tDate: " << ((int)schedule.date) << endl;
            cout << "\tMonth: " << ((int)schedule.month) << endl;
            cout << "\tYear: " << schedule.year << endl;
        }
        unsigned char numPassengers() {
            unsigned char numberOfPassengers = 0;
            for(vector<Passenger>::iterator passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++) {
                pair<bool, unsigned char> coachSeatNumber;
                try {
                    coachSeatNumber = passenger->searchTicket(departure, destination, schedule);
                } catch (TicketNotFound err) {
                    continue;
                }
                numberOfPassengers++;
            }
            return numberOfPassengers;
        }
        void displayPassengers() {
            for(vector<Passenger>::iterator passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++) {
                pair<bool, unsigned char> coachSeatNumber;
                try {
                    coachSeatNumber = passenger->searchTicket(departure, destination, schedule);
                } catch (TicketNotFound err) {
                    continue;
                }
                cout << "Coach " << ((coachSeatNumber.first == 1)? "B":"A") << " Seat Number " << ((int) coachSeatNumber.second) << " = ID: " << passenger->getPassengerID() << " -> Name: " << passenger->getName() << endl;
            }
        }
        bool compareTravel(unsigned long long DEPARTURE, unsigned long long DESTINATION, date SCHEDULE) {
            if( departure != DEPARTURE || destination != DESTINATION || compareDate(schedule, SCHEDULE) == 0)
                return 0;
            return 1;
        }
        Travel(unsigned long long DEPARTURE, unsigned long long DESTINATION, date SCHEDULE) {
            departure = DEPARTURE;
            destination = DESTINATION;
            schedule = SCHEDULE;
        }
};
vector<Travel> allTravel;

unsigned long long totalUsers = 0, totalEmployees = 0, totalStations = 0;

void adminShell() {
    string command;
    cout << "(1) Log In" << endl;
    cout << "(2) Register" << endl;
    cout << "(.) Back" << endl;
    cout << "> ";
    cin >> command;
    if( command == "1" ) {
        string username, password;
        cout << "Username: ";
        cin >> username;
        bool found = 0;
        vector<TicketCollector>::iterator tc;
        for(tc = allEmployees.begin(); tc != allEmployees.end(); tc++) {
            if( tc->getUsername() == username ) {
                found = 1;
                break;
            }
        }
        if( found == 0 ) {
            cout << "Username Not Found!" << endl;
            return;
        }
        cout << "Password: ";
        cin >> password;
        if( tc->getPassword() != password ) {
            cout << "Incorrect Password" << endl;
            return;
        }
        cout << "Welcome, " << tc->getName() << endl;
        cout << "(1) Show Passengers" << endl;
        cout << "(2) Add Station" << endl;
        cout << "(3) Show Passenger Details" << endl;
        cout << "(4) Show Stations" << endl;
        cout << "(5) Show Station Details" << endl;
        cout << "(6) Show Travel Details" << endl;
        cout << "(.) Logout" << endl;
        cout << "> ";
        cin >> command;
        if( command == "1" ) {
            for(vector<Passenger>::iterator passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++)
                cout << passenger->getPassengerID() << " -> " << passenger->getName() << endl;
        } else if ( command == "2" ) {
            string name;
            float x, y;
            cout << "Enter the Name of the Station: ";
            cin >> name;
            cout << "Enter the X-Coordinate of the Station: ";
            cin >> x;
            cout << "Enter the Y-Coordinate of the Station: ";
            cin >> y;
            Station station;
            station.setProperties(name, x, y, ++totalStations);
            allStations.push_back(station);
            cout << "Station Successfully Registered with ID " << totalStations << endl;
        } else if ( command == "3" ) {
            unsigned long long id;
            cout << "Enter the ID of the Passenger: ";
            cin >> id;
            for(vector<Passenger>::iterator passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++)
                if( passenger->getPassengerID() == id ) {
                    cout << "Passenger ID       : " << passenger->getPassengerID() << endl;
                    cout << "Passenger Name     : " << passenger->getName() << endl;
                    cout << "Passenger Username : " << passenger->getUsername() << endl;
                    passenger->viewTickets();
                    return;
                }
            cout << "Passenger Not Found!" << endl;
        } else if ( command == "4" ) {
            for(vector<Station>::iterator station = allStations.begin(); station != allStations.end(); station++)
                cout << station->getID() << " -> " << station->getName() << endl;
            cout << endl;
        } else if ( command == "5" ) {
            unsigned long long id;
            cout << "Enter the ID of the Station: ";
            cin >> id;
            for(vector<Station>::iterator station = allStations.begin(); station != allStations.end(); station++)
                if( station->getID() == id ) {
                    cout << "Station ID          : " << station->getID() << endl;
                    cout << "Station Name        : " << station->getName() << endl;
                    cout << "Station Coordinates : (" << station->getCoordinates().x << ", " << station->getCoordinates().y << ")" << endl << endl;
                    return;
                }
        } else if ( command == "6" ) {
            for(vector<Travel>::iterator travel = allTravel.begin(); travel != allTravel.end(); travel++) {
                travel->displayJourney();
                travel->displayPassengers();
                cout << endl << endl;
            }
        }
    } else if (command == "2") {
        string admin_pass;
        cout << "Enter Admin Pass: ";
        cin >> admin_pass;
        if( admin_pass != ADMIN_PASS ) {
            cout << "ADMIN PASS didn't match!" << endl;
            return;
        }
        string username, password, name;
        bool found = 1;
        while(found) {
            found = 0;
            cout << "Enter a Username: ";
            cin >> username;
            for(vector<TicketCollector>::iterator tc = allEmployees.begin(); tc != allEmployees.end(); tc++)
                if( tc->getUsername() == username ) {
                    cout << "Username Already exists, Enter Another One!" << endl;
                    found = 1;
                    break;
                }
        }
        cout << "Enter a Password: ";
        cin >> password;
        cout << "Enter your Name: ";
        cin >> name;
        TicketCollector tc(name, username, password, ++totalUsers);
        tc.setEmployeeID(++totalEmployees);
        allEmployees.push_back(tc);
        cout << "Account Registered Successfully with ID " << totalUsers << " and Employee ID " << totalEmployees << endl;
    }
}
void passengerShell() {
    string command;
    cout << "(1) Log In" << endl;
    cout << "(2) Register" << endl;
    cout << "(.) Back" << endl;
    cout << "> ";
    cin >> command;
    if( command == "1" ) {
        string username, password;
        cout << "Username: ";
        cin >> username;
        bool found = 0;
        vector<Passenger>::iterator passenger;
        for(passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++)
            if( passenger->getUsername() == username ) {
                found = 1;
                break;
            }
        if( found == 0 ) {
            cout << "Username Not Found!" << endl;
            return;
        }
        cout << "Password: ";
        cin >> password;
        if( passenger->getPassword() != password ) {
            cout << "Incorrect Password" << endl;
            return;
        }
        cout << "Welcome, " << passenger->getName() << endl;
        cout << "(1) Show Ticket History" << endl;
        cout << "(2) Book Ticket" << endl;
        cout << "(.) Logout" << endl;
        cout << "> ";
        cin >> command;
        if ( command == "1" ) {
            passenger->viewTickets();
        } else if (command == "2") {
            unsigned long long departure, destination;
            unsigned short MONTH, DATE;
            date schedule;
            cout << "Departure Station ID   : ";
            cin >> departure;
            cout << "Destination Station ID : ";
            cin >> destination;
            cout << "Year  : ";
            cin >> schedule.year;
            cout << "Month : ";
            cin >> MONTH;
            cout << "Day   : ";
            cin >> DATE;
            schedule.month = MONTH;
            schedule.date = DATE;
            bool correctDate = checkDate(schedule);
            if( correctDate == 0 ) {
                cout << "Please Enter Correct Date!" << endl;
                return;
            }
            try {
                passenger->bookTicket(departure, destination, schedule);
            } catch(TicketNotAvailable err) {
                cout << "Ticket not Available" << endl;
                return;
            }
            cout << "Ticket Booked Successfully!" << endl;
            passenger->displayLatestTicket();
        } else
            return;
    } else if( command == "2") {
        string username, password, name;
        bool found = 1;
        while(found) {
            found = 0;
            cout << "Enter a Username: ";
            cin >> username;
            for(vector<Passenger>::iterator passenger = allPassengers.begin(); passenger != allPassengers.end(); passenger++)
                if( passenger->getUsername() == username ) {
                    cout << "Username Already exists, Enter Another One!" << endl;
                    found = 1;
                    break;
                }
        }
        cout << "Enter a Password: ";
        cin >> password;
        cout << "Enter your Name: ";
        cin >> name;
        Passenger passenger(name, username, password, ++totalUsers);
        allPassengers.push_back(passenger);
        cout << "Account Registered Successfully with ID " << totalUsers << endl;
    }
}

int main() {
    string command = ".";
    cout << "IRCTC Train Booking System" << endl << endl;
    cout << "Commands" << endl;
    cout << "admin     - Administrator Log In/Registration" << endl;
    cout << "passenger - Passenger Log In/Registration" << endl;
    cout << "help      - Display This Message" << endl;
    cout << "exit      - exit from the program" << endl << endl;
    while (command != "exit") {
        cout << "$ ";
        cin >> command;
        if ( command == "admin" )
            adminShell();
        else if ( command == "passenger" )
            passengerShell();
        else if ( command == "help" ) {
            cout << "IRCTC Train Booking System" << endl << endl;
            cout << "Commands" << endl;
            cout << "admin     - Administrator Log In/Registration" << endl;
            cout << "passenger - Passenger Log In/Registration" << endl;
            cout << "help      - Display This Message" << endl;
            cout << "exit      - exit from the program" << endl << endl;
        }
    }
    
    return 0;
}

pair<bool, unsigned char> checkTicket(unsigned long long id, unsigned long long departure, unsigned long long destination, date schedule) {
    bool found = 0;
    vector<Travel>::iterator travel;
    for(travel = allTravel.begin(); travel != allTravel.end(); travel++)
        if(travel->compareTravel(departure, destination, schedule) == 1) {
            if(travel->numPassengers() > 8)
                throw TicketNotAvailable();
            found = 1;
            break;
        }
    if(found) {
        int numberOfPassengers = travel->numPassengers();
        bool coach = (numberOfPassengers >= 4)? 1: 0;
        unsigned char seat = numberOfPassengers % 4 + 1;
        pair<bool, unsigned char> coachSeatNumber;
        coachSeatNumber.first = coach;
        coachSeatNumber.second = seat;
        return coachSeatNumber;
    }
    Travel journey(departure, destination, schedule);
    allTravel.push_back(journey);
    bool coach = 0;
    unsigned char seat = 1;
    pair<bool, unsigned char> coachSeatNumber;
    coachSeatNumber.first = coach;
    coachSeatNumber.second = seat;
    return coachSeatNumber;
}
date getCurrentDate() {
    time_t time = std::time(nullptr);
    tm* now = localtime(&time);
    date currentTime;
    currentTime.year = (unsigned short) (now->tm_year+1900);
    currentTime.month = (unsigned char) (now->tm_mon+1);
    currentTime.date = (unsigned char) (now->tm_mday);
    return currentTime;
}
bool checkDate(date Date) {
    if( Date.date < 0 || Date.date > 31 || Date.month < 1 || Date.month > 12 || Date.year < 0 )
        return 0;
    date currentDate = getCurrentDate();
    if( Date.year < currentDate.year )
        return 0;
    else if( Date.year == currentDate.year )
        if( Date.month < currentDate.month )
            return 0;
        else if( Date.month == currentDate.month )
            if( Date.date <= currentDate.date )
                return 0;
    return 1;
}
