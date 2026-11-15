

#include <iostream>
#include <string>
using namespace std;

const int MAX_RIDERS = 50;
const int MAX_DRIVERS = 30;
const int MAX_RIDES = 100;

class Rider
{
public:
    string id;
    string name;
    string phone;
    string source;
    string destination;
    int seats;

    Rider()
    {
        seats = 1;
    }

    void display()
    {
        cout << "Rider ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Route: " << source << " to " << destination << "\n";
        cout << "Seats: " << seats << "\n";
    }
};

class Driver
{
public:
    string id;
    string name;
    string phone;
    string vehicle;
    bool available;

    Driver()
    {
        available = true;
    }

    void display()
    {
        cout << "Driver ID: " << id << "\n";
        cout << "Name: " << name << "\n";
        cout << "Phone: " << phone << "\n";
        cout << "Vehicle: " << vehicle << "\n";
        if (available == true)
        {
            cout << "Status: Available\n";
        }
        else
        {
            cout << "Status: Busy\n";
        }
    }
};

class Ride
{
public:
    string id;
    string rider1Id;
    string rider2Id;
    string driverId;
    string source;
    string destination;
    double distanceKm;
    double fare;
    bool shared;
    bool completed;

    Ride()
    {
        distanceKm = 0;
        fare = 0;
        shared = false;
        completed = false;
        driverId = "Not assigned";
        rider2Id = "None";
    }

    void display()
    {
        cout << "Ride ID: " << id << "\n";
        cout << "Rider(s): " << rider1Id;
        if (shared == true)
        {
            cout << ", " << rider2Id;
        }
        cout << "\nRoute: " << source << " to " << destination << "\n";
        cout << "Distance: " << distanceKm << " km\n";
        cout << "Fare per rider: Rs. " << fare << "\n";
        cout << "Driver: " << driverId << "\n";
        if (completed == true)
        {
            cout << "Status: Completed\n";
        }
        else
        {
            cout << "Status: Active\n";
        }
    }
};

class RideSharingSystem
{
private:
    Rider riders[MAX_RIDERS];
    Driver drivers[MAX_DRIVERS];
    Ride rides[MAX_RIDES];
    int riderCount;
    int driverCount;
    int rideCount;
    int nextRiderNumber;
    int nextDriverNumber;
    int nextRideNumber;

public:
    RideSharingSystem()
    {
        riderCount = 0;
        driverCount = 0;
        rideCount = 0;
        nextRiderNumber = 1;
        nextDriverNumber = 1;
        nextRideNumber = 1;
    }

    int findRiderIndex(string riderId)
    {
        int i;
        for (i = 0; i < riderCount; i++)
        {
            if (riders[i].id == riderId)
            {
                return i;
            }
        }
        return -1;
    }

    int findDriverIndex(string driverId)
    {
        int i;
        for (i = 0; i < driverCount; i++)
        {
            if (drivers[i].id == driverId)
            {
                return i;
            }
        }
        return -1;
    }

    int findAvailableDriver()
    {
        int i;
        for (i = 0; i < driverCount; i++)
        {
            if (drivers[i].available == true)
            {
                return i;
            }
        }
        return -1;
    }

    void registerRider()
    {
        if (riderCount >= MAX_RIDERS)
        {
            cout << "Rider list is full.\n";
            return;
        }

        Rider newRider;
        newRider.id = "R" + to_string(nextRiderNumber);
        nextRiderNumber++;

        cout << "Enter rider name: ";
        getline(cin, newRider.name);
        cout << "Enter phone number: ";
        getline(cin, newRider.phone);
        cout << "Enter pickup location: ";
        getline(cin, newRider.source);
        cout << "Enter destination: ";
        getline(cin, newRider.destination);
        cout << "Enter number of seats: ";
        cin >> newRider.seats;
        cin.ignore(1000, '\n');

        if (newRider.seats < 1)
        {
            newRider.seats = 1;
        }

        riders[riderCount] = newRider;
        riderCount++;
        cout << "Rider registered. Your ID is " << newRider.id << "\n";
    }

    void registerDriver()
    {
        if (driverCount >= MAX_DRIVERS)
        {
            cout << "Driver list is full.\n";
            return;
        }

        Driver newDriver;
        newDriver.id = "D" + to_string(nextDriverNumber);
        nextDriverNumber++;

        cout << "Enter driver name: ";
        getline(cin, newDriver.name);
        cout << "Enter phone number: ";
        getline(cin, newDriver.phone);
        cout << "Enter vehicle name/number: ";
        getline(cin, newDriver.vehicle);

        drivers[driverCount] = newDriver;
        driverCount++;
        cout << "Driver registered. Driver ID is " << newDriver.id << "\n";
    }

