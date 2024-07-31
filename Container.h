#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "TreeNode.h"

#include <fstream>
#include <sstream>

class Container {
public:
	// constructor and deconstructor
	Container() { video_owner = ""; };
	~Container();

	// main use method
	std::string executeCommand(std::stringstream& result, std::string command);

	// method for parsing json data, automatically appends all parsed info
	void parseJSON(std::string filename);
//private:

	// helper methosd for executeCommand
	bool likeComment(std::string comment_id);
	bool replyComment(std::string parent_comment_id, std::string comment_id, std::string author, std::string comment);
	bool displayComment(std::stringstream& stream, std::string comment_id);
	bool deleteComment(std::string comment_id);

	// helper method, appends given comment under parent
	void appendComment(Comment data);

	// search function. utilizes recursive search function in all treenodes
	TreeNode* search(std::string id);

	// private vars
	std::string video_owner;
	std::vector<TreeNode*> children;
};

#endif