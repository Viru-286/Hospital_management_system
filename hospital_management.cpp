#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


class Patient {
private:
    int id;
    char name[40];
    int age;
public:

    Patient() { id = 0; age = 0; name[0] = '\0'; }

    Patient(int i, const char* n, int a) { id = i; age = a; strcpy(name, n); }

    ~Patient() {  }


    void setData(int i, const char* n) { id = i; strcpy(name, n); }
    void setData(int i, const char* n, int a) { id = i; strcpy(name, n); age = a; } 

   
    void show() const { cout << "Patient ID:" << id << " Name:" << name << " Age:" << age << "\n"; }

    int getId() const { return id; }
    const char* getName() const { return name; }
    int getAge() const { return age; }


    friend void showPrivate(const Patient& p);


    friend ofstream& operator<<(ofstream& out, const Patient& p) {
        out << p.id << ' ' << p.name << ' ' << p.age << '\n';
        return out;
    }
    friend ifstream& operator>>(ifstream& in, Patient& p) {
        in >> p.id >> p.name >> p.age;
        return in;
    }
};

void showPrivate(const Patient& p) {
    cout << "Friend reads -> ID:" << p.id << " Name:" << p.name << "\n";
}


class Staff {
protected:
    int staffId;
    char staffName[40];
public:
    static int staffCount; 
    Staff() { staffId = 0; staffName[0] = '\0'; staffCount++; }
    Staff(int id, const char* nm) { staffId = id; strcpy(staffName, nm); staffCount++; }
    virtual ~Staff() { staffCount--; }

    virtual void show() const { cout << "Staff ID:" << staffId << " Name:" << staffName << "\n"; }

   
    int getStaffId() const { return staffId; }

    static void showCount() { cout << "Current staff count: " << staffCount << "\n"; }
};
int Staff::staffCount = 0;

class Doctor : public Staff {
private:
    char specialization[30];
public:
    Doctor() : Staff() { specialization[0] = '\0'; }
    Doctor(int id, const char* nm, const char* spec) : Staff(id, nm) { strcpy(specialization, spec); }
    virtual ~Doctor() {}


    void show() const override {
        cout << "Doctor ID:" << staffId << " Name:" << staffName << " Spec:" << specialization << "\n";
    }


    friend ofstream& operator<<(ofstream& out, const Doctor& d) {
        out << d.staffId << ' ' << d.staffName << ' ' << d.specialization << '\n';
        return out;
    }
    friend ifstream& operator>>(ifstream& in, Doctor& d) {
        in >> d.staffId >> d.staffName >> d.specialization;
        return in;
    }
};


class Appointment {
private:
    int apptId;
    int patientId;
    int doctorId;
    char date[16];
public:
    Appointment() { apptId = 0; patientId = 0; doctorId = 0; date[0] = '\0'; }
    Appointment(int a, int p, int d, const char* dt) { apptId = a; patientId = p; doctorId = d; strcpy(date, dt); }
    void show() const { cout << "ApptID:" << apptId << " P:" << patientId << " D:" << doctorId << " Date:" << date << "\n"; }

    friend ofstream& operator<<(ofstream& out, const Appointment& a) {
        out << a.apptId << ' ' << a.patientId << ' ' << a.doctorId << ' ' << a.date << '\n';
        return out;
    }
    friend ifstream& operator>>(ifstream& in, Appointment& a) {
        in >> a.apptId >> a.patientId >> a.doctorId >> a.date;
        return in;
    }
};


const int MAX_P = 50;
const int MAX_D = 20;
const int MAX_A = 100;


void printMenu() {
    cout << "\n--- Hospital Menu ---\n";
    cout << "1 Add Patient\n2 List Patients\n3 Save Patients\n4 Load Patients\n";
    cout << "5 Show Doctors\n6 Add Appointment\n7 List Appointments\n8 Friend demo (show first patient priv)\n9 Show staff count\n0 Exit\n";
    cout << "Choice: ";
}

int main() {

    Patient* patients = new Patient[MAX_P];
    Doctor* doctors = new Doctor[MAX_D];
    Appointment* appts = new Appointment[MAX_A];
    int pCount = 0, dCount = 0, aCount = 0;

   
    doctors[dCount++] = Doctor(1, "Dr_Ram", "Cardio");
    doctors[dCount++] = Doctor(2, "Dr_Sita", "General");

    while (true) {
        printMenu();
        int ch;
        if (!(cin >> ch)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Enter number.\n"; continue; }
        if (ch == 0) break;

        if (ch == 1) {
            if (pCount >= MAX_P) { cout << "Patient list full.\n"; continue; }
            int id, age; char name[40];
            cout << "Enter id: "; cin >> id;
            cout << "Enter name(no spaces): "; cin >> name;
            cout << "Enter age: "; cin >> age;
            patients[pCount++].setData(id, name, age);
            cout << "Patient added.\n";
        }
        else if (ch == 2) {
            cout << "\nPatients:\n";
            for (int i = 0; i < pCount; ++i) patients[i].show();
        }
        else if (ch == 3) {
            ofstream fout("patients.txt");
            if (!fout) { cout << "Cannot open file.\n"; continue; }
            for (int i = 0; i < pCount; ++i) fout << patients[i];
            fout.close();
            cout << "Saved patients to patients.txt\n";
        }
        else if (ch == 4) {
            ifstream fin("patients.txt");
            if (!fin) { cout << "patients.txt not found.\n"; continue; }
            pCount = 0;
            while (fin >> patients[pCount]) {
                ++pCount;
                if (pCount >= MAX_P) break;
            }
            fin.close();
            cout << "Loaded patients.\n";
        }
        else if (ch == 5) {
            cout << "\nDoctors:\n";
            for (int i = 0; i < dCount; ++i) doctors[i].show();
        }
        else if (ch == 6) {
            if (aCount >= MAX_A) { cout << "Appointments full.\n"; continue; }
            if (pCount == 0) { cout << "No patients yet.\n"; continue; }
            int apid, pidx, didx; char date[16];
            cout << "Enter appt id: "; cin >> apid;
            cout << "Choose patient index (0 to " << pCount-1 << "): "; cin >> pidx;
            cout << "Choose doctor index (0 to " << dCount-1 << "): "; cin >> didx;
            cout << "Date (YYYY-MM-DD): "; cin >> date;
            if (pidx < 0 || pidx >= pCount || didx < 0 || didx >= dCount) { cout << "Invalid index.\n"; continue; }
            appts[aCount++] = Appointment(apid, patients[pidx].getId(), doctors[didx].getStaffId(), date);
            cout << "Appointment added.\n";
        }
        else if (ch == 7) {
            cout << "\nAppointments:\n";
            for (int i = 0; i < aCount; ++i) appts[i].show();
        }
        else if (ch == 8) {
            if (pCount == 0) cout << "No patients.\n"; else showPrivate(patients[0]);
        }
        else if (ch == 9) {
            Staff::showCount();
        }
        else {
            cout << "Unknown choice.\n";
        }
    }

    
    delete[] patients;
    delete[] doctors;
    delete[] appts;

    cout << "Goodbye!\n";
    return 0;
}
