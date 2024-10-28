#include <stdio.h>
#include <stdlib.h>

enum Color { RED, BLACK };

// Structure of the Red-Black Tree node
typedef struct RBTreeNode {
    int data;
    enum Color color;
    struct RBTreeNode *left, *right, *parent;
} RBTreeNode;

// Global sentinel TNULL node
RBTreeNode *TNULL;

// Function prototypes
void initializeTNULL();
void fixDeletion(RBTreeNode **root, RBTreeNode *x);
RBTreeNode* createNode(int data);
void leftRotate(RBTreeNode **root, RBTreeNode *x);
void rightRotate(RBTreeNode **root, RBTreeNode *y);
void fixViolation(RBTreeNode **root, RBTreeNode *z);
RBTreeNode* insert(RBTreeNode* root, int data);
RBTreeNode* deleteNode(RBTreeNode* root, int data);
RBTreeNode* search(RBTreeNode* root, int data);
RBTreeNode* findMax(RBTreeNode* node);
RBTreeNode* findMin(RBTreeNode* node);
int height(RBTreeNode* node);
int blackNodeCount(RBTreeNode* node);
void inOrder(RBTreeNode* root);
void preOrder(RBTreeNode* root);
void postOrder(RBTreeNode* root);
void displayNode(RBTreeNode* node);
void display(RBTreeNode* root);
void menu();

// Function to initialize TNULL node
void initializeTNULL() {
    TNULL = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    TNULL->color = BLACK;
    TNULL->left = NULL;
    TNULL->right = NULL;
    TNULL->parent = NULL;
}

// Function to create a new node
RBTreeNode* createNode(int data) {
    RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    node->data = data;
    node->left = TNULL;
    node->right = TNULL;
    node->parent = NULL;
    node->color = RED; // New nodes are always red
    return node;
}

