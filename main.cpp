#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

class ChargingStation {
public:
    ChargingStation(int id) : stationID(id) {}

    void chargeVehicle(const string& tag, int chargingTime) {
        cout << "Vehicle " << tag << " is charging at Station " << stationID << " for " << chargingTime << " minutes." << endl;
    }

    bool isAvailable() const {
        return chargingQueue.empty();
    }

    void enqueueVehicle(const string& tag, int chargingTime) {
        chargingQueue.push({tag, chargingTime});
    }

    pair<string, int> dequeueVehicle() {
        if (!chargingQueue.empty()) {
            auto frontVehicle = chargingQueue.front();
            chargingQueue.pop();
            return frontVehicle;
        }
        return {"", 0}; // No vehicle in the queue
    }

private:
    int stationID;
    queue<pair<string, int>> chargingQueue; // {tag, chargingTime}
};

class ChargingVehicle {
public:
    ChargingVehicle(int id) : tag("#" + to_string(id)), status("Not charging"), batteryPercentage(0), chargingTime(0), isVIP(false), isEmergency(false) {}

    void setStatus(const string& newStatus) {
        status = newStatus;
    }

    string getStatus() const {
        return status;
    }

    int getBatteryPercentage() const {
        return batteryPercentage;
    }

    void setBatteryPercentage(int percentage) {
        batteryPercentage = percentage;
    }

    int getChargingTime() const {
        return chargingTime;
    }

    void setChargingTime(int time) {
        chargingTime = time;
    }

    const string& getTagNumber() const {
        return tag;
    }

    bool getVIPStatus() const {
        return isVIP;
    }

    void setVIPStatus(bool vip) {
        isVIP = vip;
    }

    bool getEmergencyStatus() const {
        return isEmergency;
    }

    void setEmergencyStatus(bool emergency) {
        isEmergency = emergency;
    }
    int getChargingCost() const {
        return chargingTime * 2; // Basic billing: $2 per minute
    }

private:
    string tag;
    string status;
    int batteryPercentage;
    int chargingTime;
    bool isVIP;
    bool isEmergency;
};

int main() {
    int n, k, chargingTime, numVIP, numEmergency;

    cout << "Enter the number of vehicles: ";
    cin >> n;

    while (cin.fail() || n <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> n;
    }

    cout << "Enter the number of charging stations: ";
    cin >> k;

    while (cin.fail() || k <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> k;
    }

    cout << "Enter the charging time for each vehicle (in minutes): ";
    cin >> chargingTime;

    while (cin.fail() || chargingTime <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> chargingTime;
    }

    cout << "Enter the number of VIP vehicles: ";
    cin >> numVIP;

    while (cin.fail() || numVIP < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> numVIP;
    }

    // Input VIP car tag numbers
    unordered_set<string> vipTags;
    for (int i = 0; i < numVIP; ++i) {
        string tag;
        cout << "Enter tag number for VIP car #" << i + 1 << ": ";
        cin >> tag;
        vipTags.insert(tag);
    }

    cout << "Enter the number of emergency vehicles: ";
    cin >> numEmergency;

    while (cin.fail() || numEmergency < 0) {
        cout << "Invalid input. Please enter a non-negative integer: ";
        cin.clear();
        cin.ignore(256, '\n');
        cin >> numEmergency;
    }

    // Input emergency car tag numbers
    unordered_set<string> emergencyTags;
    for (int i = 0; i < numEmergency; ++i) {
        string tag;
        cout << "Enter tag number for emergency car #" << i + 1 << ": ";
        cin >> tag;
        emergencyTags.insert(tag);
    }

    vector<int> availablePercentages;
    for (int i = 1; i <= 100; i++) {
        availablePercentages.push_back(i);
    }

    vector<ChargingStation*> stations;
    vector<ChargingVehicle*> vehicles;

    int vehicleID = 1;

    srand(time(nullptr));

    // Charge VIP vehicles first
    cout << "\nVIP Vehicles and Emergency Vehicles Charging Status:" << endl;
    for (const string& tag : vipTags) {
        stations.push_back(new ChargingStation(vehicleID));
        int randIndex = rand() % availablePercentages.size();
        int percentage = availablePercentages[randIndex];
        availablePercentages.erase(availablePercentages.begin() + randIndex);
        stations.back()->chargeVehicle(tag, chargingTime);

        vehicles.push_back(new ChargingVehicle(vehicleID));
        vehicles.back()->setBatteryPercentage(percentage);
        vehicles.back()->setStatus("Charging at Station " + to_string(vehicleID));
        vehicles.back()->setChargingTime(chargingTime);
        vehicles.back()->setVIPStatus(true);

        vehicleID++;
    }

    // Charge emergency vehicles next
    for (const string& tag : emergencyTags) {
        stations.push_back(new ChargingStation(vehicleID));
        int randIndex = rand() % availablePercentages.size();
        int percentage = availablePercentages[randIndex];
        availablePercentages.erase(availablePercentages.begin() + randIndex);
        stations.back()->chargeVehicle(tag, chargingTime);

        vehicles.push_back(new ChargingVehicle(vehicleID));
        vehicles.back()->setBatteryPercentage(percentage);
        vehicles.back()->setStatus("Charging at Station " + to_string(vehicleID));
        vehicles.back()->setChargingTime(chargingTime);
        vehicles.back()->setEmergencyStatus(true);

        vehicleID++;
    }

    // Charge remaining vehicles
    for (int i = 0; i < n - numVIP - numEmergency; ++i) {
        stations.push_back(new ChargingStation(vehicleID));
        int randIndex = rand() % availablePercentages.size();
        int percentage = availablePercentages[randIndex];
        availablePercentages.erase(availablePercentages.begin() + randIndex);
        stations.back()->chargeVehicle("#" + to_string(vehicleID), chargingTime);

        vehicles.push_back(new ChargingVehicle(vehicleID));
        vehicles.back()->setBatteryPercentage(percentage);
        vehicles.back()->setStatus("Charging at Station " + to_string(vehicleID));
        vehicles.back()->setChargingTime(chargingTime);

        vehicleID++;
    }

    cout << "All vehicles have been charged." << endl;

    // Print VIP vehicles charging status
    cout << "\nRemaining Vehicles Charging Status:" << endl;
    for (int i = numVIP + numEmergency; i < n; ++i) {
        auto vehicleInfo = stations[i]->dequeueVehicle();
        cout << "Vehicle : " << vehicleInfo.first << " - Charging at Station " << i + 1
             << " - Battery Percentage: " << vehicles[i]->getBatteryPercentage() << "% - Charging Cost: $"
             << vehicles[i]->getChargingCost() << endl;
    }

    cout << "\nTotal Charging Cost: $" << (n - numVIP - numEmergency) * chargingTime * 2 << endl;

    // Clean up allocated memory
    for (auto station : stations) {
        delete station;
    }

    for (auto vehicle : vehicles) {
        delete vehicle;
    }

    return 0;
}