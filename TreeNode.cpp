#include "TreeNode.h"

/*
 * TreeNode() full saturated constructor. due to data
 * being a custom class, it needs to be instansiated
 * instead of being created with default con. and then
 * assigned. sets depth to 0
 */
TreeNode::TreeNode(TreeNode* parent, Comment c) : data(c) {

	// all assignment + initialization of data
	this->parent = parent;
	depth = 0;
}

/*
 * Treenode deconstructor, kills all children
 * then itself. perfectly releases all memory
 */
TreeNode::~TreeNode() {

	// Delete child nodes
	for (TreeNode* child : children)
			delete child;
}

/*
 * reply() replies to the current tree node with the provided
 * information. it uses itself to create information that
 * may be unknown
 */
void TreeNode::reply(std::string comment_id, std::string author, std::string comment, std::string videoowner) {

	// iterate reply count, create a comment object and append it
	data.setReplyCount(data.getReplyCount()+1);
	Comment temp(data.getVideoID(), author, comment_id, 0, 0, true, data.getCommentID(),
		"0 seconds ago", data.getCrawledDate(), author==videoowner, comment);
	append(temp);
}

/*
 * append() appends a given comment to the tree
 * using dynamic memory
 */
void TreeNode::append(Comment child) {

	// create a leaf with dynamic memory and push it to the list
	TreeNode* leaf = new TreeNode(this, child);
	children.push_back(leaf);
}

/*
 * search() takes in a comment id and returns
 * a pointer to a TreeNode which has data that
 * maTches the comment id provided. this function
 * is recursive and calls itself
 */
TreeNode* TreeNode::search(std::string id) {

	// if current object matches, return
	if (data.getCommentID() == id)
		return this;

	// else rceursively run search() on all kids
	TreeNode* searchresult;
	for(TreeNode* i : children) {
		searchresult = i->search(id);
		if (searchresult != NULL)
			return searchresult;
	}

	// no kids match, no match exists in current tree. return null
	return NULL;
}

/*
 * operator<< overload. utilizes depth and iterates
 * depth by one every layer deeper. prints whole
 * chain of comments
 */
std::ostream& operator<<(std::ostream& out, const TreeNode& t) {

	// print self
	out << t.getData();

	// print all nested comments and adjust depth accordingly
	for (TreeNode* i : t.getChildren()) {
		i->setDepth(t.getDepth()+1);
		out << (*i);
	}
	return out;
}