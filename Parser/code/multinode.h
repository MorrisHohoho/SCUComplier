#ifndef _MULTINODE_H_
#define _MULTINODE_H_
#define  MAX_TOKEN_LEN 128


struct listnode{
    struct multinode* data;
    struct listnode* next;
};

struct multinode
{
    char data[MAX_TOKEN_LEN];
    int line;   //the lineno of the token   
    int layer;  //the layer of this node.
    struct multinode* parent;
    struct listnode* child;
};

/**
 * @param data: a string that the syntax tree involve, e.g."DecList"
 * @param line: the lineno of this node.
 * @return: pointer to the this node.
 * 
 * used to to create a new node, which parents node
 * and child node are NULL. The default layer is 1.
 */
struct multinode* createNode(char* data,int line);

/**
 * @param parent: parent node.
 * @param child: child node.
 * 
 * make a node(child) to become another node's(parent) child.
 */
void addNode(struct multinode* parent, struct multinode* child);


/**
 * @param root: root node of the syntax tree.
 * @param root_layer: the layer of root node, used to control to print 
 * how many ' '.
 * 
 * recursive from the root until the child is NULL,
 * once return, the parameter root_layer++
 */
void Print_Tree(struct multinode* root,int root_layer);

/**
 * @param str1: the head str
 * @param str2: the following str.
 * 
 * attach str1 and str2. 
 * e.g: str1:"IN:" str2:"123"
 *      return str: "IN:123"
 * 
 * @return: a pointer to the new string.
 */
char* join_str(char* str1,char* str2);

/**
 * @param str: octal in string.
 * 
 * @return: decimal in int.
 */
int oct2dec(const char* str);

/**
 * @param str: hexdecimal in string.
 * 
 * @return: decimal in int.
 */
int hex2dec(const char* str);

/**
 * @param str:Scientific notation in string
 * 
 * @return: decimal in float.(6 decimal places are reserved)
 */
float pow2dec(const char* str);


#endif
