struct rdnode {
	public:
	std::string topic;
	std::vector<rdnode*> parents;
	std::vector<rdnode*> children;
	bool knows;
};

class database {
	std::vector<rdnode*> nodes;
	public:
	rdnode* getter (int count) {
		return nodes[count];
	}
	int nodetotal () {
		return nodes.size();
	}
	rdnode* makenode(std::string topicname) {
		rdnode* tempnode;
		for (int count = 0; count < nodes.size(); count++) {
			tempnode = nodes[count];
			if (tempnode->topic == topicname) {
				return tempnode;
			}
		}
		rdnode* newnode = new rdnode;
		newnode->topic = topicname;
		newnode->knows = false;
		nodes.push_back(newnode);
		return newnode;
	}
	void learnable () {
		rdnode* temp;
		std::vector<rdnode*> learn;
		bool learnability = true;
		for (int count = 0; count < nodes.size(); count++) {
			learnability = true;
			temp = nodes[count];
			if (temp->knows == false) {
				rdnode* parent;
				for (int counter = temp->parents.size() - 1; counter >= 0 && learnability == true; counter--) {
					parent = temp->parents[counter];
		std::cout << "aaaaaaa" << std::endl;
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
	void depopulate() {
		rdnode* delnode;
		for (int count = 0; count < nodes.size(); count++) {
			delnode = nodes[count];
			delete delnode;
		}
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

	void knows(bool hasfile) {
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
		for (int count = 0; count < nodes.size(); count++) {
			yeah = false;
			temp = nodes[count];
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

	database* populate() {
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
					tempnode = makenode(currstring);
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
			tempnode = makenode(currstring);
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

	void printall() {
		rdnode* temp;
		for (int count = 0; count < nodes.size(); count++) {
			temp = nodes[count];
			std::cout << temp->topic << std::endl;
		}
	}

	void rmknows() {
		rdnode* temp;
		for (int count = 0; count < nodes.size(); count++) {
			temp = nodes[count];
			temp->knows = false;
		}
	}

	void student() {
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
			for (int counter = 0; counter < nodes.size() && finishloop == false; counter++) {
				tempnode = nodes[counter];
				if (tempnode->topic == person[count]) {
					tempnode->knows = true;
					finishloop = true;
				}
			}
			finishloop = false;
		}
	}

	std::vector<std::string> prerequisites(rdnode* node) {
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
				std::vector<std::string> alsoneed = prerequisites(parent);
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

	void addnode() {
		std::cout << "Enter the topic name." << std::endl;
		std::string newtopic;
		std::cin.ignore();
		std::getline(std::cin, newtopic);
		rdnode* newnode = makenode(newtopic);
		std::cout << "If this node has parents, please enter them one at a time separated by carriage returns. When you are done, enter 0 and depress the carriage return." << std::endl;
		while (newtopic != "0") {
			if (newtopic == "0") {
				std::cout << "what." << std::endl;
			}
			newtopic = "sentinelvalueeeeeeee";
			std::getline(std::cin, newtopic);
			if (newtopic == "sentinelvalueeeeeeee") {
				std::cin.ignore();
				std::getline (std::cin, newtopic);
			}
			if (newtopic != "0") {
				rdnode* parent = makenode(newtopic);
				std::string parenttopic;
				bool there = false;
				for (int count = 0; count < newnode->parents.size(); count++) {
					parent = newnode->parents[count];
					parenttopic = parent->topic;
					if (newtopic == parenttopic) {
						there = true;
					}
				}
				if (!there) {
					newnode->parents.push_back(parent);
				}
			}
		}
	}

};
