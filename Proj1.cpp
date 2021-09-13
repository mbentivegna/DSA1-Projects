//Michael Bentivegna
//Project 1

/* 
    This program implements a singly linked list abstract class that is able to support both stack and queue data structures.  It is able to read push, pull, and create 
    commands from an input file and display the appropriate output.  
*/

#include <iostream>  
#include <fstream>
#include <sstream>
#include <list>

using namespace std; 

//Singly linked list template class
template <typename T>
class SimpleList
{
    string Name;
    int size = 0;
    
    private:
        
        struct Node //Used struct instead of subclass (serves the necessary purpose)
        {
            T data; 
            Node *next; 
        };

    protected:

        void insertStart(T data);  
        void insertEnd(T data);
        T removeStart();
    
    public:

        virtual void push(T var) = 0; 		
		virtual T pop() = 0;

        SimpleList(string name) //Constructor
        {
            Name = name;
        }

        string getName() //Function to return name (important for search function)
        {
            return Name;
        }

        int getSize() //Function to return size (important for other member functions and to understand if pop is enabled)
        {
            return size;
        }

    struct Node* top = NULL; 
    struct Node* bot = NULL;
};

//Provides a new node at the end of the list and makes it the new bot node 
template <typename T>
void SimpleList<T> :: insertEnd(T var)
{
    Node *ptr = new Node();
    ptr->data= var;
    ptr->next = NULL;

    if(getSize() == 0)
    {
        top = ptr;
        bot = ptr;
    }
    else
    {
        bot->next = ptr;
        bot = ptr;
    }
    size++;
}

//Provides a new node at the start of the list and sets it to the new top node
template <typename T>
void SimpleList<T> :: insertStart(T var)
{
    Node *p = new Node();
    p->data= var;
    p->next= NULL;

    if(getSize() == 0)
    {
        top = p;
        bot = p;
    }
    else
    {
        p->next = top;
        top = p;
    }
    size++;
}

//Remove element from start of list and sets the top pointer to the next node
//Does not have to check if the list is empty, that is done when parsing through the input file
template <typename T>
T SimpleList<T> :: removeStart()
{
    T output;

    output = top->data;
    top = top->next;

    size--;

    return output;
}

//Stack class that utilizes the singly linked list abstract class (pushes and pulls from the same side)
template<typename T>
class Stack : public SimpleList<T> 
{
    public:
        Stack(string name) : SimpleList<T>(name) {}	

        T pop()
        {
            return SimpleList<T>::removeStart(); 
        }

        void push(T var) 
        {
		    SimpleList<T>::insertStart(var);
	    }
};

//Queue class that also utilizes the singly linked list abstract classes (pushes and pulls from opposite sides)
template<typename T>
class Queue : public SimpleList<T> {

    public:
        Queue(string name) : SimpleList<T>(name) {}	

        T pop()
        {
            return SimpleList<T>::removeStart();
        }

        void push(T var) 
	    {
		    SimpleList<T>::insertEnd(var);
	    }
};

//Template function for returning the location of a stack/queue if it exists
template <typename T>
SimpleList<T>* search(list<SimpleList<T> *> listSL, string name)
{
    for (auto const &look: listSL) //automatically filters through list of object pointers
    {
        if (look->getName() == name)
        {
            return look;
        }
    }
    return nullptr;
}

