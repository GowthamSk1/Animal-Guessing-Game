/* The Animal Guessing Game, Part 1
    Class: C++ Class
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;
// Main data structure
struct animalNode{
    string guess;
    string question;
    animalNode* yesAnswer;
    animalNode* noAnswer;
};
string animalFileName_SAVE = "myAnimalTreeDB.txt";

animalNode* newNode(string, string); // Creates a new node
animalNode* insertNewNode(animalNode*, string, string); // Inserts a new node recursively
void changeData(animalNode*, string, string, string);   // Rearranges the data
animalNode* play(animalNode*, string, string, char, string &);  // UI
void repeat(animalNode*, string, string, string);   // Do as long as the user wants to play (Mainly to make the program look pretty)
void printPreorder(ostream &, animalNode*);
void saveTheAnimalTree(animalNode*);
void readFromFile(ifstream &, animalNode*, animalNode* &);

int main(){
    char ans, choice;                    // User instructions
    string temp, guesstemp;              // temporary string to store the guess for replacement.
    string guess, question;              // Temporarily stores the guess and question for each node
    cout << "you want to read from file? (y/n)" << endl;
    cin >> ans;
    
    animalNode* root;                    // To store the address of the root animal node
    root = NULL;                         // Initially set it to null pointer
    animalNode* tempnode;
    tempnode = NULL;                     // Temporary node that stores the successive pointers. Initially set it to null pointer
    
    if(ans != 'y'){
        //create the root (first) animal node and initialize it with lizard guess
        root = insertNewNode(root, "lizard", question);              
        cout << "Instructions: 1. Answer by pressing (y) key or (n) key.\n"
             << "              2. Type the question and press enter.\n\n";
               
/* At this point the root has the (points to the) address of the first new node which contains the guess data and an empty question 
  member. */                                  
       do{
            cout << "Play  \"GUESS THE ANIMAL!!!\" " << endl;            // Display the title
            tempnode =  play(root,guess, question, ans, guesstemp);      // Call the play function
            repeat(tempnode, guesstemp, question, temp);
           
            cout << "\nPlay  \"GUESS THE ANIMAL!!!\" " << endl;
            cout << "\nDo you want to play again? (y/n)\n";
            cin >> choice;
        }while(tolower(choice) != 'n');
        ofstream outputfile (animalFileName_SAVE.c_str());
        if(outputfile)
        {
            saveTheAnimalTree(root);
        }
        outputfile.close();
    }else{
        animalNode *temp;
        temp = NULL;
        ifstream inputfile (animalFileName_SAVE.c_str());
        if(inputfile.fail())
        {
            cout << "error opening file";
            return 0;
        }else{
            readFromFile(inputfile,root, temp);
            cout << temp << " " <<temp->guess << " " << temp->question << endl;
        }
        inputfile.close();
    }
    return 0;
}
// This function creates a new node
animalNode* newNode(string guess1, string question1)
{
    animalNode* newNode = new animalNode;
    newNode->guess = guess1;
    newNode->question = question1;
    newNode->yesAnswer = newNode->noAnswer = NULL;
                                        //return the address of the new node that was created dynamically
    return newNode;
}
// This function inserts the new node recursively
animalNode* insertNewNode(animalNode* root, string guess1, string question1)
{
    if(!root)                           // if it is a null pointer then create the new node.
    {
        root = newNode(guess1, question1);
    }
    else if((guess1.length() && question1.length()) == 0)
    {
        root->yesAnswer = insertNewNode(root->yesAnswer, guess1, question1);
        root->noAnswer = insertNewNode(root->noAnswer, guess1, question1);
    }
    return root;
}
// This function changes the data.
void changeData(animalNode* root, string guess, string question, string temp){
    char check;
    root = insertNewNode(root, "", "");                         //inserts 2 empty nodes
    temp = root->guess;                                         // Assigns the guess that was in the curnode to a temporary node
    root->guess.clear(); // Clear the root node's guess
    
     cin.ignore();
     cout << "What is a yes or no question that I can use to tell a " << temp << " from a " << guess  << " ?"<< endl;;
     getline(cin, question);
     root->question = question;       // Get the question and store it in the original node
                                     //ask the user whether the answer to the question for the new animal is yes or no
     cout << "For the " << guess << " is the answer yes or no (y/n)" << endl;
     cin >> check;
     // adds the guess into the appropriate boxes
    if(tolower(check) == 'y')
    {
       root->yesAnswer->guess = guess;
       root->noAnswer->guess = temp;
    }
    else
    {
        root->noAnswer->guess = guess;
        root->yesAnswer->guess = temp;
    }
}
// This function lets the user play the game
animalNode* play(animalNode* root, string guess1, string question, char ans, string &guesstemp)
{
    animalNode *tempnode;                   // for the temporary node
    if(root->question.empty())              // This is a guess node
    {
        cout << "Think of an animal and press enter when ready" << endl;
        cin.ignore();
        cout << "is it a " << root->guess << " ? (y/n)" << endl;
        cin >> ans;
        if(tolower(ans) == 'y'){           // The computer wins
            cout << "I WIN!!!! The animal is " << root->guess << endl;
            exit(0);
        }
        else{                            // If it is a new animal that the program does not know
            cout << "Teach me about the animal you're thinking of";
            cout << "\nWhat is your animal? " << endl;
            cin >> guesstemp;
            if((root->noAnswer) != NULL){    // For future nodes
                tempnode = root->noAnswer;
                return tempnode;
            }
            else{
                 tempnode = root;
            }
        }
    }
    else if(root->guess.empty())            // This is a question node for the yes answer
    {
        cout << root->question << endl;     // Display the question and get the user's input
        cin >> ans;
        if(tolower(ans) == 'y') {
                if(!(root->yesAnswer->guess.empty())){  //For two question nodes back to back
                    cout << "Is it a " << root->yesAnswer->guess << " ? (y/n)" << endl;
                    cin >> ans;
                    if(tolower(ans) == 'y'){
                        cout << "I WIN!!!! The animal is " << root->yesAnswer->guess << endl; // Guessed correctly
                        exit(0);
                    }
                    else{
                        cout << "Teach me about the animal you're thinking of"; // If the answer is no ask for user's input
                        cout << "\nWhat is your animal? " << endl;
                        cin >> guesstemp;
                        if((root->yesAnswer) != NULL){ // For future nodes
                            tempnode = root->yesAnswer;
                            return tempnode;
                    }
                    else{
                        tempnode = root;
                    }
                }
            }
            else{
                tempnode = play(root->yesAnswer,guess1, question, ans, guesstemp);
            }
        }
        else
        { // This is a question node for the no answer
            if(!(root->noAnswer->guess.empty())){
                cout << "Is it a " << root->noAnswer->guess << " ? (y/n)" << endl;
                cin >> ans;
                if(tolower(ans) == 'y'){
                    cout << "Win. Guess: " << root->noAnswer->guess << endl;
                    exit(0);
                }
                else{
                    cout << "Teach me about the animal you're thinking of";
                    cout << "\nWhat is your animal? " << endl;
                    cin >> guesstemp;
                    if((root->noAnswer) != NULL){
                        tempnode = root->noAnswer;
                        return tempnode;
                    }
                    else{
                        tempnode = root;
                    }
                }
            }
            else{
                tempnode = play(root->noAnswer,guess1, question, ans, guesstemp);
            }
        }
    }
    else{
        tempnode = root;
    }
    return tempnode;
}

