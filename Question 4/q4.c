#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char code[20];
    char name[100];
    int quantity;
    int price;
    struct Node *left;
    struct Node *right;
};

// for creating a new node
struct Node *createNode(char code[], char name[], int quantity, int price)
{
    struct Node *newNode = malloc(sizeof(struct Node));

    if (newNode == NULL)
        return NULL;

    strcpy(newNode->code, code);
    strcpy(newNode->name, name);
    newNode->quantity = quantity;
    newNode->price = price;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

// nodes placed according to medicine code
struct Node *insertNode(struct Node *root, char code[], char name[],
                        int quantity, int price)
{
    if (root == NULL)
        return createNode(code, name, quantity, price);

    if (strcmp(code, root->code) < 0)
        root->left = insertNode(root->left, code, name, quantity, price);
    else if (strcmp(code, root->code) > 0)
        root->right = insertNode(root->right, code, name, quantity, price);
    else
        root->quantity = quantity;

    return root;
}

// for searching the BST
struct Node *searchNode(struct Node *root, char code[])
{
    if (root == NULL)
        return NULL;

    if (strcmp(code, root->code) == 0)
        return root;

    if (strcmp(code, root->code) < 0)
        return searchNode(root->left, code);

    return searchNode(root->right, code);
}

void displayRecord(struct Node *node)
{
    printf("%s | %s | %d | %d\n",
           node->code, node->name, node->quantity, node->price);
}

// displaying records in ascending code order
void displayInOrder(struct Node *root)
{
    if (root == NULL)
        return;

    displayInOrder(root->left);
    displayRecord(root);
    displayInOrder(root->right);
}

int loadFile(struct Node **root, char filename[])
{
    FILE *file = fopen(filename, "r");
    char line[200], code[20], name[100], extra;
    int quantity, price, lineNumber = 0, records = 0;
    int result;

    if (file == NULL)
    {
        printf("Could not open %s.\n", filename);
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        lineNumber++;

        result = sscanf(line, " %19[^|]|%99[^|]|%d|%d %c",
                        code, name, &quantity, &price, &extra);

        if (result != 4 || quantity < 0 || price < 0)
        {
            printf("Invalid record on line %d. Skipped.\n", lineNumber);
            continue;
        }

        *root = insertNode(*root, code, name, quantity, price);
        records++;
    }

    fclose(file);

    printf("%d valid record(s) loaded.\n", records);
    return records > 0;
}

// for freeing the BST
void freeTree(struct Node *root)
{
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main()
{
    struct Node *root = NULL;
    struct Node *found;
    char code[20];
    int choice;

    if (!loadFile(&root, "records.txt"))
    {
        freeTree(root);
        return 1;
    }

    do
    {
        printf("\n--- Pharmacy Inventory ---\n");
        printf("1. Search medicine\n");
        printf("2. Display inventory\n");
        printf("3. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid choice.\n");

            while (getchar() != '\n')
                ;

            choice = 0;
            continue;
        }

        if (choice == 1)
        {
            printf("Enter medicine code: ");
            scanf("%19s", code);

            found = searchNode(root, code);

            if (found == NULL)
                printf("Medicine not found.\n");
            else
                displayRecord(found);
        }
        else if (choice == 2)
        {
            printf("\nInventory:\n");
            displayInOrder(root);
        }
        else if (choice != 3)
        {
            printf("Invalid choice.\n");
        }

    } while (choice != 3);

    freeTree(root);

    return 0;
}