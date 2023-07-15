#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

struct Node
{
    char characterToDraw;
    Node* up;
    Node* down;
    Node* left;
    Node* right;
    Node();
    Node(bool boldStatus);
    void setCharacter(bool boldStatus);
};

#endif
