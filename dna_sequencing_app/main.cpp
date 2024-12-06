/*
Filip Toloczko, CS 251 11am session, Wednesday 12pm lab

Program 1: DNA Profiling
In this program, dna databases are compared to existing dna sequences in
order to match the dna sequence to a given individual. The user is able to
read in databases along with dna sequences. The user is then able to 
sort through the sequence to check for consecutive repeats of dna str's.
Finally the user can cross reference the amount of str repeats with the 
database of indiciduals in order to match the dna to a person.

CREATIVE COMPONENT:
My creative component allows the user to alter the amount of consecutive
repeats of a specific str for a given person in a database. First, the
user must use the load_db command while running the program in order to 
read in the database. Then, in order to alter the database, the user must 
use the new ' changeDna ' command, where they will be prompted to enter 
the name of an individual in the database, followed by the element in the
dnaCount ourvector that they would like to  replace, and finally the new
value they would like to replace the current value at the given element with.
*/

#include "ourvector.h"
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//Struct to store databse names and str counts
struct individualData{
  string name;
  ourvector<int> dnaCount;
};

/*
This function reads in  the first line from the selected database text file and stores 
the information into vectors of characters. These vectors are then stored in
a vector of vectors of characters, called strVector. The vector of vectors 
represents the str segments that will need to be analyzed. The following lines
are read in, each line being stored into a struct, and each struct being stored
in a vector of structs, called peopleData. fIlename is the selected filename in
the main function.
*/
void loadDB(string &filename, ourvector<individualData> &peopleData, ourvector<ourvector<char>> &strVector){
  ifstream inFile(filename);
  string segment;
  string num_str;
  char str;
  //Read in file to get the first line seperately
  getline(inFile, segment, '\n'); 
  stringstream strLetters(segment);
  while(strLetters.good()){
    ourvector<char> strs;
    string substr;
    //seperate by commas
    getline(strLetters, substr, ',');
    for(int i = 0; i < substr.size(); i++){
      strs.push_back(substr.at(i));
    }
    strVector.push_back(strs);
  }
  //read in the following lines in order to store into a struct
  while(!inFile.eof()){
    individualData personsData;
    getline(inFile, segment, '\n');  
    stringstream temp(segment);
    getline(temp, personsData.name, ',');
    while(!temp.eof()){
      getline(temp, num_str, ',');
      personsData.dnaCount.push_back(stoi(num_str));
    }
    peopleData.push_back(personsData);
  }
}

/*
This function takes care of displaying the programs database vector.
The vector is called peopleData, and it is looped through in order
to display the names and number of str's contained in each struct 
stored in peopleData.
*/
void displayDB(ourvector<individualData> &peopleData){
  //loop through peopleData to display names in the database
  for(int i = 0; i < peopleData.size()-1; i++){
    cout << peopleData[i].name << " ";
    //loop through peopleData.dnaCount to display the number of str's
    for(int j = 0; j < peopleData[0].dnaCount.size(); j++){
      cout << peopleData[i].dnaCount[j] << " ";
    }
    cout << endl;
  }
  cout << endl;
}

/*
This function takes care of displaying the programs dna sequence.
The vector is called sequence. The vector is looped through 
character by character in order to display the entire sequence.
*/
void displayDNA(ourvector<char> &sequence){
  cout << "DNA loaded: " << endl;
  //loop through character by character
  for(int i = 0; i < sequence.size(); i++){
    cout << sequence.at(i);
  }
  cout << "\n" << endl;
}

/*
loop through a given text file, filename and add every character
in the file to the vector of characters sequence
*/
void loadDNA(string &filename, ourvector<char> &sequence){
  ifstream inFile(filename);
  string segment;
  while(!inFile.eof()){
    getline(inFile, segment, '\n');
    for(int i = 0; i < segment.size(); i++){
      sequence.push_back(segment.at(i));
    }
  }
}

/*
This function takes care of loops for the process function. startIndex is the
first element of the loop, while endIndex is the last. sequence is a vector of
characters whule strVector is a vector of vectors of chars. numMatches is the 
amount of matches displayed, consecutive is the number of consecutive matches,
with i being the variable being looped
*/
void processForLoop(int &startIndex, int &endIndex, ourvector<char> &sequence, ourvector<ourvector<char>> &strVector, int &numMatches, int &consecutive, int &i){
  ourvector<char> matches = {};
  int equal = 0;
  for(int j = startIndex; j<endIndex; j++){
    matches.push_back(sequence.at(j));
  }
  for(int k = 0; k<strVector[i].size(); k++){
    if(strVector[i][k] == matches[k]){
      equal += 1;
    }
  }  
  if(equal == strVector[i].size()){
    consecutive += 1;
    if(endIndex+strVector[i].size() < sequence.size()){
      startIndex += strVector[i].size();
      endIndex += strVector[i].size();
    }  
  }
  else{
    if(consecutive >= numMatches){
      numMatches = consecutive;
    }
    consecutive = 0;
    startIndex += 1;
    endIndex += 1;
  }
}

