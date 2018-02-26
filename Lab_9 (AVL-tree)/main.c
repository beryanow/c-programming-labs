#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int key;
    unsigned char height;
    struct node *left;
    struct node *right;
} tree_node;

unsigned char tree_height(tree_node *p) {
    if (p != 0)
        return p->height;
    else
        return 0;
}

void tree_height_fix(tree_node *p) {
    unsigned char left_height = tree_height(p->left);
    unsigned char right_height = tree_height(p->right);
    if (left_height > right_height)
        p->height = (unsigned char) (left_height + 1);
    else p->height = (unsigned char) (right_height + 1);
}

tree_node *tree_rotate_right(tree_node *p) {
    tree_node *q = p->left;
    p->left = q->right;
    q->right = p;
    tree_height_fix(p);
    tree_height_fix(q);
    return q;
}

tree_node *tree_rotate_left(tree_node *q) {
    tree_node *p = q->right;
    q->right = p->left;
    p->left = q;
    tree_height_fix(q);
    tree_height_fix(p);
    return p;
}

int tree_balance_factor(tree_node *p) {
    return tree_height(p->right) - tree_height(p->left);
}


tree_node *tree_balance(tree_node *p) {
    tree_height_fix(p);
    if (tree_balance_factor(p) == 2) {
        if (tree_balance_factor(p->right) == -1)
            p->right = tree_rotate_right(p->right);
        return tree_rotate_left(p);
    }
    if (tree_balance_factor(p) == -2) {
        if (tree_balance_factor(p->left) == 1)
            p->left = tree_rotate_left(p->left);
        return tree_rotate_right(p);
    }
    return p;
}

tree_node *new_tree_node(int key) {
    tree_node *new = (tree_node *) malloc(sizeof(tree_node));
    new->key = key;
    new->height = 1;
    new->left = 0;
    new->right = 0;
    return new;
}

tree_node *tree_insert(tree_node *p, int k) {
    if (p == 0) {
        return new_tree_node(k);
    }
    if (k < p->key)
        p->left = tree_insert(p->left, k);
    else p->right = tree_insert(p->right, k);
    return tree_balance(p);
}

void free_tree(tree_node *p) {
    if (p == NULL)
        return;
    free_tree(p->left);
    free_tree(p->right);
    free(p);
}

int main() {
    struct node *tree = NULL;
    int n = 0;
    int cur = 0;
    scanf("%d", &n);
    while (n) {
        scanf("%d", &cur);
        tree = tree_insert(tree, cur);
        n--;
    }
    printf("%d\n", tree_height(tree));
    free_tree(tree);
    return 0;
}