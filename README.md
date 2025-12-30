# 🏥 Healthcare Operating System (HealthcareOS)

> A user-level operating system simulator designed for hospital environments, built on Ubuntu Linux using C and Bash.

## 📖 Overview
HealthcareOS is a customized shell environment that simulates critical hospital operations. [cite_start]Unlike general-purpose operating systems, it is designed to prioritize **patient data management**, **monitoring processes**, and **system reliability**[cite: 160].

This project demonstrates core Operating System concepts including:
* [cite_start]**Process Management:** Creation (`fork`), control (`signals`), and cleanup (`waitpid`)[cite: 166].
* [cite_start]**Memory Management:** Real-time inspection of Heap, Stack, and Data segments[cite: 167].
* [cite_start]**File Systems:** Structured patient record storage using Linux file IO[cite: 236].
* [cite_start]**System Safety:** Input validation and controlled shutdown procedures[cite: 169].

## ⚙️ Architecture
The system follows a layered architecture running in user space:
[cite_start]**User** → **HealthcareOS Shell (C)** → **Bash Scripts & System Calls** → **Ubuntu Linux Kernel** [cite: 177-184].

## 🚀 Features

### 1. Process Management (Vitals Monitoring)
* [cite_start]**Multitasking:** Spawns background processes to simulate patient vital monitoring using `fork()`[cite: 194].
* [cite_start]**Signal Handling:** Uses `SIGSTOP`, `SIGCONT`, and `SIGTERM` to pause, resume, or stop monitoring processes [cite: 202-205].
* [cite_start]**Zombie Prevention:** Implements `waitpid()` to ensure no orphaned processes remain after shutdown[cite: 210].

### 2. Patient Data Management
* [cite_start]**CRUD Operations:** Create, View, List, and Delete patient records[cite: 237].
* [cite_start]**Persistent Storage:** Data is stored in a structured directory system (`Patients/ID_Name/`)[cite: 236].
* [cite_start]**Search:** Built-in search utility to find records by name[cite: 237].

### 3. System & Memory Analysis
* [cite_start]**Memory Map:** A dedicated tool (`mem`) that compiles and runs to visualize the exact memory addresses of the Text, Data, BSS, Heap, and Stack segments [cite: 216-220].
* [cite_start]**Resource Monitoring:** Real-time checking of Disk Usage (`df`) and RAM health (`vmstat`)[cite: 223, 225].

## 🛠️ Tech Stack
* [cite_start]**OS:** Ubuntu Linux [cite: 171]
* [cite_start]**Core Logic:** C (Standard C99) [cite: 172]
* [cite_start]**Automation:** Bash Scripting [cite: 172]
* [cite_start]**Compiler:** GCC [cite: 173]

## 💻 Installation & Usage

### 1. Clone the Repository
```bash
git clone [https://github.com/YOUR_USERNAME/healthcare-os-simulator.git](https://github.com/YOUR_USERNAME/healthcare-os-simulator.git)
cd healthcare-os-simulator
