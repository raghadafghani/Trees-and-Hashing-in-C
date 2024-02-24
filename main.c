#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Medicine
{
    char name[50];
    char activeIngredient[50];
    char category[50];
    char indication[50];
    char expiryDate[50];
    int count;
    char company[50];
    struct Medicine *left;
    struct Medicine *right;
};
typedef struct Medicine *root;


int SIZE=13;
int COUNTER=0;
typedef struct hashnode *hashTable;
struct hashnode
{
    int status;
    char name[50];
    char activeIngredient[50];
    char category[50];
    char indication[50];
    char expiryDate[50];
    int count;
    char company[50];

};
//array of nodes
hashTable ar[13];



root createNode(char *name, char *activeIngredient, char *category, char *indication, char *expiryDate, int count, char *company)
{
    root newNode = (root)malloc(sizeof(struct Medicine));
    strcpy(newNode->name, name);
    strcpy(newNode->activeIngredient, activeIngredient);
    strcpy(newNode->category, category);
    strcpy(newNode->indication, indication);
    strcpy(newNode->expiryDate, expiryDate);
    newNode->count = count;
    strcpy(newNode->company, company);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


void inOrder(root root)
{
    if (root == NULL) return;
    inOrder(root->left);
    printf("%s %s %s %s %s %d %s\n", root->name, root->activeIngredient, root->category, root->indication, root->expiryDate, root->count, root->company);
    inOrder(root->right);
}

root insertNode(root root, char *name, char *activeIngredient, char *category, char *indication, char *expiryDate, int count, char *company)
{
    if (root == NULL)
    {
        return createNode(name, activeIngredient, category, indication, expiryDate, count, company);
    }
    if (strcmp(name, root->name) < 0)
    {
        root->left = insertNode(root->left, name, activeIngredient, category, indication, expiryDate, count, company);
    }
    else
    {
        root->right = insertNode(root->right, name, activeIngredient, category, indication, expiryDate, count, company);
    }
    return root;
}

root readFromFile(root *MedTree)
{
    root meds[200];
    int i = 0;
    FILE *file = fopen("meds.txt", "r");
    while(!feof(file)) // while the file is not null
    {
        char input[200];// // the whole line tall
        fgets(input, 200, file);
        char name[200], activeIngredient[200], category[200], indication[200], expiryDate[200], company[200];
        int count;
// cut the line like this and save the element
        sscanf(input, "%[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%d:%s", name, activeIngredient, category, indication, expiryDate, &count, company);
        meds[i] = createNode(name, activeIngredient, category, indication, expiryDate, count, company);
        i++;
    }
    fclose(file);
    for(int j = 0; j< i; j++)
        *MedTree = insertNode(*MedTree, meds[j]->name,meds[j]->activeIngredient,meds[j]->category,meds[j]->indication,meds[j]->expiryDate,meds[j]->count,meds[j]->company);
    return *MedTree;
}

int getIndex(char name[])
{
    int index = 0;
    int i = 0;
    int number = 0;

    // Use only the first 3 characters of the name as the key
    if (strlen(name) > 2)
    {
        for (int i = 0; i < 3; i++)
        {
            number = (number << 5) + *name++;
        }
    }
    else if (strlen(name) == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            number = (number << 5) + *name++;
        }
    }
    else
    {
        number = name[0];
    }

    while (i < SIZE)
    {
        index = ((number + (i*i)) % SIZE);
        if (ar[index]->status == 0)
        {
            break;
        }
        i++;
    }
    return index;
}



void insertHash(char *name, char *activeIngredient, char *category, char *indication, char *expiryDate, int count, char *company) {
    int index = getIndex(name);

    while (ar[index] != NULL) {
        index = (index + 1) % SIZE;
    }
    // Allocate memory for a new hash node struct
    struct hashnode *newNode = (struct hashnode *) malloc(sizeof(struct hashnode));
    // Copy data to the new hash node struct
    strcpy(newNode->name, name);
    strcpy(newNode->activeIngredient, activeIngredient);
    strcpy(newNode->category, category);
    strcpy(newNode->indication, indication);
    strcpy(newNode->expiryDate, expiryDate);
    newNode->count = count;
    strcpy(newNode->company, company);
    newNode->status = 1;
    ar[index] = newNode;
}




