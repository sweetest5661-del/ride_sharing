#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 100

typedef struct
{
    int item[CAPACITY];
    int top;
} Stack;

typedef struct
{
    int item[CAPACITY];
    int front, rear, count;
} Queue;

typedef struct ListNode
{
    int value;
    struct ListNode *next;
} ListNode;

typedef struct TreeNode
{
    int value;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct HashNode
{
    int key;
    char value[64];
    struct HashNode *next;
} HashNode;

typedef struct
{
    HashNode *bucket[31];
} HashTable;

/* Purpose: linear search checks each item and works with unsorted arrays. */
int linearSearch(const int values[], int count, int target)
{
    if (values == NULL)
    {
        return -1;
    }
    if (count <= 0)
    {
        return -1;
    }
    int currentIndex = 0;
    int foundIndex = -1;
    for (int i = 0; i < count; ++i)
    {
        currentIndex = i;
        if (values[currentIndex] == target)
        {
            foundIndex = currentIndex;
            return foundIndex;
        }
    }
    return foundIndex;
}

/* Purpose: binary search halves a sorted array interval on each comparison. */
int binarySearch(const int values[], int count, int target)
{
    if (values == NULL)
    {
        return -1;
    }
    if (count <= 0)
    {
        return -1;
    }
    int low = 0, high = count - 1;
    while (low <= high)
    {
        int middle = low + (high - low) / 2;
        if (values[middle] == target)
        {
            return middle;
        }
        if (values[middle] < target)
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }
    return -1;
}

/* Purpose: insertion sort places each next value into its correct earlier position. */
void insertionSort(int values[], int count)
{
    if (values == NULL)
    {
        return;
    }
    if (count < 2)
    {
        return;
    }
    for (int i = 1; i < count; ++i)
    {
        int value = values[i], j = i - 1;
        while (j >= 0 && values[j] > value)
        {
            values[j + 1] = values[j];
            --j;
        }
        values[j + 1] = value;
    }
}

/* Purpose: stackPush inserts an item at the top of a bounded array stack. */
int stackPush(Stack *stack, int value)
{
    if (stack == NULL)
    {
        return 0;
    }
    if (stack->top < 0)
    {
        return 0;
    }
    if (stack->top >= CAPACITY)
    {
        return 0;
    }
    int insertionIndex = stack->top;
    stack->item[insertionIndex] = value;
    stack->top = insertionIndex + 1;
    if (stack->top > CAPACITY)
    {
        stack->top = CAPACITY;
        return 0;
    }
    return 1;
}

/* Purpose: stackPop removes the newest item and stores it in the output parameter. */
int stackPop(Stack *stack, int *value)
{
    if (stack == NULL)
    {
        return 0;
    }
    if (value == NULL)
    {
        return 0;
    }
    if (stack->top <= 0 || stack->top > CAPACITY)
    {
        return 0;
    }
    int removalIndex = stack->top - 1;
    *value = stack->item[removalIndex];
    stack->top = removalIndex;
    if (stack->top < 0)
    {
        stack->top = 0;
        return 0;
    }
    return 1;
}

/* Purpose: queueAdd inserts a value at the rear of a circular array queue. */
int queueAdd(Queue *queue, int value)
{
    if (queue == NULL || queue->count < 0 || queue->count >= CAPACITY || queue->rear < 0 ||
        queue->rear >= CAPACITY)
    {
        return 0;
    }
    int insertionIndex = queue->rear;
    queue->item[insertionIndex] = value;
    int nextRear = insertionIndex + 1;
    if (nextRear == CAPACITY)
    {
        nextRear = 0;
    }
    queue->rear = nextRear;
    queue->count = queue->count + 1;
    if (queue->count > CAPACITY)
    {
        queue->count = CAPACITY;
        return 0;
    }
    return 1;
}

/* Purpose: queueDelete removes the oldest value from a circular array queue. */
int queueDelete(Queue *queue, int *value)
{
    if (queue == NULL || value == NULL || queue->count <= 0 || queue->count > CAPACITY ||
        queue->front < 0 || queue->front >= CAPACITY)
    {
        return 0;
    }
    int removalIndex = queue->front;
    *value = queue->item[removalIndex];
    int nextFront = removalIndex + 1;
    if (nextFront == CAPACITY)
    {
        nextFront = 0;
    }
    queue->front = nextFront;
    queue->count = queue->count - 1;
    if (queue->count < 0)
    {
        queue->count = 0;
        return 0;
    }
    return 1;
}

/* Purpose: listAppend allocates a node and attaches it at the end of a linked list. */
ListNode *listAppend(ListNode *head, int value)
{
    ListNode *node = (ListNode *)malloc(sizeof(*node));
    if (node == NULL)
    {
        return head;
    }
    node->value = value;
    node->next = NULL;
    if (head == NULL)
    {
        return node;
    }
    ListNode *last = head;
    while (last->next != NULL)
    {
        last = last->next;
    }
    last->next = node;
    return head;
}

/* Purpose: listRemove unlinks and frees the first matching list node. */
ListNode *listRemove(ListNode *head, int value)
{
    if (head == NULL)
    {
        return NULL;
    }
    ListNode *previous = NULL;
    ListNode *current = head;
    while (current != NULL && current->value != value)
    {
        previous = current;
        current = current->next;
    }
    if (current == NULL)
    {
        return head;
    }
    if (previous == NULL)
    {
        head = current->next;
    }
    else
    {
        previous->next = current->next;
    }
    current->next = NULL;
    free(current);
    return head;
}

/* Purpose: listFree releases all nodes allocated for a linked list. */
void listFree(ListNode *head)
{
    if (head == NULL)
    {
        return;
    }
    ListNode *current = head;
    size_t releasedNodes = 0;
    while (current != NULL)
    {
        ListNode *next = current->next;
        current->next = NULL;
        free(current);
        current = next;
        ++releasedNodes;
    }
    if (releasedNodes > 0)
    {
        printf("Released %zu linked-list nodes.\n", releasedNodes);
    }
    else
    {
        puts("The linked list was already empty.");
    }
}

/* Purpose: treeInsert recursively stores a value in binary search tree order. */
TreeNode *treeInsert(TreeNode *root, int value)
{
    if (root == NULL)
    {
        root = (TreeNode *)malloc(sizeof(*root));
        if (root == NULL)
        {
            return NULL;
        }
        root->value = value;
        root->left = NULL;
        root->right = NULL;
    }
    else if (value < root->value)
    {
        root->left = treeInsert(root->left, value);
    }
    else if (value > root->value)
    {
        root->right = treeInsert(root->right, value);
    }
    return root;
}

/* Purpose: inorder traversal visits BST keys in increasing order. */
void treePrint(const TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    const TreeNode *leftChild = root->left;
    const TreeNode *rightChild = root->right;
    if (leftChild != NULL)
    {
        treePrint(leftChild);
    }
    int currentValue = root->value;
    printf("%d ", currentValue);
    if (rightChild != NULL)
    {
        treePrint(rightChild);
    }
    if (leftChild == NULL && rightChild == NULL)
    {
        return;
    }
}

/* Purpose: treeFree recursively releases the complete binary search tree. */
void treeFree(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }
    TreeNode *leftChild = root->left;
    TreeNode *rightChild = root->right;
    if (leftChild != NULL)
    {
        treeFree(leftChild);
    }
    if (rightChild != NULL)
    {
        treeFree(rightChild);
    }
    root->left = NULL;
    root->right = NULL;
    free(root);
}

