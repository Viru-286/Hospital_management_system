# Hospital Management System (Advanced C++ Version)

## ✅ Features

### 👤 Patient Management

* Add patient
* List patients
* Save patients to file
* Load patients from file

### 🩺 Doctor Management

* Add doctor
* List doctor details
* Includes specialization + consultation fee

### 📅 Appointment System

* Schedule appointments between patients and doctors
* Validate IDs using template repository

### 🛡 Error Handling

* All user input and operations protected using `try/catch`
* Errors thrown using simple `throw "message";`

### 🧩 Template Repository (Custom Made)

A template class:

```
template <typename T, int CAP>
class Repository;
```

Stores items without vectors — uses a raw array internally.

### 🔒 Compile-Time Check

Ensures every stored type has `getId()` using minimal template checks.

---

## 📁 File Structure

```
Hospital_Management/
 ├── hospital_templates_trycatch_min.cpp   # Main Source Code
 ├── patients.txt                          # Auto-saved data file
 ├── Hospital Advanced Report              # Full Report Document
 └── README.md                             # This File
```

---

## 🛠️ How to Compile & Run

### **Compile**

```
g++ -std=c++17 hospital_templates_trycatch_min.cpp -o hospital
```

### **Run**

```
./hospital
```

---

## 📜 Menu Structure

```
=== Hospital (templates + try/catch) ===
1 Add Patient
2 List Patients
3 Save Patients
4 Load Patients
5 Add Doctor
6 List Doctors
7 Schedule Appointment
8 List Appointments
9 Friend Demo
0 Exit
```

---