/*
This function takes care of the while loop for the process function. It
has the same parameters as processForLoop.
*/
void processWhileLoop(int &endIndex, ourvector<char> &sequence, ourvector<ourvector<char>> &strVector, int &startIndex, int &consecutive, int &numMatches, int &i){
  while(endIndex < sequence.size()-strVector.size()){
    processForLoop(startIndex, endIndex, sequence, strVector, numMatches, consecutive, i);
  }
}


/*
This function takes care of processing the database and the sequence. It checks
how many consecutive str's are found in the vector sequence that match the vector
strVector, and the number of matches is pushed back to the vector individualMatches.
*/
void process(ourvector<char> &sequence, ourvector<ourvector<char>> &strVector, ourvector<int> &individualMatches){
  cout << "Processing DNA..." << endl;
  for(int i = 1; i<strVector.size(); i++){
    int startIndex = 0;
    int endIndex = strVector[i].size();
    int numMatches = 0;
    int consecutive = 0;
    processWhileLoop(endIndex, sequence, strVector, startIndex, consecutive, numMatches, i);
    individualMatches.push_back(numMatches);
  }
}

/*
This function displays what is processed in the process function. It does so by looping
through strVector to display the name of the str, and individualMatches to display
the number of consecutive matches
*/
void displayProcess(ourvector<ourvector<char>> &strVector, ourvector<int> &individualMatches){
  cout << "DNA processed, STR counts: " << endl;
  for(int k = 1; k < strVector.size(); k++){
    for(int j = 0; j<strVector[k].size(); j++){
      cout << strVector[k][j];
    }
    cout << ": " << individualMatches[k-1] << endl;
  }
}

/*
This function searches for a name in the database that matches the processed data.
It loops through peopleData and individualMatches in order to compare the results 
and determine a match.
*/
void search(ourvector<int> &individualMatches, ourvector<individualData> &peopleData){
  cout << "Searching database..." << endl;
  int positiveMatches = 0;
  for(int i = 0; i<peopleData.size(); i++){
    int equal = 0;
    for(int k = 0; k<peopleData.at(i).dnaCount.size(); k++){
      if(individualMatches[k] == peopleData.at(i).dnaCount.at(k)){
        equal+=1;
      }
      else{
        continue;
      }
    }
    if(equal == individualMatches.size()){
      cout << "Found in database!  DNA matches: ";
      cout << peopleData.at(i).name << endl;
      positiveMatches+=1;
      break;
    }
    else{
      continue;
    }
  }
  if(positiveMatches == 0){
    cout << "Not found in database." << endl;
  }
}


/*
This function gets the users command and stores it in the
variable command.
*/
void getCommand(string &command){
  cout << "Enter command or # to exit: " << endl;
  cin >> command;
}


/*
This function determines if the database is valid. It checks using filename as the
users desired file, peopleData to loop through structs of individuals, strVector to
loop through vectors of chars, and databaseLoaed determines if the file exists or not
*/
void checkDatabase(string &filename, ourvector<individualData> &peopleData, ourvector<ourvector<char>> &strVector, int &databaseLoaded){
  ifstream test(filename); 
  if(!test){
    cout << "Loading database..." << endl;
    cout << "Error: unable to open '" << filename << "'" << endl;
  }
  else{
    loadDB(filename, peopleData, strVector);
    cout << "Loading database..." << endl;
    databaseLoaded = 1;
      }
}

/*
This function runs the load_db command using strVector and peopleData.
*/
void commandDB(ourvector<ourvector<char>> &strVector, ourvector<individualData> &peopleData, int &databaseLoaded){
  strVector = {};      
  peopleData = {};
  string filename;
  cin >> filename;
  checkDatabase(filename, peopleData, strVector, databaseLoaded);
}

/*
This function runs the load_dna command using sequence and dnaLoaded.
*/
void commandDNA(ourvector<char> &sequence, int &dnaLoaded){
  sequence = {};
  string filename;
  cin >> filename;
  cout << "Loading DNA..." << endl; 
  ifstream test(filename); 
  if(!test){
    cout << "Error: unable to open '" << filename << "'" << endl;
  }
  else{
    loadDNA(filename, sequence);
    dnaLoaded = 1;
  }
}

