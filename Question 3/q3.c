#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int id;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
};

// for creating a new node
struct Node *createNode(int id)
{
    struct Node *newNode = malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    newNode->id = id;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    return newNode;
}

void insertNode(struct Node *root, int id)
{
    struct Node *queue[100];
    struct Node *newNode;
    int front = 0;
    int rear = 0;
    struct Node *temp;

    newNode = createNode(id);

    if (newNode == NULL)
        return;

    queue[rear++] = root;

    while (front < rear)
    {
        temp = queue[front++];

        if (temp->left == NULL)
        {
            temp->left = newNode;
            newNode->parent = temp;
            return;
        }
        else
        {
            queue[rear++] = temp->left;
        }

        if (temp->right == NULL)
        {
            temp->right = newNode;
            newNode->parent = temp;
            return;
        }
        else
        {
            queue[rear++] = temp->right;
        }
    }
}

// search tree for the participant ID 
struct Node *searchNode(struct Node *root, int id)
{
    struct Node *found;

    if (root == NULL)
        return NULL;

    if (root->id == id)
        return root;

    found = searchNode(root->left, id);

    if (found != NULL)
        return found;

    return searchNode(root->right, id);
}

// displaying leaf nodes
void displayLeaves(struct Node *root)
{
    if (root == NULL)
        return;

    if (root->left == NULL && root->right == NULL)
    {
        printf("%d ", root->id);
        return;
    }

    displayLeaves(root->left);
    displayLeaves(root->right);
}

void displayParent(struct Node *node)
{
    if (node->parent == NULL)
        printf("This is the root and has no parent.\n");
    else
        printf("Parent: %d\n", node->parent->id);
}

void displaySibling(struct Node *node)
{
    if (node->parent == NULL)
    {
        printf("The root has no sibling.\n");
    }
    else if (node->parent->left == node)
    {
        if (node->parent->right != NULL)
            printf("Sibling: %d\n", node->parent->right->id);
        else
            printf("This node has no sibling.\n");
    }
    else
    {
        if (node->parent->left != NULL)
            printf("Sibling: %d\n", node->parent->left->id);
        else
            printf("This node has no sibling.\n");
    }
}

void displayGrandchildren(struct Node *node)
{
    int found = 0;

    if (node->left != NULL)
    {
        if (node->left->left != NULL)
        {
            printf("%d ", node->left->left->id);
            found = 1;
        }

        if (node->left->right != NULL)
        {
            printf("%d ", node->left->right->id);
            found = 1;
        }
    }

    if (node->right != NULL)
    {
        if (node->right->left != NULL)
        {
            printf("%d ", node->right->left->id);
            found = 1;
        }

        if (node->right->right != NULL)
        {
            printf("%d ", node->right->right->id);
            found = 1;
        }
    }

    if (found == 0)
        printf("This node has no grandchildren.");

    printf("\n");
}

// freeing all nodes in the tree
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
    int values[70] = {
        42, 17, 68, 9, 23, 55, 81, 4, 13, 20, 31,
        49, 61, 75, 90, 2, 7, 11, 15, 19, 21, 27,
        35, 45, 52, 58, 64, 72, 78, 85, 95, 1, 3,
        5, 6, 8, 10, 12, 14, 16, 18, 22, 24, 26, 29,
        33, 37, 41, 44, 47, 50, 54, 57, 60, 63, 66,
        70, 74, 77, 80, 83, 87, 92, 97, 25, 28, 30,
        34, 39
    };

    struct Node *root;
    struct Node *node;
    int i;
    int choice;
    int id;

    root = createNode(values[0]);

    if (root == NULL)
        return 1;

    for (i = 1; i < 70; i++)
        insertNode(root, values[i]);

    do
    {
        printf("\n--- Tournament Tree ---\n");
        printf("1. Display root\n");
        printf("2. Display leaf participants\n");
        printf("3. Display parent\n");
        printf("4. Display sibling\n");
        printf("5. Display grandchildren\n");
        printf("6. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Please enter a number from 1 to 6.\n");

            while (getchar() != '\n')
                ;

            choice = 0;
            continue;
        }

        if (choice >= 3 && choice <= 5)
        {
            printf("Enter participant identifier: ");

            if (scanf("%d", &id) != 1)
            {
                printf("Please enter a valid identifier.\n");

                while (getchar() != '\n')
                    ;

                continue;
            }

            node = searchNode(root, id);

            if (node == NULL)
            {
                printf("Participant %d does not exist.\n", id);
                continue;
            }
        }

        switch (choice)
        {
            case 1:
                printf("Root: %d\n", root->id);
                break;

            case 2:
                printf("Leaf participants: ");
                displayLeaves(root);
                printf("\n");
                break;

            case 3:
                displayParent(node);
                break;

            case 4:
                displaySibling(node);
                break;

            case 5:
                printf("Grandchildren: ");
                displayGrandchildren(node);
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 6);

    freeTree(root);

    return 0;
}