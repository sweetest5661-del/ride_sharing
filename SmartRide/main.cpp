#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/* Rename the C file's standalone entry point so its algorithms can be included here. */
#define main c_algorithms_main
#include "algorithms.c"
#undef main

using namespace std;

struct Rider
{
    string id, name, phone, from, to;
    int seats;
};

struct Driver
{
    string id, name, phone, vehicle;
    bool available;
};

struct Ride
{
    string id, rider, partner, driver, from, to;
    double km, fare;
    bool shared, done;
};

struct Edge
{
    int to;
    double km;
};

struct Route
{
    double km;
    vector<string> places;
};

/* Purpose: read a complete line safely, trim its boundaries, and report empty input clearly. */
string inputText(const string &prompt)
{
    cout << prompt;
    string value;
    bool receivedLine = static_cast<bool>(getline(cin, value));
    if (!receivedLine)
    {
        return "";
    }
    size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos)
    {
        return "";
    }
    size_t last = value.find_last_not_of(" \t\r\n");
    size_t length = last - first + 1;
    if (length == 0)
    {
        return "";
    }
    string trimmedValue = value.substr(first, length);
    if (trimmedValue.find_first_not_of(" \t\r\n") == string::npos)
    {
        return "";
    }
    return trimmedValue;
}

/* Purpose: keep asking until the user enters an integer in the allowed range. */
int inputInt(const string &prompt, int low, int high)
{
    while (true)
    {
        string value = inputText(prompt);
        if (cin.eof())
        {
            return low;
        }
        if (value.empty())
        {
            cout << "Input cannot be empty.\n";
            continue;
        }
        try
        {
            size_t used = 0;
            int number = stoi(value, &used);
            if (used == value.size() && number >= low && number <= high)
            {
                return number;
            }
        }
        catch (...)
        {
        }
        cout << "Enter a number from " << low << " to " << high << ".\n";
    }
}

/* Purpose: print location names in route order with arrows between them. */
void printRoute(const vector<string> &path)
{
    for (size_t i = 0; i < path.size(); ++i)
    {
        if (i != 0)
        {
            cout << " -> ";
        }
        cout << path[i];
    }
}

class SmartRide
{
    vector<Rider> riders;
    vector<Driver> drivers;
    vector<Ride> rides;
    vector<string> places;
    vector<vector<Edge>> graph;
    unordered_map<string, int> placeIds;
    int riderNumber = 1, driverNumber = 1, rideNumber = 1;

public:
    /* Purpose: create sample road connections for immediate shortest-route use. */
    SmartRide()
    {
        bool roadsLoaded = true;
        roadsLoaded = connect("ISBT", "Clock Tower", 4) && roadsLoaded;
        roadsLoaded = connect("ISBT", "Rajpur Road", 8) && roadsLoaded;
        roadsLoaded = connect("Clock Tower", "Rajpur Road", 2) && roadsLoaded;
        roadsLoaded = connect("Clock Tower", "Railway Station", 5) && roadsLoaded;
        roadsLoaded = connect("Rajpur Road", "Railway Station", 3) && roadsLoaded;
        roadsLoaded = connect("Railway Station", "Airport", 25) && roadsLoaded;
        roadsLoaded = connect("Rajpur Road", "Airport", 30) && roadsLoaded;
        if (!roadsLoaded)
        {
            cout << "One or more sample roads could not be loaded.\n";
        }
        if (places.empty())
        {
            cout << "The road map starts empty. Add roads before booking.\n";
        }
        else
        {
            cout << "Sample road locations ready: " << places.size() << '\n';
        }
        if (graph.size() != places.size())
        {
            cout << "Road map index does not match its graph storage.\n";
        }
    }

