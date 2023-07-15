#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include"Node.h"

#define MAX_CANVAS_SIZE 50

struct Command
{
    int penStatus;
    char direction;
    int distance;
    char boldStatus;
    char printStatus;
    Command():penStatus(0),direction('\0'),distance(0),boldStatus('\0'),printStatus('\0')
    {}
};

class Canvas
{
    Node*head;
    Node*currentRow;
    Node*currentColumn;
    Node*drawingPen;
    int penX;
    int penY;
    int noOfTotalNodes;
    bool parseCommand(char commandLine[],Command & cmd) const;
    bool containAllNumbers(char*str) const;
    void insertNode(Node * cNode);
    void recWriteToStream(Node*cNode,ostream& out) const;
    void recFreeNodes(Node*cNode);
public:
    Canvas();
    ~Canvas();
    bool runCommand(char* cmdLine);
    void createGrid();
    void displayGrid() const;
    void writeToOutputFile(ofstream&output) const;
};

#endif
