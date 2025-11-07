#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;


class Patient {
private:
    int  id;
    char name[40];
    int  age;
public:
    Patient() { id = 0; age = 0; name[0] = '\0'; }
    Patient(int i, const char* n, int a) { id = i; age = a; strcpy(name, n); }

    void setData(int i, const char* n)           { id = i; strcpy(name, n); }
    void setData(int i, const char* n, int a)    { id = i; strcpy(name, n); age = a; }

    void show() const { cout << "Patient ID:" << id << " Name:" << name << " Age:" << age << "\n"; }

    int         getId()   const { return id; }
    const char* getName() const { return name; }
    int         getAge()  const { return age; }

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
    cout << "[friend] ID:" << p.getId() << " Name:" << p.getName() << "\n";
}

class Staff {
protected:
    int  staffId;
    char staffName[40];
public:
    static int staffCount;
    Staff() { staffId = 0; staffName[0] = '\0'; ++staffCount; }
    Staff(int id, const char* nm) { staffId = id; strcpy(staffName, nm); ++staffCount; }
    virtual ~Staff() { --staffCount; }

    virtual void show() const {
        cout << "Staff ID:" << staffId << " Name:" << staffName << "\n";
    }
    int  getStaffId()  const { return staffId; }
    const char* getStaffName() const { return staffName; }

    static void showCount() { cout << "Current staff count: " << staffCount << "\n"; }
};
int Staff::staffCount = 0;

class Doctor : public Staff {
private:
    char specialization[30];
    double fee;
public:
    Doctor() : Staff() { specialization[0] = '\0'; fee = 0.0; }
    Doctor(int id, const char* nm, const char* spec, double f) : Staff(id, nm) {
        strcpy(specialization, spec); fee = f;
    }
    void show() const override {
        cout << "Doctor ID:" << staffId << " Name:" << staffName
             << " Spec:" << specialization << " Fee:" << fee << "\n";
    }

    int getId() const { return staffId; }

    friend ofstream& operator<<(ofstream& out, const Doctor& d) {
        out << d.staffId << ' ' << d.staffName << ' ' << d.specialization << ' ' << d.fee << '\n';
        return out;
    }
    friend ifstream& operator>>(ifstream& in, Doctor& d) {
        in >> d.staffId >> d.staffName >> d.specialization >> d.fee;
        return in;
    }
};


class Appointment {
private:
    int  apptId;
    int  patientId;
    int  doctorId;
    char date[16];
public:
    Appointment() { apptId = 0; patientId = 0; doctorId = 0; date[0] = '\0'; }
    Appointment(int a, int p, int d, const char* dt) { apptId = a; patientId = p; doctorId = d; strcpy(date, dt); }

    void show() const {
        cout << "ApptID:" << apptId << " P:" << patientId << " D:" << doctorId << " Date:" << date << "\n";
    }

    int getId() const { return apptId; }

    friend ofstream& operator<<(ofstream& out, const Appointment& a) {
        out << a.apptId << ' ' << a.patientId << ' ' << a.doctorId << ' ' << a.date << '\n';
        return out;
    }
    friend ifstream& operator>>(ifstream& in, Appointment& a) {
        in >> a.apptId >> a.patientId >> a.doctorId >> a.date;
        return in;
    }
};


template <typename T>
struct RequireGetId {

    typedef int (T::*MemberFunPtr)() const;
    static void check() { MemberFunPtr ptr = &T::getId; (void)ptr; }
};

template <typename T, int CAP>
class Repository {
private:
    T   data[CAP];
    int count;
 
    void enforce() const { RequireGetId<T>::check(); }
    int indexOfId(int id) const {
        for (int i = 0; i < count; ++i) {
            if (data[i].getId() == id) return i;
        }
        return -1;
    }
public:
    Repository() : count(0) { enforce(); }

    int  size() const { return count; }
    bool empty() const { return count == 0; }

    const T& at(int idx) const {
        if (idx < 0 || idx >= count) throw "Index out of range";
        return data[idx];
    }
    T& at(int idx) {
        if (idx < 0 || idx >= count) throw "Index out of range";
        return data[idx];
    }

    void add(const T& t) {
        if (count >= CAP) throw "Repository full";
        if (indexOfId(t.getId()) != -1) throw "Duplicate ID";
        data[count++] = t;
    }

