#include <iostream>
#include <string.h>
#include <fstream>
#define MAX_CANVAS_SIZE 50
using namespace std;


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










int main()
{
	ifstream inputFile;
	ofstream outputFile;
	char fileName[100],cmdLine[50],outputFileName[100]="paint.txt";
	Canvas canvas;
	int i=1;
	cout<<"INPUT FILE:\n";
	cout<<"Enter the Input Filename: ";
	cin.getline(fileName,100);
	cout<<"Opening the file...\n";
	inputFile.open(fileName,ios::in);
	if(inputFile.is_open()==false)
	{
		cout<<"FAILURE: Invalid Filename or Directory!\n";
		cout<<"Terminating the Program...\n";
		return 1;
	}
	cout<<"SUCCESS: File Opened Successfully!\n\n";
	cout<<"CANVAS:\n";
	cout<<"Creating the Grid on Canvas...\n";
	canvas.createGrid();
	cout<<"Grid Created Successfully!\n\n";
	cout<<"Initiating Reading Commands From Input File (filename: "<<fileName<<")...\n\n";
	cout<<"COMMANDS FROM FILE:\n";
	while(inputFile.eof()==false)
	{
		inputFile.getline(cmdLine,50);
		cout<<"Reading Line#"<<i<<": "<<cmdLine<<"\n";
		if(canvas.runCommand(cmdLine))
			cout<<"SUCCESS: Command Executed Successfully!\n";
		else
			cout<<"INVALID COMMAND: Command didn't Executed and ignored!\n";
		cout<<"\n";
		i++;
	}
	cout<<"End of File Reached!\n";
	cout<<"Finished Reading All Commands From Input File!\n\n";
	cout<<"OUTPUT FILE:\n";
	cout<<"Opening Output File (filename: "<<outputFileName<<")...\n";
	outputFile.open(outputFileName,ios::out);
	if(outputFile.is_open()==false)
	{
		cout<<"FAILURE: Access Denied!\n";
		cout<<"Terminating the Program...\n";
		return 1;
	}
	cout<<"Opened Output File Successfully!\n";
	cout<<"Writing the final state of the Canvas to Output File (filename: "<<outputFileName<<")...\n";
	canvas.writeToOutputFile(outputFile);
	cout<<"Wrote to output file successfully (filename: "<<outputFileName<<")!\n";
	cout<<"Closing the input file (filename: "<<fileName<<")...\n";
	inputFile.close();
	cout<<"SUCCESS: Input file closed Successfully!\n";
	cout<<"Closing the output file (filename: "<<outputFileName<<")...\n";
	outputFile.close();
	cout<<"SUCCESS: Output file closed Successfully!\n";
	cout<<"\nExiting the Program...\n";
	return 0;
}
