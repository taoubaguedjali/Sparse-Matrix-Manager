#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Node {
    int row;
    int col;
    int val;
    struct Node *next;
} Node;

// Allocate a 2D matrix
int** allocateMatrix(int rows, int cols) {
    int **mat = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++)
        mat[i] = (int*)malloc(cols * sizeof(int));
    return mat;
}

void freeMatrix(int** mat, int rows) {
    for (int i = 0; i < rows; i++) free(mat[i]);
    free(mat);
}

// 3. Input the matrix from user
void inputMatrix(int** mat, int rows, int cols) {
    printf("Enter elements of the %dx%d matrix:\n", rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            printf("mat[%d][%d] = ", i, j);
            scanf("%d", &mat[i][j]);
        }
}

// 4. Display the matrix in dense form
void displayDense(int** mat, int rows, int cols) {
    printf("\nDense Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

// Count non-zero elements
int countNonZero(int** mat, int rows, int cols) {
    int count = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (mat[i][j] != 0) count++;
    return count;
}

// 5. Check if the matrix is sparse (≤ 30% non-zero)
bool isSparseMatrix(int** mat, int rows, int cols) {
    int total = rows * cols;
    if (total == 0) return false;
    return (countNonZero(mat, rows, cols) * 100.0 / total) <= 30.0;
}

// 6. Convert dense matrix to singly linked list (non-zero elements only)
Node* convertToLinkedList(int** mat, int rows, int cols) {
    Node *head = NULL, *tail = NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (mat[i][j] != 0) {
                Node *newNode = (Node*)malloc(sizeof(Node));
                newNode->row = i;
                newNode->col = j;
                newNode->val = mat[i][j];
                newNode->next = NULL;
                if (head == NULL) head = tail = newNode;
                else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }
        }
    }
    return head;
}