// function call repeats until the user does not want to play. Unnecessary but makes the program prettier
void repeat(animalNode* root, string guess, string question, string temp){
    if(root->question.empty())
    {
        changeData(root, guess, question, temp);
    }
}
/*  To debug IGNORE
    else if(guess1.empty())
    {
         // then it is a question node guess member is blank and both yes and no will point
                               // to child nodes
        root->noAnswer = insertNode(root->noAnswer,guess1,question1);
    }
    else if(question1.empty()) //it is a guess node
    {
        root->yesAnswer = insertNode(root->yesAnswer,guess1,question1);
    }


    }
    else if((animalNode->question)== "")
    // then it is a guess node the ques member is blank and both child pointers
                                        // will be null pointers (leafnodes)
    {

    }
    */

// printTheTree - print the tree to the output stream in pre-order manner
void printPreorder(ostream &outputStream,animalNode* curNode)
{   // See if this node is a guess node
    if(curNode->question.empty()) // it is
    {
        outputStream << "G" << endl;
        outputStream << curNode->guess << endl;
    }else{
        outputStream << "Q" << endl;
        outputStream << curNode->question << endl;
        printPreorder(outputStream,curNode->yesAnswer);
        printPreorder(outputStream,curNode->noAnswer);
    }
}
void saveTheAnimalTree(animalNode* root){
    char choice;
    ofstream outfile(animalFileName_SAVE.c_str());
    if(outfile.fail()){
        cout << "Error opening file " << endl;
        exit(0);
    }else{
        cout << "Do you want to save the data to a file or print it or both? \n(s to save file)/(p to print to screen)/(b for both)" << endl;
        cin >> choice;
        if(tolower(choice) == 'p')
        {
            printPreorder(cout,root);
        }
        else if(tolower(choice) == 's')
        {
            printPreorder(outfile,root);
            outfile << flush;

        }
    }
}


void readFromFile(ifstream &inputfile, animalNode* curNode, animalNode* &temp)
{                                       //null
    string tempor;
    getline(inputfile, tempor, '\n');
//    inputfile.seekg(0,ios::end);
//    if(inputfile.tellg() == 0)
//    {
//        cout << "File is empty exit" << endl;
//        exit(0);
//    }
//    else
//    {
        if(tempor == "G") // it is
        {
            getline(inputfile,tempor, '\n');
                        // Creates a new node)
            temp = insertNewNode(curNode, tempor, "");
        }else if(tempor == "Q"){
            getline(inputfile, tempor, '\n');
            curNode = insertNewNode(curNode,"", tempor); //root node
            // tempRootPtr = tempPtr;
            readFromFile(inputfile, curNode->yesAnswer, temp);
            readFromFile(inputfile,curNode->noAnswer, temp);
        }
        temp = curNode;
//    }
}


//51 cout << guess.length() << question.length() << endl;
//52 cout << root->guess.length() << root->question.length()<< root->yesAnswer->guess << " " << root->noAnswer->guess << " " << endl;
//107cout << "stored in root guess " << root->guess << " "root->guess.length()<< endl;
//108cout << "Answer by forming a question that can be used to differentiate your animal to the one I guessed.\n";
