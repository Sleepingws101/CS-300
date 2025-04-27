//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Michael Duke
// Version     : 1.0
// Description : ProjectTwo- Binary Search Tree
//============================================================================


#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include "CSVparser.hpp"
#include <map>
#include <fstream>
#include <math.h>
#include <stdio.h>
using namespace std;


//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

//Course class to hold the course infomation
struct Course {
	string courseId;
	string courseTitle;
	std::vector<string> prerequisites;

	Course() {

	}
};

//Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	//default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	//initialize with course 
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};


//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */

class BinaryTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	void printSchedule(Node* node);
	void printCourseInfo(Node* node, string courseId);


public:
	BinaryTree();
	virtual ~BinaryTree();
	void DeleteRecursive(Node* node);
	void Insert(Course course);
	int PrereqCourseNum(Course course);
	void PrintSchedule();
	void PrintCourseInfo(string courseId);
};


//constructor for binary tree, initializes root
BinaryTree::BinaryTree() {
	root = nullptr;
}
//deconstructor for binary tree, passes root to function that deletes each node recursively
BinaryTree::~BinaryTree() {
	DeleteRecursive(root);
}

void BinaryTree::DeleteRecursive(Node* node) {
	if (node) {
		DeleteRecursive(node->left);
		DeleteRecursive(node->right);
		delete node;
	}
}

//used to insert node into tree
void BinaryTree::Insert(Course course) {
	if (root == nullptr)
		root = new Node(course);
	else
		this->addNode(root, course);
}

//returns the number of prerequisite courses for course
int BinaryTree::PrereqCourseNum(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.prerequisites.size(); i++) {
		if (course.prerequisites.at(i).length() > 0)
			count++;
	}
	return count;

}


void BinaryTree::PrintSchedule() {
	this->printSchedule(root);
}

//using the public Insert method, determines correct location alphabetically
void BinaryTree::addNode(Node* node, Course course) {

	if (node->course.courseId.compare(course.courseId) > 0) {
		//if empty, adds node to tree
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}

	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}

void BinaryTree::printSchedule(Node* node) {

	if (node != nullptr) {
		printSchedule(node->left);
		cout << node->course.courseId << ", " << node->course.courseTitle << endl;
		printSchedule(node->right);
	}
	return;
}
//displays course info, including prerequisites, checks prerequisite values
void BinaryTree::printCourseInfo(Node* curr, string courseId) {
	while (curr != nullptr) {

		if (curr->course.courseId.compare(courseId) == 0) {

			cout << endl << curr->course.courseId << ", " << curr->course.courseTitle << endl;
			unsigned int size = PrereqCourseNum(curr->course);
			cout << "Prerequisite(s): ";

			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.prerequisites.at(i);
				if (i != size - 1)
					cout << ", ";
			}

			if (i == 0)
				cout << "No prerequisites.";
			cout << endl;
			return;
		}

		else if (courseId.compare(curr->course.courseId) < 0)
			curr = curr->left;

		else
			curr = curr->right;
	}

	cout << "Course " << courseId << " not found." << endl;
}

//loads the courses from the cin choice, for csv file
bool loadCourses(string csvPath, BinaryTree* binaryTree) {
	//attempts to open file
	try {
		ifstream CourseInfo(csvPath);

		if (CourseInfo.is_open()) {
			while (!CourseInfo.eof()) {
				//vector holds each course
				vector<string> courseInfo;
				string courseData;

				getline(CourseInfo, courseData);
				while (courseData.length() > 0) {
					unsigned int comma = courseData.find(", ");
					if (comma < 100) {
						courseInfo.push_back(courseData.substr(0, comma));
						courseData.erase(0, comma + 1);
					}

					else {
						courseInfo.push_back(courseData.substr(0, courseData.length()));
						courseData = "";
					}
				}
				//loads values into course
				Course course;
				course.courseId = courseInfo[0];
				course.courseTitle = courseInfo[1];

				for (unsigned int i = 2; i < courseInfo.size(); i++) {
					course.prerequisites.push_back(courseInfo[i]);
				}
				//inserts into binary tree
				binaryTree->Insert(course);
			}
			//closes file
			CourseInfo.close();
			return true;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Caught an exception: " << e.what() << std::endl;
	}
	return false;
}

//main method
int main(int argc, char* argv[]) {
	string csvPath, courseNum;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseNum = argv[2];
		break;
	default:
		csvPath = "";
		break;
	}
	//defines the tree and prints welcome to user
	BinaryTree* binaryTree = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;

	string choice = "0";
	int userChoice = choice[0] - '0';
	//while not exitted (9)
	while (userChoice != 9) {
		cout << "   1. Load Data Structure" << endl;
		cout << "   2. Print List of Courses" << endl;
		cout << "   3. Print Course" << endl;
		cout << "   9. Exit" << endl;
		cout << "\nPlease select an option";
		cin >> choice;
		//checks number of digits for user choice
		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;

		bool success = 1;

		switch (userChoice) {

		case 1:

			if (binaryTree == nullptr)
				binaryTree = new BinaryTree();
			if (csvPath.length() == 0) {
				cout << "Enter the file name that contains the course data: ";
				cin >> csvPath;
			}
			//Checks if file was loaded successfully, and prints feedback to user
			success = loadCourses(csvPath, binaryTree);
			if (success)
				cout << "Courses have been loaded successfully!\n" << endl;
			else
				cout << "Unable to locate file.\n" << endl;
			csvPath = "";
			break;

		//validates that tree exists, displays schedule
		case 2:

			if (binaryTree != nullptr && success) {
				cout << "Here is the schedule:\n" << endl;
				binaryTree->PrintSchedule();
				cout << endl;
			}

			else
				cout << "Load courses first (Option 1)\n" << endl;
			break;

		case 3:

			if (binaryTree != nullptr && success) {
				if (courseNum.length() == 0) {
					cout << "What course would you like to search? ";
					cin >> courseNum;
					for (auto& userChoice : courseNum) userChoice = toupper(userChoice);
				}
				binaryTree->PrintCourseInfo(courseNum);
				cout << endl;
			}
			else
				cout << "Load courses first (Option 1)\n" << endl;
			courseNum = "";
			break;

		default:
			if (userChoice != 9)
				cout << choice << " is not a valid input.\n" << endl;
			break;
		}
	}
	//when exitted, says good bye to user
	cout << "\nGood bye!" << endl;
	return 0;
}