/* Purpose: hashPut inserts or updates a key using separate chaining. */
int hashPut(HashTable *table, int key, const char *value)
{
    if (table == NULL || value == NULL)
    {
        return 0;
    }
    unsigned bucket = (unsigned)key % 31U;
    for (HashNode *node = table->bucket[bucket]; node != NULL; node = node->next)
    {
        if (node->key == key)
        {
            snprintf(node->value, sizeof(node->value), "%s", value);
            return 1;
        }
    }
    HashNode *node = (HashNode *)malloc(sizeof(*node));
    if (node == NULL)
    {
        return 0;
    }
    node->key = key;
    snprintf(node->value, sizeof(node->value), "%s", value);
    node->next = table->bucket[bucket];
    table->bucket[bucket] = node;
    return 1;
}

/* Purpose: hashGet searches one bucket chain and returns the stored value. */
const char *hashGet(const HashTable *table, int key)
{
    if (table == NULL)
    {
        return NULL;
    }
    unsigned bucket = (unsigned)key % 31U;
    const HashNode *current = table->bucket[bucket];
    if (current == NULL)
    {
        return NULL;
    }
    while (current != NULL)
    {
        if (current->key == key)
        {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

/* Purpose: hashClear frees every chained record and resets the hash table. */
void hashClear(HashTable *table)
{
    if (table == NULL)
    {
        return;
    }
    for (int bucketIndex = 0; bucketIndex < 31; ++bucketIndex)
    {
        HashNode *current = table->bucket[bucketIndex];
        if (current == NULL)
        {
            continue;
        }
        while (current != NULL)
        {
            HashNode *next = current->next;
            current->next = NULL;
            free(current);
            current = next;
        }
        table->bucket[bucketIndex] = NULL;
    }
}

/* Purpose: demonstrate linear search, binary search, and insertion sort. */
void demoSearch(void)
{
    int sortedValues[] = {2, 5, 8, 12, 16, 23};
    int sortedCount = (int)(sizeof(sortedValues) / sizeof(sortedValues[0]));
    int requestedValue = 12;

    puts("\nSearch examples");
    printf("The sample sorted array has %d values.\n", sortedCount);
    printf("Looking for %d with linear search.\n", requestedValue);
    int linearPosition = linearSearch(sortedValues, sortedCount, requestedValue);
    printf("Linear search returned index %d.\n", linearPosition);

    puts("Binary search uses the same sorted input array.");
    int binaryPosition = binarySearch(sortedValues, sortedCount, requestedValue);
    printf("Binary search returned index %d.\n", binaryPosition);

    int missingValue = 99;
    puts("A missing value is also checked before sorting.");
    int missingPosition = linearSearch(sortedValues, sortedCount, missingValue);
    printf("Search for %d returned index %d.\n", missingValue, missingPosition);

    int valuesToSort[] = {9, 3, 7, 1, 5};
    int sortCount = (int)(sizeof(valuesToSort) / sizeof(valuesToSort[0]));
    puts("\nInsertion sort example");
    printf("Number of values before sorting: %d\n", sortCount);
    printf("Values before sorting: ");
    for (int i = 0; i < sortCount; ++i)
    {
        printf("%d ", valuesToSort[i]);
    }
    putchar('\n');

    insertionSort(valuesToSort, sortCount);

    printf("Values after sorting:  ");
    for (int i = 0; i < sortCount; ++i)
    {
        printf("%d ", valuesToSort[i]);
    }
    putchar('\n');

    int negativeValues[] = {4, -2, 7, 0, -5};
    int negativeCount = (int)(sizeof(negativeValues) / sizeof(negativeValues[0]));
    puts("Insertion sort also handles negative values.");
    insertionSort(negativeValues, negativeCount);
    printf("Sorted values with negatives: ");
    for (int i = 0; i < negativeCount; ++i)
    {
        printf("%d ", negativeValues[i]);
    }
    putchar('\n');

    int singleValue[] = {42};
    puts("A one-item array is a valid input for both search methods.");
    printf("Linear search for 42 returned index %d.\n", linearSearch(singleValue, 1, 42));
    printf("Binary search for 42 returned index %d.\n", binarySearch(singleValue, 1, 42));
    printf("Binary search for 7 in that array returned index %d.\n",
           binarySearch(singleValue, 1, 7));

    int alreadySorted[] = {-3, 0, 4, 8};
    int alreadySortedCount = (int)(sizeof(alreadySorted) / sizeof(alreadySorted[0]));
    puts("An already sorted array can be searched too.");
    printf("Binary search for 8 returned index %d.\n",
           binarySearch(alreadySorted, alreadySortedCount, 8));
    printf("Linear search for -3 returned index %d.\n",
           linearSearch(alreadySorted, alreadySortedCount, -3));
}

/* Purpose: demonstrate bounded stacks, circular queues, lists, BSTs, and hashing. */
void demoStructures(void)
{
    int value = 0;
    puts("\nStack example (last-in, first-out)");
    Stack stack = {{0}, 0};
    int firstPush = stackPush(&stack, 10);
    int secondPush = stackPush(&stack, 20);
    printf("First push status: %d\n", firstPush);
    printf("Second push status: %d\n", secondPush);
    printf("Current stack size: %d\n", stack.top);
    if (stackPop(&stack, &value))
    {
        printf("First pop returns the newest value: %d\n", value);
    }
    if (stackPop(&stack, &value))
    {
        printf("Second pop returns the older value: %d\n", value);
    }
    if (!stackPop(&stack, &value))
    {
        puts("An empty stack correctly rejects pop.");
    }
    for (int i = 0; i < CAPACITY; ++i)
    {
        if (!stackPush(&stack, i))
        {
            printf("Stack stopped accepting values at position %d.\n", i);
            break;
        }
    }
    printf("Stack size at its limit: %d\n", stack.top);
    if (!stackPush(&stack, 999))
    {
        puts("The full stack correctly rejects another push.");
    }
    while (stackPop(&stack, &value))
    {
        if (stack.top == CAPACITY - 2 || stack.top == 0)
        {
            printf("Stack pop left %d items.\n", stack.top);
        }
    }

    puts("\nCircular queue example (first-in, first-out)");
    Queue queue = {{0}, 0, 0, 0};
    queueAdd(&queue, 10);
    queueAdd(&queue, 20);
    queueAdd(&queue, 30);
    printf("Queue count after three additions: %d\n", queue.count);
    if (queueDelete(&queue, &value))
    {
        printf("First deletion returns the oldest value: %d\n", value);
    }
    if (queueDelete(&queue, &value))
    {
        printf("Second deletion returns the next value: %d\n", value);
    }
    queueAdd(&queue, 40);
    queueAdd(&queue, 50);
    queueAdd(&queue, 60);
    printf("Queue count after adding values at the wrapped rear: %d\n", queue.count);
    for (int i = 0; i < queue.count; ++i)
    {
        int position = (queue.front + i) % CAPACITY;
        printf("Queue item %d: %d\n", i + 1, queue.item[position]);
    }
    while (queueDelete(&queue, &value))
    {
        printf("Removing queued value: %d\n", value);
    }
    if (!queueDelete(&queue, &value))
    {
        puts("An empty queue correctly rejects deletion.");
    }
    for (int i = 0; i < CAPACITY; ++i)
    {
        if (!queueAdd(&queue, i + 1))
        {
            printf("Queue stopped accepting values at position %d.\n", i);
            break;
        }
    }
    printf("Queue count at its limit: %d\n", queue.count);
    if (!queueAdd(&queue, 999))
    {
        puts("The full queue correctly rejects another value.");
    }
    for (int i = 0; i < 3; ++i)
    {
        if (queueDelete(&queue, &value))
        {
            printf("Removed %d from the full queue front.\n", value);
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        if (queueAdd(&queue, 200 + i))
        {
            printf("Added %d after the rear wrapped around.\n", 200 + i);
        }
    }
    while (queueDelete(&queue, &value))
    {
        /* Drain the queue so the next example starts with an empty structure. */
    }

    puts("\nSingly linked list example");
    ListNode *head = NULL;
    head = listAppend(head, 4);
    head = listAppend(head, 8);
    head = listAppend(head, 12);
    head = listAppend(head, 16);
    int listLength = 0;
    for (ListNode *current = head; current != NULL; current = current->next)
    {
        printf("List item %d: %d\n", listLength + 1, current->value);
        ++listLength;
    }
    printf("Linked list length: %d\n", listLength);
    puts("Removing the head value 4.");
    head = listRemove(head, 4);
    puts("Removing the middle value 12.");
    head = listRemove(head, 12);
    puts("Removing a value that is not in the list leaves it unchanged.");
    head = listRemove(head, 99);
    listLength = 0;
    for (ListNode *current = head; current != NULL; current = current->next)
    {
        printf("Remaining list item %d: %d\n", listLength + 1, current->value);
        ++listLength;
    }
    printf("Remaining linked list length: %d\n", listLength);
    listFree(head);
    head = NULL;
    puts("All remaining list nodes have been freed.");

    puts("\nBinary search tree example");
    TreeNode *root = NULL;
    int treeValues[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};
    int treeCount = (int)(sizeof(treeValues) / sizeof(treeValues[0]));
    for (int i = 0; i < treeCount; ++i)
    {
        root = treeInsert(root, treeValues[i]);
    }
    puts("The inserted values are displayed in sorted inorder order:");
    treePrint(root);
    putchar('\n');
    puts("A duplicate key does not create another tree node.");
    root = treeInsert(root, 8);
    treePrint(root);
    putchar('\n');
    treeFree(root);
    root = NULL;
    puts("All binary search tree nodes have been freed.");

    puts("\nHash table example with separate chaining");
    HashTable table = {{0}};
    hashPut(&table, 7, "Rider A");
    hashPut(&table, 38, "Rider B");
    hashPut(&table, 69, "Rider C");
    puts("These keys share a bucket because their remainders are equal.");
    printf("Value for key 7: %s\n", hashGet(&table, 7));
    printf("Value for key 38: %s\n", hashGet(&table, 38));
    printf("Value for key 69: %s\n", hashGet(&table, 69));
    hashPut(&table, 38, "Updated Rider B");
    printf("Updated value for key 38: %s\n", hashGet(&table, 38));
    if (hashGet(&table, 1000) == NULL)
    {
        puts("A missing key correctly returns no value.");
    }
    hashClear(&table);
    puts("All hash table nodes have been freed.");
}

/* Purpose: display the numbered choices for standalone C demonstrations. */
void showMenu(void)
{
    puts("\nC data structure demonstrations");
    puts("1 Search and sort  2 Data structures  0 Exit");
    puts("Search and sort demonstrates linear search, binary search, and insertion sort.");
    puts("Binary search expects its input array to already be sorted.");
    puts("Data structures demonstrates an array stack and a circular array queue.");
    puts("It also demonstrates linked lists, binary search trees, and a hash table.");
    puts("The examples show normal use and how empty or full structures respond.");
    puts("Choose 0 whenever you want to return from this standalone C program.");
    puts("Each example also releases any dynamically allocated nodes before it ends.");
    puts("The menu repeats so each example can be selected more than once.");
    puts("Search and sort uses integer arrays with small readable sample values.");
    puts("Data structure examples use fixed-size arrays or dynamically allocated nodes.");
    puts("No extra header or support source file is needed for this C demonstration.");
    puts("The stack and queue show their capacity limits and empty behavior.");
    puts("The list and tree examples also release their allocated memory.");
    puts("The hash table example shows how multiple keys share a bucket.");
    puts("These examples use one algorithm or data structure per task.");
}

/* Purpose: call the selected demonstration from switch cases until exit. */
int main(void)
{
    int choice = -1;
    while (choice != 0)
    {
        showMenu();
        printf("Choose: ");
        if (scanf("%d", &choice) != 1)
        {
            return 0;
        }
        switch (choice)
        {
            case 0:
                break;
            case 1:
                demoSearch();
                break;
            case 2:
                demoStructures();
                break;
            default:
                puts("Invalid choice.");
                break;
        }
    }
    return 0;
}
