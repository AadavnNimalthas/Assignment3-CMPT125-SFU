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
int menu();

int main() {


    return 0;
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
        fprintf(stderr, "Memory reallocation error in appendContact\n");
        // Free the newContact if reallocation fails
        free(newContact->firstName);
        free(newContact->familyName);
        free(newContact->address);
        free(newContact);
        return contacts;
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

contact **removeContactByIndex(contact **contacts, int index) {


}

int removeContactByFullName(contact **contacts, char *firstName, char *lastName) {


}

void listContacts(contact **contacts) {


}

void saveContactsToFile(contact **contacts, char *filename) {


}

void printContactsToFile(contact **contacts, char *filename) {


}

contact **loadContactsFromFile(char *filename) {


}

contact **mergeContactsFromFile(char *filename) {


}

contact **appendContactsFromFile(contact **contacts, char *filename) {


}

contact *editContact() {


}

int menu() {
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
     * Switch Statement to Find Function
     */
    switch (choice) {
        case 1:
            printf("1");
            break;

        case 2:
            printf("2");
            break;

        case 3:
            printf("3");
            break;

        case 4:
            printf("4");
            break;

        case 5:
            printf("5");
            break;

        case 6:
            printf("6");
            break;

        case 7:
            printf("7");
            break;

        case 8:
            printf("8");
            break;

        case 9:
            printf("9");
            break;

        case 10:
            printf("10");
            break;

        case 11:
            printf("11");
            break;

        case 12:
            printf("12");
            break;

    }
}