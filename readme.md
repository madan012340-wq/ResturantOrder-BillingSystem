# Restaurant Management & Ordering System

A comprehensive C++ console application designed to handle customer food/beverage ordering and staff menu/sales management. The system features role-based authentication, strict phone-number verification, persistent menu management, order cancellation, and detailed financial reporting.

---

## Key Features

###  Authentication & Access Control
* **Customer Access**: Access using key ID `101` with zero password requirement.
* **Staff/Management Access**: Access using staff ID `102` and password `2222` with a 3-attempt lock mechanism.

###  Customer Interface
* **Menu Viewing**: Browse food items and prices loaded dynamically from local storage.
* **Order Placement**: Select items by code and set quantities with automatic real-time date tagging.
* **Order Cancellation**: Cancel the most recent active order placed in the current session.
* **Phone Verification & Checkout**: Validates a 10-digit phone number (must start with `98`) upon checkout, attaches it to the session's transaction log, and prints a finalized bill.

###  Staff & Management Interface
* **Menu Editing & Management**:
  * View current active menu.
  * Add new menu items (item code, name, price).
  * Edit existing menu items (rename, update price, or delete).
* **Sales Analysis & Reports**:
  * **Sales Log**: View all historical transactions with date, phone number, and items purchased.
  * **Customer Lookup**: Search full order histories using a customer's phone number.
  * **Income Breakdown**: Generate daily, monthly, and yearly income reports with per-customer spending summaries.

---

## Data & Storage Files

The system automatically manages two flat text files for persistent storage:

1. `menu.txt`: Stores the current menu configuration (`ItemCode ItemName Price`). If missing, the application generates a default menu with 21 items upon startup.
2. `orders.txt`: Records transaction details line-by-line:
   ```text
   Day Month Year CustomerID ItemCode ItemName Quantity UnitPrice TotalCost Phone
   ```

---

## System Requirements & Prerequisites

* **Language Standard**: C++11 or higher
* **Compiler**: `g++`, `clang++`, MSVC, or any standard C++ compiler
* **Operating System**: Cross-platform (Windows, Linux, macOS)

---

## Compilation & Running

### Using GCC / Clang (Terminal / Command Prompt)

1. Compile the program:
   ```bash
   g++ -std=c++11 project_sem2.cpp -o restaurant_system
   ```

2. Run the executable:
   * **Linux / macOS**:
     ```bash
     ./restaurant_system
     ```
   * **Windows**:
     ```cmd
     restaurant_system.exe
     ```

---

## Credentials & Quick Reference

| Role | User ID | Password | Main Features |
| :--- | :--- | :--- | :--- |
| **Customer** | `101` | *None* | View Menu, Order, Cancel Last Order, Phone Checkout |
| **Management** | `102` | `2222` | Edit/Add Menu Items, View Sales Logs, Search by Phone, Revenue Reports |

---

## Menu Workflow Overview

### Management Menu Options
1. **View menu**: Display active menu.
2. **Add new menu item**: Append a new dish to `menu.txt`.
3. **Edit menu**: Rename, change price, or delete an existing item code.
4. **View all sales logs**: Display raw order history.
5. **Daily income report**: Enter day, month, and year to see daily total and per-customer breakdown.
6. **Monthly income report**: Enter month and year to see monthly total revenue.
7. **Yearly income report**: Enter year to review full annual revenue.
8. **Search customer by phone**: View total lifetime spend and item breakdown for a given phone number.
