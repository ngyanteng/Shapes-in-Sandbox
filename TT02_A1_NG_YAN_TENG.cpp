/**********|**********|**********|
Program: TT02_A1_NG_YAN_TENG.cpp
Course: OOPDS
Year: 2018/19 Trimester 2
Name: Ng Yan Teng
ID: 1171303161
Lecture Section: TC01
Tutorial Section: TT02
Email: teng8111@gmail.com/1171303161@student.mmu.edu.my
Phone: 010-3333933
**********|**********|**********/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <algorithm>

using namespace std;

typedef vector<char> vchar;
typedef vector<int> vint;

class Shape
{
	protected:
	int rowPoint;
	int colPoint;
	int colAddress;
	static bool operationSuccess;
	static int fileNo;
	static int instructionList[10];
	static int attempts;
	static vector<vchar> box;
	static vector<vint> unoccupiedSpots;
	
	public:
	virtual void fixShape() {};
	Shape();
	virtual ~Shape();
	void setInstruction();
	void setBox();
	void randRow(int &r);
	void randCol(int &c, int& add, int n, vector<vint> vect);
	void displayResults();
	bool boundaryCheck(int shapeHeight, int shapeLength, int randH, int randL);
	void saveToFile(string result, int fileno);
};

int Shape::instructionList[10];
int Shape::attempts;
int Shape::fileNo;
bool Shape::operationSuccess;
vector<vchar> Shape::box;
vector<vint> Shape::unoccupiedSpots;

Shape::Shape()
{}

Shape::~Shape()
{}

void Shape::setInstruction()				
{																//Instruction List[i]:
	int temp;													// 1. Length of Box [0]
	string filename;											// 2. Height of Box [1]
																// 3. Maximum number of attempts [2]						
	cout<<"\nWhich file would you like to read(e.g. Batch1): \n";// 4. Number of Squares [3]
	getline(cin,filename);										// 5. Length of Square [4]
	filename+=".txt";											// 6. Number of Rectangles [5]
																// 7. Length of Rectangle [6]
	ifstream inFile;											// 8. Height of Rectangle [7]
	inFile.open(filename);										// 9. Number of Triangles [8]
																// 10. Height of Triangle [9]
	if (!inFile) 										
	{													
		cerr << "Unable to open file.";		
		exit(1);							
	}										
	
	for(int i=0;i<10;i++)
		inFile >>instructionList[i];
	
	inFile.close();
	
	attempts=instructionList[2];
	fileNo=1;
	operationSuccess=true;
}


void Shape::setBox()
{
	vchar tempchar;
	vint tempint;
	
	for(int i=0;i<instructionList[0];i++)		
	{
		tempchar.push_back('.');
	}
	for(int j=0;j<instructionList[1];j++)
	{
		box.push_back(tempchar);			//Initialize box to '.'
	}
		
	for(int j=0;j<instructionList[0];j++)		
	{
		tempint.push_back(j);
	}
	for(int i=0;i<instructionList[1];i++)
	{
		unoccupiedSpots.push_back(tempint);	//To be used for excluding points which are occupied or invalid(cause shape to go out of bounds)
	}

}

void Shape::randRow(int &r)
{
	r=rand()%instructionList[1];	//Generates a random number within no. of rows of box
}

void Shape::randCol(int &c, int &add, int n, vector<vint> vect)
{
	add=rand()%vect[n].size();		//Generates a random number within remaining no. of unoccupied columns from row generated above
	c=vect[n][add];					//Gives the exact number contained inside unoccupiedSpots[x][y], which x,y are generated above
}

bool Shape::boundaryCheck(int shapeHeight, int shapeLength, int randH, int randL) //used for checking before writing the shape into box
{
	if((shapeHeight+randH+1)>instructionList[1] || (shapeLength+randL+1)>instructionList[0]) //check if the shape will be out of bounds by its bottom rightmost point
	{																							//if yes, remove the invalid point that was generated
		unoccupiedSpots[randH].erase(remove(unoccupiedSpots[randH].begin(), unoccupiedSpots[randH].end(), randL), unoccupiedSpots[randH].end());
		return false;	//above is erase-remove idiom, vector.erase(remove(vector.begin(),vector.end(),x),vector.end()), x is value to erase
	}
	
	for(int i=0;i<shapeHeight;i++)
	{
		for(int j=0;j<shapeLength;j++)
		{
			if(box.at(randH+i).at(randL+j)!='.')	//check if the shape will overlap any other shapes
			{
				unoccupiedSpots[randH].erase(remove(unoccupiedSpots[randH].begin(), unoccupiedSpots[randH].end(), randL), unoccupiedSpots[randH].end());
				return false;	//above is erase-remove idiom
			}
		}
	}
	
	return true;
}

void Shape::saveToFile(string result, int fileno)	//saves the operation outcome into a text file
{
	ostringstream os;

	os<<result<<"."<<fileno<<".txt";
	
	string templine=os.str();
	
	ofstream writefile(templine);
    
    for(int i=0;i<instructionList[1];i++)			//writes the operation outcome into the text file
    {
        for(int j=0;j<instructionList[0];j++)
        writefile<<box[i][j];
		writefile<<endl;
    }
}

