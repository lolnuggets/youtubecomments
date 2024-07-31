#ifndef TREENODE_H_
#define TREENODE_H_

#include "Comment.h"

#include <vector>

class TreeNode {
public:

	// constructor & deconstructor
	TreeNode(TreeNode* parent, Comment data);
	~TreeNode();

	// basic safe accessors
	Comment getData() const { return data; };
	std::vector<TreeNode*> getChildren() const { return children; };
	TreeNode* getParent() const { return parent; };
	unsigned int getDepth() const { return data.getDepth(); };

	// setter methods
	std::vector<TreeNode*>* editChildren() { return &children; };
	void setDepth(unsigned int val) { data.setDepth(val); };
	void setReplyCount(unsigned int val) { data.setReplyCount(val); };

	// helper method for editing data
	void like() { data.setLikeCount(data.getLikeCount()+1); };
	void reply(std::string comment_id, std::string author, std::string comment, std::string videoowner);

	// helper method for appending comment to childdren
	void append(Comment child);

	// recursive search
	TreeNode* search(std::string id);

	// operator<< overload
	friend std::ostream& operator<<(std::ostream& out, const TreeNode& t);
private:

	// main private vars
	TreeNode* parent;
	Comment data;
	std::vector<TreeNode*> children;

	unsigned int depth;
};

#endif