    void showRiders()    {
        int i;
        if (riderCount == 0)
        {
            cout << "No riders are registered.\n";
            return;
        }
        for (i = 0; i < riderCount; i++)
        {
            cout << "\n";
            riders[i].display();
        }
    }

    void showDrivers()
    {
        int i;
        if (driverCount == 0)
        {
            cout << "No drivers are registered.\n";
            return;
        }
        for (i = 0; i < driverCount; i++)
        {
            cout << "\n";
            drivers[i].display();
        }
    }

    void bookRide(bool sharedRide)
    {
        if (rideCount >= MAX_RIDES)
        {
            cout << "Ride list is full.\n";
            return;
        }

        string riderId;
        int riderIndex;
        int secondRiderIndex = -1;
        int driverIndex;
        int i;
        double distanceKm;

        cout << "Enter your Rider ID: ";
        getline(cin, riderId);
        riderIndex = findRiderIndex(riderId);
        if (riderIndex == -1)
        {
            cout << "Rider ID not found. Register the rider first.\n";
            return;
        }

        if (sharedRide == true)
        {
            for (i = 0; i < riderCount; i++)
            {
                if (i != riderIndex &&
                    riders[i].source == riders[riderIndex].source &&
                    riders[i].destination == riders[riderIndex].destination)
                {
                    secondRiderIndex = i;
                    break;
                }
            }

            if (secondRiderIndex == -1)
            {
                cout << "No other rider has the same pickup and destination yet.\n";
                return;
            }
        }

        cout << "Enter trip distance in kilometres: ";
        cin >> distanceKm;
        cin.ignore(1000, '\n');
        if (distanceKm <= 0)
        {
            cout << "Distance must be greater than zero.\n";
            return;
        }

        Ride newRide;
        newRide.id = "TR" + to_string(nextRideNumber);
        nextRideNumber++;
        newRide.rider1Id = riders[riderIndex].id;
        newRide.source = riders[riderIndex].source;
        newRide.destination = riders[riderIndex].destination;
        newRide.distanceKm = distanceKm;
        newRide.shared = sharedRide;

        /* Example fare: Rs. 12 per kilometre for a solo ride. */
        newRide.fare = distanceKm * 12;
        if (sharedRide == true)
        {
            newRide.rider2Id = riders[secondRiderIndex].id;
            newRide.fare = newRide.fare / 2;
        }

        driverIndex = findAvailableDriver();
        if (driverIndex != -1)
        {
            newRide.driverId = drivers[driverIndex].id;
            drivers[driverIndex].available = false;
        }

        rides[rideCount] = newRide;
        rideCount++;
        cout << "Ride booked.\n\n";
        newRide.display();
    }

    void showRides()
    {
        int i;
        if (rideCount == 0)
        {
            cout << "No rides have been booked.\n";
            return;
        }
        for (i = 0; i < rideCount; i++)
        {
            cout << "\n";
            rides[i].display();
        }
    }

    void completeRide()
    {
        string rideId;
        int i;

        cout << "Enter Ride ID to complete: ";
        getline(cin, rideId);

        for (i = 0; i < rideCount; i++)
        {
            if (rides[i].id == rideId)
            {
                if (rides[i].completed == true)
                {
                    cout << "This ride is already completed.\n";
                    return;
                }

                rides[i].completed = true;
                int driverIndex = findDriverIndex(rides[i].driverId);
                if (driverIndex != -1)
                {
                    drivers[driverIndex].available = true;
                }
                cout << "Ride completed. The driver is available again.\n";
                return;
            }
        }
        cout << "Ride ID not found.\n";
    }

    void run()
    {
        int choice = 0;
        while (choice != 9)
        {
            cout << "\n===== SMARTRIDE =====\n";
            cout << "1. Register rider\n";
            cout << "2. Register driver\n";
            cout << "3. Book solo ride\n";
            cout << "4. Find and book shared ride\n";
            cout << "5. Show riders\n";
            cout << "6. Show drivers\n";
            cout << "7. Show rides\n";
            cout << "8. Complete a ride\n";
            cout << "9. Exit\n";
            cout << "Choose an option: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Please enter a number from 1 to 9.\n";
                continue;
            }
            cin.ignore(1000, '\n');

            if (choice == 1)
            {
                registerRider();
            }
            else if (choice == 2)
            {
                registerDriver();
            }
            else if (choice == 3)
            {
                bookRide(false);
            }
            else if (choice == 4)
            {
                bookRide(true);
            }
            else if (choice == 5)
            {
                showRiders();
            }
            else if (choice == 6)
            {
                showDrivers();
            }
            else if (choice == 7)
            {
                showRides();
            }
            else if (choice == 8)
            {
                completeRide();
            }
            else if (choice != 9)
            {
                cout << "Invalid option.\n";
            }
        }

        cout << "Goodbye.\n";
    }
};

int main()
{
    RideSharingSystem system;
    system.run();
    return 0;
}
