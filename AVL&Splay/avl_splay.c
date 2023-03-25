#include <stdio.h>
#include <stdlib.h>

//Struct
typedef struct nodeTree {
    struct nodeTree *rightChild; //Right child of the node
    struct nodeTree *leftChild; //Left child of the node
    int frequencyOfKey; //The frequency of the given key
    int heightNode; //Height of the node
    int dataValue; //The data value the node holds
} NodeTree; //Typedef

//AVL Functions
void insertNodeAVL(NodeTree *rootNode, int newKey); //Insert function
void updateHeightAVL(NodeTree *rootNode, int newKey); //Updating the height after each insertion
void balanceCondition(NodeTree *currentNode, int newKey); //Checking the balance condition after each insertion
void rotationRightAVL(NodeTree *currentNode); //Rotation to right
void rotationLeftAVL(NodeTree *currentNode); //Rotation to left
int getNodeHeight(NodeTree *currentNode); //Return the height of the node
NodeTree* getParentAVL(NodeTree *childNode); //Return the parent of the node

//Splay Functions
void insertNodeSplay(NodeTree *rootNode, int newKey); //Insert function
void splay(NodeTree *rootNode, int newKey); //Splaying the node after each insertion
void splayCondition(NodeTree *currentNode, int newKey); //Checking how to splay the node
void rotationRightSplay(NodeTree *currentNode); //Rotation to right
void rotationLeftSplay(NodeTree *currentNode); //Rotation to left
NodeTree* getParentSplay(NodeTree *childNode); //Return the parent of the node

//Common Functions
NodeTree* getNewNode(int newKey); //Creating a new node to insert to tree
void preOrderPrintTree(NodeTree *currentNode); //Printing the pre-order traversal of the tree

NodeTree *rootAVL, *rootSplay; //Root nodes of AVL and Splay trees
int totalCostAVL = 0, totalCostSplay = 0; //Total costs of AVL and Splay trees

int main() {
    FILE *inputFile = fopen("input1.txt", "r");
    int keyAVL, keySplay;
    fscanf(inputFile, "%d", &keyAVL); //Scaning the each number as integer
    rootAVL = getNewNode(keyAVL); //Creating the root of the AVL tree with the first input
    rootSplay = getNewNode(keyAVL); //Creating the root of the splay tree with the first input
    while (!feof(inputFile)) { //Read the input file until there's no number left unread
        fscanf(inputFile, "%d", &keyAVL);
        keySplay = keyAVL;
        insertNodeAVL(rootAVL, keyAVL); //Keep sending the numbers to be inserted in the AVL tree
        insertNodeSplay(rootSplay, keySplay); //Keep sending the numbers to be inserted in the splay tree
    }
    //Formating the output
    printf("AVL tree:\n");
    preOrderPrintTree(rootAVL);
    printf("\nTotal cost: %d\n", totalCostAVL);
    printf("\nSplay tree:\n");
    preOrderPrintTree(rootSplay);
    printf("\nTotal cost: %d\n", totalCostSplay);
}

//Insert the given nodes to the AVL tree
void insertNodeAVL(NodeTree *rootNode, int newKey) {
    NodeTree *currentNode = rootNode;
    while (currentNode != NULL) { //Continue to traverse the AVL tree until a NULL node is reached
        totalCostAVL++; //Increase the total cost of the AVL tree in each insertion & search
        if (currentNode->dataValue > newKey) { //If the key to be inserted is smaller than the current node, move to the left
            if (currentNode->leftChild != NULL) //If no null node is reached, continue the traversal
                currentNode = currentNode->leftChild;
            else { //If a null node is reached, insert a new node with the given key value
                currentNode->leftChild = getNewNode(newKey);
                updateHeightAVL(rootNode, newKey); //Send the AVL tree to get its height updated
                break;
            }
        } else if (currentNode->dataValue < newKey) { //If the key to be inserted is larger than the current node, move to the right
            if (currentNode->rightChild != NULL) //If no null node is reached, continue the traversal
                currentNode = currentNode->rightChild;
            else { //If a null node is reached, insert a new node with the given key value
                currentNode->rightChild = getNewNode(newKey);
                updateHeightAVL(rootNode, newKey); //Send the AVL tree to get its height updated
                break;
            }
        } else if (currentNode->dataValue == newKey) { //If a node with the given key already exists, don't insert again
            currentNode->frequencyOfKey++;
            break;
        }
    }
}

