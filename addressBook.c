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
contact **removeContactByIndex(contact **contacts, int index);
int removeContactByFullName(contact **contacts, char *firstName,char *lastName);
void listContacts(contact **contacts);
void saveContactsToFile(contact **contacts, char *filename);
void printContactsToFile(contact **contacts, char *filename);
contact **loadContactsFromFile(char *filename);
contact **mergeContactsFromFile(contact **contacts, char *filename);
contact **appendContactsFromFile(contact **contacts, char *filename);
contact *editContact(contact **contacts, int index );

/*Menu Function Prototypes*/
void menu(contact **contacts);

int main() {
    //main function code here
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

    // Determine the size of the current array
    int size = 0;
    while (contacts[size] != NULL) {
        size++;
    }

    // Reallocate memory for the new size
    contact **newContacts = realloc(contacts, (size + 2) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory reallocation error in appendContact\n");
        // Free the newContact if reallocation fails
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        exit(EXIT_FAILURE);
    }

    // Append the new contact
    newContacts[size] = newContact;
    newContacts[size + 1] = NULL;  // NULL terminate the array

    printf("Contact appended successfully by appendContact\n");
    return newContacts;
}

contact **insertContactAlphabetical(contact **contacts, contact *newContact) {
    if (contacts == NULL || newContact == NULL) {
        return contacts;
    }

    // Determine the size of the current array
    int size = 0;
    while (contacts[size] != NULL) {
        size++;
    }

    // Reallocate memory for the new size
    contact **newContacts = realloc(contacts, (size + 2) * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Memory reallocation error in insertContactAlphabetical\n");
        // Free the newContact if reallocation fails
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return contacts;
    }

    // Find the correct index to insert the new contact
    int index = 0;
    while (index < size && strcmp(newContacts[index]->familyName, newContact->familyName) < 0) {
        index++;
    }
    // If family names are the same, sort by first name
    while (index < size && strcmp(newContacts[index]->familyName, newContact->familyName) == 0 && strcmp(newContacts[index]->firstName, newContact->firstName) < 0) {
        index++;
    }

    // Shift the contacts to make room for the new contact
    for (int i = size; i > index; i--) {
        newContacts[i] = newContacts[i - 1];
    }

    // Insert the new contact
    newContacts[index] = newContact;
    newContacts[size + 1] = NULL;  // NULL terminate the array

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

    // Free the memory of the contact to be removed
    free(contacts[index]->firstName);
    free(contacts[index]->familyName);
    free(contacts[index]->address);
    free(contacts[index]);

    // Shift contacts to fill the gap
    for (int i = index; i < size - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contacts[size - 1] = NULL;

    // Reallocate memory for the new size
    contact **newContacts = realloc(contacts, size * sizeof(contact *));
    if (newContacts == NULL) {
        fprintf(stderr, "Error: Memory reallocation failed in removeContactByIndex\n");
        return contacts; // Return the original array if reallocation fails
    }

    printf("Contact removed successfully by removeContactByIndex\n");
    return newContacts;
}

int removeContactByFullName(contact ***contacts) {
    // Check for NULL parameters
    if (contacts == NULL || *contacts == NULL) {
        printf("Error: value of contacts received in removeContactByFullName was NULL\n");
        return 0;
    }

    char firstName[50];
    char familyName[50];

    // Prompt the user for the first name and family name
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

    // Find and remove the contact
    int i = 0;
    while ((*contacts)[i] != NULL) {
        if (strcmp((*contacts)[i]->firstName, firstName) == 0 && strcmp((*contacts)[i]->familyName, familyName) == 0) {
            // Free the memory for the contact
            free((*contacts)[i]->firstName);
            free((*contacts)[i]->familyName);
            free((*contacts)[i]->address);
            free((*contacts)[i]);

            // Shift the remaining contacts to fill in the gap
            for (int j = i; (*contacts)[j + 1] != NULL; j++) {
                (*contacts)[j] = (*contacts)[j + 1];
            }

            // Reallocate the memory for the contacts array
            int newSize = i;
            contact **newContacts = realloc(*contacts, sizeof(contact *) * (newSize + 1));
            if (newContacts == NULL) {
                fprintf(stderr, "Error: Memory reallocation failed in removeContactByFullName\n");
                return 0;
            }

            *contacts = newContacts;
            (*contacts)[newSize] = NULL; // Ensure the new end of the array is NULL

            printf("Contact '%s %s' removed successfully\n", firstName, familyName);
            return 1;
        }
        i++;
    }

    // If no matching contact is found
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
    // Check if filename is NULL
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }

    // Check if contacts array is NULL
    if (contacts == NULL) {
        printf("Error: contacts formal parameter passed value NULL in saveContactsToFile\n");
        return;
    }

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: file not opened in saveContactsToFile\n");
        return;
    }

    // Determine the number of contacts
    int count = 0;
    while (contacts[count] != NULL) {
        count++;
    }

    // Write the number of contacts to the file
    fprintf(file, "%d\n", count);

    // Iterate through the contacts and write each to the file
    for (int i = 0; contacts[i] != NULL; i++) {
        contact *c = contacts[i];
        fprintf(file, "%s\n", c->firstName);
        fprintf(file, "%s\n", c->familyName);
        fprintf(file, "%s\n", c->address);
        fprintf(file, "%lld\n", c->phoneNum);
        fprintf(file, "%d\n", c->age);
    }

    // Close the file
    fclose(file);
}

void printContactsToFile(contact **contacts, char *filename) {
    // Check if filename is NULL
    if (filename == NULL) {
        printf("Error: filename formal parameter passed value NULL in printContactsToFile\n");
        return;
    }

    // Check if contacts array is NULL
    if (contacts == NULL) {
        printf("Error: addressBook formal parameter passed value NULL in printContactsToFile\n");
        return;
    }

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: file not opened in printContactsToFile\n");
        return;
    }

    // Count the number of contacts
    int totalContacts = countContacts(contacts);

    // Print the address book header
    fprintf(file, "Address Book Report\n");
    fprintf(file, "-------------------\n");

    // Iterate through the contacts and write each to the file
    for (int i = 0; i < totalContacts; i++) {
        contact *c = contacts[i];
        fprintf(file, "%d. %s %s\n", i + 1, c->firstName, c->familyName);
        fprintf(file, "Phone: %lld\n", c->phoneNum);
        fprintf(file, "Address: %s\n", c->address);
        fprintf(file, "Age: %d\n", c->age);
    }

    // Print the footer
    fprintf(file, "-------------------\n");
    fprintf(file, "Total Contacts: %d\n", totalContacts);

    // Close the file
    fclose(file);
}

contact **loadContactsFromFile(char *filename) {


}

contact **mergeContactsFromFile(contact **contacts, char *filename) {


}

contact **appendContactsFromFile(contact **contacts, char *filename) {


}

contact *editContact() {


}

void menu(contact **contacts) {
    int choice = 0;
    /*
     * Menu Output
     */
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

    /*
     * Switch Statement for Menu Options
     */
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
            contact **contacts;
            int contactsInList = countContacts(contacts);
            for (int i = 0; i < contactsInList; i++) {
                free(contacts[i]->firstName);
                free(contacts[i]->familyName);
                free(contacts[i]->address);
                free(contacts[i]->age);
                free(contacts[i]);
            }
        free(contacts);
    }

}