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
		temp = points->nodes[count];
		if (temp->knows == false) {
			rdnode* parent;
			for (int counter = 0; counter < temp->parents.size() && learnability == true; counter++) {
				parent = temp->parents[counter];
				if (parent->knows == false) {
					learnability = false;
				}
			}
			if (learnability == true) {
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

void knowsr(rdnode* temp) {
	rdnode* thing;
	bool ask = true;
	for (int count = 0; count < temp->children.size(); count++) {
		thing = temp->children[count];
		rdnode* third;
		for (int counter = 0; count < thing->parents.size(); count++) {
			third = thing->parents[count];
			if (third->knows == false) {
				ask = false;
			}
		}
		if (ask == true) {
			std::cout << "Do you understand " << thing->topic << " (y/n)?" << std::endl;
			char answer;
			std::cin >> answer;
			if (answer != 'y' && answer != 'n') {
				std::cout << "try again" << std::endl;
			}
			if (answer == 'y') {
				thing->knows = true;
			}
		}
		ask = false;
	}
}

void knows(database* points) {
	rdnode* temp;
	char answer;
	for (int count = 0; count < points->nodes.size(); count++) {
		temp = points->nodes[count];
		if (temp->parents.size() == 0) {
			std::cout << "Do you understand " << temp->topic << " (y/n)?" << std::endl;
			std::cin >> answer;
			if (answer != 'y' && answer != 'n') {
				std::cout << "try again" << std::endl;
				count--;
			}
			if (answer == 'y') {
				temp->knows = true;
				knowsr(temp);
			}
		}		
	}
}

void populate(char filename[], database* points) {
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
			needtoknow.push_back(parent->topic);
			std::vector<std::string> alsoneed = prerequisites(parent, points);
			for (int counter = 0; counter < alsoneed.size(); counter++) {
				needtoknow.push_back(alsoneed[counter]);
			}
		}
	}
	return needtoknow;
}

int main() {
	char filename[30];
	std::cout << "Enter file name for database." << std::endl;
	std::cin >> filename;
	database* points = new database;
	populate(filename, points);
	//printall(points); //this is useless except for debugging
	bool keeplooping = true;
	std::cout << "Do you have a student file? (y/n)?" << std::endl;
	char answer;
	std::cin >> answer;
	if (answer == 'y') {
		student(points);
	}
	else {
		knows(points);
	}
	while (keeplooping) {
		std::cout << "Your options: Change student (a), List all learnable topics for current student (b), Find out what you need to learn before you can learn a specified topic (c)" << std::endl;
		std::cin >> answer;
		if (answer == 'a') {
			rmknows(points);
			std::cout << "Do you have a student file? (y/n)?" << std::endl;
			char answer;
			std::cin >> answer;
			if (answer == 'y') {
				student(points);
			}
			else {
				knows(points);
			}
		}
		if (answer == 'b') {
			learnable(points);
		}
		if (answer == 'c') {
			std::string topicname;
			std::cin >> topicname;
			rdnode* node;
			bool assigned = false;
			for (int count = 0; count < points->nodes.size() && assigned == false; count++) {
				node = points->nodes[count];
				if (node->topic == topicname) {
					assigned = true;
				}
			}
			if (assigned == false) {
				std::cout << "That topic isn't in the database. You may have spelled or formatted it incorrectly; make sure all your letters are lowercase." << std::endl;
			}
			else {
				prerequisites(node, points);
			}
		}
	}
	depopulate(points);
	return 0;
}
