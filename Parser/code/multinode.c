#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "multinode.h"

struct multinode *createNode(char *data, int line)
{
    struct multinode *node = (struct multinode *)malloc(sizeof(struct multinode));
    strcpy(node->data, data);
    node->line = line;
    node->layer = 1;
    node->parent = NULL;
    node->child = NULL;
    return node;
}

void addNode(struct multinode *parent, struct multinode *child)
{
    if (parent == NULL || child == NULL)
        return;
    child->parent = parent;

    struct listnode *node = (struct listnode *)malloc(sizeof(struct listnode));
    node->data = child;
    node->next = NULL;
    if (parent->child == NULL)
    {
        parent->child = node;
    }
    else
    {
        struct listnode *temp = parent->child;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = node;
    }
}

void Print_Tree(struct multinode *root, int root_layer)
{
    if (root == NULL)
    {
        return;
    }
    for (int i = root->layer; i < root_layer; i++)
    {
        printf("  ");
    }

    if (root->line == -1)
        printf("%s\n", root->data);

    else
        printf("%s (%d)\n", root->data, root->line);

    struct listnode *temp = root->child;
    free(root);
    root = NULL;

    if (temp != NULL)
        root_layer++;

    while (temp != NULL)
    {
        Print_Tree(temp->data, root_layer);
        temp = temp->next;
    }
    free(temp);
    temp = NULL;

    return;
}

char *join_str(char *a, char *b)
{
    char *res = (char *)malloc(sizeof(char *) * (strlen(a) + strlen(b)));
    sprintf(res, "%s%s", a, b);
    return res;
}

int oct2dec(const char *str)
{
    int len = strlen(str);
    int ret = 0;
    int pow = 0;
    for (int i = len - 1; i > 0; i--)
    {
        int tmp = str[i] - '0';
        for (int j = 0; j < pow; j++)
        {
            tmp *= 8;
        }
        ret += tmp;
        pow++;
    }
    return ret;
}

int hex2dec(const char *str)
{
    int len = strlen(str);
    int ret = 0;
    int pow = 0;
    int tmp = 0;
    for (int i = len - 1; i > 1; i--)
    {
        if (str[i] >= 'a')
            tmp = str[i] - 'a' + 10;
        else if (str[i] >= 'A')
            tmp = str[i] - 'A' + 10;
        else
            tmp = str[i] - '0';
        for (int j = 0; j < pow; j++)
        {
            tmp *= 16;
        }
        ret += tmp;
        pow++;
    }
    return ret;
}

float pow2dec(const char *str)
{
    int pow = 0;
    int tmp_num = 0;
    int tmp_pow = 0;
    // spilt the str
    char *e_ptr = strrchr(str, 'e');
    if (e_ptr == NULL)
        e_ptr = strrchr(str, 'E');

    if (e_ptr != NULL)
    {
        for (int i = strlen(e_ptr) - 1; i >= 0; i--)
        {
            if (e_ptr[i] < '0' || e_ptr[i] > '9')
                break;
            tmp_num = e_ptr[i] - '0';
            for (int j = 0; j < tmp_pow; j++)
            {
                tmp_num *= 10;
            }
            pow += tmp_num;
            tmp_pow++;
        }

        char *tmp_ptr = e_ptr;
        tmp_ptr--;
        float exp = 0;
        tmp_pow = 0;
        while (tmp_ptr >= str)
        {
            if (*tmp_ptr == '.')
            {
                if (*(e_ptr + 1) == '-')
                    pow += tmp_pow;
                else
                    pow -= tmp_pow;
                tmp_ptr--;
            }
            else
            {
                tmp_num = *tmp_ptr - '0';
                for (int i = 0; i < tmp_pow; i++)
                    tmp_num *= 10;

                exp += tmp_num;
                tmp_pow++;
                tmp_ptr--;
            }
        }
        if (*(e_ptr + 1) == '-')
        {
            for (int i = 0; i < pow; i++)
            {
                exp /= 10;
            }
        }
        else
        {
            for (int i = 0; i < pow; i++)
            {
                exp *= 10;
            }
        }
        return exp;
    }

    else
    {
        float exp = 0.0;
        pow = 0;
        for (int i = strlen(str) - 1; i >= 0; i--)
        {
            if (str[i] == '.')
                pow += tmp_pow;
            else
            {
                tmp_num = str[i] - '0';
                for (int i = 0; i < tmp_pow; i++)
                    tmp_num *= 10;
                exp += tmp_num;
                tmp_pow++;
            }
        }
        for (int i = 0; i < pow; i++)
        {
            exp /= 10;
        }

        return exp;
    }
}
