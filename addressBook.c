/*
 * Name: Aadavn Nimalthas
 * Date Completed: ?
 *
 * Dynamic Address Book Application in C
 *
 * Design and implement an address book application in C that uses dynamic memory allocation to
 * manage a list of contacts.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Global Constants*/
#define MAX_INPUT 5
#define MAX_NAME_LENGTH 100
#define MAX_ADDRESS_LENGTH 100

/*Structures*/
typedef struct contact {
    char *firstName;
    char *familyName;
    long long phoneNum; /* 10-digit phone number stored as a 64-bit integer */
    char *address;
    int age;
} contact;

/*Required Function Prototypes*/
int countContacts(contact **contacts);
contact *readNewContact();
contact **appendContact(contact **contacts, contact *newContact);
contact **insertContactAlphabetical(contact **contacts, contact *newContact);
contact **removeContactByIndex(contact **contacts);
int removeContactByFullName(contact ***contacts);
void listContacts(contact **contacts);
void saveContactsToFile(contact **contacts, char *filename);
void printContactsToFile(contact **contacts, char *filename);
contact **loadContactsFromFile(contact ** addressBook, char *filename);
contact **mergeContactsFromFile(contact **contacts, char *filename);
contact **appendContactsFromFile(contact **contacts, char *filename);
contact **editContact(contact **contacts, int index );

/*
 *Menu Function Prototype
 */
void menu(contact **contacts);

/*
 * Option Handler Functions
 */
void handleAppendContact(contact **contacts);
void handleInsertContactAlphabetically(contact **contacts);
void handleRemoveContactByIndex(contact **contacts);
void handleRemoveContactByFullName(contact **contacts);
void handleFindAndEditContact(contact **contacts);
void handleListContacts(contact **contacts);
void handlePrintContactsToFileInputFormat(contact **contacts);
void handlePrintContactsToFileHumanReadable(contact **contacts);
void handleLoadContactsFromFile(contact **contacts);
void handleAppendContactsFromFile(contact **contacts);
void handleMergeContactsFromFile(contact **contacts);
void handleExitProgram(contact **contacts);

/*
 *Helper Function
 */
int compareContacts(contact *a, contact *b);
int isDuplicate(contact **contacts, int size, contact *newContact);

int main() {

    contact **contacts;
    menu(contacts);
    return 0;
}

int countContacts(contact **contacts) {
    int count = 0;
    while (contacts[count] != NULL) {
        count++;
    }
    return count;
}

