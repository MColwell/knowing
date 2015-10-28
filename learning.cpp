#include<iostream>
#include<fstream>
#include<string>
#include<vector>
/*to do:
	change all cin to reading from a file UNLESS IT'S A FILE NAME TO READ
	fix reading in a file name to be a C style string and use it properly
	when adding parents, add children (check for dupes) and when adding children, add parents (again)
	function: figure out every topic you need to learn before you can learn a specified topic
	figure out what other functions might be useful
	populate datastructures.txt more, change name to something better like programming.txt or w/e
	implement student files(just a list of known topics)
	implement interface for users to specify a student file so knows is automatically run with it
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
			for (int counter = 0; counter < temp->parents.size(); counter++) {
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
	std::cout << "makenode " << topicname << std::endl;
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
	std::cout << "adding " << topicname << std::endl;
	return newnode;
}

void knowsr(rdnode* temp) {
	//std::cout << "knowsr" << std::endl;
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
	//std::cout << "knows" << std::endl;
	rdnode* temp;
	char answer;
	for (int count = 0; count < points->nodes.size(); count++) {
		temp = points->nodes[count];
		if (temp->parents.size() == 0) {
			std::cout << "Do you understand " << temp->topic << " (y/n)?" << std::endl; // later implement reading in files (so you can change between students without losing and redoing data)
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

void populate(const std::string filename, database* points) {
	std::vector<std::string> conf;
	std::cout << filename << std::endl;
	std::ifstream inputconf("datastructures.txt"); //fix this implementation later! char*[]
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

int main() {
	/*
	std::cout << "This program creates a tree where each node has a string (topic) an arbitrary number of children (represented by a vector of pointers) and a boolean value (known == true, unknown == false). This tree is intended to be used as a guideline for learning and teaching. If a student does not fully understand a topic, the boolean should be false for that topic. Learning the children topics should only be done after one learns the node's topic." << std::endl;
	std::cout << "The program begins by creating a Root Node that does not actually represent a topic. The topic in the Root Node is NULL. Any children of the Root Node do not require prior knowledge." << std::endl;
	treenode* root = new treenode;
	root->topic == NULL;
	root->children[0] == NULL;
	boolean knows = true;
	*/
	std::string filename = "datastructures.txt";	//in the future, cin filename
	database* points = new database;
	populate(filename, points);
	printall(points); //this is useless except for debugging
	knows(points);
	learnable(points);
	depopulate(points);
	return 0;
}
