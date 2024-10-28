#include <stdio.h>
#include <stdlib.h>

// Define Colors
#define RED 1
#define BLACK 0

typedef struct RBTreeNode {
    int data;
    int color;
    struct RBTreeNode* left;
    struct RBTreeNode* right;
    struct RBTreeNode* parent;
} RBTreeNode;

typedef struct RBTree {
    RBTreeNode* root;
    RBTreeNode* TNULL;
} RBTree;

// Utility function to create a new Red-Black Tree node
RBTreeNode* newNode(int data, RBTreeNode* TNULL) {
    RBTreeNode* node = (RBTreeNode*)malloc(sizeof(RBTreeNode));
    node->data = data;
    node->color = RED;
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

// Left rotate
void leftRotate(RBTree* tree, RBTreeNode* x) {
    RBTreeNode* y = x->right;
    x->right = y->left;
    if (y->left != tree->TNULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotate
void rightRotate(RBTree* tree, RBTreeNode* y) {
    RBTreeNode* x = y->left;
    y->left = x->right;
    if (x->right != tree->TNULL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NULL) tree->root = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
}

// Fix violations after insertion
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
        if (k == tree->root) break;
    }
    tree->root->color = BLACK;
}
// Fix violations after deletion
void fixDelete(RBTree* tree, RBTreeNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBTreeNode* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBTreeNode* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Replace node
void rbTransplant(RBTree* tree, RBTreeNode* u, RBTreeNode* v) {
    if (u->parent == NULL) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

// Find minimum node in the tree
RBTreeNode* minimum(RBTree* tree, RBTreeNode* node) {
    while (node->left != tree->TNULL) node = node->left;
    return node;
}
// Search node
RBTreeNode* search(RBTreeNode* root, RBTreeNode* TNULL, int key) {
    if (root == TNULL || root->data == key) return root;
    if (key < root->data) return search(root->left, TNULL, key);
    return search(root->right, TNULL, key);
}
// Delete node
void deleteNode(RBTree* tree, int data) {
    RBTreeNode* z = search(tree->root, tree->TNULL, data);
    if (z == tree->TNULL) {
        printf("Node %d not found.\n", data);
        return;
    }

    RBTreeNode* y = z;
    int yOriginalColor = y->color;
    RBTreeNode* x;

    if (z->left == tree->TNULL) {
        x = z->right;
        rbTransplant(tree, z, z->right);
    } else if (z->right == tree->TNULL) {
        x = z->left;
        rbTransplant(tree, z, z->left);
    } else {
        y = minimum(tree, z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) x->parent = y;
        else {
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
    if (yOriginalColor == BLACK) fixDelete(tree, x);
}
// Insert node
void insert(RBTree* tree, int data) {
    RBTreeNode* node = newNode(data, tree->TNULL);
    RBTreeNode* y = NULL;
    RBTreeNode* x = tree->root;

    while (x != tree->TNULL) {
        y = x;
        if (node->data < x->data) x = x->left;
        else x = x->right;
    }

    node->parent = y;
    if (y == NULL) tree->root = node;
    else if (node->data < y->data) y->left = node;
    else y->right = node;

    if (node->parent == NULL) {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NULL) return;

    fixInsert(tree, node);
}
// Count nodes
int countNodes(RBTreeNode* node, RBTreeNode* TNULL) {
    if (node == TNULL) return 0;
    return 1 + countNodes(node->left, TNULL) + countNodes(node->right, TNULL);
}

// Display tree function
void displayTree(RBTreeNode* root, RBTreeNode* TNULL, int space) {
    if (root == TNULL) return;
    space += 10;
    displayTree(root->right, TNULL, space);
    printf("\n");
    for (int i = 10; i < space; i++) printf(" ");
    printf("%d(%s)\n", root->data, (root->color == RED) ? "R" : "B");
    displayTree(root->left, TNULL, space);
}

// Free the tree nodes
void destroyTree(RBTreeNode* node, RBTreeNode* TNULL) {
    if (node == TNULL) return;
    destroyTree(node->left, TNULL);
    destroyTree(node->right, TNULL);
    free(node);
}

int main() {
    RBTree* tree = createRBTree();
    int choice, data;

    do {
        printf("\n1. Insert\n2. Search\n3. Count Nodes\n4. Delete\n5. Display\n6. Destroy Tree\n7. Exit\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter data to insert: ");
                scanf("%d", &data);
                insert(tree, data);
                break;

            case 2:
                printf("Enter data to search: ");
                scanf("%d", &data);
                RBTreeNode* node = search(tree->root, tree->TNULL, data);
                if (node != tree->TNULL) printf("Node %d found.\n", data);
                else printf("Node %d not found.\n", data);
                break;

            case 3:
                printf("Total nodes: %d\n", countNodes(tree->root, tree->TNULL));
                break;

            case 4:
                printf("Enter data to delete: ");
                scanf("%d", &data);
                deleteNode(tree, data);
                break;

            case 5:
                printf("Red-Black Tree:\n");
                displayTree(tree->root, tree->TNULL, 0);
                break;

            case 6:
                destroyTree(tree->root, tree->TNULL);
                tree->root = tree->TNULL;
                printf("Tree destroyed.\n");
                break;

            case 7:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 7);

    // Destroy the tree on exit
    destroyTree(tree->root, tree->TNULL);
    free(tree->TNULL);
    free(tree);

    return 0;
}
