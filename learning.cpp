#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"database.h"
/*to do:
	figure out what other functions might be useful
	populate datastructures.txt more, change name to something better like programming.txt or w/e
	function a: write current database to a new file
	function b: update current student file by knowing something new
	function c: pick a learnable topic and print an explanation
		add a pointer to the node, point the pointer to an 'explanation' struct which is an array of 3 bigggg strings, find out if there's a maximum string size, let people pick level of depth for explanation, 3 explanations for each topic with different explanation depth
	make database with actual available courses and their dependencies so I can run the learnable function to figure out what classes I can take next semester also need my personal student file for that with the classes I've taken, maybe only include CSCI courses and their prerequisites in the database
	Fix segfaults :(
*/

int main() {
	database* points = new database;
	points->populate();
	bool keeplooping = true;
	std::cout << "Do you have a student file? (y/n)?" << std::endl;
	char answer;
	std::cin >> answer;
	if (answer == 'y') {
		points->knows(true);
	}
	else {
		points->knows(false);
	}
	while (keeplooping) {
		std::cout << "Your options: Change student (a), List all learnable topics for current student (b), Find out what you need to learn before you can learn a specified topic (c), Print all topics in the database (d), Remove current database and build a new database (e), Add a database's nodes to the current database (f), Add node to database (g), Exit program (h)" << std::endl;
		std::cin >> answer;
		if (answer == 'a') {
			points->rmknows();
			std::cout << "Do you have a student file? (y/n)?" << std::endl;
			char answer;
			std::cin >> answer;
			if (answer == 'y') {
				points->knows(true);
			}
			else {
				points->knows(false);
			}
		}
		if (answer == 'b') {
			points->learnable();
		}
		if (answer == 'c') {
			std::cout << "Type in the name of the topic. ";
			std::string topicname;
			std::cin.ignore();
			std::getline (std::cin, topicname);
			rdnode* node;
			bool assigned = false;
			std::vector<std::string> gottalearn;
			for (int count = 0; count < points->nodetotal() && assigned == false; count++) {
				node = points->getter(count);
				if (node->topic == topicname) {
					std::cout << "That topic exists in the database." << std::endl;
					assigned = true;
				}
			}
			if (assigned == false) {
				std::cout << "That topic isn't in the database. You may have spelled or formatted it incorrectly; make sure all your letters are lowercase." << std::endl;
			}
			else {
				gottalearn = points->prerequisites(node);
				if (gottalearn.size() == 0) {
					std::cout << "You can learn that now!" << std::endl;
				}
				else {
					std::cout << "You need to learn ";
					for (int count = 0; count < gottalearn.size(); count++) {
						std::cout << gottalearn[count];
						if (count + 1 < gottalearn.size()) {
							std::cout << " and ";
						}
					}
					std::cout << " before you can learn " << topicname << "." << std::endl;
				}
			}
		}
		if (answer == 'd') {
			points->printall();
		}
		if (answer == 'e') {
			points->depopulate();
			delete points;
			database* points = points->populate();
		}
		if (answer == 'f') {
			points->populate();
		}
		if (answer == 'g') {
			points->addnode();
		}
		if (answer == 'h') {
			keeplooping = false;
		}
	}
	points->depopulate();
	delete points;
	return 0;
}
