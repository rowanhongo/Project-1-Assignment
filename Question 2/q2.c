#include <stdio.h>
#include <stdlib.h>

struct Node
{
    char id[20];
    char type[20];
    int priority;
    struct Node *prev;
    struct Node *next;
};

struct Queue
{
    struct Node *head;
    struct Node *tail;
    struct Node *current;
};

/* Create a new sample node */
struct Node *createNode()
{
    struct Node *newNode = malloc(sizeof(struct Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    printf("Sample ID: ");
    scanf("%19s", newNode->id);

    printf("Sample type: ");
    scanf("%19s", newNode->type);

    do
    {
        printf("Priority (1=Urgent, 2=Normal, 3=Routine): ");

        if (scanf("%d", &newNode->priority) != 1)
        {
            printf("Please enter a number from 1 to 3.\n");

            while (getchar() != '\n')
                ;

            newNode->priority = 0;
        }
        else if (newNode->priority < 1 || newNode->priority > 3)
        {
            printf("Please enter 1, 2 or 3.\n");
        }

    } while (newNode->priority < 1 || newNode->priority > 3);

    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

void insertEnd(struct Queue *q, struct Node *newNode)
{
    if (q->head == NULL)
    {
        q->head = newNode;
        q->tail = newNode;
        q->current = newNode;
    }
    else
    {
        newNode->prev = q->tail;
        q->tail->next = newNode;
        q->tail = newNode;
    }
}

void displayCurrent(struct Queue *q)
{
    if (q->current == NULL)
    {
        printf("Queue is empty.\n");
        return;
    }

    printf("ID: %s, Type: %s, Priority: %d\n",
           q->current->id, q->current->type, q->current->priority);
}

void moveNext(struct Queue *q)
{
    if (q->current == NULL)
        printf("Queue is empty.\n");
    else if (q->current->next == NULL)
        printf("Already at the last sample.\n");
    else
    {
        q->current = q->current->next;
        displayCurrent(q);
    }
}

void movePrevious(struct Queue *q)
{
    if (q->current == NULL)
        printf("Queue is empty.\n");
    else if (q->current->prev == NULL)
        printf("Already at the first sample.\n");
    else
    {
        q->current = q->current->prev;
        displayCurrent(q);
    }
}

void displayForward(struct Queue *q)
{
    struct Node *temp = q->head;

    if (temp == NULL)
    {
        printf("Queue is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        printf("%s %s %d\n", temp->id, temp->type, temp->priority);
        temp = temp->next;
    }
}

void displayBackward(struct Queue *q)
{
    struct Node *temp = q->tail;

    if (temp == NULL)
    {
        printf("Queue is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        printf("%s %s %d\n", temp->id, temp->type, temp->priority);
        temp = temp->prev;
    }
}

/* Free all nodes before the program ends */
void freeList(struct Queue *q)
{
    struct Node *temp;

    while (q->head != NULL)
    {
        temp = q->head;
        q->head = q->head->next;
        free(temp);
    }

    q->tail = NULL;
    q->current = NULL;
}

int main()
{
    struct Queue q = {NULL, NULL, NULL};
    struct Node *newNode;
    int number, i, choice;

    printf("Enter number of initial samples: ");

    while (scanf("%d", &number) != 1 || number < 0)
    {
        printf("Please enter a valid number: ");

        while (getchar() != '\n')
            ;
    }

    for (i = 0; i < number; i++)
    {
        printf("\nSample %d\n", i + 1);

        newNode = createNode();

        if (newNode != NULL)
            insertEnd(&q, newNode);
    }

    do
    {
        printf("\n1. Display current\n");
        printf("2. Move next\n");
        printf("3. Move previous\n");
        printf("4. Review forward\n");
        printf("5. Review backward\n");
        printf("6. Add sample\n");
        printf("7. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Please enter a number from 1 to 7.\n");

            while (getchar() != '\n')
                ;

            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                displayCurrent(&q);
                break;

            case 2:
                moveNext(&q);
                break;

            case 3:
                movePrevious(&q);
                break;

            case 4:
                displayForward(&q);
                break;

            case 5:
                displayBackward(&q);
                break;

            case 6:
                newNode = createNode();

                if (newNode != NULL)
                {
                    insertEnd(&q, newNode);
                    printf("Sample added.\n");
                }
                break;

            case 7:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Enter 1 to 7.\n");
        }

    } while (choice != 7);

    freeList(&q);

    return 0;
}