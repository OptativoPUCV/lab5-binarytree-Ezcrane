#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};


int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL || key == NULL || value == NULL) return;
    if (tree->root == NULL)	
    {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
    }
    else
    {
        TreeNode * aux = tree->root;
        while (aux != NULL)
            {
                if (is_equal(tree, key, aux->pair->key) == 1) return;
                if (tree->lower_than(key, aux->pair->key) == 1)
                {
                    if (aux->left == NULL)
                    {
                        aux->left = createTreeNode(key, value);
                        aux->left->parent = aux;
                        tree->current = aux->left;
                        return;
                    }
                    else
                    {
                        aux = aux->left;
                    }
                }
                else
                {
                    if (aux->right == NULL)
                    {
                        aux->right = createTreeNode(key, value);
                        aux->left->parent = aux;
                        tree->current = aux->right;
                        return;
                    }
                    else
                    {
                        aux = aux->right;
                    }
                }
            }
    }
}

TreeNode * minimum(TreeNode * x)
{
    while (x->left != NULL)
    {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
    if (tree == NULL || node == NULL) return;
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent == NULL)
        {
            tree->root = NULL;
        }
        else
        {
            if (node->parent->left == node)
            {
                node->parent->left = NULL;
            }
            else if(node->parent->right == node)
            {
                node->parent->right = NULL;
            }
        }
        free(node);
    }
    else if (node->left == NULL || node->right == NULL)
    {
        TreeNode * hijo;
        if(node->left != NULL)
        {
            hijo = node->left;
        }
        else
        {
            hijo = node->right;    
        }
        if(node != tree->root)
        {
            if(node->parent->left == node)
            {
                hijo->parent = node->parent;
                node->parent->left = hijo;
            }
            else
            {
                hijo->parent = node->parent;
                node->parent->right = hijo;
            }
            free(node);
        }
        else
        {
            tree->root = hijo;  
        }
    }
    else if(node->left != NULL && node->right != NULL)
    {
        TreeNode * aux = minimum(node->right);
        void * key = aux->pair->key;
        void * value = aux->pair->value;
        removeNode(tree, aux);
        node->pair->key = key;
        node->pair->value = value;
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) 
{
    if(tree == NULL || key == NULL || tree->root == NULL) return NULL;
    TreeNode * aux = tree->root;
    while(aux != NULL)
    {
        if(is_equal(tree, key, aux->pair->key) == 1)
        {
            tree->current = aux;
            return aux->pair;
        }
        else if (tree->lower_than(key, aux->pair->key) == 1)
        {
            aux = aux->left;
            tree->current = aux;
        }
        else
        {
            aux = aux->right;
            tree->current = aux;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
    TreeNode * aux = tree->root;
    tree->current = aux;
    TreeNode * UB = NULL;

    while(aux != NULL)
        {
            if(tree->lower_than(key, aux->pair->key) == 1)
            {
                UB = aux;
                aux = aux->left;
                tree->current = aux;
            }
            else if (tree->lower_than(aux->pair->key, key))
            {
                aux = aux->right;
                tree->current = aux;
            }
            else if (is_equal(tree, key, aux->pair->key) == 1) return aux->pair;
        }
    return UB->pair;
}

Pair * firstTreeMap(TreeMap * tree) 
{
    TreeNode * aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
    TreeNode * aux = tree->current;
    if(aux == NULL) return NULL;
    if(aux->right != NULL)
    {
        TreeNode * next = minimum(aux->right);
        if(next == NULL)
        {
            return NULL;
        }
        tree->current = next;
        return next->pair;
    }
    else
    {
        while(aux->parent != NULL && aux->parent->right == aux)
        {
            aux = aux->parent;
            tree->current = aux;
        }
        if (aux->parent == NULL) return NULL;
        tree->current = aux->parent;
        return aux->parent->pair;
    }
}
