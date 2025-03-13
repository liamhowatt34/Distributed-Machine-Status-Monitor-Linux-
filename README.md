🚀 Distributed Machine Status Monitor (Linux)

📌 Overview

The Distributed Machine Status Monitor is a Linux-based distributed system designed to monitor the status of machines using POSIX message queues and shared memory for inter-process communication. The system consists of three main components:

🔹 DC - Generates machine status messages and sends them to the message queue at random intervals.🔹 DR - Reads messages from the queue, updates the machine state using shared memory, and logs updates.🔹 DX - Periodically modifies machine state data in shared memory to simulate corruption scenarios.

✨ Features

✅ Uses POSIX message queues for asynchronous message passing.✅ Implements shared memory for maintaining a global machine state.✅ Error handling and logging for tracking system events and failures.✅ Ensures proper resource cleanup upon program termination.

🛠️ Prerequisites

This program is designed for Linux and requires the following:

🏗 GCC (GNU Compiler Collection)

📡 POSIX-compliant message queues and shared memory support

🏭 make (for building the project)

⚙️ Installation & Compilation

Clone the repository and build the project:

$ git clone <repository_url>
$ cd distributed-machine-status-monitor
$ make

🚦 Usage

▶️ Start DR

$ ./Common/bin/DR

📌 This component creates and manages the message queue and shared memory.

▶️ Start DC

$ ./DC/bin/DC

📌 Each DC generates machine status messages at randomized intervals and sends them to the queue.

▶️ Start DX (Optional)

$ ./Common/bin/DX

📌 This component injects errors into shared memory to test system robustness.

📂 File Structure

project_root/
│── Common/
│   ├── bin/                # 📌 Contains DR and DX executables
│   ├── inc/
│   │   ├── util.h          # ⚙️ Utility functions
│   │   ├── log.h           # 📝 Logging functions
│   ├── src/
│   │   ├── log.c           # 📝 Logging functions implementation
│   ├── obj/                # 🏗 Object files
│   ├── makefile            # 🔨 Makefile for building common components
│── DC/
│   ├── bin/                # 📌 Contains DC executable
│   ├── inc/
│   │   ├── message.h       # ✉️ Message structure
│   ├── src/
│   │   ├── main.c          # 🎯 DC implementation
│   │   ├── message.c       # ✉️ Message queue handling
│   ├── obj/                # 🏗 Object files
│   ├── makefile            # 🔨 Makefile for DC
│── DR/
│   ├── inc/
│   │   ├── master_list.h   # 📊 Shared memory structure
│   ├── src/
│   │   ├── main.c          # 🎯 DR implementation
│   │   ├── master_list.c   # 📊 Shared memory functions
│   ├── makefile            # 🔨 Makefile for DR
│── DX/
│   ├── inc/
│   │   ├── dx.h            # 🔧 DX definitions
│   ├── src/
│   │   ├── dx.c            # 🎯 DX functions
│   │   ├── main.c          # 🎯 Main entry for DX
│   ├── makefile            # 🔨 Makefile for DX
│── tmp/
│   ├── DC.log              # 📜 Log file for DC
│   ├── DR.log              # 📜 Log file for DR
│   ├── DX.log              # 📜 Log file for DX
│── makefile                # 🔨 Root Makefile for building all components
│── README.md               # 📖 Documentation

🔄 Message Queue and Shared Memory Details

📩 Message Queue: Used to communicate between DC and DR.

🗂️ Shared Memory: Used by DR to store the status of all active machines.

🔄 Synchronization: The system ensures proper access and cleanup of message queues and shared memory.

🛑 Error Handling & Cleanup

✅ Logs errors and status updates in tmp/.✅ Properly releases message queues and shared memory upon termination.✅ Ensures DCs exit gracefully when DR shuts down.