    /* Purpose: use the location hash index to find a vertex or add it to the graph. */
    int addLocation(const string &name)
    {
        if (name.empty())
        {
            return -1;
        }
        auto found = placeIds.find(name);
        if (found != placeIds.end())
        {
            return found->second;
        }
        int id = static_cast<int>(places.size());
        places.push_back(name);
        graph.emplace_back();
        auto inserted = placeIds.emplace(name, id);
        if (!inserted.second)
        {
            graph.pop_back();
            places.pop_back();
            return inserted.first->second;
        }
        return id;
    }

    /* Purpose: update or create both weighted edges for a two-way road. */
    bool connect(const string &first, const string &second, double km)
    {
        if (first.empty() || second.empty() || first == second || !isfinite(km) || km <= 0)
        {
            return false;
        }
        int a = addLocation(first), b = addLocation(second);
        bool updated = false;
        for (Edge &edge : graph[a])
        {
            if (edge.to == b)
            {
                edge.km = km;
                updated = true;
            }
        }
        if (!updated)
        {
            graph[a].push_back({b, km});
        }
        updated = false;
        for (Edge &edge : graph[b])
        {
            if (edge.to == a)
            {
                edge.km = km;
                updated = true;
            }
        }
        if (!updated)
        {
            graph[b].push_back({a, km});
        }
        return true;
    }

    /* Purpose: Dijkstra's min-priority queue finds a minimum-distance path in the road graph. */
    Route shortestPath(const string &from, const string &to) const
    {
        auto sourceEntry = placeIds.find(from);
        auto targetEntry = placeIds.find(to);
        double infinity = numeric_limits<double>::infinity();
        if (sourceEntry == placeIds.end() || targetEntry == placeIds.end())
        {
            return {infinity, {}};
        }
        int source = sourceEntry->second;
        int target = targetEntry->second;
        vector<double> distance(places.size(), infinity);
        vector<int> previous(places.size(), -1);
        using Item = pair<double, int>;
        priority_queue<Item, vector<Item>, greater<Item>> pending;
        distance[source] = 0;
        pending.push({0, source});
        while (!pending.empty())
        {
            Item item = pending.top();
            pending.pop();
            double currentDistance = item.first;
            int current = item.second;
            if (currentDistance != distance[current])
            {
                continue;
            }
            if (current == target)
            {
                break;
            }
            for (const Edge &edge : graph[current])
            {
                double nextDistance = currentDistance + edge.km;
                if (nextDistance < distance[edge.to])
                {
                    distance[edge.to] = nextDistance;
                    previous[edge.to] = current;
                    pending.push({nextDistance, edge.to});
                }
            }
        }
        if (!isfinite(distance[target]))
        {
            return {infinity, {}};
        }
        vector<int> reversePath;
        for (int at = target; at != -1; at = previous[at])
        {
            reversePath.push_back(at);
        }
        vector<string> path;
        for (auto it = reversePath.rbegin(); it != reversePath.rend(); ++it)
        {
            path.push_back(places[*it]);
        }
        return {distance[target], path};
    }

    /* Purpose: collect and validate rider details, then assign a unique rider ID. */
    void registerRider()
    {
        Rider rider;
        rider.name = inputText("Name: ");
        rider.phone = inputText("Phone: ");
        rider.from = inputText("Pickup: ");
        rider.to = inputText("Destination: ");
        if (rider.name.empty() || rider.phone.empty() || rider.from.empty() || rider.to.empty() ||
            rider.from == rider.to)
        {
            cout << "Invalid details; rider was not registered.\n";
            return;
        }
        rider.seats = inputInt("Seats (1-4): ", 1, 4);
        rider.id = "R" + to_string(riderNumber++);
        addLocation(rider.from);
        addLocation(rider.to);
        riders.push_back(rider);
        cout << "Rider registered: " << rider.id << '\n';
    }