/*
This function runs the process command using various integers to keep track of
whether other commands have been called, and vectors containing matches and
data.
*/
void commandProcess(ourvector<int> &individualMatches, int &databaseLoaded, int &dnaLoaded, ourvector<char> &sequence, ourvector<ourvector<char>> &strVector, int &dnaProcessed){
  individualMatches = {};
  if(databaseLoaded != 1){
    cout << "No database loaded." << endl;
  }
  else if(dnaLoaded != 1){
    cout << "No DNA loaded." << endl;
  }
  else{
    process(sequence, strVector, individualMatches);
    dnaProcessed = 1;      
  }
}


/*
This function runs the search command using various ints and vectors
*/
void commandSearch(int &databaseLoaded, int &dnaLoaded, int &dnaProcessed, ourvector<int> &individualMatches, ourvector<individualData> &peopleData){
  if(databaseLoaded != 1){
    cout << "No database loaded." << endl;
  }
  else if(dnaLoaded != 1){
    cout << "No DNA loaded." << endl;
  }
  else if(dnaProcessed != 1){
    cout << "No DNA processed." << endl;
  }
  else{
    search(individualMatches, peopleData);
  }
}

/*
This function runs the display command using various vectors and ints.
*/
void commandDisplay(int &databaseLoaded, ourvector<individualData> &peopleData, int &dnaLoaded, ourvector<char> &sequence, int &dnaProcessed, ourvector<ourvector<char>> &strVector, ourvector<int> &individualMatches){
  if(databaseLoaded == 1){
    cout << "Database loaded: " << endl;
    displayDB(peopleData);
  }
  else{
    cout << "No database loaded.\n" << endl;
  }
  if(dnaLoaded == 1){
    displayDNA(sequence);
  }
  else{
    cout << "No DNA loaded.\n" << endl;
  }
  if(dnaProcessed == 1){
    displayProcess(strVector, individualMatches);
  }
  else{
    cout << "No DNA has been processed." << endl;
  }
}

/*
This function runs a while loop for my creative component
*/
void changeDnaLoop(int &position, ourvector<individualData> &peopleData, int &newValue, int &i){
  while(true){
    cout << "Enter position to change: ";
    cin >> position;
    if((position >= 0) && (position < peopleData.at(i).dnaCount.size())){
      cout << "Enter new value: ";
      cin >> newValue;
      peopleData.at(i).dnaCount.at(position) = newValue;
      break;
    }
    else{
       continue;
    }
  }
}

/*
This function runs the changeDna command, which is my creative component.
*/
void changeDNA(ourvector<individualData> &peopleData){
  cout << "Enter person to modify: ";
  int position;
  int newValue;
  int personInDatabase = 0;
  string personName;
  cin >> personName;
  for (int i = 0; i < peopleData.size(); i++){
    if(personName == peopleData.at(i).name){  
      personInDatabase = 1;
      changeDnaLoop(position, peopleData, newValue, i);
    }
  }
  if(personInDatabase != 1){
    cout << personName << " Is not in database. No DNA changed" << endl;
  }
}


/*
This is the main function in which all the commands are called, and where the main
while loop running the program is located.
*/
int main() {
  ourvector<char> sequence;
  ourvector<ourvector<char>> strVector;
  ourvector<individualData> peopleData;
  ourvector<int> individualMatches;
  int databaseLoaded = 0;
  int dnaLoaded = 0;
  int dnaProcessed = 0;
  string command; 
  
  cout << "Welcome to the DNA Profiling Application." << endl;
    
  while(true){  
    getCommand(command);
    if(command == "load_db"){
      commandDB(strVector,peopleData,databaseLoaded);
    }  
    if(command == "load_dna"){
      commandDNA(sequence, dnaLoaded);
    }
    if(command == "process"){
      commandProcess(individualMatches, databaseLoaded, dnaLoaded, sequence, strVector, dnaProcessed);
    }
    if(command == "search"){
      commandSearch(databaseLoaded, dnaLoaded, dnaProcessed, individualMatches, peopleData);
    }
    if(command == "display"){
      commandDisplay(databaseLoaded, peopleData, dnaLoaded, sequence, dnaProcessed, strVector, individualMatches);
    }  
    if(command == "changeDna"){
      changeDNA(peopleData);
    }
    if(command == "#"){
      return 1;
    } 
  }
  return 0;
}