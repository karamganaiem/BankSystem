
# Bank Management System - README

## Overview

This project implements a **Bank Management System** that manages accounts, transactions, and various banking operations using C. It provides functionalities to create, edit, delete accounts, perform transfers, deposits, withdrawals, and view transaction history. The program employs linked lists for dynamic data handling and supports various user interactions through a command-line interface.

---

## Features

1. **Account Management**:
   - Create new accounts.
   - Edit account details.
   - Delete accounts.
   - View account information.

2. **Transaction Handling**:
   - Deposit and withdraw money.
   - Transfer funds between accounts.
   - View transaction history.

3. **Data Validation**:
   - Ensures account numbers and transaction details are valid before processing.

4. **Dynamic Data Storage**:
   - Uses linked lists for accounts and transactions, allowing efficient memory management and scalability.

---

## Data Structures

### 1. **Node**
A generic linked list node.
```c
typedef struct Node {
    void *data;
    struct Node *next;
} Node;
```

### 2. **Account**
Holds account-specific information.
```c
typedef struct Account {
    unsigned int accountNumber;
    char *accountHolder;
    int balance;
} Account;
```

### 3. **Transaction**
Represents a transfer between two accounts.
```c
typedef struct Transaction {
    unsigned int fromAccount;
    unsigned int toAccount;
    int amount;
} Transaction;
```

### 4. **Bank**
Holds the linked lists of accounts and transactions.
```c
typedef struct Bank {
    Node *accounts;
    Node *transactions;
} Bank;
```

---

## Key Functions

### 1. **Menu Operations**
- **`print_print()`**: Displays the main menu options.
- **`main()`**: Handles user input and routes to respective functionalities.

### 2. **Bank Initialization and Cleanup**
- **`compile_bank_data()`**: Initializes the bank data structure.
- **`free_bank_data(Bank *ourMainBank)`**: Frees memory allocated for accounts and transactions.

### 3. **Account Operations**
- **`register_account(Bank *ourMainBank)`**: Creates a new account.
- **`delete_account(Bank *ourMainBank)`**: Deletes an account.
- **`edit_account(Bank *ourMainBank)`**: Updates account details.
- **`view_account(Bank *ourMainBank)`**: Displays account information.

### 4. **Transaction Handling**
- **`do_withdraw(Bank *ourMainBank, unsigned int accountNumber)`**: Withdraws money from an account.
- **`do_deposit(Bank *ourMainBank, unsigned int accountNumber)`**: Deposits money into an account.
- **`do_transferring(Bank *ourMainBank)`**: Transfers money between accounts.
- **`search_transactions(Bank *ourMainBank, unsigned int accountNumber)`**: Views transactions related to a specific account.

### 5. **Utilities**
- **`scan_string()`**: Reads a string input from the user.
- **`remove_struct(Node *accounts, unsigned int accountNumber)`**: Removes an account from the linked list.
- **`check_if_acc_exists()`**: Checks if an account exists.
- **`turn_string_to_int()`**: Converts a string to an integer.
- **`apply_transactions()`**: Processes a batch of transactions.

---

## Usage

### Compiling and Running
1. **Compile**:
   ```bash
   gcc -o bank_management bank_management.c
   ```
2. **Run**:
   ```bash
   ./bank_management
   ```

### Menu Options
- **`0`**: Exit the program.
- **`1`**: Create a new account.
- **`2`**: Delete an account.
- **`3`**: Update account details.
- **`4`**: Deposit or withdraw money.
- **`5`**: Transfer funds between accounts.
- **`6`**: View account details.

---

## Example Interaction

```plaintext
Choose an option:
0. Exit
1. Create account
2. Delete account
3. Update account
4. Deposit / Withdraw money
5. Transferring
6. View account
> 1
Enter account number: 12345
Enter account holder name: John Doe
Enter initial balance: 5000
Account successfully created!
```

---

## Memory Management

- All data structures are dynamically allocated.
- Ensure proper cleanup using **`free_bank_data()`** to prevent memory leaks.

---

## Future Enhancements

1. Add file-based persistence to save accounts and transactions.
2. Implement user authentication for secure account access.
3. Add advanced filtering and reporting for transaction history.

---

## Contributing

Feel free to contribute by submitting pull requests or suggesting features. All contributions are welcome!

---

## License

This project is open-source and licensed under the MIT License.