// Left rotate function
void leftRotate(RBTreeNode **root, RBTreeNode *x) {
    RBTreeNode *y = x->right;
    x->right = y->left;
    if (y->left != TNULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotate function
void rightRotate(RBTreeNode **root, RBTreeNode *y) {
    RBTreeNode *x = y->left;
    y->left = x->right;
    if (x->right != TNULL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL)
        *root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// Fix the red-black tree after insertion
void fixViolation(RBTreeNode **root, RBTreeNode *z) {
    while (z != *root && z->parent->color == RED) {
        RBTreeNode *grandparent = z->parent->parent;
        if (z->parent == grandparent->left) {
            RBTreeNode *uncle = grandparent->right;
            if (uncle->color == RED) {
                grandparent->color = RED;
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z = grandparent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                grandparent->color = RED;
                rightRotate(root, grandparent);
            }
        } else {
            RBTreeNode *uncle = grandparent->left;
            if (uncle->color == RED) {
                grandparent->color = RED;
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z = grandparent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                grandparent->color = RED;
                leftRotate (root, grandparent);
            }
        }
    }
    (*root)->color = BLACK;
}

// Insert a new node with given data
RBTreeNode* insert(RBTreeNode* root, int data) {
    RBTreeNode *newNode = createNode(data);
    RBTreeNode *y = NULL;
    RBTreeNode *x = root;

    while (x != TNULL) {
        y = x;
        if (data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    newNode->parent = y;
    if (y == NULL)
        root = newNode;
    else if (data < y->data)
        y->left = newNode;
    else
        y->right = newNode;

    fixViolation(&root, newNode);
    return root;
}

// Delete a node from the Red-Black Tree
RBTreeNode* deleteNode(RBTreeNode* root, int data) {
    RBTreeNode *z = root, *x, *y;
    
    while (z != TNULL) {
        if (z->data == data)
            break;
        if (data < z->data)
            z = z->left;
        else
            z = z->right;
    }

    if (z == TNULL) {
        printf("Data not found in tree.\n");
        return root;
    }

    y = z;
    enum Color yOriginalColor = y->color;

    if (z->left == TNULL) {
        x = z->right;
        if (z->parent == NULL)
            root = z->right;
        else if (z == z->parent->left)
            z->parent->left = z->right;
        else
            z->parent->right = z->right;
        if (z->right) z->right->parent = z->parent;
    } else if (z->right == TNULL) {
        x = z->left;
        if (z->parent == NULL)
            root = z->left;
        else if (z == z->parent->left)
            z->parent->left = z->left;
        else
            z->parent->right = z->left;
        if (z->left) z->left->parent = z->parent;
    } else {
        y = z->right;
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            if (x) x->parent = y;
        else {
            if (y->parent) y->parent->left = x;
            if (x) x->parent = y->parent;
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }
        if (z->parent == NULL)
            root = y;
        else if (z == z->parent->left)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        y->color = z->color;
        y->left = z->left;
        if (y->left) y->left->parent = y;
    }

    if (yOriginalColor == BLACK)
        fixDeletion(&root, x);
    
    free(z);
    return root;
}

// Fix the red-black tree after deletion
void fixDeletion(RBTreeNode **root, RBTreeNode *x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBTreeNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if ((w->left == TNULL || w->left->color == BLACK) &&
                (w->right == TNULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == TNULL || w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right) w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            RBTreeNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent );
                w = x->parent->left;
            }
            if ((w->right == TNULL || w->right->color == BLACK) &&
                (w->left == TNULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == TNULL || w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left) w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    if (x) x->color = BLACK;
}

// Search for a value in the tree
// Search for a value in the tree
RBTreeNode* search(RBTreeNode* root, int data) {
    // Traverse the tree
    while (root != TNULL) {
        if (root->data == data) {
            return root; // Found the node
        }
        if (data < root->data) {
            root = root->left; // Go left
        } else {
            root = root->right; // Go right
        }
    }
    return TNULL; // Return TNULL if not found
}

// Find the maximum value in the tree
RBTreeNode* findMax(RBTreeNode* node) {
    while (node->right != TNULL)
        node = node->right;
    return node;
}

// Find the minimum value in the tree
RBTreeNode* findMin(RBTreeNode* node) {
    while (node->left != TNULL)
        node = node->left;
    return node;
}

// Calculate height of the tree
int height(RBTreeNode* node) {
    if (node == TNULL)
        return 0;
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// Count the number of black nodes along any path from the root to a leaf
int blackNodeCount(RBTreeNode* node) {
    int count = 0;
    while (node != TNULL) {
        if (node->color == BLACK)
            count++;
        node = node->left;
    }
    return count;
}

// In-order traversal
void inOrder(RBTreeNode* root) {
    if (root == TNULL)
        return;
    inOrder(root->left);
    printf("%d (%s) ", root->data, root->color == RED ? "Red" : "Black");
    inOrder(root->right);
}

// Pre-order traversal
void preOrder(RBTreeNode* root) {
    if (root == TNULL)
        return;
    printf("%d (%s) ", root->data, root->color == RED ? "Red" : "Black");
    preOrder(root->left);
    preOrder(root->right);
}

// Post-order traversal
void postOrder(RBTreeNode* root) {
    if (root == TNULL)
        return;
    postOrder(root->left);
    postOrder(root->right);
    printf("%d (%s) ", root->data, root->color == RED ? "Red" : "Black");
}

// Display a single node with its color and children
void displayNode(RBTreeNode* node) {
    if (node != TNULL) {
        printf("Node: %d (Color: %s)\n", node->data, (node->color == RED) ? "RED" : "BLACK");
        
        if (node->left != TNULL)
            printf("  Left Child: %d\n", node->left->data);
        else
            printf("  Left Child: NULL\n");

        if (node->right != TNULL)
            printf("  Right Child: %d\n", node->right->data);
        else
            printf("  Right Child: NULL\n");

        printf("\n");

        displayNode(node->left);
        displayNode(node->right);
    }
}

// Display function that starts the recursive display from the root node
void display(RBTreeNode* root) {
    if (root == TNULL) {
        printf("The tree is empty.\n");
    } else {
        printf("Displaying the Red-Black Tree:\n\n");
        displayNode(root);
    }
}

int main() {
    initializeTNULL();
    
    RBTreeNode* root = TNULL;
    int choice, data;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Insert\n2. Delete\n3. Search\n4. Find Max\n5. Find Min\n6. Height\n");
        printf("7. Black Node Count\n8. In-order Traversal\n9. Pre-order Traversal\n10. Post-order Traversal\n11. Display\n0. Exit\n");
        printf(" Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter data to insert: ");
                scanf("%d", &data);
                root = insert(root, data);
                break;
            case 2:
                printf("Enter data to delete: ");
                scanf("%d", &data);
                root = deleteNode(root, data);
                break;
            case 3:
                 printf("Enter data to search: ");
                 scanf("%d", &data);
                  RBTreeNode* searchResult = search(root, data);
                  if (searchResult != TNULL) // Check against TNULL instead of NULL
                     printf("Data found: %d (%s)\n", searchResult->data, searchResult->color == RED ? "Red" : "Black");
                  else
                      printf("Data not found.\n");
                  break;
            case 4:
                if (root != TNULL) {
                    RBTreeNode* maxNode = findMax(root);
                    printf("Max value: %d (%s)\n", maxNode->data, maxNode->color == RED ? "Red" : "Black");
                } else {
                    printf("Tree is empty.\n");
                }
                break;
            case 5:
                if (root != TNULL) {
                    RBTreeNode* minNode = findMin(root);
                    printf("Min value: %d (%s)\n", minNode->data, minNode->color == RED ? "Red" : "Black");
                } else {
                    printf("Tree is empty.\n");
                }
                break;
            case 6:
                printf("Height of the tree: %d\n", height(root));
                break;
            case 7:
                printf("Black node count: %d\n", blackNodeCount(root));
                break;
            case 8:
                printf("In-order traversal: ");
                inOrder(root);
                printf("\n");
                break;
            case 9:
                printf("Pre-order traversal: ");
                preOrder(root);
                printf("\n");
                break;
            case 10:
                printf("Post-order traversal: ");
                postOrder(root);
                printf("\n");
                break;
            case 11:
                display(root);
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}