    /* Purpose: validate driver information and add an available driver record. */
    void registerDriver()
    {
        Driver driver;
        driver.name = inputText("Name: ");
        driver.phone = inputText("Phone: ");
        driver.vehicle = inputText("Vehicle/plate: ");
        if (driver.name.empty())
        {
            cout << "Driver name cannot be empty.\n";
            return;
        }
        if (driver.phone.empty())
        {
            cout << "Driver phone cannot be empty.\n";
            return;
        }
        if (driver.vehicle.empty())
        {
            cout << "Vehicle or plate cannot be empty.\n";
            return;
        }
        bool phoneHasDigit = false;
        for (char character : driver.phone)
        {
            if (isdigit(static_cast<unsigned char>(character)))
            {
                phoneHasDigit = true;
            }
        }
        if (!phoneHasDigit)
        {
            cout << "Phone number must contain at least one digit.\n";
            return;
        }
        for (const Driver &existing : drivers)
        {
            if (existing.phone == driver.phone)
            {
                cout << "That phone number is already registered.\n";
                return;
            }
        }
        driver.id = "D" + to_string(driverNumber);
        driver.available = true;
        drivers.push_back(driver);
        ++driverNumber;
        cout << "Driver registered: " << driver.id << '\n';
        cout << "Driver is available for ride assignment.\n";
    }

    /* Purpose: read and store a validated positive two-way road distance. */
    void addRoad()
    {
        string first = inputText("First location: ");
        string second = inputText("Second location: ");
        string value = inputText("Distance in km: ");
        try
        {
            size_t used = 0;
            double km = stod(value, &used);
            if (used == value.size() && connect(first, second, km))
            {
                cout << "Road saved.\n";
                return;
            }
        }
        catch (...)
        {
        }
        cout << "Invalid road details.\n";
    }

    /* Purpose: show the shortest route returned by Dijkstra's algorithm. */
    void findRoute() const
    {
        string from = inputText("From: ");
        string to = inputText("To: ");
        if (from.empty() || to.empty())
        {
            cout << "Both route locations are required.\n";
            return;
        }
        if (from == to)
        {
            cout << "Start and destination must be different.\n";
            return;
        }
        Route route = shortestPath(from, to);
        if (route.places.empty())
        {
            cout << "No connected route was found for those locations.\n";
            cout << "Add the missing road connections and try again.\n";
            return;
        }
        cout << "Shortest distance: " << fixed << setprecision(2) << route.km << " km\n";
        cout << "Route: ";
        printRoute(route.places);
        cout << '\n';
    }

    /* Purpose: match a same-route rider when requested and assign an available driver. */
    void bookRide(bool shared)
    {
        string id = inputText("Rider ID: ");
        int first = -1, second = -1, driverIndex = -1;
        for (size_t i = 0; i < riders.size(); ++i)
        {
            if (riders[i].id == id)
            {
                first = static_cast<int>(i);
            }
        }
        if (first < 0)
        {
            cout << "Rider not found.\n";
            return;
        }
        for (const Ride &ride : rides)
        {
            if (!ride.done && (ride.rider == id || ride.partner == id))
            {
                cout << "Rider already has an active ride.\n";
                return;
            }
        }
        if (shared)
        {
            for (size_t i = 0; i < riders.size(); ++i)
            {
                bool same =
                    riders[i].from == riders[first].from && riders[i].to == riders[first].to;
                bool fits = riders[i].seats + riders[first].seats <= 4;
                bool active = false;
                for (const Ride &ride : rides)
                {
                    if (!ride.done && (ride.rider == riders[i].id || ride.partner == riders[i].id))
                    {
                        active = true;
                    }
                }
                if (static_cast<int>(i) != first && same && fits && !active)
                {
                    second = static_cast<int>(i);
                    break;
                }
            }
        }
        if (shared && second < 0)
        {
            cout << "No suitable sharing rider is waiting.\n";
            return;
        }
        for (size_t i = 0; i < drivers.size(); ++i)
        {
            if (drivers[i].available)
            {
                driverIndex = static_cast<int>(i);
                break;
            }
        }
        if (driverIndex < 0)
        {
            cout << "No driver is available.\n";
            return;
        }
        Route route = shortestPath(riders[first].from, riders[first].to);
        if (route.places.empty())
        {
            cout << "No connected route; add roads first.\n";
            return;
        }
        Ride ride;
        ride.id = "T" + to_string(rideNumber++);
        ride.rider = id;
        ride.partner = shared ? riders[second].id : "-";
        ride.driver = drivers[driverIndex].id;
        ride.from = riders[first].from;
        ride.to = riders[first].to;
        ride.km = route.km;
        ride.fare = route.km * 12;
        ride.shared = shared;
        ride.done = false;
        drivers[driverIndex].available = false;
        rides.push_back(ride);
        cout << "Booked " << ride.id << ", Rs. " << ride.fare / (shared ? 2 : 1) << " per rider.\n";
        printRoute(route.places);
        cout << '\n';
    }

