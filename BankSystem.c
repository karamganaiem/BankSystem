#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WITHDRAW 98765

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct Account {
    unsigned int accountNumber;
    char *accountHolder;
    int balance;
} Account;

typedef struct Transaction {
    unsigned int fromAccount;
    unsigned int toAccount;
    int amount;
} Transaction;

typedef struct Bank {
    Node *accounts;
    Node *transactions;
} Bank;


void print_print();

Bank *compile_bank_data();

void free_bank_data(Bank *ourMainBank);

void register_account(Bank *ourMainBank);

void delete_account(Bank *ourMainBank);

void edit_account(Bank *ourMainBank);

void do_mission(Bank *ourMainBank);

void do_transferring(Bank *ourMainBank);

void view_account(Bank *ourMainBank);

char *scan_string();

Node *remove_struct(Node *accounts, unsigned int accountNumber);

int check_if_acc_exists(Bank *ourMainBank, unsigned int accountNumber);

int check_if_input_is_number(char *amountInString);

int turn_string_to_int(const char *string);

void search_transactions(Bank *ourMainBank, unsigned int accountNumber);

int apply_transactions(Bank *ourMainBank, char *instructions);

Node *split_string_to_ints(char *transactionsInput);

Account *find_acc(Bank *ourMainBank, unsigned int accountNumber);

Account *find_acc_for_copy(Node *node, unsigned int accountNumber);

Account *copy_account(Account *account);

int check_if_transactions_are_valid(Bank *ourMainBank, Node *transactions);

void do_withdraw(Bank *ourMainBank, unsigned int accountNumber);

void do_deposit(Bank *ourMainBank, unsigned int accountNumber);

void copy_accounts_for_valid(Bank *ourMainBank, unsigned int accountNumber,
                             Node **head, Node **temp);

int main() {
    char mainMenuInput;
    // we initalize our data structs
    Bank *ourMainBank = compile_bank_data();
    do {
        print_print();
        //we scan the input from the user
        scanf(" %c", &mainMenuInput);
        //we switch to options
        switch (mainMenuInput) {
            case '0':
//if the user inputs zero,clean data structures and terminate the execution
                free_bank_data(ourMainBank);
                return 0;
            case '1':
                register_account(ourMainBank);
                break;
            case '2':
                delete_account(ourMainBank);
                break;
            case '3':
                edit_account(ourMainBank);
                break;
            case '4':
                do_mission(ourMainBank);
                break;
            case '5':
                do_transferring(ourMainBank);
                break;
            case '6':
                view_account(ourMainBank);
                break;
            default:
                //if the user inputs invalid option we print:
                printf("Invalid option\n");
                break;
        }
    } while (mainMenuInput != '0');
    return 0;
}

/**
 * this function prints out the main menu
 * */
void print_print() {
    //We print the main menu
    printf("Choose an option:\n"
           "0. Exit\n"
           "1. Create account\n"
           "2. Delete account\n"
           "3. Update account\n"
           "4. Deposit / Withdraw money\n"
           "5. Transferring\n"
           "6. View account\n");
}

/**
 * Registers a new account in the bank.
 *
 * @param ourMainBank Pointer to the Bank structure.
 */
