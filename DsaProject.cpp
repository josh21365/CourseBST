// Joshua Velazquez 4/13/2026

// Imports
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <set>
#include <unordered_set>
#include <string>
#include <cctype>
#include <limits>

using namespace std;

class BinaryTree; // Forward declare BinaryTree so it can be used in function prototypes

// Foward declarations
void LoadFileIntoBST(string fileName, BinaryTree& bst);
int checkUserinput();

struct Course { // The course struct to store the course information, this will be the data stored in the binary tree nodes
    string courseNumber;
    string courseName;
	vector<string> prerequsites; // Vector to store the prerequsites of the course, this allows for multiple prereqs
};

class BinaryTree { // The binary tree class to store the courses in a binary search tree structure

private:
    
	struct Node { // The node struct to store the course data and pointers to left and right
		Course data; // The course data stored in the node
        Node* left; // Pointer left
		Node* right; // Pointer right

		Node(const Course& course) : data(course), left(nullptr), right(nullptr) { // Constructor for the node, takes in a course and initializes the data and sets left and right to nullptr
        }
    };

	Node* root = nullptr; // The root of the tree, starts as nullptr

    

	Node* InsertRecursive(Node* node, const Course& course) { // The recursive insert function, takes in a node and a course to insert
		if (node == nullptr) { // If the node is empty meaning we have found the correct spot to insert
			Node* newNode = new Node(course); // Create a new node with the course data
			return newNode; // Return the new node to be connected to the tree
        }

		if (course.courseNumber < node->data.courseNumber) { // If the course number is less than the current node's course number
			node->left = InsertRecursive(node->left, course); // Then move recursively to the left if less
        }

		else { // Otherwise the course number is greater than or equal to the current node's course number
			node->right = InsertRecursive(node->right, course); // Move recursively to the right if greater or equal
        }
		return node; // Return the current node to be connected to the tree as we unwind the recursion
    }

    // Recursively delete all nodes in post-order to free memory
    void DeleteAllNodes(Node* node) {
		if (node == nullptr) return; // If the node is empty return, this is the base case for the recursion
		DeleteAllNodes(node->left); // Recursively delete left subtree
		DeleteAllNodes(node->right); // Recursively delete right subtree
		delete node; // Delete the current node after its children have been deleted
    }

	Node* SearchRecursive(Node* node, const string courseNumber) { // The recursive search function, takes in a node and a course number to search for
		if (node == nullptr) return nullptr; // If the node is empty we have reached the end of the tree without finding the course, return nullptr

		if (courseNumber == node->data.courseNumber) return node; // If the course number matches the current node's course number we have found the course, return the node

        else if (courseNumber < node->data.courseNumber) { // Now we go left if the ID is less than node's ID
            return SearchRecursive(node->left, courseNumber); // Recursively go left
        }
        else { // Otherwise we go right
            return SearchRecursive(node->right, courseNumber); // Recursively go right
        }
    }

	void InOrder(Node* node) { // The in-order traversal function to print the courses in sorted order, takes in a node to start from
		if (node != nullptr) { // If the node is not empty
            
			InOrder(node->left); // First we go left (Smaller value)

			cout << node->data.courseNumber << ", " << node->data.courseName << endl; // Then we print the current node's course number and name

			InOrder(node->right); // Finally we go right (Greater value)
        }
    }

public:

    ~BinaryTree() { // Destructor to free all nodes
		DeleteAllNodes(root); // Call the recursive delete function starting from the root
		root = nullptr; // Set root to nullptr after deleting all nodes
    }

    void PrintAllCourses() { // Used to print every course
        if (root == nullptr) { // If the root is empty
            cout << "No courses are available." << endl << endl; // We have no more courses
            return; // Leave
        }

        InOrder(root); // Calls the InOrder function to display the courses
		cout << endl; // Print an extra line after printing all courses for better formatting
    };

	Node* Search(const string& courseNumber) { // Calls the search function with the course number to find
		return SearchRecursive(root, courseNumber); // Start the search from the root
    };

	void Insert(const Course& course) { // Calls the insert function with the course to insert
		root = InsertRecursive(root, course); // Start the insert from the root, and update the root in case it was empty
    };

