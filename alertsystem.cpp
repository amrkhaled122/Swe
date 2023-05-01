#include <iostream>
#include <vector>

using namespace std;

class Child {
public:
    double latest_location;
    double latest_crying_instance;
    double latest_heart_rate;
    double latest_oxygen_saturation;
    double latest_body_temperature;
    string parent;
    string emergency_contact;
};

class AlertSystem;

class Observer {
public:
    virtual void update(AlertSystem* alert_system) = 0;
};

class AlertSystem : public Child {
private:
    vector<Observer*> observers;

public:
    void attach(Observer* observer) {
        observers.push_back(observer);
    }

    void detach(Observer* observer) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notify() {
        for (auto observer : observers) {
            observer->update(this);
        }
    }

    void set_child_data(double location, double crying_instance, double heart_rate, double oxygen_saturation, double body_temperature, string parent, string emergency_contact) {
        latest_location = location;
        latest_crying_instance = crying_instance;
        latest_heart_rate = heart_rate;
        latest_oxygen_saturation = oxygen_saturation;
        latest_body_temperature = body_temperature;
        this->parent = parent;
        this->emergency_contact = emergency_contact;
        check_alert();
    }

    void check_alert() {
        if (latest_heart_rate < 60 || latest_heart_rate > 120 || latest_oxygen_saturation < 92 || latest_body_temperature < 35 || latest_body_temperature > 39 || latest_location == 1 || latest_location == 2) {
            notify();
        }
    }
};

class EmergencyAlert : public Observer {
public:
    void update(AlertSystem* alert_system) {
        cout << "Emergency alert sent to " << alert_system->parent << " and " << alert_system->emergency_contact << endl;
    }
};

int main() {
    AlertSystem alert_system;
    EmergencyAlert emergency_alert;

    alert_system.attach(&emergency_alert);

    // Test case 1: No alert should be sent
    alert_system.set_child_data(5.5, 0, 80, 98, 37, "John", "Jane");

    // Test case 2: Heart rate alert should be sent
    alert_system.set_child_data(5.5, 0, 55, 98, 37, "John", "Jane");

    // Test case 3: Oxygen saturation alert should be sent
    alert_system.set_child_data(5.5, 0, 80, 90, 37, "John", "Jane");

    // Test case 4: Body temperature alert should be sent
    alert_system.set_child_data(5.5, 0, 80, 98, 40, "John", "Jane");

    // Test case 5: Location alert should be sent
    alert_system.set_child_data(1, 0, 80, 98, 37, "John", "Jane");

    alert_system.detach(&emergency_alert);

    return 0;
}


