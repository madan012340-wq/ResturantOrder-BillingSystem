# Restaurant Management & Ordering System

A C++ console application designed to handle customer food/beverage ordering and staff management for a restaurant. The program supports dual-role authentication (Customer vs. Staff), order placing and cancellation, menu expansion, persistent file storage, and sales report analytics.

---

## Features

- **Authentication & Role Selection**:
  - **Customer Access**: Enter ID `101` for simple ordering.
  - **Staff/Management Access**: Secured with ID (`102`) and password (`2222`). Includes a 3-attempt lock protection.
- **Customer Portal**:
  - View food & beverage menu with pricing.
  - Place multi-item orders recorded with automatic timestamps.
  - Cancel the last placed order.
  - View current order summary and print final bill at checkout.
- **Management Portal**:
  - **Add Menu Items**: Dynamically add new food items and prices to the menu.
  - **Sales Records**: View complete order history across all customers.
  - **Customer Lookup**: Search total spending and order history by Customer ID.
  - **Income Analytics**: Filter revenue reports by specific Day, Month, or Year.
- **Data Persistence**: Uses flat text files (`orders.txt` and `extra_items.txt`) to persist orders and added menu items across program restarts.

---

## File Structure & Storage

The program auto-generates/reads the following local text files:

- `orders.txt`: Stores historical transaction logs in the format:  
  `Day Month Year CustomerID ItemCode ItemName Quantity Price TotalCost`
- `extra_items.txt`: Stores menu additions created dynamically by management in the format:  
  `ItemCode ItemName Price`

---

## System Requirements & Prerequisites

- **Language**: C++11 or higher
- **Compiler**: Any standard C++ compiler (`g++`, `clang++`, or MSVC)
- **OS**: Windows, macOS, or Linux

---

## Compilation & Execution

### Using GCC / Clang (Terminal / Command Prompt)

1. Compile the source file:
   ```bash
   g++ -std=c++11 main.cpp -o restaurant_system
   ```

2. Run the compiled executable:
   - **Linux / macOS**:
     ```bash
     ./restaurant_system
     ```
   - **Windows**:
     ```cmd
     restaurant_system.exe
     ```

---

## Usage Guide

### Default Credentials

| Role | User ID | Password | Access Capabilities |
| :--- | :--- | :--- | :--- |
| **Customer** | `101` | *None* | View menu, place/cancel orders, view summary, checkout bill |
| **Staff / Admin** | `102` | `2222` | Add menu items, search customer sales, view daily/monthly/yearly income |

---

### Menu Workflow Overview

#### 1. Customer Interface (ID: `101`)
1. **View Menu**: Lists all default items (e.g., Momo, Chowmein, Thakali Sets, Beverages) alongside any custom-added items.
2. **Take Order**: Prompt for item code and quantity. Automatically updates `orders.txt`.
3. **Cancel Order**: Removes the most recent order line for your session from both memory and `orders.txt`.
4. **View Current Order**: Displays items ordered in the active session and subtotal cost.
5. **Print Bill & Checkout**: Outputs total receipt and exits customer workflow.

#### 2. Staff Interface (ID: `102`, Password: `2222`)
1. **Add new menu item**: Enter item code, single-word name (e.g., `FriedRice`), and price. Saves to `extra_items.txt`.
2. **Check sold foods**: Prints all logged orders and cumulative grand total income.
3. **Search customer**: Input Customer ID to view their lifetime order history and total spend.
4. **Daily income**: Input `Day`, `Month`, `Year` (e.g., `5 10 2026`) to filter total daily revenue.
5. **Monthly income**: Input `Month` and `Year` to calculate monthly total earnings.
6. **Yearly income**: Input `Year` to calculate annual revenue.