	void PrintCourse(const string& courseNumber) { // Used to print a specific course and its prereqs
        
		Node* node = Search(courseNumber); // Search for the course using the search function, this will return a pointer to the node containing the course or nullptr if not found

        if (node == nullptr) { // If the pointer is empty
            cout << "Course not found." << endl << endl; // It was not found
            return; // Exit
        }

		cout << node->data.courseNumber << ", " << node->data.courseName << endl; // Print the course number and name

		if (node->data.prerequsites.empty()) { // If the prereqs vector is empty
            cout << "No prerequisites" << endl; // There are no prereqs
        }
        else { // Otherwise
            cout << "Prerequisites: "; // Begin printing the prereqs

			for (const string& prereq : node->data.prerequsites) { // Loop through the prereqs vector and print each prereq
                cout << prereq << ", "; // Space by using a comma
            }
			cout << endl; // End the line after printing all prereqs
        }
    };

};

int main()
{
	int userChoice; // Variable to store the user's menu choice
	BinaryTree bst; // Create an instance of the binary tree to store the courses
	string courseNumber; // Variable to store the course number input by the user when they want to print a specific course

	cout << "Welcome to course planner." << endl << endl; // Welcome message

    while (true) {
		cout << "  1. Load file" << endl; // Option to load a file
		cout << "  2. Print all courses" << endl; // Option to print all courses
		cout << "  3. Search course" << endl; // Option to search for a specific course
		cout << "  9. Exit" << endl << endl; // Option to exit the program

		cout << "What would you like to do? "; // Prompt the user for their choice
        // Read and validate user input using checkUserinput (reads the whole line)
        userChoice = checkUserinput();

		switch (userChoice) { // Check the user's input and switch based on the menu choice
        case 1:
			LoadFileIntoBST("CS 300 ABCU_Advising_Program_Input.csv", bst); // Call the function to load the courses from the file into the binary search tree
			cout << "The courses have been loaded into the BST." << endl << endl;
            break;

        case 2:
			cout << "Here is the sample schedule: " << endl << endl; // Message before printing all courses
            bst.PrintAllCourses(); // Call the function to print all courses in the binary search tree
			break;

     case 3:
			cout << "What course do you want to know about? ";
			{
				string input;
				// Read the whole line to avoid leftover-newline issues from previous >> usage
				if (!getline(cin, input) || input.find_first_not_of(" \t\r\n") == string::npos) {
					// If the first getline read an empty line (common when previous input left a newline), prompt again
					cout << "Please enter a course: ";
					getline(cin, input);
				}
				// trim
				size_t start = input.find_first_not_of(" \t\r\n"); // Find the first non-whitespace character to trim leading whitespace
				size_t end = input.find_last_not_of(" \t\r\n"); // Find the last non-whitespace character to trim trailing whitespace
				string courseInput = (start == string::npos) ? string() : input.substr(start, end - start + 1); // Trim the input to remove leading and trailing whitespace
				for (char& c : courseInput) c = toupper((unsigned char)c); // Convert the input to uppercase to match the course numbers in the BST
				bst.PrintCourse(courseInput); // Call the function to print the specific course and its prereqs based on the user's input
				cout << endl; // Print an extra line after printing the course for better formatting
			}
			break;

        case 9:
			cout << "Thank you for using the course planner!" << endl; // Exit message
            return 0;

        default:
			cout << userChoice << " is not a valid option." << endl; // Message for invalid menu choice
            break;
        }
    }
}