void readFromFilehash()
{
    FILE *fp = fopen("Pharmacy.txt", "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char *name = strtok(line, ":");
        char *activeIngredient = strtok(NULL, ":");
        char *category = strtok(NULL, ":");
        char *indication = strtok(NULL, ":");
        char *expiryDate = strtok(NULL, ":");
        char *countString = strtok(NULL, ":");
        int count = atoi(countString);
        char *company = strtok(NULL, ":");
        // Remove the newline character at the end of the company name
        company[strlen(company) - 1] = '\0';

        insertHash(name, activeIngredient, category, indication, expiryDate, count, company);
    }

    fclose(fp);
}



void addNewMedicine(root *root)
{
    char name[50], activeIngredient[50], category[50], indication[50], expiryDate[50], company[50];
    int count;
    printf("Enter the name of the medicine: ");
    scanf("%s", name);
    printf("Enter the active ingredient: ");
    scanf("%s", activeIngredient);
    printf("Enter the category: ");
    scanf("%s", category);
    printf("Enter the indication: ");
    scanf("%s", indication);
    printf("Enter the expiry date: ");
    scanf("%s", expiryDate);
    printf("Enter the count: ");
    scanf("%d", &count);
    printf("Enter the company: ");
    scanf("%s", company);
    *root = insertNode(*root, name, activeIngredient, category, indication, expiryDate, count, company);
}

root findAndUpdateMedicine(root root, char *name)
{
    if (root == NULL)
    {
        printf("Medicine not found.\n");
        return root;
    }
    if (strcmp(name, root->name) == 0)
    {
        printf("Medicine found.\n");
        printf("Do you want to update the information? (y/n)");
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y')
        {
            printf("Enter new information:\n");
            printf("Name: ");
            scanf("%s", root->name);
            printf("Active Ingredient: ");
            scanf("%s", root->activeIngredient);
            printf("Category: ");
            scanf("%s", root->category);
            printf("Indication: ");
            scanf("%s", root->indication);
            printf("Expiry Date: ");
            scanf("%s", root->expiryDate);
            printf("Count: ");
            scanf("%d", &root->count);
            printf("Company: ");
            scanf("%s", root->company);
        }
        return root;
    }
    if (strcmp(name, root->name) < 0)
    {
        root->left = findAndUpdateMedicine(root->left, name);
    }
    else
    {
        root->right = findAndUpdateMedicine(root->right, name);
    }
    return root;
}



root updateMedicineQuantity(root root, char *name, int newQuantity)
{
    if (root == NULL)
    {
        printf("Medicine not found.\n");
        return root;
    }
    if (strcmp(name, root->name) == 0)
    {
        root->count = newQuantity;
        return root;
    }
    if (strcmp(name, root->name) < 0)
    {
        root->left = updateMedicineQuantity(root->left, name, newQuantity);
    }
    else
    {
        root->right = updateMedicineQuantity(root->right, name, newQuantity);
    }
    return root;
}




void listMedicinesInCategory(root root, char *category)
{
    if (root == NULL)
    {
        return;
    }
    listMedicinesInCategory(root->left, category);
    if (strcmp(root->category, category) == 0)
    {
        printf("%s %s %s %s %s %d %s\n", root->name, root->activeIngredient, root->category, root->indication, root->expiryDate, root->count, root->company);
    }
    listMedicinesInCategory(root->right, category);
}



