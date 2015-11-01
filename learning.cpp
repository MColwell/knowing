#include<iostream>
#include<fstream>
#include<string>
#include<vector>
/*to do:
	figure out what other functions might be useful
	populate datastructures.txt more, change name to something better like programming.txt or w/e
*/
struct rdnode {
	public:
	std::string topic;
	std::vector<rdnode*> parents;
	std::vector<rdnode*> children;
	bool knows;
};

struct database {
	std::vector<rdnode*> nodes;
};

void learnable (database* points) {
	rdnode* temp;
	std::vector<rdnode*> learn;
	bool learnability = true;
	for (int count = 0; count < points->nodes.size(); count++) {
		learnability = true;
		std::cout << "aaaaaa" << std::endl;
		temp = points->nodes[count];
		if (temp->knows == false) {
			rdnode* parent;
			std::cout << "aaabbbbaaa" << std::endl;
			for (int counter = temp->parents.size() - 1; counter >= 0 && learnability == true; counter--) {
				parent = temp->parents[counter];
				if (parent->knows == false) {
					learnability = false;
				}
			}
			if (learnability == true) {
				std::cout << "accca" << std::endl;
				learn.push_back(temp);
			}
		}
	}
	std::cout << "You can learn: " << learn.size() << " topics." << std::endl;
	for (int count = 0; count < learn.size(); count++) {
		temp = learn[count];
		std::cout << temp->topic << " " << std::endl;
	}
}

void depopulate(database* points) {
	rdnode* delnode;
	for (int count = 0; count < points->nodes.size(); count++) {
		delnode = points->nodes[count];
		delete delnode;
	}
	delete points;
}

rdnode* makenode(std::string topicname, database* points) {
	rdnode* tempnode;
	for (int count = 0; count < points->nodes.size(); count++) {
		tempnode = points->nodes[count];
		if (tempnode->topic == topicname) {
			return tempnode;
		}
	}
	rdnode* newnode = new rdnode;
	newnode->topic = topicname;
	newnode->knows = false;
	points->nodes.push_back(newnode);
	return newnode;
}

bool asker(rdnode* temp) {
	char answer;
	std::cout << "Do you understand " << temp->topic << " (y/n)?" << std::endl;
	std::cin >> answer;
	if (answer == 'y') {
		return true;
	}
	if (answer == 'n') {
		return false;
	}
	std::cout << "That's not y or n. Try again." << std::endl;
	return asker(temp);
}

void parentsknown(rdnode* temp) {
	temp->knows = true;
	for (int count = 0; count < temp->parents.size(); count++) {
		parentsknown(temp->parents[count]);
	}
}

void knows(database* points, bool hasfile) {
	rdnode* temp;
	bool yeah;
	std::vector<std::string> person;
	if (hasfile == true) {
		std::cout << "Enter name of student file." << std::endl;
		char filename[30];
		std::cin >> filename;
		std::ifstream inputconf(filename);
		std::string line;
		while (getline (inputconf, line)) {
			person.push_back(line);
		}
		inputconf.close();
	}
	for (int count = 0; count < points->nodes.size(); count++) {
		yeah = false;
		temp = points->nodes[count];
		if (temp->knows == false) {
			if (!hasfile) {
				temp->knows = asker(temp);
				if (temp->knows) {
					yeah = true;
				}
			}
			else {
				for (int counter = 0; counter < person.size() && yeah == false; counter++) {
					if (person[counter] == temp->topic) {
						yeah = true;
						temp->knows = true;
					}
				}
			}
			if (yeah) {
				parentsknown(temp);
			}
		}
	}
}

database* populate(database* points) {
	char filename[30];
	std::cout << "Enter file name for database." << std::endl;
	std::cin >> filename;
	std::vector<std::string> conf;
	std::ifstream inputconf(filename);
	std::string line;
	while (getline (inputconf, line)) {
		conf.push_back(line);
	}
	inputconf.close();
	for (int counter = 0; counter < conf.size(); counter++) {
		line = conf[counter];
		rdnode* tempnode;
		std::string currstring;
		int count = 0;
		std::vector<rdnode*> parenting;
		for(; count < line.size(); count++) {
			if (line[count] != ',' && line[count] != '#') {
				currstring.push_back(line[count]);
			}
			if (line[count] == ',' || line[count] == '#') {
				tempnode = makenode(currstring, points);
				currstring.clear();
				parenting.push_back(tempnode);
			}
			if (line[count] == '#') {
				break;
			}
		}
		count++;
		for (; count < line.size(); count++) {
			currstring.push_back(line[count]);
		}
		tempnode = makenode(currstring, points);
		rdnode* currentparent;
		bool add = true;
		for (count = 0; count < parenting.size(); count++) {
			currentparent = parenting[count];
			for (int counter = 0; counter < tempnode->parents.size(); counter++) {
				if (tempnode->parents[counter] == currentparent) {
					add = false;
				}
			}
			if (add == true) {
				tempnode->parents.push_back(parenting[count]);
				currentparent->children.push_back(tempnode);
			}
		}
	}
	return points;
}