    /* Purpose: list all stored rider, driver, ride, and road-location records. */
    void listRecords() const
    {
        cout << "\nRiders\n";
        for (const Rider &r : riders)
        {
            cout << r.id << " | " << r.name << " | " << r.from << " -> " << r.to << '\n';
        }
        cout << "Drivers\n";
        for (const Driver &d : drivers)
        {
            cout << d.id << " | " << d.name << " | " << d.vehicle << " | "
                 << (d.available ? "Available" : "Busy") << '\n';
        }
        cout << "Rides\n";
        for (const Ride &r : rides)
        {
            cout << r.id << " | " << r.rider << " + " << r.partner << " | Rs. " << r.fare << " | "
                 << (r.done ? "Finished" : "Active") << '\n';
        }
    }

    /* Purpose: complete or cancel a ride and release the assigned driver. */
    void finishRide(bool cancel)
    {
        string id = inputText("Ride ID: ");
        for (Ride &ride : rides)
        {
            if (ride.id == id)
            {
                if (ride.done)
                {
                    cout << "Ride is already finished.\n";
                    return;
                }
                ride.done = true;
                for (Driver &driver : drivers)
                {
                    if (driver.id == ride.driver)
                    {
                        driver.available = true;
                    }
                }
                cout << (cancel ? "Ride cancelled.\n" : "Ride completed.\n");
                return;
            }
        }
        cout << "Ride not found.\n";
    }

    /* Purpose: use the C linear-search algorithm to find a rider by array index. */
    void searchRider() const
    {
        string id = inputText("Rider ID: ");
        if (id.empty())
        {
            cout << "Rider ID cannot be empty.\n";
            return;
        }
        vector<int> matches;
        for (const Rider &rider : riders)
        {
            matches.push_back(rider.id == id ? 1 : 0);
        }
        int riderCount = static_cast<int>(matches.size());
        int index = linearSearch(matches.data(), riderCount, 1);
        if (index < 0)
        {
            cout << "Rider not found.\n";
            return;
        }
        const Rider &r = riders[static_cast<size_t>(index)];
        cout << "Found rider after checking " << index + 1 << " record(s).\n";
        cout << r.id << " | " << r.name << " | " << r.phone << " | " << r.from << " -> " << r.to
             << '\n';
    }

    /* Purpose: save rider records and a sorted primary index of rider IDs and file offsets. */
    void saveData() const
    {
        ofstream data("smartride_data.txt");
        ofstream index("smartride_data.idx");
        if (!data || !index)
        {
            cout << "Could not open data files.\n";
            return;
        }
        vector<pair<string, streamoff>> keys;
        for (const Rider &r : riders)
        {
            streamoff offset = data.tellp();
            data << quoted(r.id) << ' ' << quoted(r.name) << ' ' << quoted(r.phone) << ' '
                 << quoted(r.from) << ' ' << quoted(r.to) << ' ' << r.seats << '\n';
            keys.push_back({r.id, offset});
        }
        sort(keys.begin(), keys.end());
        for (const auto &key : keys)
        {
            index << quoted(key.first) << ' ' << key.second << '\n';
        }
        cout << "Saved " << riders.size() << " riders and the primary index.\n";
    }