void listMedicinesForDisease(root root, char *disease)
{
    if (root == NULL)
    {
        return;
    }
    listMedicinesForDisease(root->left, disease);
    if (strstr(root->indication, disease) != NULL)
    {
        printf("name %s",root->name);
        printf("\t\tAvailability: ");
        if(root->count > 0)
            printf("\t\tAvailable\n");
        else
        {
            printf("\t\tNot Available\n");
            printf("\n");
        }


    }
    listMedicinesForDisease(root->right, disease);
}



root deleteMedicine(root root, char *name)
{
    if (root == NULL) return root;
    if (strcmp(name, root->name) < 0)
    {
        root->left = deleteMedicine(root->left, name);
    }
    else if (strcmp(name, root->name) > 0)
    {
        root->right = deleteMedicine(root->right, name);
    }
    else
    {
        if (root->left == NULL)
        {
            root = root->right;
        }
        else if (root->right == NULL)
        {
            root = root->left;
        }
        else
        {
            root->right = deleteMedicine(root->right, root->name);
        }
    }
    return root;
}

void deleteMedicinesFromCompany(root *root, char *company)
{
    if (*root == NULL) return;
    if (strcmp((*root)->company, company) == 0)
    {
        *root = deleteMedicine(*root, (*root)->name);
        deleteMedicinesFromCompany(root, company);
    }
    else
    {
        deleteMedicinesFromCompany(&((*root)->left), company);
        deleteMedicinesFromCompany(&((*root)->right), company);
    }
}


void saveToFileHelper(FILE *fp, root root)
{
    if (root == NULL) return;
    saveToFileHelper(fp, root->left);
    fprintf(fp, "%s %s %s %s %s %d %s\n", root->name, root->activeIngredient, root->category, root->indication, root->expiryDate, root->count, root->company);
    saveToFileHelper(fp, root->right);
}


void saveToFile(root root)
{
    FILE *fp = fopen("Pharmacy.txt", "w");
    saveToFileHelper(fp, root);
    fclose(fp);
}

/////////////////////////////////////Hashing//////////////////////////////////////////
// Check if the number is prime or not
int prime(int n)
{
    int i;
    for (i = 2; i <= n/2; i++)
    {
        if (n % i != 0)
            continue;
        else
            return 1;
    }
    return 0;
}

// Return the new size for the array which must be the first prime number greater than 2*old size
int getTSize(int oldSize)
{
    int check = oldSize * 2;
    int newSize;
    while (prime(check) == 1)
    {
        check++;
    }
    newSize = check;
    return newSize;
}

// Return the index where the data must be inserted in the array by hashing
int getIndexForAuto(char name[], hashTable newAr[])
{
    int index;
    int i = 0;
    int number = 0;

    // Use only the first 3 characters of the name as the key
    if (strlen(name) > 2)
    {
        for (int i = 0; i < 3; i++)
        {
            number = (number << 5) + *name++;
        }
    }
    else if (strlen(name) == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            number = (number << 5) + *name++;
        }
    }
    else
    {
        number = name[0];
    }

    while (i < SIZE)
    {
        index = ((number + (i*i)) % SIZE);
        if (newAr[index]->status == 0)
        {
            break;
        }
        i++;
    }
    return index;
}

void reHash()
{
    int oldSize = SIZE;
    SIZE = getTSize(SIZE);
    hashTable newAr[SIZE];

    int i;
    for (i = 0; i < oldSize; i++)
    {
        if (ar[i]->status == 1)
        {
            int index = getIndexForAuto(ar[i]->name, newAr);
            strcpy(newAr[index]->name, ar[i]->name);
            strcpy(newAr[index]->activeIngredient, ar[i]->activeIngredient);
            strcpy(newAr[index]->category, ar[i]->category);
            strcpy(newAr[index]->indication, ar[i]->indication);
            strcpy(newAr[index]->expiryDate, ar[i]->expiryDate);
            newAr[index]->count = ar[i]->count;
            strcpy(newAr[index]->company, ar[i]->company);
            newAr[index]->status = 1;
        }
    }
    memcpy(ar, newAr, sizeof(newAr));
}