//Update the height of the nodes after each successful insertion
void updateHeightAVL(NodeTree* rootNode, int newKey) {
    NodeTree *currentNode = rootNode;
    if (currentNode->dataValue > newKey) //If the newly inserted key is smaller than the current value, move to the left
        updateHeightAVL(currentNode->leftChild, newKey);
    else if (currentNode->dataValue < newKey) //If the newly inserted key is larger than the current value, move the the right
        updateHeightAVL(currentNode->rightChild, newKey);
    int newHeight; //The new height of the node
    if (getNodeHeight(currentNode->leftChild) > getNodeHeight(currentNode->rightChild)) //If the left node's height is larger, adjust the height based on left subtree
        newHeight = getNodeHeight(currentNode->leftChild) + 1;
    else //If the right node's height is larger or equal to left's, adjust the height based on the right subtree
        newHeight = getNodeHeight(currentNode->rightChild) + 1;
    currentNode->heightNode = newHeight; //Update the height of the each node above the newly inserted key
    balanceCondition(currentNode, newKey); //Send all the updated nodes to check if there's any balance condition violation
}

void balanceCondition(NodeTree *currentNode, int newKey) { //Check the balance condition of each node to determine if there's any violation in the AVL tree
    if (getNodeHeight(currentNode->leftChild) - getNodeHeight(currentNode->rightChild) > 1) { //If the balance factor is greater than 1
        if (newKey < currentNode->leftChild->dataValue) //Right rotation case (Single rotation)
            rotationRightAVL(currentNode);
        else if (newKey > currentNode->leftChild->dataValue) { //Left-Right rotation case (Double rotation)
            rotationLeftAVL(currentNode->leftChild);
            rotationRightAVL(currentNode);
        }
    } else if (getNodeHeight(currentNode->leftChild) - getNodeHeight(currentNode->rightChild) < -1) { //If the balance factor is smaller than -1
        if (newKey < currentNode->rightChild->dataValue) { //Right-Left rotation case (Double rotation)
            rotationRightAVL(currentNode->rightChild);
            rotationLeftAVL(currentNode);
        } else if (newKey > currentNode->rightChild->dataValue) //Left rotation case (Single rotation)
            rotationLeftAVL(currentNode);
    }
}

void rotationRightAVL(NodeTree *currentNode) { //Rotate the subtree with the given current root to the right
    totalCostAVL++; //Increase the total cost of the AVL tree in each successful rotation
    NodeTree *left = currentNode->leftChild; //Left child of the current node
    NodeTree *leftRight = currentNode->leftChild->rightChild; //Right child of the left child of the current node
    if (currentNode->dataValue == rootAVL->dataValue) { //If the given current node is the root of the AVL tree, update it with the new root
       rootAVL = left;
   } else {
       NodeTree *parent = getParentAVL(currentNode); //If not, then update the parent of the newly rotated subtree
       if (currentNode->dataValue > parent->dataValue)
           parent->rightChild = left;
       else
           parent->leftChild = left;
   }
    left->rightChild = currentNode; //After rotation, the current node is the right child of its own left child
    currentNode->leftChild = leftRight; //After rotation, the right child of the left child is the left child of the current node
    int newHeight; //Update the heights after the rotation
    if (getNodeHeight(currentNode->leftChild) > getNodeHeight(currentNode->rightChild))
        newHeight = getNodeHeight(currentNode->leftChild) + 1;
    else
        newHeight = getNodeHeight(currentNode->rightChild) + 1;
    currentNode->heightNode = newHeight;
    if (getNodeHeight(left->leftChild) > getNodeHeight(left->rightChild))
        newHeight = getNodeHeight(left->leftChild) + 1;
    else
        newHeight = getNodeHeight(left->rightChild) + 1;
    left->heightNode = newHeight;
}

void rotationLeftAVL(NodeTree *currentNode) { //Rotate the subtree with the given current root to the left
    totalCostAVL++; //Increase the total cost of the AVL tree after each successful rotation
    NodeTree *right = currentNode->rightChild; //Right child of the current node
    NodeTree *rightLeft = currentNode->rightChild->leftChild; //Left child of the right child of the current node
    if (currentNode->dataValue == rootAVL->dataValue) //If the given current node is the root of the AVL tree, update it with the new root
        rootAVL = right;
    else { //If not, then update the parent of the newly rotated subtree
        NodeTree *parent = getParentAVL(currentNode);
        if (currentNode->dataValue > parent->dataValue)
            parent->rightChild = right;
        else
            parent->leftChild = right;
    }
    right->leftChild = currentNode; //After rotation, the current node is the left child of its own right child
    currentNode->rightChild = rightLeft; //After rotation, the left child of the right childe of the current node is its own right child.
    int newHeight; //Update the heights after the rotation
    if (getNodeHeight(currentNode->leftChild) > getNodeHeight(currentNode->rightChild))
        newHeight = getNodeHeight(currentNode->leftChild) + 1;
    else
        newHeight = getNodeHeight(currentNode->rightChild) + 1;
    currentNode->heightNode = newHeight;
    if (getNodeHeight(right->leftChild) > getNodeHeight(right->rightChild))
        newHeight = getNodeHeight(right->leftChild) + 1;
    else
        newHeight = getNodeHeight(right->rightChild) + 1;
    right->heightNode = newHeight;
}

