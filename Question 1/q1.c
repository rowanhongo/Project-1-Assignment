#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Order {
    char orderID[20];
    char customerName[50];
    char productCategory[50];
    int orderValue;
};

void swap(struct Order *a, struct Order *b)
{
    struct Order temp = *a;
    *a = *b;
    *b = temp;
}

int comesBefore(struct Order a, struct Order b)
{
    if (a.orderValue != b.orderValue)
    {
        return a.orderValue > b.orderValue;
    }

    if (strcmp(a.customerName, b.customerName) != 0)
    {
        return strcmp(a.customerName, b.customerName) < 0;
    }

    return strcmp(a.orderID, b.orderID) < 0;
}

int partition(struct Order orders[], int low, int high)
{
    struct Order pivot = orders[high];
    int i = low - 1;
    int j;

    for (j = low; j < high; j++)
    {
        if (comesBefore(orders[j], pivot))
        {
            i++;
            swap(&orders[i], &orders[j]);
        }
    }

    swap(&orders[i + 1], &orders[high]);

    return i + 1;
}

void quickSort(struct Order orders[], int low, int high)
{
    if (low < high)
    {
        int pivotPosition = partition(orders, low, high);

        quickSort(orders, low, pivotPosition - 1);
        quickSort(orders, pivotPosition + 1, high);
    }
}

int main()
{
    FILE *file;
    FILE *output;
    struct Order *orders;

    int capacity = 10;
    int count = 0;
    int totalValue = 0;
    int i;

    orders = malloc(capacity * sizeof(struct Order));

    if (orders == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    file = fopen("orders.txt", "r");

    if (file == NULL)
    {
        printf("Could not open orders.txt.\n");
        free(orders);
        return 1;
    }

    while (fscanf(file, "%19s %49s %49s %d",
                  orders[count].orderID,
                  orders[count].customerName,
                  orders[count].productCategory,
                  &orders[count].orderValue) == 4)
    {
        totalValue += orders[count].orderValue;
        count++;

        if (count == capacity)
        {
            capacity = capacity * 2;

            orders = realloc(orders, capacity * sizeof(struct Order));

            if (orders == NULL)
            {
                printf("Memory allocation failed.\n");
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);

    if (count > 1)
    {
        quickSort(orders, 0, count - 1);
    }

    output = fopen("priority_orders.txt", "w");

    if (output == NULL)
    {
        printf("Could not create priority_orders.txt.\n");
        free(orders);
        return 1;
    }

    for (i = 0; i < count; i++)
    {
        fprintf(output, "%s %s %s %d\n",
                orders[i].orderID,
                orders[i].customerName,
                orders[i].productCategory,
                orders[i].orderValue);
    }

    fclose(output);

    printf("Number of orders processed: %d\n", count);
    printf("Total order value: %d\n", totalValue);

    free(orders);

    return 0;
}