void Shape::displayResults()					//displays the final operation outcome on screen
{
	for(int i=0;i<box.size();i++)
	{
		for(int j=0;j<box[i].size();j++)
		{
			cout<<box[i][j];
		}
		cout<<endl;
	}
}

class Square : public Shape		//Square will be called last in main()
{
	public:
	void fixShape() 
	{	
		int loop=instructionList[3];	//number of square requested in Batch file
		
		if(operationSuccess)	//check if last operation(fixing Rectangle) was a success or failure
		{
			for(int n=0;n<loop;)
			{
				if(attempts<=0 || unoccupiedSpots.empty())	//check if there is enough number of attempts remaining or if there is still valid points
				{
					cout<<"\nOut of attempts or insufficient space for remaining shapes. Program terminated.\n";
					saveToFile("Failed", fileNo);
					fileNo++;
					operationSuccess=false;
					break;
				}
				randRow(rowPoint);
				randCol(colPoint, colAddress, rowPoint, unoccupiedSpots);
					if(boundaryCheck(instructionList[4],instructionList[4],rowPoint,colPoint))
					{
						for(int i=0;i<instructionList[4];i++)
							for(int j=0;j<instructionList[4];j++)
							{
								box[rowPoint+i][colPoint+j]='A';
								unoccupiedSpots[rowPoint+i].erase(remove(unoccupiedSpots[rowPoint+i].begin(), unoccupiedSpots[rowPoint+i].end(), colPoint+j), unoccupiedSpots[rowPoint+i].end());
							}	//above is erase-remove idiom
						n++;
					}
				attempts--;	//remaining attempt decreases no matter fail or success in fixing one shape
			}
		}
		if(operationSuccess)	//if no failure occurs in the entire operation, save file as success
		{
			saveToFile("Successful", fileNo);
			fileNo++;
			cout<<"\nOperation success.\n";
		}
	}
};

class Rectangle : public Shape		//Rectangle is called second in main()
{
	public:
	void fixShape()
	{
		int loop=instructionList[5];	//number of rectangle requested in Batch file
		
		if(operationSuccess)	//check if last operation(fixing Triangle) was a success or failure
		{
			for(int n=0;n<loop;)
			{
				if(attempts<=0 || unoccupiedSpots.empty())
				{
					cout<<"Out of attempts or insufficient space for remaining shapes. Program terminated.\n";
					saveToFile("Failed", fileNo);
					fileNo++;
					operationSuccess=false;
					break;
				}
				randRow(rowPoint);
				randCol(colPoint, colAddress, rowPoint, unoccupiedSpots);
				if(boundaryCheck(instructionList[6],instructionList[7],rowPoint,colPoint))
				{
					for(int i=0;i<instructionList[6];i++)
						for(int j=0;j<instructionList[7];j++)
						{
							box[rowPoint+i][colPoint+j]='B';
							unoccupiedSpots[rowPoint+i].erase(remove(unoccupiedSpots[rowPoint+i].begin(), unoccupiedSpots[rowPoint+i].end(), colPoint+j), unoccupiedSpots[rowPoint+i].end());
						}	//above is erase-remove idiom
					n++;
				}
				attempts--;
			}
		}
	}
};

class Triangle : public Shape	//Triangle is called first
{
	public:
	void fixShape()
	{
		int i,j,space,pointShift=0,loop=instructionList[8]; //number of Triangle requested in Batch file
		
		//operationSuccess not used here since its first to go
		for(int n=0;n<loop;)
		{
			if(attempts<=0 || unoccupiedSpots.empty())
			{
				cout<<"Out of attempts or insufficient space for remaining shapes. Program terminated.\n";
				saveToFile("Failed", fileNo);
				fileNo++;
				operationSuccess=false;
				break;
			}
			randRow(rowPoint);
			randCol(colPoint, colAddress, rowPoint, unoccupiedSpots);
			if(boundaryCheck(instructionList[9],2*instructionList[9]-1,rowPoint,colPoint))
			{
				for(i=1;i<=3;i++)
				{
					for(space=i;space<instructionList[9];space++)
					{
						pointShift++;
					}
					for(j=1;j<=(2*i-1);j++)
					{
						box[rowPoint+i-1][colPoint+pointShift+j-1]='C';
						unoccupiedSpots[rowPoint+i-1].erase(remove(unoccupiedSpots[rowPoint+i-1].begin(), unoccupiedSpots[rowPoint+i-1].end(), colPoint+pointShift+j-1), unoccupiedSpots[rowPoint+i-1].end());
					}	//above is erase-remove idiom
					pointShift=0;
				}
				n++;
			}
			attempts--;
		}
	}
};

int main()
{
	srand(5);
	Shape shapes;
	Triangle a;
	Rectangle b;
	Square c;
	
	
	Shape *Shape1=&a;
	Shape *Shape2=&b;
	Shape *Shape3=&c;
	 
	shapes.setInstruction();
	shapes.setBox();
	
	Shape1->fixShape();
	Shape2->fixShape();
	Shape3->fixShape();
	shapes.displayResults();
	
	
	return 0;
}