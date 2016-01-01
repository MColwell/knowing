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
		//std::cout << "Current node count: " << nodes.size() << std::endl;
		//std::cout << "About to make tempnode" << std::endl;
		rdnode* tempnode;
		//std::cout << "Tempnode made. " << std::endl;
		if (topicname.size() == 0) {
			return 0;
		}
		if (nodes.size() == 0) {
			rdnode* newnode = new rdnode;
			newnode->topic = topicname;
			newnode->knows = false;
			nodes.push_back(newnode);
			return newnode;
		}
		for (int count = 0; count < nodes.size(); count++) {
			//std::cout << "In for loop." << std::endl;
			tempnode = nodes[count];
			//std::cout << "aaa" << std::endl;
			if (tempnode->topic == topicname) {
				//std::cout << "Topic " << topicname << " already in database." << std::endl;
				return tempnode;
			}
		}
		//std::cout << "Topic " << topicname << " not in database." << std::endl;
		rdnode* newnode = new rdnode;
		newnode->topic = topicname;
		newnode->knows = false;
		//std::cout << "New node created, adding node to database. " << std::endl;
		nodes.push_back(newnode);
		//std::cout << "Node added to database." << std::endl;
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
/**/					//std::cout << "aaaaaaa" << std::endl;
//std::cout << parent->topic << std::endl;
					if (parent->knows == false) {
						//std::cout << "bbbbbb" << std::endl;
						learnability = false;
					}
				}
				if (learnability == true) {
					//std::cout << "truuuuuu" << std::endl;
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
		nodes.clear();
		//std::cout << "node count: " << nodes.size() << std::endl;
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
		//std::cout << "currently " << nodes.size() << " nodes." << std::endl;
		char filename[30];
		std::cout << "Enter file name for database." << std::endl;
		std::cin >> filename;
		std::vector<std::string> conf;
		conf.clear();
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
			currstring.clear();
			int count = 0;
			std::vector<rdnode*> parenting;
			parenting.clear();
			for(bool done = false; count < line.size() && done == false; count++) {
				if (line[count] != ',' && line[count] != '#') {
					//std::cout << "a" << std::endl;
					currstring.push_back(line[count]);
					//std::cout << "b " << currstring << " b.1" << std::endl;
				}
				if (line[count] == ',' || line[count] == '#') {
					//std::cout << "c " << currstring << " c.1" << std::endl;
					tempnode = makenode(currstring);
					//std::cout << "c.2" << std::endl;
					currstring.clear();
					//std::cout << "c.5" << std::endl;
					parenting.push_back(tempnode);
					//std::cout << "d" << std::endl;
				}
				if (line[count] == '#') {
					//std::cout << "e " << count << std::endl;
					done = true;
				}
				//std::cout << "f" << std::endl;
			}
			//std::cout << "g" << std::endl;
			for (; count < line.size(); count++) {
				//std::cout << "pushing " << count << line[count] << " onto " << currstring << std::endl;
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
			newtopic = "sentinelvalue";
			std::getline(std::cin, newtopic);
			if (newtopic.compare("sentinelvalue") == 0) {
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
