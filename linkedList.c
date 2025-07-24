#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->next = NULL;
    return newNode;
}

struct Node* insertAtEnd(struct Node* head, int value) {
    struct Node* newNode = createNode(value);
    
    if (head == NULL) {
        return newNode;
    }
    
    struct Node* current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return head;
}

struct Node* deleteNode(struct Node* head, int value) {
    struct Node* current = head;
    struct Node* prev = NULL;
    
    // If head node has the value
    if (current != NULL && current->data == value) {
        struct Node* newHead = current->next;
        free(current);
        printf("Node with value %d deleted successfully!\n", value);
        return newHead;
    }
    
    // Search for the value
    while (current != NULL && current->data != value) {
        prev = current;
        current = current->next;
    }
    
    // If value not found
    if (current == NULL) {
        printf("Value %d not found in list!\n", value);
        return head;
    }
    
    // Remove the node
    prev->next = current->next;
    free(current);
    printf("Node with value %d deleted successfully!\n", value);
    
    return head;
}

void printList(struct Node* head) {
    if (head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    struct Node* current = head;
    printf("Current list: ");
    while (current != NULL) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    struct Node* head = NULL;
    int choice, value;

    while (1) {
        printf("\n=== Linked List Operations ===\n");
        printf("1. Insert a node\n");
        printf("2. Delete a node\n");
        printf("3. Display the list\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                head = insertAtEnd(head, value);
                printf("Node with value %d inserted successfully!\n", value);
                printList(head);
                break;

            case 2:
                if (head == NULL) {
                    printf("List is empty! Nothing to delete.\n");
                    break;
                }
                printf("Enter value to delete: ");
                scanf("%d", &value);
                head = deleteNode(head, value);
                printList(head);
                break;

            case 3:
                printList(head);
                break;

            case 4:
                printf("Exiting program.\n");
                // Free the entire list before exiting
                while (head != NULL) {
                    struct Node* temp = head;
                    head = head->next;
                    free(temp);
                }
                return 0;

            default:
                printf("Invalid choice! Please enter a number between 1 and 4.\n");
        }
    }

    return 0;
}
