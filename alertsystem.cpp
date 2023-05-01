#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Child {
public:
    string latest_location;
    float latest_crying_instance;
    float latest_heart_rate;
    float latest_oxygen_saturation;
    float latest_body_temperature;
    string parent;
    string emergency_contact;
};

class AlertSystem;

class Observer {
public:
    virtual void update(AlertSystem* alert_system, vector<string> alert_types) = 0;
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

    void notify(vector<string> alert_types) {
        for (auto observer : observers) {
            observer->update(this, alert_types);
        }
    }

    void set_child_data(string location, double crying_instance, double heart_rate, double oxygen_saturation, double body_temperature, string parent, string emergency_contact) {
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
        vector<string> alert_types;

        if (latest_heart_rate < 60 || latest_heart_rate > 120) {
            alert_types.push_back("heart rate is " + to_string(latest_heart_rate));
        }

        if (latest_oxygen_saturation < 92) {
            alert_types.push_back("oxygen saturation is " + to_string(latest_oxygen_saturation));
        }

        if (latest_body_temperature < 35 || latest_body_temperature > 39) {
            alert_types.push_back("body temperature is " + to_string(latest_body_temperature));
        }

        if (latest_location == "oven") {
            alert_types.push_back("child is near the oven");
        }

        if (latest_location == "electronic device") {
            alert_types.push_back("child is near a not fully isolated electronic device");
        }

        if (!alert_types.empty()) {
            notify(alert_types);
        }
        else { cout << " No alert sent " << endl; }
    }
};

class EmergencyAlert : public Observer {
public:
    void update(AlertSystem* alert_system, vector<string> alert_types) {
        cout << "Emergency alert sent about ";

        for (size_t i = 0; i < alert_types.size(); i++) {
            if (i == alert_types.size() - 1) {
                cout << "and ";
            }

            cout << alert_types[i];

            if (i != alert_types.size() - 1) {
                cout << ", ";
            }
        }

        cout << " to " << alert_system->parent << " and " << alert_system->emergency_contact << endl;
    }
};

int main() {
    AlertSystem alert_system;
    EmergencyAlert emergency_alert;

    alert_system.attach(&emergency_alert);

    // Test case 1: No alert should be sent
    alert_system.set_child_data("bedroom", 0, 80, 98, 37, "John", "Jane");
    // No alert should be sent

    // Test case 2: Heart rate below 60, oxygen saturation below 92
    alert_system.set_child_data("kitchen", 0, 50, 90, 37, "Jane", "John");
    // Output: Emergency alert sent about heart rate is 50 and oxygen saturation is 90 to Jane and John

    // Test case 3: Temperature above 39, child near the oven
    alert_system.set_child_data("oven", 0, 80, 98, 40, "Jane", "John");
    // Output: Emergency alert sent about child is near the oven and body temperature is 40 to Jane and John

    // Test case 4: Heart rate above 120, child near a not fully isolated electronic device
    alert_system.set_child_data("electronic device", 0, 130, 98, 37, "Jane", "John");
    // Output: Emergency alert sent about heart rate is 130 and child is near a not fully isolated electronic device to Jane and John

    // Test case 5: Multiple alerts should be sent
    alert_system.set_child_data("living room", 0, 50, 91, 34, "Jane", "John");
    // Output: Emergency alert sent about heart rate is 50, oxygen saturation is 95, and body temperature is 34 to Jane and John

    alert_system.detach(&emergency_alert);

    return 0;
}