    const T& getById(int id) const {
        int i = indexOfId(id);
        if (i == -1) throw "ID not found";
        return data[i];
    }
    T& getById(int id) {
        int i = indexOfId(id);
        if (i == -1) throw "ID not found";
        return data[i];
    }

    void removeAt(int idx) {
        if (idx < 0 || idx >= count) throw "Index out of range";
        for (int i = idx; i < count - 1; ++i) data[i] = data[i + 1];
        --count;
    }

    void save(const char* filename) const {
        ofstream out(filename);
        if (!out) throw "Cannot open file for writing";
        for (int i = 0; i < count; ++i) out << data[i];
        out.close();
    }
    void load(const char* filename) {
        ifstream in(filename);
        if (!in) throw "Cannot open file for reading";
        count = 0;
        T temp;
        while (in >> temp) {
            if (count >= CAP) throw "Repository capacity exceeded while loading";
            data[count++] = temp;
        }
        in.close();
    }
};

void printMenu() {
    cout << "\n=== Hospital (templates + try/catch, minimal headers) ===\n";
    cout << "1 Add Patient\n2 List Patients\n3 Save Patients\n4 Load Patients\n";
    cout << "5 Add Doctor\n6 List Doctors\n7 Schedule Appointment\n8 List Appointments\n";
    cout << "9 Friend Demo (first patient)\n0 Exit\nChoice: ";
}

int main() {

    Repository<Patient,     50> patients;
    Repository<Doctor,      20> doctors;
    Repository<Appointment,100> appts;

    try {
        doctors.add(Doctor(1, "Dr_Ram",  "Cardio",  900.0));
        doctors.add(Doctor(2, "Dr_Sita", "General", 800.0));
    } catch (const char* msg) {
        cout << "[Init Error] " << msg << "\n";
    }

    while (true) {
        printMenu();
        int ch;
        if (!(cin >> ch)) { cin.clear(); cin.ignore(10000, '\n'); cout << "Enter a number.\n"; continue; }
        if (ch == 0) break;

        try {
            if (ch == 1) {
                int id, age; char name[40];
                cout << "Enter id: ";   cin >> id;
                cout << "Enter name(no spaces): "; cin >> name;
                cout << "Enter age: ";  cin >> age;
                Patient p; p.setData(id, name, age); 
                patients.add(p);
                cout << "Patient added.\n";
            }
            else if (ch == 2) {
                cout << "\n-- Patients (" << patients.size() << ") --\n";
                for (int i = 0; i < patients.size(); ++i) patients.at(i).show();
            }
            else if (ch == 3) {
                patients.save("patients.txt");
                cout << "Saved to patients.txt\n";
            }
            else if (ch == 4) {
                patients.load("patients.txt");
                cout << "Loaded patients.txt\n";
            }
            else if (ch == 5) {
                int id; char nm[40]; char spec[30]; double fee;
                cout << "Enter doctor id: "; cin >> id;
                cout << "Enter name(no spaces): "; cin >> nm;
                cout << "Enter specialization(no spaces): "; cin >> spec;
                cout << "Enter fee: "; cin >> fee;
                doctors.add(Doctor(id, nm, spec, fee));
                cout << "Doctor added.\n";
            }
            else if (ch == 6) {
                cout << "\n-- Doctors (" << doctors.size() << ") --\n";
                for (int i = 0; i < doctors.size(); ++i) doctors.at(i).show();
            }
            else if (ch == 7) {
                if (patients.size() == 0 || doctors.size() == 0) throw "Need at least one patient and one doctor";
                int apid, pid, did; char dt[16];
                cout << "Appt id: "; cin >> apid;
                cout << "Patient id: "; cin >> pid;
                cout << "Doctor id: "; cin >> did;
                cout << "Date (YYYY-MM-DD): "; cin >> dt;
                
                (void)patients.getById(pid);
                (void)doctors.getById(did);

                appts.add(Appointment(apid, pid, did, dt));
                cout << "Appointment scheduled.\n";
            }
            else if (ch == 8) {
                cout << "\n-- Appointments (" << appts.size() << ") --\n";
                for (int i = 0; i < appts.size(); ++i) appts.at(i).show();
            }
            else if (ch == 9) {
                if (patients.size() == 0) cout << "No patients.\n";
                else showPrivate(patients.at(0));
            }
            else {
                cout << "Unknown choice.\n";
            }
        }
        catch (const char* msg) {
            cout << "[Error] " << msg << "\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