void freeLinkedList(Node* head) {
    Node *temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Display the linked list
void displayLinkedList(Node* head) {
    printf("\nSparse Linked List:\n");
    Node *curr = head;
    while (curr) {
        printf("(%d, %d, %d) → ", curr->row, curr->col, curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

// 7. Search for an element by position (i, j)
int searchElement(Node* head, int i, int j) {
    Node *curr = head;
    while (curr) {
        if (curr->row == i && curr->col == j)
            return curr->val;
        curr = curr->next;
    }
    return 0;
}

// Insert at the beginning
Node* insertAtBeginning(Node* head, int r, int c, int v) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->row = r; newNode->col = c; newNode->val = v;
    newNode->next = head;
    return newNode;
}

// Insert at the end
Node* insertAtEnd(Node* head, int r, int c, int v) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->row = r; newNode->col = c; newNode->val = v;
    newNode->next = NULL;
    if (!head) return newNode;
    Node *curr = head;
    while (curr->next) curr = curr->next;
    curr->next = newNode;
    return head;
}

// 8. Insert in a random position (beginning / end / middle) based on array representation
Node* insertRandomPosition(Node* head, int r, int c, int v) {
    if (!head) return insertAtBeginning(head, r, c, v);

    int count = 0;
    Node *curr = head;
    while (curr) { count++; curr = curr->next; }

    int pos = rand() % (count + 1);   // 0 = beginning, count = end
    if (pos == 0) return insertAtBeginning(head, r, c, v);
    if (pos == count) return insertAtEnd(head, r, c, v);

    // Insert in the middle
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->row = r; newNode->col = c; newNode->val = v;
    curr = head;
    for (int i = 1; i < pos; i++) curr = curr->next;
    newNode->next = curr->next;
    curr->next = newNode;
    return head;
}

// 9. Delete an element by position (i, j)
Node* deleteElement(Node* head, int i, int j) {
    if (!head) return NULL;
    if (head->row == i && head->col == j) {
        Node *temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    Node *curr = head;
    while (curr->next) {
        if (curr->next->row == i && curr->next->col == j) {
            Node *temp = curr->next;
            curr->next = curr->next->next;
            free(temp);
            return head;
        }
        curr = curr->next;
    }
    return head;
}

// 10. Reconstruct the dense matrix from the linked list
int** reconstructMatrix(Node* head, int rows, int cols) {
    int** mat = allocateMatrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            mat[i][j] = 0;

    Node *curr = head;
    while (curr) {
        if (curr->row < rows && curr->col < cols)
            mat[curr->row][curr->col] = curr->val;
        curr = curr->next;
    }
    return mat;
}

// Largest non-zero value
int largestNonZero(Node* head) {
    if (!head) return 0;
    int maxV = head->val;
    Node *curr = head->next;
    while (curr) {
        if (curr->val > maxV) maxV = curr->val;
        curr = curr->next;
    }
    return maxV;
}

// Row with the largest number of non-zero elements
int rowWithMostNonZero(Node* head, int rows) {
    int *rowCount = (int*)calloc(rows, sizeof(int));
    Node *curr = head;
    while (curr) {
        if (curr->row < rows) rowCount[curr->row]++;
        curr = curr->next;
    }
    int maxRow = 0, maxC = 0;
    for (int i = 0; i < rows; i++) {
        if (rowCount[i] > maxC) { maxC = rowCount[i]; maxRow = i; }
    }
    free(rowCount);
    return maxRow;
}

// Column with the largest number of non-zero elements
int colWithMostNonZero(Node* head, int cols) {
    int *colCount = (int*)calloc(cols, sizeof(int));
    Node *curr = head;
    while (curr) {
        if (curr->col < cols) colCount[curr->col]++;
        curr = curr->next;
    }
    int maxCol = 0, maxC = 0;
    for (int j = 0; j < cols; j++) {
        if (colCount[j] > maxC) { maxC = colCount[j]; maxCol = j; }
    }
    free(colCount);
    return maxCol;
}

// Additional feature 1: Sum of two sparse matrices of the same dimensions
Node* sumSparseMatrices(Node* head1, Node* head2, int rows, int cols) {
    int** temp = allocateMatrix(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) temp[i][j] = 0;

    Node *curr = head1;
    while (curr) { temp[curr->row][curr->col] = curr->val; curr = curr->next; }
    curr = head2;
    while (curr) { temp[curr->row][curr->col] += curr->val; curr = curr->next; }

    Node* sumHead = convertToLinkedList(temp, rows, cols);
    freeMatrix(temp, rows);
    return sumHead;
}

// Additional feature 2: Compare memory usage
void compareMemory(int rows, int cols, int nz) {
    size_t denseMem = rows * sizeof(int*) + rows * cols * sizeof(int);
    size_t listMem = nz * sizeof(Node);
    printf("\nDense array memory usage: %zu bytes\n", denseMem);
    printf("Linked list memory usage: %zu bytes\n", listMem);
    printf(listMem < denseMem ? "Linked list is more memory efficient.\n" : "Dense array uses less or equal memory.\n");
}

// Additional feature 3: Save sparse matrix to a readable text file
void saveToFile(Node* head, int rows, int cols) {
    FILE *fp = fopen("sparse_matrix.txt", "w");
    if (!fp) { printf("Error opening file!\n"); return; }
    fprintf(fp, "Rows: %d Cols: %d\n", rows, cols);
    Node *curr = head;
    while (curr) {
        fprintf(fp, "%d %d %d\n", curr->row, curr->col, curr->val);
        curr = curr->next;
    }
    fclose(fp);
    printf("Matrix saved to sparse_matrix.txt successfully.\n");
}

// Additional feature 4: Load sparse matrix from file
Node* loadFromFile(int *rows, int *cols) {
    FILE *fp = fopen("sparse_matrix.txt", "r");
    if (!fp) { printf("File not found!\n"); return NULL; }
    fscanf(fp, "Rows: %d Cols: %d\n", rows, cols);
    Node *head = NULL, *tail = NULL;
    int r, c, v;
    while (fscanf(fp, "%d %d %d", &r, &c, &v) == 3) {
        Node *newNode = (Node*)malloc(sizeof(Node));
        newNode->row = r; newNode->col = c; newNode->val = v;
        newNode->next = NULL;
        if (!head) head = tail = newNode;
        else { tail->next = newNode; tail = newNode; }
    }
    fclose(fp);
    return head;
}

int main() {
    srand(time(NULL));
    int rows = 0, cols = 0;
    int **denseMat = NULL;
    Node *sparseList = NULL;
    int choice;

    printf("=== Sparse Matrix Manager ===\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Input a new matrix (dense)\n");
        printf("2. Display dense matrix\n");
        printf("3. Check if matrix is sparse\n");
        printf("4. Convert to linked list\n");
        printf("5. Display linked list\n");
        printf("6. Search element by position (i,j)\n");
        printf("7. Insert new non-zero element (random position)\n");
        printf("8. Delete element by position (i,j)\n");
        printf("9. Reconstruct dense matrix from list\n");
        printf("10. Find largest non-zero value\n");
        printf("11. Row with most non-zero elements\n");
        printf("12. Column with most non-zero elements\n");
        printf("13. Add two sparse matrices\n");
        printf("14. Compare memory usage\n");
        printf("15. Save to text file\n");
        printf("16. Load from text file\n");
        printf("17. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (denseMat) freeMatrix(denseMat, rows);
            if (sparseList) freeLinkedList(sparseList);
            printf("Number of rows: "); scanf("%d", &rows);
            printf("Number of columns: "); scanf("%d", &cols);
            denseMat = allocateMatrix(rows, cols);
            inputMatrix(denseMat, rows, cols);
            sparseList = NULL;
            printf("Matrix input successfully.\n");
        }
        else if (choice == 2) {
            if (!denseMat) { printf("No dense matrix yet!\n"); continue; }
            displayDense(denseMat, rows, cols);
        }
        else if (choice == 3) {
            if (!denseMat) { printf("Enter a matrix first!\n"); continue; }
            printf(isSparseMatrix(denseMat, rows, cols) ? "The matrix is sparse.\n" : "The matrix is NOT sparse.\n");
        }
        else if (choice == 4) {
            if (!denseMat) { printf("Enter a matrix first!\n"); continue; }
            if (sparseList) freeLinkedList(sparseList);
            sparseList = convertToLinkedList(denseMat, rows, cols);
            printf("Conversion to linked list completed.\n");
            displayLinkedList(sparseList);
        }
        else if (choice == 5) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            displayLinkedList(sparseList);
        }
        else if (choice == 6) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            int i, j;
            printf("Row i: "); scanf("%d", &i);
            printf("Column j: "); scanf("%d", &j);
            printf("Value at (%d,%d) = %d\n", i, j, searchElement(sparseList, i, j));
        }
        else if (choice == 7) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            int r, c, v;
            printf("Row: "); scanf("%d", &r);
            printf("Column: "); scanf("%d", &c);
            printf("Value (must be non-zero): "); scanf("%d", &v);
            if (v == 0) { printf("Value must be non-zero!\n"); continue; }
            sparseList = insertRandomPosition(sparseList, r, c, v);
            printf("Element inserted at random position.\n");
            displayLinkedList(sparseList);
        }
        else if (choice == 8) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            int i, j;
            printf("Row i: "); scanf("%d", &i);
            printf("Column j: "); scanf("%d", &j);
            sparseList = deleteElement(sparseList, i, j);
            printf("Element deleted (if it existed).\n");
            displayLinkedList(sparseList);
        }
        else if (choice == 9) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            if (denseMat) freeMatrix(denseMat, rows);
            denseMat = reconstructMatrix(sparseList, rows, cols);
            printf("Dense matrix reconstructed successfully.\n");
            displayDense(denseMat, rows, cols);
        }
        else if (choice == 10) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            printf("Largest non-zero value = %d\n", largestNonZero(sparseList));
        }
        else if (choice == 11) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            printf("Row with most non-zero elements = %d\n", rowWithMostNonZero(sparseList, rows));
        }
        else if (choice == 12) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            printf("Column with most non-zero elements = %d\n", colWithMostNonZero(sparseList, cols));
        }
        else if (choice == 13) {   // Sum of two sparse matrices
            if (rows == 0 || cols == 0) { printf("Enter the first matrix first!\n"); continue; }
            printf("Enter the second matrix (same dimensions %dx%d):\n", rows, cols);
            int **dense2 = allocateMatrix(rows, cols);
            inputMatrix(dense2, rows, cols);
            Node *sparse2 = convertToLinkedList(dense2, rows, cols);
            Node *sumList = sumSparseMatrices(sparseList, sparse2, rows, cols);
            printf("\nResult of addition (Sparse):\n");
            displayLinkedList(sumList);
            freeLinkedList(sparse2);
            freeMatrix(dense2, rows);
            freeLinkedList(sumList);   // only for display
        }
        else if (choice == 14) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            int nz = 0;
            Node *c = sparseList;
            while (c) { nz++; c = c->next; }
            compareMemory(rows, cols, nz);
        }
        else if (choice == 15) {
            if (!sparseList) { printf("No linked list yet!\n"); continue; }
            saveToFile(sparseList, rows, cols);
        }
        else if (choice == 16) {
            if (denseMat) freeMatrix(denseMat, rows);
            if (sparseList) freeLinkedList(sparseList);
            sparseList = loadFromFile(&rows, &cols);
            if (sparseList) {
                printf("Loaded successfully (dimensions: %dx%d)\n", rows, cols);
                displayLinkedList(sparseList);
            }
        }
        else if (choice == 17) {
            if (denseMat) freeMatrix(denseMat, rows);
            if (sparseList) freeLinkedList(sparseList);
            printf("Thank you for using the program!\n");
            break;
        }
        else printf("Invalid choice!\n");
    }
    return 0;
}