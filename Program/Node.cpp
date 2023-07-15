#include<iostream>
using namespace std;
#include"Node.h"

Node::Node():characterToDraw(' '),up(NULL),down(NULL),left(NULL),right(NULL)
{}
Node::Node(bool boldStatus):characterToDraw('*'),up(NULL),down(NULL),left(NULL),right(NULL)
{
    if(boldStatus)
        characterToDraw='#';
}
void Node::setCharacter(bool boldStatus=false)
{
    if(boldStatus)
        characterToDraw='#';
    else if(characterToDraw!='#')
        characterToDraw='*';
}
