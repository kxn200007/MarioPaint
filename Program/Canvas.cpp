#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<string.h>
using namespace std;
#include"Node.h"
#include"Canvas.h"

//Private Functions:
bool Canvas::parseCommand(char* commandLine,Command & cmd) const
{
    char* operand=strtok(commandLine,",");
    if(strlen(operand)!=1 || (operand[0]!='1'&&operand[0]!='2'))
        return false;
    cmd.penStatus=atoi(operand);
    operand=strtok(NULL,",");
    if(strlen(operand)!=1 || (operand[0]!='N'&&operand[0]!='E'&&operand[0]!='S'&&operand[0]!='W'))
        return false;
    cmd.direction=operand[0];
    operand=strtok(NULL,",");
    int dist=atoi(operand);
    if(containAllNumbers(operand)==false || dist<=0 || (cmd.direction=='N' && dist>penY) || (cmd.direction=='E' && dist>=MAX_CANVAS_SIZE-penX) || (cmd.direction=='S' && dist>=MAX_CANVAS_SIZE-penY) || (cmd.direction=='W' && dist>penX))
    {
        return false;
    }
    cmd.distance=dist;
    operand=strtok(NULL,",");
    if(cmd.penStatus==2 && operand!=NULL && strcmp(operand,"")!=0)
    {
        if(strlen(operand)!=1)
            return false;
        if(operand[0]=='B')
            cmd.boldStatus=operand[0];
        else if(operand[0]=='P')
            cmd.printStatus=operand[0];
        else
            return false;
    }
    if(cmd.boldStatus=='B')
        operand=strtok(NULL,",");
    if(cmd.printStatus!='P' && operand!=NULL && strcmp(operand,"")!=0)
    {
        if(strlen(operand)!=1)
            return false;
        if(operand[0]=='P')
            cmd.printStatus=operand[0];
        else
            return false;
    }
    return true;
}
bool Canvas::containAllNumbers(char*str) const
{
    int i=0;
    while(str[i]!='\0')
    {
        if(str[i]<'0' || str[i]>'9')
            return false;
        i++;
    }
    return true;
}
void Canvas::insertNode(Node * cNode)
{
    if(cNode==NULL)
    {
        return;
    }
    if(head==NULL)
    {
        head=cNode;
        drawingPen=head;
        currentRow=head;
        currentColumn=head;
        noOfTotalNodes++;
        return;
    }
    if(noOfTotalNodes%MAX_CANVAS_SIZE==0)
    {
        currentRow->down=cNode;
        cNode->up=currentRow;
        currentRow=cNode;
        currentColumn=currentRow;
        noOfTotalNodes++;
    }
    else
    {
        currentColumn->right=cNode;
        cNode->left=currentColumn;
        if(currentColumn->up!=NULL)
        {
            cNode->up=currentColumn->up->right;
            cNode->up->down=cNode;
        }
        currentColumn=cNode;
        noOfTotalNodes++;
    }
}
void Canvas::recWriteToStream(Node*cNode,ostream& out) const
{
    if(cNode==NULL)
    {
        return;
    }
    out<<cNode->characterToDraw;
    recWriteToStream(cNode->right,out);
    if(cNode->left==NULL)
    {
        out<<'\n';
        recWriteToStream(cNode->down,out);
    }
}
void Canvas::recFreeNodes(Node*cNode)
{
    if(cNode==NULL)
    {
        return;
    }
    Node*rCNode=cNode->right,*dCNode=cNode->down,*lCNode=cNode->left;
    cNode->right=NULL;
    cNode->down=NULL;
    delete cNode;
    cNode=NULL;
    recFreeNodes(rCNode);
    if(lCNode==NULL)
    {
        recFreeNodes(dCNode);
    }
}
//Public Functions:
Canvas::Canvas():head(NULL),currentRow(NULL),currentColumn(NULL),drawingPen(NULL),penX(0),penY(0),noOfTotalNodes(0)
{}
Canvas::~Canvas()
{
    Node*temp=head;
    recFreeNodes(temp);
}
bool Canvas::runCommand(char cmdLine[])
{
    Command cmd;
    if(!parseCommand(cmdLine,cmd))
    {
        return false;
    }
    bool boldStatus=false;
    if(cmd.boldStatus=='B')
        boldStatus=true;
    if(cmd.direction=='N')
    {
        for(int i=0;i<cmd.distance && drawingPen->up!=NULL;i++)
        {
            drawingPen=drawingPen->up;
            if(cmd.penStatus==2)
                drawingPen->setCharacter(boldStatus);
        }
        penY-=cmd.distance;
    }
    else if(cmd.direction=='E')
    {
        for(int i=0;i<cmd.distance && drawingPen->right!=NULL;i++)
        {
            drawingPen=drawingPen->right;
            if(cmd.penStatus==2)
                drawingPen->setCharacter(boldStatus);
        }
        penX+=cmd.distance;
    }
    else if(cmd.direction=='S')
    {
        for(int i=0;i<cmd.distance && drawingPen->down!=NULL;i++)
        {
            drawingPen=drawingPen->down;
            if(cmd.penStatus==2)
                drawingPen->setCharacter(boldStatus);
        }
        penY+=cmd.distance;
    }
    else if(cmd.direction=='W')
    {
        for(int i=0;i<cmd.distance && drawingPen->left!=NULL;i++)
        {
            drawingPen=drawingPen->left;
            if(cmd.penStatus==2)
                drawingPen->setCharacter(boldStatus);
        }
        penX-=cmd.distance;
    }
    if(cmd.printStatus=='P')
    {
        displayGrid();
        cout<<"\n";
    }
    return true;
}
void Canvas::createGrid()
{
    Node*temp;
    for(int i=0;i<MAX_CANVAS_SIZE*MAX_CANVAS_SIZE;i++)
    {
        temp=new Node();
        insertNode(temp);
    }
}
void Canvas::displayGrid() const
{
    Node*temp=head;
    recWriteToStream(temp, cout);
}
void Canvas::writeToOutputFile(ofstream&output) const
{
    Node*temp=head;
    recWriteToStream(temp,output);
}