//Breaks down input file commands for use
int parseText(string input, string output)
{

    list<SimpleList<int> *> listSLi;
    list<SimpleList<double> *> listSLd;
    list<SimpleList<string> *> listSLs;

    ifstream inF;
    ofstream outF;

    inF.open(input);
    outF.open(output);

    string oneLine;

    //Enables each line to be read one at a time until the end of the file is reached
    while (getline(inF, oneLine))
    {
        outF << "PROCESSING COMMAND: " << oneLine << "\n";

        string operation, name, item;
        stringstream word(oneLine); //Separates each line word by word

        word >> operation;
        word >> name;
        word >> item;

        if (operation == "create") //Makes new Stack/Queue if search function finds that one does not already exist
        { 

            if(name[0] == 'i')
            {
                SimpleList<int> *pSLi;
                pSLi = search<int>(listSLi, name); //check if it exists

                if(pSLi == nullptr)
                {
                    if(item == "stack"){
                        pSLi = new Stack<int>(name);
                        listSLi.push_front(pSLi);
                    }

                    else if(item == "queue"){
                        pSLi = new Queue<int>(name);
                        listSLi.push_front(pSLi);
                    }

                }
                else
                {
                    outF << "ERROR: This name already exists!\n";
                }

            }

            if(name[0] == 'd')
            {
                SimpleList<double> *pSLd;
                pSLd = search<double>(listSLd, name);

                if(pSLd == nullptr)
                {
                    if(item == "stack"){
                        pSLd = new Stack<double>(name);
                        listSLd.push_front(pSLd);
                    }

                    else if(item == "queue"){
                        pSLd = new Queue<double>(name);
                        listSLd.push_front(pSLd);
                    }

                }
                else
                {
                    outF << "ERROR: This name already exists!\n";
                }
            }

            if(name[0] == 's')
            {
                SimpleList<string> *pSLs;
                pSLs = search<string>(listSLs, name);

                if(pSLs == nullptr)
                {
                    if(item == "stack"){
                        pSLs = new Stack<string>(name);
                        listSLs.push_front(pSLs);
                    }

                    else if(item == "queue"){
                        pSLs = new Queue<string>(name);
                        listSLs.push_front(pSLs);
                    }

                }
                else
                {
                    outF << "ERROR: This name already exists!\n";
                }
            }
        }

        else if (operation == "push") //Pushes "item" if the designated stack/queue exists
        {
            if(name[0] == 'i')
            {
                SimpleList<int> *pSLi;
                pSLi = search<int>(listSLi, name);

                if(pSLi == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    int numI = stoi(item);
                    pSLi->push(numI);
                }
            }

            if(name[0] == 'd')
            {
                SimpleList<double> *pSLd;
                pSLd = search<double>(listSLd, name);

                if(pSLd == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    double numD = stod(item);
                    pSLd->push(numD);
                }
            }

            if(name[0] == 's')
            {
                SimpleList<string> *pSLs;
                pSLs = search<string>(listSLs, name);

                if(pSLs == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    pSLs->push(item);
                }
            }
        }

        else if (operation == "pop") //Pops next item in stack/queue if the stack/queue is not empty and it exists
        {
            if(name[0] == 'i')
            {
                SimpleList<int> *pSLi;
                pSLi = search<int>(listSLi, name);

                if(pSLi == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    if(pSLi->getSize() == 0)
                    {
                        outF << "ERROR: This list is empty!\n";
                    }
                    else
                    {
                        int popped = pSLi->pop();
                        outF << "Value popped: " << popped << "\n";
                    }
                }
            }

            if(name[0] == 'd')
            {
                SimpleList<double> *pSLd;
                pSLd = search<double>(listSLd, name);

                if(pSLd == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    if(pSLd->getSize() == 0)
                    {
                        outF << "ERROR: This list is empty!\n";
                    }
                    else
                    {
                        double popped = pSLd->pop();
                        outF << "Value popped: " << popped << "\n";
                    }
                }
            }

            if(name[0] == 's')
            {
                SimpleList<string> *pSLs;
                pSLs = search<string>(listSLs, name);

                if(pSLs == nullptr)
                {
                    outF << "ERROR: This name does not exist!\n";
                }
                else
                {
                    if(pSLs->getSize() == 0)
                    {
                        outF << "ERROR: This list is empty!\n";
                    }
                    else
                    {
                        string popped = pSLs->pop();
                        outF << "Value popped: " << popped << "\n";
                    }
                }
            }
        }
        
    }
    return 0;
}

//Asks user for a designated input to read from and output file to write to
//Calls the parsing function that will be the HQ of the program
int main() 
{ 
    string inputFile, outputFile;
    cout << "Enter name of input file: ";
    cin >> inputFile;
    cout << "Enter name of output file: ";
    cin >> outputFile;

    parseText(inputFile, outputFile);

    return 0; 
} 