contact *readNewContact() {
    contact *newContact = malloc(sizeof(contact));
    if (newContact == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for Contact in readNewContact\n");
        return NULL;
    }

    newContact->firstName = malloc(MAX_NAME_LENGTH * sizeof(char));
    newContact->familyName = malloc(MAX_NAME_LENGTH * sizeof(char));
    newContact->address = malloc(MAX_ADDRESS_LENGTH * sizeof(char));

    if (newContact->firstName == NULL || newContact->familyName == NULL || newContact->address == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for contact strings\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return NULL;
    }

    int tries = 1;
    int validCheck = 0;

    printf("Enter the first name: ");
    scanf("%s", newContact->firstName);

    if (strlen(newContact->firstName) == 0) {
        fprintf(stderr, "Error: unable to read the first name\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return NULL;
    }

    printf("Enter the family name: ");
    scanf("%s", newContact->familyName);

    if (strlen(newContact->familyName) == 0) {
        fprintf(stderr, "Error: unable to read the family name\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return NULL;
    }

    printf("Enter the address: ");
    scanf("%s", newContact->address);

    if (strlen(newContact->address) == 0) {
        fprintf(stderr, "Error: unable to read the address\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return NULL;
    }

    printf("Enter the 10-digit phone number that must not start with 0: ");
    while (validCheck == 0 && tries <= MAX_INPUT) {
        if (scanf("%lld", &newContact->phoneNum) == 1 && newContact->phoneNum >= 1000000000LL && newContact->phoneNum <= 9999999999LL) {
            validCheck = 1;
        } else {
            tries++;
            if (tries <= MAX_INPUT) {
                fprintf(stderr, "Error: Invalid phone number. Try again: ");
            }
        }
    }

    if (validCheck == 0) {
        newContact->phoneNum = 0;
        fprintf(stderr, "Error: Could not read a valid phone number\n");
    }

    validCheck = 0;
    tries = 1;

    printf("Enter the age: ");
    while (validCheck == 0 && tries <= MAX_INPUT) {
        if (scanf("%d", &newContact->age) == 1 && newContact->age >= 1 && newContact->age <= 150) {
            validCheck = 1;
        } else {
            tries++;
            if (tries <= MAX_INPUT) {
                fprintf(stderr, "Error: Invalid age. Try again: ");
            }
        }
    }

    if (validCheck == 0) {
        newContact->age = 0;
        fprintf(stderr, "Error: Could not read a valid age\n");
    }

    return newContact;
}

contact **appendContact(contact **contacts, contact *newContact) {
    if (contacts == NULL || newContact == NULL) {
        return contacts;
    }

    int size = 0;
    while (contacts[size] != NULL) {
        size++;
    }

    contact **newContacts = realloc(contacts, (size + 2) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory reallocation error in appendContact\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        exit(EXIT_FAILURE);
    }

    newContacts[size] = newContact;
    newContacts[size + 1] = NULL;

    printf("Contact appended successfully by appendContact\n");
    return newContacts;
}

contact **insertContactAlphabetical(contact **contacts, contact *newContact) {
    if (contacts == NULL || newContact == NULL) {
        return contacts;
    }

    int size = 0;
    while (contacts[size] != NULL) {
        size++;
    }

    contact **newContacts = realloc(contacts, (size + 2) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Memory reallocation error in insertContactAlphabetical\n");
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return contacts;
    }

    int index = 0;
    while (index < size && strcmp(newContacts[index]->familyName, newContact->familyName) < 0) {
        index++;
    }

    while (index < size && strcmp(newContacts[index]->familyName, newContact->familyName) == 0 && strcmp(newContacts[index]->firstName, newContact->firstName) < 0) {
        index++;
    }


    for (int i = size; i > index; i--) {
        newContacts[i] = newContacts[i - 1];
    }

    newContacts[index] = newContact;
    newContacts[size + 1] = NULL;

    printf("Contact was successfully added in alphabetical order\n");
    return newContacts;
}

contact **removeContactByIndex(contact **contacts) {
    if (contacts == NULL) {
        fprintf(stderr, "Error: value of addressBook received in removeContactByIndex was NULL\n");
        return NULL;
    }

    int index;
    printf("Enter index to remove (0 based): ");
    if (scanf("%d", &index) != 1) {
        fprintf(stderr, "Error: Value of index supplied could not be read.\n");
        return contacts;
    }

    int size = 0;
    while (contacts[size] != NULL) {
        size++;
    }

    if (index < 0 || index >= size) {
        fprintf(stderr, "Error: Index out of range in removeContactByIndex\n");
        return contacts;
    }

    free(contacts[index]->firstName);
    free(contacts[index]->familyName);
    free(contacts[index]->address);
    free(contacts[index]);

    for (int i = index; i < size - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contacts[size - 1] = NULL;

    contact **newContacts = realloc(contacts, size * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed in removeContactByIndex\n");
        return contacts;
    }

    printf("Contact removed successfully by removeContactByIndex\n");
    return newContacts;
}

int removeContactByFullName(contact ***contacts) {
    if (contacts == NULL || *contacts == NULL) {
        printf("Error: value of contacts received in removeContactByFullName was NULL\n");
        return 0;
    }

    char firstName[50];
    char familyName[50];

    printf("Enter first name: ");
    if (scanf("%49s", firstName) != 1) {
        fprintf(stderr, "Error: Value of first name supplied could not be read.\n");
        return 0;
    }

    printf("Enter family name: ");
    if (scanf("%49s", familyName) != 1) {
        fprintf(stderr, "Error: Value of family name supplied could not be read.\n");
        return 0;
    }

    int i = 0;
    while ((*contacts)[i] != NULL) {
        if (strcmp((*contacts)[i]->firstName, firstName) == 0 && strcmp((*contacts)[i]->familyName, familyName) == 0) {
            free((*contacts)[i]->firstName);
            free((*contacts)[i]->familyName);
            free((*contacts)[i]->address);
            free((*contacts)[i]);

            for (int j = i; (*contacts)[j + 1] != NULL; j++) {
                (*contacts)[j] = (*contacts)[j + 1];
            }

            int newSize = i;
            contact **newContacts = realloc(*contacts, sizeof(contact *) * (newSize + 1));
            if (newContacts == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed in removeContactByFullName\n");
                return 0;
            }

            *contacts = newContacts;
            (*contacts)[newSize] = NULL;

            printf("Contact '%s %s' removed successfully\n", firstName, familyName);
            return 1;
        }
        i++;
    }

    printf("Contact '%s %s' not found\n", firstName, familyName);
    return 2;
}

void listContacts(contact **contacts) {
    int count = countContacts(contacts);

    if (count == 0) {
        fprintf(stderr, "No contacts available.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        contact *c = contacts[i];
        printf("%d. %s %s\n", i + 1, c->firstName, c->familyName);
        printf("Phone: %lld\n", c->phoneNum);
        printf("Address: %s\n", c->address);
        printf("Age: %d\n", c->age);
    }
}

void saveContactsToFile(contact **contacts, char *filename) {
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }

    if (contacts == NULL) {
        printf("Error: contacts formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: file not opened in saveContactsToFile\n");
        return;
    }

    int count = 0;
    while (contacts[count] != NULL) {
        count++;
    }

    fprintf(file, "%d\n", count);

    for (int i = 0; contacts[i] != NULL; i++) {
        contact *c = contacts[i];
        fprintf(file, "%s\n", c->firstName);
        fprintf(file, "%s\n", c->familyName);
        fprintf(file, "%s\n", c->address);
        fprintf(file, "%lld\n", c->phoneNum);
        fprintf(file, "%d\n", c->age);
    }

    fclose(file);
}

void printContactsToFile(contact **contacts, char *filename) {
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in printContactsToFile\n");
        return;
    }

    if (contacts == NULL) {
        printf("Error: addressBook formal parameter passed value NULL in printContactsToFile\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: file not opened in printContactsToFile\n");
        return;
    }

    int totalContacts = countContacts(contacts);

    fprintf(file, "Address Book Report\n");
    fprintf(file, "-------------------\n");

    for (int i = 0; i < totalContacts; i++) {
        contact *c = contacts[i];
        fprintf(file, "%d. %s %s\n", i + 1, c->firstName, c->familyName);
        fprintf(file, "Phone: %lld\n", c->phoneNum);
        fprintf(file, "Address: %s\n", c->address);
        fprintf(file, "Age: %d\n", c->age);
    }

    fprintf(file, "-------------------\n");
    fprintf(file, "Total Contacts: %d\n", totalContacts);

    fclose(file);
}

contact **loadContactsFromFile(contact **addressBook, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File to load not found\n");
        return NULL;
    }

    int numContacts;
    if (fscanf(file, "%d", &numContacts) != 1) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in loadContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    if (addressBook != NULL) {
        for (int i = 0; addressBook[i] != NULL; i++) {
            free(addressBook[i]->firstName);
            free(addressBook[i]->familyName);
            free(addressBook[i]->address);
            free(addressBook[i]);
        }
        free(addressBook);
    }

    addressBook = (contact **)malloc((numContacts + 1) * sizeof(contact *));
    if (addressBook == NULL) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in loadContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < numContacts; i++) {
        addressBook[i] = (contact *)malloc(sizeof(contact));
        if (addressBook[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in loadContactsFromFile\n", i);
            for (int j = 0; j < i; j++) {
                free(addressBook[j]->firstName);
                free(addressBook[j]->familyName);
                free(addressBook[j]->address);
                free(addressBook[j]);
            }
            free(addressBook);
            fclose(file);
            return NULL;
        }

        addressBook[i]->firstName = (char *)malloc(100 * sizeof(char));
        addressBook[i]->familyName = (char *)malloc(100 * sizeof(char));
        addressBook[i]->address = (char *)malloc(200 * sizeof(char));
        if (addressBook[i]->firstName == NULL || addressBook[i]->familyName == NULL || addressBook[i]->address == NULL) {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            exit(EXIT_FAILURE);
        }

        fscanf(file, "%s %s %lld %s %d", addressBook[i]->firstName, addressBook[i]->familyName, &addressBook[i]->phoneNum, addressBook[i]->address, &addressBook[i]->age);
        if (addressBook[i]->phoneNum < 1000000000LL || addressBook[i]->phoneNum > 9999999999LL) {
            fprintf(stderr, "Error: Invalid phone number.\n");
            addressBook[i]->phoneNum = 0;
        }
        if (addressBook[i]->age < 0 || addressBook[i]->age > 120) {
            fprintf(stderr, "Error: Invalid age.\n");
            addressBook[i]->age = 0;
        }
    }
    addressBook[numContacts] = NULL;

    fclose(file);
    printf("Contacts loaded from file: %s\n", filename);
    return addressBook;
}

int isDuplicate(contact **contacts, int size, contact *newContact) {
    for (int i = 0; i < size; i++) {
        if (strcmp(contacts[i]->firstName, newContact->firstName) == 0 &&
            strcmp(contacts[i]->familyName, newContact->familyName) == 0) {
            return 1;
        }
    }
    return 0;
}

contact **appendContactsFromFile(contact **contacts, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File to load not found\n");
        return NULL;
    }

    int numContacts;
    if (fscanf(file, "%d", &numContacts) != 1) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in appendContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    int currentSize = 0;
    while (contacts[currentSize] != NULL) {
        currentSize++;
    }

    contact **newContacts = (contact **)realloc(contacts, (currentSize + numContacts + 1) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in appendContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    int newSize = currentSize;
    for (int i = 0; i < numContacts; i++) {
        contact *newContact = (contact *)malloc(sizeof(contact));
        if (newContact == NULL) {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in appendContactsFromFile\n", i);
            for (int j = currentSize; j < newSize; j++) {
                free(newContacts[j]->firstName);
                free(newContacts[j]->familyName);
                free(newContacts[j]->address);
                free(newContacts[j]);
            }
            free(newContacts);
            fclose(file);
            return NULL;
        }

        newContact->firstName = (char *)malloc(100 * sizeof(char));
        newContact->familyName = (char *)malloc(100 * sizeof(char));
        newContact->address = (char *)malloc(200 * sizeof(char));
        if (newContact->firstName == NULL || newContact->familyName == NULL || newContact->address == NULL) {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            exit(EXIT_FAILURE); // As per the requirement to exit on string allocation error
        }

        fscanf(file, "%s %s %lld %s %d", newContact->firstName, newContact->familyName, &newContact->phoneNum, newContact->address, &newContact->age);
        if (newContact->phoneNum < 1000000000LL || newContact->phoneNum > 9999999999LL) {
            fprintf(stderr, "Error: Invalid phone number.\n");
            newContact->phoneNum = 0;
        }
        if (newContact->age < 0 || newContact->age > 120) {
            fprintf(stderr, "Error: Invalid age.\n");
            newContact->age = 0;
        }

        if (!isDuplicate(newContacts, newSize, newContact)) {
            newContacts[newSize] = newContact;
            newSize++;
        } else {
            free(newContact->firstName);
            free(newContact->familyName);
            free(newContact->address);
            free(newContact);
        }
    }
    newContacts[newSize] = NULL;

    fclose(file);
    printf("Appended contacts from %s\n", filename);
    return newContacts;
}

int compareContacts(contact *a, contact *b) {
    int cmp = strcmp(a->firstName, b->firstName);
    if (cmp == 0) {
        cmp = strcmp(a->familyName, b->familyName);
    }
    return cmp;
}

contact **mergeContactsFromFile(contact **contacts, char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: File to load not found\n");
        return NULL;
    }

    int numContacts;
    if (fscanf(file, "%d", &numContacts) != 1) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in mergeContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    int currentSize = 0;
    while (contacts[currentSize] != NULL) {
        currentSize++;
    }

    contact **newContacts = (contact **)malloc((currentSize + numContacts + 1) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory allocation error, addressBook in mergeContactsFromFile\n");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < currentSize; i++) {
        newContacts[i] = contacts[i];
    }

    int newSize = currentSize;
    for (int i = 0; i < numContacts; i++) {
        contact *newContact = (contact *)malloc(sizeof(contact));
        if (newContact == NULL) {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in mergeContactsFromFile\n", i);
            for (int j = currentSize; j < newSize; j++) {
                free(newContacts[j]->firstName);
                free(newContacts[j]->familyName);
                free(newContacts[j]->address);
                free(newContacts[j]);
            }
            free(newContacts);
            fclose(file);
            return NULL;
        }

        newContact->firstName = (char *)malloc(100 * sizeof(char));
        newContact->familyName = (char *)malloc(100 * sizeof(char));
        newContact->address = (char *)malloc(200 * sizeof(char));
        if (newContact->firstName == NULL || newContact->familyName == NULL || newContact->address == NULL) {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated\n", i);
            exit(EXIT_FAILURE);
        }

        fscanf(file, "%s %s %lld %s %d", newContact->firstName, newContact->familyName, &newContact->phoneNum, newContact->address, &newContact->age);
        if (newContact->phoneNum < 1000000000LL || newContact->phoneNum > 9999999999LL) {
            fprintf(stderr, "Error: Invalid phone number.\n");
            newContact->phoneNum = 0;
        }
        if (newContact->age < 0 || newContact->age > 120) {
            fprintf(stderr, "Error: Invalid age.\n");
            newContact->age = 0;
        }

        if (!isDuplicate(newContacts, newSize, newContact)) {
            int j = newSize - 1;
            while (j >= 0 && compareContacts(newContacts[j], newContact) > 0) {
                newContacts[j + 1] = newContacts[j];
                j--;
            }
            newContacts[j + 1] = newContact;
            newSize++;
        } else {
            free(newContact->firstName);
            free(newContact->familyName);
            free(newContact->address);
            free(newContact);
        }
    }
    newContacts[newSize] = NULL;

    fclose(file);
    printf("Appended contacts from %s\n", filename);
    return newContacts;
}

contact **editContact(contact **contacts, int index) {
    int count = 0;
    while (contacts[count] != NULL) {
        count++;
    }

    if (count == 0) {
        printf("No contacts available to edit\n");
        return contacts;
    }

    printf("Enter index of contact to edit (0-%d): ", count - 1);
    char input[10];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0'; // Remove newline character

    if (sscanf(input, "%d", &index) != 1 || index < 0 || index >= count) {
        printf("Error: Invalid Index\n");
        return contacts;
    }

    printf("Editing contact: %s %s\n", contacts[index]->firstName, contacts[index]->familyName);

    while (1) {
        printf("1. Edit First Name\n");
        printf("2. Edit Last Name\n");
        printf("3. Edit Address\n");
        printf("4. Edit Phone Number\n");
        printf("5. Edit Age\n");
        printf("6. Cancel\n");
        printf("Choose an option: ");

        int option;
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove newline character
        if (sscanf(input, "%d", &option) != 1) {
            printf("Error: Invalid option\n");
            continue;
        }

        switch (option) {
            case 1:
                printf("Enter new first name: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                free(contacts[index]->firstName);
                contacts[index]->firstName = strdup(input);
                if (contacts[index]->firstName == NULL) {
                    fprintf(stderr, "Error: Memory allocation error for string in editContact\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 2:
                printf("Enter new family name: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                free(contacts[index]->familyName);
                contacts[index]->familyName = strdup(input);
                if (contacts[index]->familyName == NULL) {
                    fprintf(stderr, "Error: Memory allocation error for string in editContact\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 3:
                printf("Enter new address: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                free(contacts[index]->address);
                contacts[index]->address = strdup(input);
                if (contacts[index]->address == NULL) {
                    fprintf(stderr, "Error: Memory allocation error for string in editContact\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 4:
                printf("Enter new phone number: Enter 10-digit phone number that must not start with 0: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                long long newPhoneNum;
                if (sscanf(input, "%lld", &newPhoneNum) != 1 || strlen(input) != 10 || input[0] == '0') {
                    printf("Error: Invalid phone number.\n");
                    continue;
                }
                contacts[index]->phoneNum = newPhoneNum;
                break;

            case 5:
                printf("Enter new age: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Remove newline character
                int newAge;
                if (sscanf(input, "%d", &newAge) != 1 || newAge < 0 || newAge > 120) {
                    printf("Error: Invalid age.\n");
                    continue;
                }
                contacts[index]->age = newAge;
                break;

            case 6:
                printf("Edit cancelled\n");
                return contacts;

            default:
                printf("Error: Invalid option\n");
                break;
        }
    }

    return contacts;
}

void menu(contact **contacts) {
    int choice = 0;

    printf("Address Book Menu\n");
    printf("1.  Append Contact\n");
    printf("2.  Insert Contact in Alphabetical Order\n");
    printf("3.  Remove Contact by Index\n");
    printf("4.  Remove Contact by Full Name\n");
    printf("5.  Find and Edit Contact\n");
    printf("6.  List Contacts\n");
    printf("7.  Print Contacts to File with the format of an input file\n");
    printf("8.  Print Contacts to File (Human Readable)\n");
    printf("9.  Load Contacts from File Replacing Existing Contacts\n");
    printf("10. Append Contacts from File\n");
    printf("11. Merge Contacts from File\n");
    printf("12. Exit\n");
    printf("Enter your choice: ");

    scanf("%d", &choice);

    switch (choice) {
        /*
         * Append Contact
         */
        case 1:
            printf("1");
            break;

        /*
         * Insert Contact in Alphabetical Order
         */
        case 2:
            printf("2");
            break;

        /*
         * Remove Contact by Index
         */
        case 3:
            printf("3");
            break;

        /*
         * Remove Contact by Full Name
         */
        case 4:
            printf("4");
            break;

        /*
         * Find and Edit Contact
         */
        case 5:
            printf("5");
            break;

        /*
         * List Contacts
         */
        case 6:
            printf("6");
            break;

        /*
         * Print Contacts to File with the format of an input file
         */
        case 7:
            printf("7");
            break;

        /*
         * Print Contacts to File (Human Readable)
         */
        case 8:
            printf("8");
            break;

        /*
         * Load Contacts from File Replacing Existing Contacts
         */
        case 9:
            printf("9");
            break;

        /*
         * Append Contacts from File
         */
        case 10:
            printf("10");
            break;

        /*
         * Merge Contacts from File
         */
        case 11:
            printf("11");
            break;

        /*
         * Exit Program
         */
        case 12:
            printf("12");
            break;
    }

}