int getNodeHeight(NodeTree *currentNode) { //Return the height of
    int height;
    if (currentNode != NULL) //If the node exists, return its height (The null condition must be checked, otherwise an error occurs)
        height = currentNode->heightNode;
    else //If no such node exists, return 0
        height = 0;
    return height;
}

NodeTree* getParentAVL(NodeTree *childNode) { //Get the parent node of the given child node
    NodeTree *currentNode = rootAVL;
    NodeTree *parent = currentNode;
    while (currentNode->dataValue != childNode->dataValue) { //Start the traversal from the root until the given key's parent is found
        parent = currentNode;
        if (childNode->dataValue > currentNode->dataValue) //If the searched key is greater than the current node, move onto the right
            currentNode = currentNode->rightChild;
        else if (childNode->dataValue < currentNode->dataValue) //If the searched key is smaller than the current node, move onto the left
            currentNode = currentNode->leftChild;
    } return parent; //Return the initialized parent node
}

void insertNodeSplay(NodeTree *rootNode, int newKey) { //Insert the given keys to the splay tree
    NodeTree *currentNode = rootNode;
    while (currentNode != NULL) { //Traverse the splay tree until a null node is reached
        totalCostSplay++; //Increase the total cost of the splay tree in each insertion & search
        if (currentNode->dataValue > newKey) { //If the given key is less than the current node, move onto the left
            if (currentNode->leftChild != NULL) //If the current node is not null, then continue traversing
                currentNode = currentNode->leftChild;
            else { //If there's no such node to traverse, insert the node
                currentNode->leftChild = getNewNode(newKey);
                splay(rootNode, newKey); //Sent the node to the splay action
                break;
            }
        } else if (currentNode->dataValue < newKey) { //If the given key is greater than the current node, move onto the right
            if (currentNode->rightChild != NULL) //If the current node is not null, then continue traversing
                currentNode = currentNode->rightChild;
            else { //If there's no such node to traverse, insert the node
                currentNode->rightChild = getNewNode(newKey);
                splay(rootNode, newKey); //Sent the node to the splay action
                break;
            }
        } else if (currentNode->dataValue == newKey) { //If the given key is already inserted, don't insert again. However, splay again
            currentNode->frequencyOfKey++;
            splay(rootNode, newKey);
            break;
        }
    }
}

void splay(NodeTree *rootNode, int newKey) { //Command the splaying action until the latest node inserted reaches the root
    NodeTree *currentNode = rootNode;
    while (rootSplay->dataValue != newKey) { //Check whether the latest node inserted has reached the root
        while (currentNode->dataValue != newKey) { //Traverse the tree, until the searched node is found
            if (currentNode->dataValue > newKey) //If the key is less than the current node, move onto the left
                currentNode = currentNode->leftChild;
            else if (currentNode->dataValue < newKey) //If the key is greater than the current node, move onto the right
                currentNode = currentNode->rightChild;
        }
        splayCondition(currentNode, newKey); //Send the node to the splay condition function to determine how the action should be done
        currentNode = rootNode; //Start the traversal from the top again
    }
}

