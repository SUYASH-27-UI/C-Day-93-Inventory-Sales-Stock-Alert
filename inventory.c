
#include <stdio.h>

#define FILE_NAME "products.txt"
#define MAX_PRODUCTS 200
#define LOW_STOCK_LIMIT 5

struct Product {
    int id;
    char name[50];
    int quantity;
    double price;
};

int loadProducts(struct Product products[]) {
    FILE *file = fopen(FILE_NAME, "r");

    if (file == NULL) return 0;

    int count = 0;

    while (count < MAX_PRODUCTS &&
           fscanf(file, "%d %49s %d %lf",
                  &products[count].id,
                  products[count].name,
                  &products[count].quantity,
                  &products[count].price) == 4) {
        count++;
    }

    fclose(file);
    return count;
}

int saveProducts(struct Product products[], int count) {
    FILE *file = fopen(FILE_NAME, "w");

    if (file == NULL) {
        printf("Unable to save inventory!\n");
        return 0;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d %s %d %.2f\n",
                products[i].id,
                products[i].name,
                products[i].quantity,
                products[i].price);
    }

    fclose(file);
    return 1;
}

int findProduct(struct Product products[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) return i;
    }

    return -1;
}

void viewInventory(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    double totalValue = 0;

    if (count == 0) {
        printf("No products found.\n");
        return;
    }

    printf("\n===== INVENTORY REPORT =====\n");

    for (int i = 0; i < count; i++) {
        printf("%d | %s | Stock: %d | Price: Rs. %.2f\n",
               products[i].id,
               products[i].name,
               products[i].quantity,
               products[i].price);

        totalValue += products[i].quantity * products[i].price;

        if (products[i].quantity <= LOW_STOCK_LIMIT) {
            printf("  WARNING: Low stock for %s!\n",
                   products[i].name);
        }
    }

    printf("\nTotal inventory value: Rs. %.2f\n", totalValue);
}

void sellProduct(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int id, quantity;

    printf("Enter product ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(products, count, id);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("Product: %s\n", products[index].name);
    printf("Available stock: %d\n", products[index].quantity);
    printf("Enter quantity to sell: ");

    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Enter a valid positive quantity.\n");
        while (getchar() != '\n');
        return;
    }

    if (quantity > products[index].quantity) {
        printf("Insufficient stock!\n");
        return;
    }

    products[index].quantity -= quantity;

    double bill = quantity * products[index].price;

    if (!saveProducts(products, count)) {
        printf("Sale could not be saved.\n");
        return;
    }

    printf("\n===== SALES RECEIPT =====\n");
    printf("Product: %s\n", products[index].name);
    printf("Quantity sold: %d\n", quantity);
    printf("Total bill: Rs. %.2f\n", bill);
    printf("Remaining stock: %d\n", products[index].quantity);

    if (products[index].quantity <= LOW_STOCK_LIMIT) {
        printf("WARNING: Stock is running low!\n");
    }
}

void restockProduct(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int id, quantity;

    printf("Enter product ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(products, count, id);

    if (index == -1) {
        printf("Product not found!\n");
        return;
    }

    printf("Enter quantity to add: ");

    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Enter a valid positive quantity.\n");
        while (getchar() != '\n');
        return;
    }

    if (products[index].quantity > 1000000 - quantity) {
        printf("Quantity is too large.\n");
        return;
    }

    products[index].quantity += quantity;

    if (saveProducts(products, count)) {
        printf("Stock updated successfully!\n");
        printf("New stock: %d\n", products[index].quantity);
    }
}

void searchProduct(void) {
    struct Product products[MAX_PRODUCTS];
    int count = loadProducts(products);
    int id;

    printf("Enter product ID: ");

    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        while (getchar() != '\n');
        return;
    }

    int index = findProduct(products, count, id);

    if (index == -1) {
        printf("Product not found!\n");
    } else {
        printf("\nProduct: %s\n", products[index].name);
        printf("Quantity: %d\n", products[index].quantity);
        printf("Price: Rs. %.2f\n", products[index].price);
    }
}

int main(void) {
    int choice;

    do {
        printf("\n===== INVENTORY SALES SYSTEM =====\n");
        printf("1. View Inventory\n");
        printf("2. Sell Product\n");
        printf("3. Restock Product\n");
        printf("4. Search Product\n");
        printf("5. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a number.\n");
            while (getchar() != '\n');
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                viewInventory();
                break;
            case 2:
                sellProduct();
                break;
            case 3:
                restockProduct();
                break;
            case 4:
                searchProduct();
                break;
            case 5:
                printf("Inventory system closed.\n");
                break;
            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 5);

    return 0;
}