void register_account(Bank *ourMainBank) {
    // Prompt the user to enter an account number
    printf("Enter account number:\n");
    unsigned int accountNumber;
    scanf(" %u", &accountNumber);

    // Check if the account number is valid and doesn't already exist
    if ((accountNumber != 0) &&
        (check_if_acc_exists(ourMainBank, accountNumber) == 0)) {
        // Prompt the user to enter the account holder's name
        printf("Enter account holder:\n");
        char *name = scan_string();

        // Create a new Account structure
        Account *newAccount = (Account *) malloc(sizeof(Account));
        newAccount->accountNumber = accountNumber;
        newAccount->accountHolder = name;
        newAccount->balance = 0;

        // Create a new Node for the account
        Node *newNode = (Node *) malloc(sizeof(Node));
        newNode->data = newAccount;
        newNode->next = NULL;

        // Add the new account to the bank's list of accounts
        if (ourMainBank->accounts == NULL) {
            ourMainBank->accounts = newNode;
        } else {
            Node *current = ourMainBank->accounts;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
        // Print success message
        printf("Account created successfully\n");
    } else {
        // Print error message if account number already exists
        printf("Account number already exists\n");
    }
}

/**
 * Deletes an account from the bank.
 *
 * @param ourMainBank Pointer to the Bank structure.
 */
void delete_account(Bank *ourMainBank) {
    // Prompt the user to enter an account number
    printf("Enter account number:\n");
    unsigned int accountNumber;
    scanf(" %u", &accountNumber);

    // Check if the account number is valid and exists
    if ((accountNumber != 0) &&
        (check_if_acc_exists(ourMainBank, accountNumber) == 1)) {
        // Remove the account from the bank's list of accounts
        ourMainBank->accounts =
                remove_struct(ourMainBank->accounts, accountNumber);

        // Print success message
        printf("Account deleted successfully\n");
    } else {
        // Print error message if account not found
        printf("Account not found\n");
    }
}

/**
 * Edits the account holder's name for a given account number.
 *
 * @param ourMainBank Pointer to the Bank structure.
 */
void edit_account(Bank *ourMainBank) {
    // Prompt the user to enter an account number
    printf("Enter account number:\n");
    unsigned int accountNumber;
    scanf(" %u", &accountNumber);

    // Check if the account exists and the account number is valid
    if ((check_if_acc_exists(ourMainBank, accountNumber) == 1) &&
        (accountNumber != 0)) {
        // Prompt the user to enter the new account holder's name
        printf("Enter new account holder:\n");
        char *editedName = scan_string();

        // Find the account based on the account number
        Account *editedAcc = find_acc(ourMainBank, accountNumber);

        // Update the account holder's name
        editedAcc->accountHolder = editedName;
    } else {
        // Print error message if account not found
        printf("Account not found\n");
    }
}

/**
 * Perform a mission related to depositing or withdrawing money from an account.
 *
 * @param ourMainBank Pointer to the Bank structure.
 */
void do_mission(Bank *ourMainBank) {
    // Prompt the user to enter an account number
    printf("Enter account number:\n");
    unsigned int accountNumber;
    scanf(" %u", &accountNumber);

    // Check if the account exists and the account number is valid
    if ((check_if_acc_exists(ourMainBank, accountNumber) == 0) ||
        (accountNumber == 0)) {
        // Print error message if account not found or invalid account number
        printf("Account not found\n");
        return;
    }

    // Prompt the user to choose between deposit and withdraw
    printf("Would you like to deposit or withdraw money?\n");
    char *userStringInput = scan_string();
    char deposit[8] = "deposit";
    char withdraw[9] = "withdraw";

    // Check if the user chose to deposit money
    if (strcmp(userStringInput, deposit) == 0) {
        do_withdraw(ourMainBank, accountNumber);
    }
        // Check if the user chose to withdraw money
    else if (strcmp(userStringInput, withdraw) == 0) {
        do_deposit(ourMainBank, accountNumber);
    } else {
        // Print error message for invalid action
        printf("Invalid action\n");
        return;
    }
}

/**
 * View the details of a specific account.
 */
void view_account(Bank *ourMainBank) {
    // Prompt the user to enter the account number
    printf("Enter account number:\n");
    unsigned int accountNumber;
    scanf(" %u", &accountNumber);

    // Check if the account exists
    if (check_if_acc_exists(ourMainBank, accountNumber) != 1) {
        // Print error message for account not found
        printf("Account not found\n");
        return;
    } else {
        // Retrieve the account details
        Account *currentAccount = find_acc(ourMainBank, accountNumber);
        // Print the account information
        printf("Account #%d (%s)\n", currentAccount->accountNumber,
               currentAccount->accountHolder);
        printf("Balance: %d\n", currentAccount->balance);
        //printf("Transactions:\n");
        // Search and display transactions for the account
        search_transactions(ourMainBank, accountNumber);
    }
}

/**
 * @brief Scan a string input from the user.
 *
 * reads a string input from the user until a newline character is encountered.
 *
 * @return A dynamically allocated string containing the user's input.
 */
char *scan_string() {
    // Allocate initial memory
    char *fullName = (char *) malloc(sizeof(char));
    int index = 0;
    char input;
    scanf(" %c", &input);

    // Read characters until newline is encountered
    while (input != '\n') {
        fullName[index] = input;
        // Reallocate memory to accommodate the new character
        fullName = (char *) realloc(fullName, index + 2);
        // Read the next character
        scanf("%c", &input);
        index++;
    }
    // Add null terminator at the end of the string
    fullName[index] = '\0';
    return fullName;
}

/**
 * @brief Remove a node from a linked list based on the account number.
 *
 * removes a node from a linked list of structures (accounts) based on the
 * specified account number.
 *
 * @param accounts The head of the linked list.
 * @param accountNumber The account number to search for and remove.
 * @return The updated head of the linked list after removing the node.
 */
Node *remove_struct(Node *accounts, unsigned int accountNumber) {
    if (accounts == NULL) {
        // If the list is empty, nothing to remove
        return NULL;
    }

    if (((Account *) accounts->data)->accountNumber == accountNumber) {
        Node *temporaryNode = accounts;
        accounts = accounts->next;
        // Remove the first node and free memory
        free(temporaryNode);
        // Return the updated head of the list
        return accounts;
    } else {
        Node *current = accounts;
        Node *previous = NULL;

        while (current != NULL) {
            if (((Account *) current->data)->accountNumber == accountNumber) {
                // Skip the current node in the list
                previous->next = current->next;
                // Free memory for the removed node
                free(current);
                // Return the updated head of the list
                return accounts;
            }
            previous = current;
            current = current->next;
        }
        // Account not found, return the original list
        return accounts;
    }
}

/**
 * @brief Check if an account with the account number exists in the bank.
 *
 * traverses the linked list of accounts in the bank and checks if an account
 * with the given account number exists.
 *
 * @param ourMainBank The bank structure containing the linked list of accounts.
 * @param accountNumber The account number to search for.
 * @return 1 if an account with the specified account number exists,0 otherwise
 */
int check_if_acc_exists(Bank *ourMainBank, unsigned int accountNumber) {
    // Start from the head of the accounts linked list
    Node *current = ourMainBank->accounts;

    while (current != NULL) {
        if (((Account *) current->data)->accountNumber == accountNumber) {
            // Account with the given account number exists
            return 1;
        }
        // Move to the next node
        current = current->next;
    }
    // Account not found
    return 0;
}

/**
 * Find and return the account with the specified account number in the bank.
 * Returns a pointer to the Account structure if found, or NULL if not found.
 */
Account *find_acc(Bank *ourMainBank, unsigned int accountNumber) {
    // Start from the head of the accounts linked list
    Node *current = ourMainBank->accounts;
    while (current != NULL) {
        if (((Account *) current->data)->accountNumber == accountNumber) {
            // Return the account with the matching account number
            return (Account *) current->data;
        }
        // Move to the next node
        current = current->next;
    }
    // Account not found
    return NULL;
}

/**
 * @brief Check if a string represents a valid number.
 *
 * This function checks if the provided string contains only  digits and
 * represents a valid number.
 *
 * @param amountInString The string to check.
 * @return 1 if the string represents a valid number, 0 otherwise.
 */
int check_if_input_is_number(char *amountInString) {
    // Get the length of the string
    unsigned long long length = strlen(amountInString);
    for (length = length - 1; length > 0; --length) {
        if ((amountInString[length] >= '0') &&
            (amountInString[length] <= '9')) {
            // Continue checking the next character if it is a digit
            continue;
        } else {
            // Return 0 if a non-digit character is encountered
            return 0;
        }
    }
    // All characters are digits, return 1 indicating a valid number
    return 1;
}

/**
 * @brief Convert a string to an integer.
 *
 * This function converts the provided string to an integer.
 *
 * @param string The string to convert.
 * @return The converted integer value, or 0 if an error occurs or the
 * string is not a valid number.
 */
int turn_string_to_int(const char *string) {
    int result = 0;
    int sign = 1;
    int i = 0;
    // Process each character of the string
    while (string[i] != '\0') {
        // Ensure the character is a valid digit
        if (string[i] >= '0' && string[i] <= '9') {
            // Convert the character to an integer and update the result
            result = result * 10 + (string[i] - '0');
            i++;
        } else {
            // Invalid character found, return 0 or handle error as desired
            return 0;
        }
    }
    // Return the final result with the sign applied
    return sign * result;
}

/**
 * @brief Compile bank data and initialize a Bank structure.
 *
 * This function allocates memory for a Bank structure, initializes the
 * accounts and transactions pointers to NULL,
 *
 * @return The pointer to the initialized Bank structure.
 */
Bank *compile_bank_data() {
    // Allocate memory for the Bank structure
    Bank *ourBank = (Bank *) malloc(sizeof(Bank));

    // Initialize the accounts and transactions pointers to NULL
    ourBank->accounts = NULL;
    ourBank->transactions = NULL;

    // Return the pointer to the initialized Bank structure
    return ourBank;
}

/**
 * @brief Splits a string of transaction instructions into individual
 * transactions.
 *
 * This function takes a string of transaction instructions and splits it
 * into individual transactions. Each transaction is represented by a Node in
 * a linked list.
 *
 * @param transactionsInput The string containing transaction instructions.
 * @return A pointer to the head of the linked list of transactions,
 * or NULL if the input is invalid.
 */
Node *split_string_to_ints(char *transactionsInput) {
    // Linked list to store transactions
    Node *transactions = NULL;
    Node *current = NULL;

    // Tokenize the input string to extract individual transactions
    char *token = strtok(transactionsInput, "-");
    while (token != NULL) {
        // Extract the "from" account
        char *from = token;
        if (from == NULL) {
            return NULL;
        }
        // Extract the "to" account
        char *to = strtok(NULL, ":");
        if (to == NULL) {
            return NULL;
        }
        // Extract the amount
        char *amount = strtok(NULL, ",");
        if (amount == NULL) {
            return NULL;
        }
        // Convert strings to integers
        int fromId = check_if_input_is_number(from);
        int toId = check_if_input_is_number(to);
        int theAmount = check_if_input_is_number(amount);
        if ((fromId == 0) || (theAmount == 0) || (toId == 0)) {
            return NULL;
        }
        int realAmount = turn_string_to_int(amount);
        unsigned int realFromId = turn_string_to_int(from);
        unsigned int realToId = turn_string_to_int(to);

        // Create a new transaction object
        Transaction *newTransaction = malloc(sizeof(Transaction));
        newTransaction->amount = realAmount;
        newTransaction->fromAccount = realFromId;
        newTransaction->toAccount = realToId;

        // Create a new node for the transaction
        Node *newNode = malloc(sizeof(Node));
        newNode->data = newTransaction;
        newNode->next = NULL;

        // Add the new node to the linked list
        if (transactions == NULL) {
            transactions = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            current = current->next;
        }

        // Move to the next transaction
        token = strtok(NULL, "-");
    }
    return transactions;
}

/**
 * @brief Creates a copy_account of an Account object.
 *
 * creates a copy_account of the given Account object.
 *
 * @param account The original Account object to be copied.
 * @return Account* A pointer to the newly created copy_account of the Account object.
 */
Account *copy_account(Account *account) {
    // Allocate memory for the copy_account
    Account *copy = malloc(sizeof(Account));

    // Copy the account number and balance from the original account
    copy->accountNumber = account->accountNumber;
    copy->balance = account->balance;

    // Set the account holder of the copy_account to NULL
    copy->accountHolder = NULL;

    // Return the newly created copy_account
    return copy;
}

/**
 * @brief Find an account with the specified account number for copying.
 *
 * This function searches through the linked list of accounts to find an account
 * with the account number, If found, it returns a pointer to the account.
 * If not found, it returns NULL.
 *
 * @param node The head node of the linked list to search.
 * @param accountNumber The account number to search for.
 * @return A pointer to the found account, or NULL if not found.
 */
Account *find_acc_for_copy(Node *node, unsigned int accountNumber) {
    while (node != NULL) {
        // Check if the account number matches
        if (((Account *) node->data)->accountNumber == accountNumber) {
            // Account found, return the pointer
            return (Account *) node->data;
        }
        node = node->next;
    }
    // Account not found
    return NULL;
}

/**
 * @brief Checks if the given transactions are valid based on the account balances.
 *
 * @param ourMainBank        The Bank structure containing the accounts.
 * @param transactions The linked list of transactions to be validated.
 * @return 1 if all transactions are valid, 0 otherwise.
 */
int check_if_transactions_are_valid(Bank *ourMainBank, Node *transactions) {
    Node *current = transactions;
    Node *head = NULL, *temp = NULL;

    while (current != NULL) {
        Transaction *newTransaction = (Transaction *) current->data;
        if (!check_if_acc_exists(ourMainBank, newTransaction->toAccount)
            || !check_if_acc_exists(ourMainBank, newTransaction->fromAccount)) {
            free(head);
            return 0;
        }
        copy_accounts_for_valid(ourMainBank, newTransaction->fromAccount, &head,
                                &temp);
        copy_accounts_for_valid(ourMainBank, newTransaction->toAccount, &head,
                                &temp);
        current = current->next;
    }

    current = transactions;
    while (current != NULL) {
        Transaction *newTransaction = (Transaction *) current->data;
        Account *from = find_acc_for_copy(head, newTransaction->fromAccount);
        Account *to = find_acc_for_copy(head, newTransaction->toAccount);

        if (newTransaction->amount > from->balance) {
            free(head);
            return 0;
        } else {
            from->balance -= newTransaction->amount;
            to->balance += newTransaction->amount;
        }

        current = current->next;
    }

    free(head);
    return 1;  // All transactions are valid
}

/**
 * @brief Copies the account to the temporary linked list if it doesn't exist already.
 *
 * @param ourMainBank       The Bank structure containing the accounts.
 * @param accountNumber The account number to be copied.
 * @param head       The head of the temporary linked list.
 * @param temp       The temporary node for insertion.
 */
void copy_accounts_for_valid(Bank *ourMainBank, unsigned int accountNumber,
                             Node **head, Node **temp) {
    // Find the account in the main bank's linked list of accounts
    Account *account = find_acc_for_copy(ourMainBank->accounts, accountNumber);

    // Check if the account is not already present in the temporary linked list
    if (find_acc_for_copy(*head, account->accountNumber) == NULL) {
        // Create a new node for the copied account
        Node *node = (Node *) malloc(sizeof(Node));
        // Copy the account data
        node->data = copy_account(account);
        node->next = NULL;

        // Add the new node to the temporary linked list
        if (*head == NULL) {
// If the temporary linked list is empty,  assign the new node as the head
            *head = node;
            *temp = *head;
        } else {
// otherwise, add the new node to the end of the temporary linked list
            (*temp)->next = node;
            *temp = (*temp)->next;
        }
    }
}

/**
 * @brief Applies a series of transactions to the bank.
 *
 * This function applies the given transactions to the provided bank.
 * It validates the transactions and performs the necessary balance updates.
 *
 * @param ourMainBank The bank structure to apply the transactions to.
 * @param instructons The string containing the transaction instructions.
 * @return 1 if all transactions are successfully applied, 0 otherwise.
 */
int apply_transactions(Bank *ourMainBank, char *instructions) {
    Node *transactions = split_string_to_ints(instructions);
    //return 0 if transactions are NULL
    if (transactions == NULL) {
        return 0;
    }
    // return 0 if the transactions are not valid
    if (!check_if_transactions_are_valid(ourMainBank, transactions)) {
        return 0;
    }
    //loop through the data structures and
    Node *current = transactions;
    while (current != NULL) {
        Transaction *newTransaction = (Transaction *) current->data;
        Account *from = find_acc(ourMainBank, newTransaction->fromAccount);
        Account *to = find_acc(ourMainBank, newTransaction->toAccount);

        if (from->balance > newTransaction->amount) {
            // Deduct amount from the sender's account
            from->balance = from->balance - newTransaction->amount;
            // Add amount to the recipient's account
            to->balance = to->balance + newTransaction->amount;
        } else {
            // Insufficient balance in the sender's account
            return 0;
        }
        current = current->next;
    }
    current = ourMainBank->transactions;
    while (current->next != NULL) {
        current = current->next;
    }
    // return 1 for success
    current->next = transactions;
    return 1;
}

/**
 * Perform transferring based on user instructions.
 *
 * @param ourMainBank The bank structure to apply the transactions to.
 */
void do_transferring(Bank *ourMainBank) {
    // Prompt the user to enter instructions
    printf("Enter instructions:\n");
    char *instructions = scan_string();
    if (apply_transactions(ourMainBank, instructions) == 1) {
        //print success message
        printf("Instructions executed successfully\n");
    } else {
        //print fail message
        printf("Invalid instructions\n");
    }
}

/**
 * @brief Search and print transactions related to a specific account.
 *
 * @param ourbourMainBank Pointer to the Bank structure.
 * @param accountNumber Account number to search transactions for.
 */
void search_transactions(Bank *ourMainBank, unsigned int accountNumber) {
    Node *current = ourMainBank->transactions;
    int flagForNoTransactions = 0;
    int counterForPrint = 0;
    while (current != NULL) {
//Check if the transaction is related to the specified account as  the recipient
        if ((((Transaction *) current->data)->toAccount == accountNumber) &&
            (((Transaction *) current->data)->toAccount != 0) &&
            (((Transaction *) current->data)->toAccount != WITHDRAW)) {
            int amount = ((Transaction *) current->data)->amount;
            unsigned int sender = ((Transaction *) current->data)->fromAccount;
            if (counterForPrint == 0) {
                printf("Transactions:\n");
                counterForPrint += 1;
            }
            printf("%d from %d\n", amount, sender);
            flagForNoTransactions = 1;
        }
//Check if the transaction is related to the specified account as the sender
        if ((((Transaction *) current->data)->fromAccount == accountNumber) &&
            (((Transaction *) current->data)->toAccount != 0) &&
            (((Transaction *) current->data)->toAccount != WITHDRAW)) {
            int amount = ((Transaction *) current->data)->amount;
            unsigned int recipient = ((Transaction *) current->data)->toAccount;
            if (counterForPrint == 0) {
                printf("Transactions:\n");
                counterForPrint += 1;
            }
            printf("%d to %d\n", amount, recipient);
            flagForNoTransactions = 1;
        }
        // Check if the transaction is a deposit made by the specified account
        if (((Transaction *) current->data)->toAccount == 0 &&
            ((Transaction *) current->data)->fromAccount == accountNumber) {
            int amount = ((Transaction *) current->data)->amount;
            if (counterForPrint == 0) {
                printf("Transactions:\n");
                counterForPrint += 1;
            }
            printf("Deposited %d\n", amount);
            flagForNoTransactions = 1;
        }
        //Check if the transaction is a withdrawal made by the specified account
        if (((Transaction *) current->data)->toAccount == WITHDRAW &&
            ((Transaction *) current->data)->fromAccount == accountNumber) {
            int amount = ((Transaction *) current->data)->amount;
            if (counterForPrint == 0) {
                printf("Transactions:\n");
                counterForPrint += 1;
            }
            printf("Withdrew %d\n", amount);
            flagForNoTransactions = 1;

        }
        current = current->next;
    }
    if (flagForNoTransactions == 0) {
        printf("No transactions\n");
    }
}

/**
 * @brief Frees the memory allocated for the Bank structure and its
 * associated  linked lists.
 *
 * @param ourMainBank The Bank structure to be freed.
 */
void free_bank_data(Bank *ourMainBank) {
    // Free the linked list of accounts
    Node *ourAccountsNode = ourMainBank->accounts;
    while (ourAccountsNode != NULL) {
        Node *nextAccountNode = ourAccountsNode->next;
        Account *account = (Account *) ourAccountsNode->data;
        free(account->accountHolder);
        free(account);
        free(ourAccountsNode);
        ourAccountsNode = nextAccountNode;
    }
    // Free the linked list of transactions
    Node *ourTransactionNode = ourMainBank->transactions;
    while (ourTransactionNode != NULL) {
        Node *nextTransactionNode = ourTransactionNode->next;
        Transaction *transaction = (Transaction *) ourTransactionNode->data;
        free(transaction);
        free(ourTransactionNode);
        ourTransactionNode = nextTransactionNode;
    }
    // Free the ourMainBank structure itself
    free(ourMainBank);
}

/**
 * @brief Perform a withdrawal from the specified account.
 *
 * prompts the user to enter the amount to withdraw from the account, updates
 * the account balance accordingly, creates a new transaction for the
 * withdrawal and appends it to the list
 *
 * @param ourMainBank The Bank structure containing the account and
 * transaction  data.
 * @param accountNumber The account number of the account to perform the
 * withdrawal from.
 */
void do_withdraw(Bank *ourMainBank, unsigned int accountNumber) {

    // Prompt the user to enter the amount to deposit
    printf("How much money would you like to deposit?\n");
    char *amountInString = scan_string();
    int realAmount = turn_string_to_int(amountInString);

    // Check if the entered amount is a valid number
    if (check_if_input_is_number(amountInString) == 1) {
        if (realAmount < 0) {
            // Print error message for invalid amount
            printf("Invalid amount\n");
            return;
        }

        // Find the current account based on the account number
        Account *currentAccount = find_acc(ourMainBank, accountNumber);

        // Update the account balance by adding the deposited amount
        currentAccount->balance = currentAccount->balance + realAmount;

        // Create a new transaction for the deposit
        Transaction *newTransaction = malloc(sizeof(Transaction));
        newTransaction->amount = realAmount;
        newTransaction->toAccount = 0;
        newTransaction->fromAccount = accountNumber;

        // Create a new node for the transaction and append it to the
        // transactions list
        Node *newNode = malloc(sizeof(Node));
        newNode->data = newTransaction;
        newNode->next = NULL;
        if (ourMainBank->transactions == NULL) {
            ourMainBank->transactions = newNode;
        } else {
            Node *current = ourMainBank->transactions;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
        // Print success message with the updated account balance
        printf("Money deposited successfully; your new balance is %d\n",
               currentAccount->balance);
    }
}

/**
 * @brief Perform a deposit to the specified account.
 *
 * prompts the user to enter the amount to deposit into the account, checks
 * updates the account balance accordingly, creates a new transaction for the
 * deposit and appends it to the transactions list.
 *
 * @param ourMainBank Bank structure containing the account and transaction data
 * @param accountNumber account number of the account to perform the deposit to
 */
void do_deposit(Bank *ourMainBank, unsigned int accountNumber) {
    // Prompt the user to enter the amount to withdraw
    printf("How much money would you like to withdraw?\n");
    char *amountInString = scan_string();
    int realAmount = turn_string_to_int(amountInString);
    // Check if the entered amount is a valid number
    if (check_if_input_is_number(amountInString) == 1) {
        if (realAmount < 0) {
            // Print error message for invalid amount
            printf("Invalid amount\n");
            return;
        }
        // Find the current account based on the account number
        Account *currentAccount = find_acc(ourMainBank, accountNumber);
        // Check if the account has sufficient balance for the withdrawal
        if (currentAccount->balance < realAmount) {
            // Print error message for insufficient balance
            printf("Not enough money\n");
            return;
        }
        // Update the account balance by subtracting the withdrawn amount
        currentAccount->balance = currentAccount->balance - realAmount;
        // Create a new transaction for the withdrawal
        Transaction *newTransaction = malloc(sizeof(Transaction));
        newTransaction->amount = realAmount;
        newTransaction->toAccount = WITHDRAW;
        newTransaction->fromAccount = accountNumber;
        // Create a new node for the transaction and append it to the
        // transactions list
        Node *newNode = malloc(sizeof(Node));
        newNode->data = newTransaction;
        newNode->next = NULL;
        if (ourMainBank->transactions == NULL) {
            ourMainBank->transactions = newNode;
        } else {
            Node *current = ourMainBank->transactions;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
        // Print success message with the updated account balance
        printf("Money withdrawn successfully; your new balance is %d\n",
               currentAccount->balance);
    }
}