void LoadFileIntoBST(string fileName, BinaryTree& bst) { // Function to load the courses from the file into the binary search tree, takes in the file name and a reference to the binary tree to insert into
	ifstream file(fileName); // Open the file using an ifstream object
	if (!file.is_open()) { // If the file could not be opened, report a fatal error and exit the function
		cout << "FATAL ERROR: could not open file" << endl; // Report the error
		return; // Exit the function
    }

	vector<Course> tempCourses; // Temporary vector to store the courses read from the file before validating and inserting into the BST
	unordered_set<string> validCourseNumbers; // Unordered set to store the valid course numbers read from the file, this will be used for quick lookup when validating prerequisites

	string line; // Variable to store each line read from the file
	size_t lineno = 0; // Line number for error reporting

    // PASS 1: READ FILE ONLY
    while (getline(file, line)) {
		++lineno; // Increment line number for each line read

        // detect blank (whitespace only) lines
		if (line.find_first_not_of(" \t\r\n") == string::npos) { // If the line is empty or only whitespace, report a fatal error with the line number and exit
			cout << "FATAL ERROR: empty line detected at line" << lineno << endl; // Report the error with the line number
			file.close(); // Close the file before exiting
            return;
        }

		vector<string> tokens; // Vector to store the tokens from the line after splitting by comma
		string token; // Temporary variable to hold each token while splitting
		stringstream ss(line); // Create a stringstream from the line to use for splitting by comma
		while (getline(ss, token, ',')) { // Split the line by comma and store each token in the vector
            // trim token
			size_t start = token.find_first_not_of(" \t\r\n"); // Find the first non-whitespace character to trim leading whitespace
			size_t end = token.find_last_not_of(" \t\r\n"); // Find the last non-whitespace character to trim trailing whitespace
			if (start == string::npos) token = ""; // If the token is all whitespace, set it to an empty string
			else token = token.substr(start, end - start + 1); // Otherwise, trim the token to remove leading and trailing whitespace
			tokens.push_back(token); // Add the trimmed token to the vector of tokens
        }

		if (tokens.size() < 2) { // If there are less than 2 tokens, it means the line does not have a valid course number and name, report a fatal error with the line number and exit
			cout << "FATAL ERROR: invalid format on line" << lineno << endl; // Report the error with the line number
            file.close(); // Close the file before exiting
			return; // Exit the function
        }

		Course course; // Create a new course object to store the course information from the line
		course.courseNumber = tokens[0]; // The first token is the course number, store it in the course object
		course.courseName = tokens[1]; // The second token is the course name, store it in the course object
		for (size_t i = 2; i < tokens.size(); ++i) { // The remaining tokens are the prerequisites, loop through them starting from index 2
			if (!tokens[i].empty()) course.prerequsites.push_back(tokens[i]); // If the token is not empty, add it to the course's prerequisites vector
        }

        tempCourses.push_back(course); // Add the course to the temporary courses vector
        validCourseNumbers.insert(course.courseNumber); // Add the course number to the set of valid course numbers
    }

	file.close(); // Close the file after reading all lines

    // PASS 2: VALIDATE
	for (const Course& course : tempCourses) { // Loop through the temporary courses vector to validate that all prerequisites are valid course numbers
		for (const string& prereq : course.prerequsites) { // Loop through the prerequisites of the current course
			if (validCourseNumbers.find(prereq) == validCourseNumbers.end()) { // If the prerequisite is not found in the set of valid course numbers, it means there is a missing prerequisite, report a fatal error with the course number and the missing prerequisite and exit
				cout << "FATAL ERROR: Missing prerequisite " << prereq << " for course " << course.courseNumber << endl; // Report the error with the course number and the missing prerequisite
                return;
            }
        }
    }

    // PASS 3: INSERT INTO BST
    for (const Course& course : tempCourses) {
        bst.Insert(course);
    }
}

int checkUserinput() {
    string line;
    while (true) {
        if (!getline(cin, line)) return 9; // treat EOF as exit
        // skip whitespace-only lines
		if (line.find_first_not_of(" \t\r\n") == string::npos) { // If the line is empty or only whitespace, prompt the user to enter an integer and continue the loop to read input again
			cout << "Invalid input; please enter an integer: "; // Prompt the user for valid input
			continue; // Continue the loop to read input again
        }
        try {
			size_t pos = 0; // Try to convert the line to an integer using stoi, which will throw an exception if the input is not a valid integer or if it is out of range
			int val = stoi(line, &pos); // Convert the line to an integer and store the position of the first character after the number in pos
			while (pos < line.size() && isspace((unsigned char)line[pos])) ++pos; // Skip any trailing whitespace after the number
			if (pos != line.size()) throw invalid_argument("extra chars"); // If there are extra characters after the number, throw an invalid_argument exception
			return val; // If the conversion is successful and there are no extra characters, return the integer value
		}
		catch (const invalid_argument&) { // If an invalid_argument exception is caught, it means the input was not a valid integer, prompt the user to enter an integer and continue the loop to read input again
			cout << "Invalid input; please enter an integer: "; // Prompt the user for valid input
			continue; // Continue the loop to read input again
		}
		catch (const out_of_range&) { // If an out_of_range exception is caught, it means the input was a number but it is too large to fit in an int, prompt the user to enter a valid integer and continue the loop to read input again
			cout << "Number out of range; please enter a valid integer: "; // Prompt the user for valid input
			continue; // Continue the loop to read input again
        }
    }
}