// Insert a new medicine into the hash table
void insert()
{
    int index;
    char name[50];
    char activeIngredient[50];
    char category[50];
    char indication[50];
    char expiryDate[50];
    int count;
    char company[50];

    printf("\nEnter the name: ");
    scanf("%s", name);
    printf("\nEnter the active ingredient: ");
    scanf("%s", activeIngredient);
    printf("\nEnter the category: ");
    scanf("%s", category);
    printf("\nEnter the indication: ");
    scanf("%s", indication);
    printf("\nEnter the expiry date: ");
    scanf("%s", expiryDate);
    printf("\nEnter the count: ");
    scanf("%d", &count);
    printf("\nEnter the company: ");
    scanf("%s", company);

    index = getIndex(name);

    strcpy(ar[index]->name, name);
    strcpy(ar[index]->activeIngredient, activeIngredient);
    strcpy(ar[index]->category, category);
    strcpy(ar[index]->indication, indication);
    strcpy(ar[index]->expiryDate, expiryDate);
    ar[index]->count = count;
    strcpy(ar[index]->company, company);

    ar[index]->status = 1;
    COUNTER++;

    // Check if the data is more than half of the array, in this case rehash
    if (COUNTER >= (SIZE/2))
    {
        reHash();
    }
}



void search()
{
    char name[50];
    printf("\nEnter the name of the medicine: ");
    scanf("%s", name);
    int index = getIndex(name);
    if (ar[index]->status == 1 && strcmp(ar[index]->name, name) == 0)
    {
        printf("\nName: %s\n", ar[index]->name);
        printf("Active Ingredient: %s\n", ar[index]->activeIngredient);
        printf("Category: %s\n", ar[index]->category);
        printf("Indication: %s\n", ar[index]->indication);
        printf("Expiry Date: %s\n", ar[index]->expiryDate);
        printf("Count: %d\n", ar[index]->count);
        printf("Company: %s\n", ar[index]->company);
    }
    else
    {
        printf("\nMedicine not found\n");
    }
}



void printHash()
{
    for(int i=0; i<SIZE; i++)
    {
        if(ar[i]->status==1)
        {
            printf("*************************************************************************************\n");
            printf("\nthe index[%d] contain: \nname: %s\nactiveIngredient: %s\t category: %s\t indication: %s\t expiryDate: %s\t  count: %d\t company: %s\n", i, ar[i]->name, ar[i]->activeIngredient, ar[i]->category, ar[i]->indication, ar[i]->expiryDate, ar[i]->count,ar[i]->company);
        }
    }
}


//delete the index from data to make it avilable for another inputs
void deleteHash()
{
    char x[30];
    printf("enter the name you want to delete data for: ");
    scanf("%s",x);

    for(int i=0; i< SIZE; i++)
    {
        if((strcmp(x, ar[i]->name)==0) && (ar[i]->status==1))
        {
            ar[i]->status=0;

            for(int j=0; j<50; j++)
            {
                ar[i]->name[j] = '\0';
                ar[i]->activeIngredient[j] = '\0';
                ar[i]->category[j] = '\0';
                ar[i]->indication[j] = '\0';
                ar[i]->expiryDate[j] = '\0';
                ar[i]->company[j] = '\0';
            }

            // free the index
            free(ar[i]);
            printf("\nthe number & cell are deleted\n");
            break;
        }
    }
    COUNTER--;
}




//search by name and show the index that contain the data for the mentioned patiunt
void SearchFor()
{
    char x[30];
    printf("enter the number you want to search for: ");
    scanf("%s",x);
    for(int i=0; i< SIZE; i++)
    {
        if((strcmp(x, ar[i]->name)==0) && (ar[i]->status==1))
        {
            printf("\nthe index [%d] is contaning the data for this name\n", i);
            break;
        }
    }

}