void printall(database* points) {
	rdnode* temp;
	for (int count = 0; count < points->nodes.size(); count++) {
		temp = points->nodes[count];
		std::cout << temp->topic << std::endl;
	}
}

void rmknows(database* points) {
	rdnode* temp;
	for (int count = 0; count < points->nodes.size(); count++) {
		temp = points->nodes[count];
		temp->knows = false;
	}
}

void student(database* points) {
	std::cout << "Enter name of student file." << std::endl;
	std::vector<std::string> person;
	char filename[30];
	std::cin >> filename;
	std::ifstream inputconf(filename);
	std::string line;
	while (getline (inputconf, line)) {
		person.push_back(line);
	}
	inputconf.close();
	rdnode* tempnode;
	bool finishloop = false;
	for (int count = 0; count < person.size(); count++) {
		for (int counter = 0; counter < points->nodes.size() && finishloop == false; counter++) {
			tempnode = points->nodes[counter];
			if (tempnode->topic == person[count]) {
				tempnode->knows = true;
				finishloop = true;
			}
		}
		finishloop = false;
	}
}

std::vector<std::string> prerequisites(rdnode* node, database* points) {
	std::vector<std::string> needtoknow;
	rdnode* parent;
	for (int count = 0; count < node->parents.size(); count++) {
		parent = node->parents[count];
		if (parent->knows == false) {
			bool alreadythere = false;
			for (int counter = 0; counter < needtoknow.size(); counter++) {
				if (parent->topic == needtoknow[counter]) {
					alreadythere = true;
				}
			}
			if (alreadythere == false) {
				needtoknow.push_back(parent->topic);
			}
			std::vector<std::string> alsoneed = prerequisites(parent, points);
			alreadythere = false;
			for (int counter = 0; counter < alsoneed.size(); counter++) {
				for (int counting = 0; counting < needtoknow.size(); counting++) {
					if (needtoknow[counting] == alsoneed[counter]) {
						alreadythere = true;
					}
				}
				if (alreadythere == false) {
					needtoknow.push_back(alsoneed[counter]);
				}
				alreadythere = false;
			}
		}
	}
	return needtoknow;
}

int main() {
	database* points = new database;
	points = populate(points);
	bool keeplooping = true;
	std::cout << "Do you have a student file? (y/n)?" << std::endl;
	char answer;
	std::cin >> answer;
	if (answer == 'y') {
		knows(points, true);
	}
	else {
		std::cout << "else is working" << std::endl;
		knows(points, false);
	}
	while (keeplooping) {
		std::cout << "Your options: Change student (a), List all learnable topics for current student (b), Find out what you need to learn before you can learn a specified topic (c), Print all topics in the database (d), Remove current database and build a new database (e), Add a database's nodes to the current database (f), Exit program (g)" << std::endl;
		std::cin >> answer;
		if (answer == 'a') {
			rmknows(points);
			std::cout << "Do you have a student file? (y/n)?" << std::endl;
			char answer;
			std::cin >> answer;
			if (answer == 'y') {
				knows(points, true);
			}
			else {
				knows(points, false);
			}
		}
		if (answer == 'b') {
			learnable(points);
		}
		if (answer == 'c') {
			std::cout << "Type in the name of the topic. ";
			std::string topicname;
			//std::cout << "before" << std::endl;
			std::cin.ignore();
			std::getline (std::cin, topicname);
			//std::cout << "after" << std::endl;
			rdnode* node;
			bool assigned = false;
			std::vector<std::string> gottalearn;
			for (int count = 0; count < points->nodes.size() && assigned == false; count++) {
				node = points->nodes[count];
				if (node->topic == topicname) {
					std::cout << "That topic exists in the database." << std::endl;
					assigned = true;
				}
			}
			if (assigned == false) {
				std::cout << "That topic isn't in the database. You may have spelled or formatted it incorrectly; make sure all your letters are lowercase." << std::endl;
			}
			else {
				gottalearn = prerequisites(node, points);
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
			printall(points);
		}
		if (answer == 'e') {
			depopulate(points);
			database* points = populate(points);
		}
		if (answer == 'f') {
			points = populate(points);
		}
		if (answer == 'g') {
			keeplooping = false;
		}
	}
	depopulate(points);
	return 0;
}