void splayCondition(NodeTree *currentNode, int newKey) { //Check how the given node should be splayed
    NodeTree *parentOfCurNode = getParentSplay(currentNode); //Parent of the given node
    NodeTree *grandParentOfCurNode = getParentSplay(parentOfCurNode); //Grandparent of the given node
    if (newKey > parentOfCurNode->dataValue && parentOfCurNode->dataValue == rootSplay->dataValue) //Left rotation case
        rotationLeftSplay(parentOfCurNode);
    else if (newKey < parentOfCurNode->dataValue && parentOfCurNode->dataValue == rootSplay->dataValue) //Right rotation case
        rotationRightSplay(parentOfCurNode);
    if (grandParentOfCurNode != NULL) {
        if (newKey > parentOfCurNode->dataValue && parentOfCurNode->dataValue > grandParentOfCurNode->dataValue) { //Left-Left rotation case (zig-zig)
            rotationLeftSplay(grandParentOfCurNode);
            rotationLeftSplay(parentOfCurNode);
        }
        else if (newKey < parentOfCurNode->dataValue && parentOfCurNode->dataValue < grandParentOfCurNode->dataValue) { //Right-Right rotation case (zig-zig)
            rotationRightSplay(grandParentOfCurNode);
            rotationRightSplay(parentOfCurNode);
        }
        else if (newKey > parentOfCurNode->dataValue && parentOfCurNode->dataValue < grandParentOfCurNode->dataValue) { //Left-Right rotation case (zig-zag)
            rotationLeftSplay(parentOfCurNode);
            rotationRightSplay(grandParentOfCurNode);
        }
        else if (newKey < parentOfCurNode->dataValue && parentOfCurNode->dataValue > grandParentOfCurNode->dataValue) { //Right-Left rotation case (zig-zag)
            rotationRightSplay(parentOfCurNode);
            rotationLeftSplay(grandParentOfCurNode);
        }
    }
}

void rotationRightSplay(NodeTree *currentNode) { //Right rotation for the splay tree
    totalCostSplay++; //Increase the cost of the splay tree in each successful rotation
    NodeTree *left = currentNode->leftChild; //Left child of the current node
    NodeTree *leftRight = left->rightChild; //Right child of the left child of the current node
    if (currentNode->dataValue == rootSplay->dataValue) { //If the given node is root of the splay tree, update the root
        rootSplay = left;
    } else { //If not, update the parent of the associated subtree
        NodeTree *parent = getParentSplay(currentNode);
        if (currentNode->dataValue > parent->dataValue)
            parent->rightChild = left;
        else
            parent->leftChild = left;
    }
    left->rightChild = currentNode; //After rotation, current node is the right child of its own left child
    currentNode->leftChild = leftRight; //After rotation, right child of the left child is the left child of the current node
}

void rotationLeftSplay(NodeTree *currentNode) { //Left rotation for the splay tree
    totalCostSplay++; //Increase the total cost of the splay tree in each successful rotation
    NodeTree *right = currentNode->rightChild; //Right child of the current node
    NodeTree *rightLeft = right->leftChild; //Left child of the right child of the current node
    if (currentNode->dataValue == rootSplay->dataValue) //If the given node is root of the splay tree, update the root
        rootSplay = right;
    else { //If not, update the parent of the associated subtree
        NodeTree *parent = getParentSplay(currentNode);
        if (currentNode->dataValue > parent->dataValue)
            parent->rightChild = right;
        else
            parent->leftChild = right;
    }
    right->leftChild = currentNode; //After rotation, current node is the left childe of its own right child
    currentNode->rightChild = rightLeft; //After rotation, left child of the right child is the left child of the current node
}

NodeTree* getParentSplay(NodeTree *childNode) { //Return the parent node of the given child node
    NodeTree *currentNode = rootSplay;
    NodeTree *parent = currentNode;
    while (currentNode->dataValue != childNode->dataValue) { //Traverse the splay tree until the parent node of the given child node is found
        parent = currentNode;
        if (childNode->dataValue > currentNode->dataValue) //If the searched key is larger than the current node, move onto the right
            currentNode = currentNode->rightChild;
        else if (childNode->dataValue < currentNode->dataValue) //If the searched key is less than the current node, move onto the left
            currentNode = currentNode->leftChild;
    } return parent; //Return the parent node
}

NodeTree* getNewNode(int newKey) { //Create a new node to be inserted in either the AVL tree or the splay tree
    NodeTree *node = (NodeTree *) malloc(sizeof(NodeTree)); //Memory allocate the new node
    node->rightChild = NULL; //Right child of the node hasn't been declared yet. Therefore, null is assigned
    node->leftChild = NULL; //Left child of the node hasn't been declared yet. Therefore, null is assigned
    node->frequencyOfKey = 1; //The node is newly added, it hasn't been replicated yet. Therefore, the initial frequency is 1
    node->dataValue = newKey; //The given key value is assigned
    node->heightNode = 1; //The node has no children yet. Therefore, the initial height is 1
    return node; //Return the node
}

void preOrderPrintTree(NodeTree *currentNode){ //Printing the preorder traversal of AVL tree or the Splay tree
    if (currentNode != NULL) { //Continue traversing until any NULL is reached
        printf("%d ", currentNode->dataValue);
        preOrderPrintTree(currentNode->leftChild);
        preOrderPrintTree(currentNode->rightChild);
    }
}