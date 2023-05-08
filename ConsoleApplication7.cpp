#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Parent {
public:
    string name;
    int phone_number;
};

class EmergencyContact {
public:
    string name;
    int phone_number;
};

class Child {
public:
    string name;
    string latest_location;
    bool latest_crying_instance;
    float latest_heart_rate;
    float latest_oxygen_saturation;
    float latest_body_temperature;
    Parent parent;
    EmergencyContact emergency_contact;
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

    void set_child_data(string location, bool crying_instance, float heart_rate, float oxygen_saturation, float body_temperature, Parent parent, EmergencyContact emergency_contact) {
        latest_location = location;
        latest_crying_instance = crying_instance;
        latest_heart_rate = heart_rate;
        latest_oxygen_saturation = oxygen_saturation;
        latest_body_temperature = body_temperature;
        this->parent = parent;
        this->emergency_contact = emergency_contact;

        vector<string> alert_types;
        if (latest_crying_instance) {
            alert_types.push_back("crying");
        }
        if (latest_heart_rate < 60 || latest_heart_rate > 120) {
            alert_types.push_back("heart rate");
        }
        if (latest_oxygen_saturation < 92) {
            alert_types.push_back("oxygen saturation");
        }
        if (latest_body_temperature < 35 || latest_body_temperature > 39) {
            alert_types.push_back("body temperature");
        }
        if (latest_location == "oven") {
            alert_types.push_back("child is near the oven");
        }

        if (latest_location == "electronic device") {
            alert_types.push_back("child is near a not fully isolated electronic device");
        }

        notify(alert_types);
    }
};

class PhoneAlert : public Observer {
public:
    void update(AlertSystem* alert_system, vector<string> alert_types) {
        cout << "Sending phone alerts: ";
        for (auto alert_type : alert_types) {
            cout << alert_type << " , ";
        }
        cout << endl;
    }
};

class DisplayAlert : public Observer {
public:
    void update(AlertSystem* alert_system, vector<string> alert_types) {
        cout << "Displaying alerts: ";
        for (auto alert_type : alert_types) {
            cout << alert_type << " , ";
        }
        cout << endl;
    }
};

int main() {
    Parent parent;
    parent.name = "John doe";
    parent.phone_number = 123456789;

    EmergencyContact emergency_contact;
    emergency_contact.name = "Jane Doe";
    emergency_contact.phone_number = 987654321;

    Child child;
    child.name = "John Jr";
    child.parent = parent;
    child.emergency_contact = emergency_contact;

    AlertSystem alert_system;
    PhoneAlert phone_alert;
    DisplayAlert display_alert;

    alert_system.attach(&phone_alert);
    alert_system.attach(&display_alert);

    // Test case 1: every safe condition is satsified , no alerts should be triggered
    alert_system.set_child_data("Living Room", 0, 80.0, 98.0, 36.5, parent,emergency_contact);
    // Test case 2: Child is in a dangerous location,  alerts should be triggered
    alert_system.set_child_data("oven", 0, 80.0, 98.0, 36.5, parent, emergency_contact);
    // Test case 3: Child is crying, alerts should be triggered
    alert_system.set_child_data("Living Room", 1, 80.0, 98.0, 36.5, parent, emergency_contact);
    // Test case 4: Child's heart rate is low, alerts should be triggered
    alert_system.set_child_data("Living Room", 0, 50.0, 98.0, 36.5, parent, emergency_contact);
    // Test case 5: Child's oxygen saturation is low, alerts should be triggered
    alert_system.set_child_data("Living Room", 0, 80.0, 90.0, 36.5, parent, emergency_contact);
    // Test case 6: Child's body temperature is high and oxygen saturation is low, alerts should be triggered
    alert_system.set_child_data("Living Room", 0, 80.0, 90.0, 40.0, parent, emergency_contact);
 
    alert_system.detach(&phone_alert);
    alert_system.detach(&display_alert);

    return 0;

}