void printHashFile()
{
    FILE* w=fopen("Pharmacy.data", "w");
    for(int i=0; i<SIZE; i++)
    {
        if(ar[i]->status==1)
            fprintf(w, "%s#%s#%s#%s#%s#%d#%s\n", ar[i]->name, ar[i]->activeIngredient, ar[i]->category, ar[i]->indication, ar[i]->expiryDate, ar[i]->count, ar[i]->company);
    }
    fclose(w);
}


//show the list of the hash function
void hashlist()
{

    // malloc the array
    for (int i = 0; i < SIZE; i++) {
        ar[i] = NULL;
    }


    printf("hi");

    readFromFilehash();
    printf("hi");
    // show the list
    int z;
    printf("\n1-insert\t 2-print\t3-show table size\t4-Print used hash function\n5-Search for a specific medicine\t 6-delete\t7-print in file\n");
    scanf("%d",&z);
    while(1)
    {
        if(z==1)
        {
            insert();
            printHash()
        }
        else if(z==2)
        {
            printHash();
        }
        else if(z==3)
        {
            printf("the size = %d", SIZE);
        }
        else if(z==4)
        {
            printf("the used hash function: Quadratic Hashing ---> h(x) = (function + i*i) %% size\n");
        }
        else if(z==5)
        {
            SearchFor();
        }
        else if(z==6)
        {
            deleteHash();
        }
        else if(z==7)
        {
            printHashFile();
            printf("data printed on file\n");
        }
        printf("\n1-insert\t 2-print\t3-show table size\t4-Print used hash function\n5-Search for a specific medicine\t 6-delete \t7-print in file\n");
        scanf("%d",&z);
    }
}


int main()
{
    struct node *root = NULL;
    char name[50];
    char category[50];
    char disease[50];
    char company[50];

    int choice;
    while (1)
    {
        printf("Menu:\n");
        printf("1. Read Meds.txt file and create the tree\n");
        printf("2. Insert a new medicine from the user with all its associated information\n");
        printf("3. Find a medicine and give the user the option to update its information (if found)\n");
        printf("4. Update the quantity of a given medicine\n");
        printf("5. List the medicines in the tree in alphabetical order with their associated information\n");
        printf("6. List all medicines of same category (i.e., anti-inflammatory) and specify if available or not\n");
        printf("7. List all medicines that treat a specific disease and specify if available or not\n");
        printf("8. Delete a medicine from the tree\n");
        printf("9. Delete all medicines from a specific company\n");
        printf("10. Save all information to a file called Pharmacy.txt\n");
        printf("11. hash table\n");
        printf("12. exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
        case 1:
            readFromFile(&root);
            break;
        case 2:
            addNewMedicine(&root);
            break;
        case 3:
            printf("Enter the name of the medicine to find: ");
            scanf("%s", name);
            readFromFile(&root);
            findAndUpdateMedicine(root, name);
            break;
        case 4:
            printf("Enter the name of the medicine to update: ");
            scanf("%s", name);
            break;
        case 5:
            inOrder(root);
            break;
        case 6:
            printf("Enter the category to list medicines: ");
            scanf("%s", category);
            listMedicinesInCategory(root, category);
            break;
        case 7:
            printf("Enter the disease to list medicines: ");
            scanf("%s", disease);
            listMedicinesForDisease(root, disease);
            break;
        case 8:
            printf("Enter the name of the medicine to delete: ");
            scanf("%s", name);
            root = deleteMedicine(root, name);
            break;
        case 9:
            //  char company[50];
            printf("Enter the company to delete medicines: ");
            scanf("%s", company);
            deleteMedicinesFromCompany(root, company);
            break;
        case 10:
            saveToFile(root);
            break;
        case 11:
            hashlist();
            break;
        case 12:
            exit(0);
        default:
            printf("Invalid choice. Please enter a valid choice\n");
        }
    }
    return 0;
}