    /* Purpose: reload saved rider records and ensure their locations exist in the road graph. */
    void loadData()
    {
        ifstream data("smartride_data.txt");
        if (!data)
        {
            cout << "No saved data file found.\n";
            return;
        }
        riders.clear();
        Rider r;
        while (data >> quoted(r.id) >> quoted(r.name) >> quoted(r.phone) >> quoted(r.from) >>
               quoted(r.to) >> r.seats)
        {
            riders.push_back(r);
            addLocation(r.from);
            addLocation(r.to);
            int number = 0;
            if (r.id.size() > 1)
            {
                number = stoi(r.id.substr(1));
            }
            riderNumber = max(riderNumber, number + 1);
        }
        cout << "Loaded " << riders.size() << " riders.\n";
    }

    /* Purpose: binary-search the sorted file index and read a rider by saved byte offset. */
    void findRiderIndex() const
    {
        ifstream file("smartride_data.idx");
        vector<pair<string, streamoff>> keys;
        string id;
        streamoff offset;
        while (file >> quoted(id) >> offset)
        {
            keys.push_back({id, offset});
        }
        string wanted = inputText("Saved rider ID: ");
        int low = 0, high = static_cast<int>(keys.size()) - 1, found = -1;
        while (low <= high)
        {
            int middle = low + (high - low) / 2;
            if (keys[middle].first == wanted)
            {
                found = middle;
                break;
            }
            if (keys[middle].first < wanted)
            {
                low = middle + 1;
            }
            else
            {
                high = middle - 1;
            }
        }
        if (found < 0)
        {
            cout << "Rider not in saved index.\n";
            return;
        }
        ifstream data("smartride_data.txt");
        data.seekg(keys[found].second);
        Rider r;
        if (data >> quoted(r.id) >> quoted(r.name) >> quoted(r.phone) >> quoted(r.from) >>
            quoted(r.to) >> r.seats)
        {
            cout << r.id << " | " << r.name << " | " << r.phone << " | " << r.from << " -> " << r.to
                 << '\n';
        }
        else
        {
            cout << "Could not read saved record.\n";
        }
    }

    /* Purpose: display the menu choices handled by the main switch. */
    void showMenu() const
    {
        cout << "\n================ SmartRide ================\n";
        cout << "1  Register a rider\n";
        cout << "2  Register a driver\n";
        cout << "3  Book a solo ride\n";
        cout << "4  Book a shared ride\n";
        cout << "5  List saved and active records\n";
        cout << "6  Complete an active ride\n";
        cout << "7  Cancel an active ride\n";
        cout << "8  Add a two-way road\n";
        cout << "9  Find a shortest route with Dijkstra\n";
        cout << "10 Search riders with linear search\n";
        cout << "11 Save rider records and primary index\n";
        cout << "12 Load saved rider records\n";
        cout << "13 Find a saved rider with binary search\n";
        cout << "14 Open the C algorithm demonstrations\n";
        cout << "0  Save and exit\n";
        cout << "============================================\n";
    }
};

/* Purpose: show choices and dispatch only function calls from switch cases. */
int main()
{
    SmartRide app;
    int choice = -1;
    while (choice != 0)
    {
        app.showMenu();
        choice = inputInt("Choose: ", 0, 14);
        switch (choice)
        {
            case 0:
                app.saveData();
                break;
            case 1:
                app.registerRider();
                break;
            case 2:
                app.registerDriver();
                break;
            case 3:
                app.bookRide(false);
                break;
            case 4:
                app.bookRide(true);
                break;
            case 5:
                app.listRecords();
                break;
            case 6:
                app.finishRide(false);
                break;
            case 7:
                app.finishRide(true);
                break;
            case 8:
                app.addRoad();
                break;
            case 9:
                app.findRoute();
                break;
            case 10:
                app.searchRider();
                break;
            case 11:
                app.saveData();
                break;
            case 12:
                app.loadData();
                break;
            case 13:
                app.findRiderIndex();
                break;
            case 14:
                c_algorithms_main();
                break;
            default:
                break;
        }
    }
    return 0;
}
