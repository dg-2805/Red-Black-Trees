#include <stdio.h>
#include <stdlib.h>

// Define Colors
#define RED 1
#define BLACK 0

typedef struct RBTreeNode {
    int data;
    int color; // 1 for RED, 0 for BLACK
    struct RBTreeNode* left;
    struct RBTreeNode* right;
    struct RBTreeNode* parent;
} RBTreeNode;

typedef struct RBTree {
    RBTreeNode* root;
    RBTreeNode* TNULL; // NULL node for convenience
} RBTree;

// Utility function to create a new Red-Black Tree node
RBTreeNode* newNode(int data, RBTreeNode* TNULL) {
    RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    node->data = data;
    node->color = RED;  // New node is always red initially
    node->left = TNULL;
    node->right = TNULL;
    node->parent = NULL;
    return node;
}

// Initialize the Red-Black Tree
RBTree* createRBTree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->TNULL = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    tree->TNULL->color = BLACK;
    tree->root = tree->TNULL;
    return tree;
}

// Left rotate subtree rooted with x
void leftRotate(RBTree* tree, RBTreeNode* x) {
    RBTreeNode* y = x->right;
    x->right = y->left;

    if (y->left != tree->TNULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

// Right rotate subtree rooted with y
void rightRotate(RBTree* tree, RBTreeNode* y) {
    RBTreeNode* x = y->left;
    y->left = x->right;

    if (x->right != tree->TNULL) {
        x->right->parent = y;
    }

    x->parent = y->parent;
    if (y->parent == NULL) {
        tree->root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

// Fix violations after insertion to maintain Red-Black properties
void fixInsert(RBTree* tree, RBTreeNode* k) {
    RBTreeNode* u;

    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) {
            u = k->parent->parent->right;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(tree, k->parent->parent);
            }
        } else {
            u = k->parent->parent->left;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(tree, k->parent->parent);
            }
        }
        if (k == tree->root) {
            break;
        }
    }
    tree->root->color = BLACK;
}

// Insertion function
void insert(RBTree* tree, int data) {
    RBTreeNode* node = newNode(data, tree->TNULL);
    RBTreeNode* y = NULL;
    RBTreeNode* x = tree->root;

    while (x != tree->TNULL) {
        y = x;
        if (node->data < x->data) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == NULL) {
        tree->root = node;
    } else if (node->data < y->data) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NULL) {
        return;
    }

    fixInsert(tree, node);
}

// Fix violations after deletion to maintain Red-Black properties
void fixDelete(RBTree* tree, RBTreeNode* x) {
    RBTreeNode* s;
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                s = x->parent->right;
            }

            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(tree, s);
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                s = x->parent->left;
            }

            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(tree, s);
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Transplant function for deletion
void rbTransplant(RBTree* tree, RBTreeNode* u, RBTreeNode* v) {
    if (u->parent == NULL) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Find the minimum node
RBTreeNode* minimum(RBTreeNode* node, RBTreeNode* TNULL) {
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

// Deletion function
void deleteNode(RBTree* tree, int data) {
    RBTreeNode* z = tree->root;
    RBTreeNode* x, *y;
    RBTreeNode* TNULL = tree->TNULL;

    while (z != TNULL) {
        if (z->data == data) {
            break;
        }
        if (data < z->data) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (z == TNULL) {
        printf("Node not found!\n");
        return;
    }

    y = z;
    int yOriginalColor = y->color;
    if (z->left == TNULL) {
        x = z->right;
        rbTransplant(tree, z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(tree, z, z->left);
    } else {
        y = minimum(z->right, TNULL);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK) {
        fixDelete(tree, x);
    }
}

// Display function to print each node's data, color, and child nodes
void displayNode(RBTreeNode* node, RBTreeNode* TNULL) {
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
        displayNode(node->left, TNULL);
        displayNode(node->right, TNULL);
    }
}

// Menu-driven function
int main(){
    RBTree* tree = createRBTree();
    int choice, value;

    do {
        printf("\nRed-Black Tree Operations Menu:\n");
        printf("1. Insert a node\n");
        printf("2. Delete a node\n");
        printf("3. Display tree nodes\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                insert(tree, value);
                printf("Node inserted.\n");
                break;
            case 2:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                deleteNode(tree, value);
                printf("Node deleted.\n");
                break;
            case 3:
                printf("Displaying tree nodes:\n");
                displayNode(tree->root, tree->TNULL);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);
    return 0;
}