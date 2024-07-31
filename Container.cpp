#include "Container.h"

/*
 * Container deconstructor, kills all children,
 * then self to release all memory
 */
Container::~Container() {

	// Delete child nodes
	for (TreeNode* child : children)
		delete child;
}

/*
 * executeCommand() takes a stringstream and a command as input
 * it parses the command to execute the correct method and the
 * output will be streamed into the stringstream.
 */
std::string Container::executeCommand(std::stringstream& result, std::string command) {

	// look for keywords to decide which command is being used
	if(command.find("reply_to_comment") != std::string::npos) {

		// arguments are all data that isnt the command keyword
		std::string arguments = command.substr(17);

		// seperate information by spaces
		size_t pos = 0;
		std::string parent_id = arguments.substr(pos, arguments.find(' ', pos)-pos);

		pos = arguments.find(' ', pos)+1;
		std::string comment_id = arguments.substr(pos, arguments.find(' ', pos)-pos);

		pos = arguments.find(' ', pos)+1;
		std::string author = arguments.substr(pos, arguments.find(' ', pos)-pos);

		// seperate by " , as comment data is enclosed in "
		pos = arguments.find('\"', pos)+1;
		std::string comment = arguments.substr(pos, arguments.find('\"', pos)-pos);

		replyComment(parent_id, comment_id, author, comment);

	} else if(command.find("reply_to_video") != std::string::npos) {

		// arguments are all data that isnt the command keyword
		std::string arguments = command.substr(15);

		// nearly same process as reply_to_comment
		size_t pos = 0;
		std::string comment_id = arguments.substr(pos, arguments.find(' ', pos)-pos);

		pos = arguments.find(' ', pos)+1;
		std::string author = arguments.substr(pos, arguments.find(' ', pos)-pos);

		pos = arguments.find('\"', pos)+1;
		std::string comment = arguments.substr(pos, arguments.find('\"', pos)-pos);

		// append to video instead of another commen
		std::string videoid = children[0]->getData().getVideoID();
		Comment temp(videoid, author, comment_id, 0, 0, false, "",
			"0 seconds ago", "null", author==video_owner, comment);;
		appendComment(temp);

	// parse through rest of command
	} else if(command.find("like_comment") != std::string::npos) {
		likeComment(command.substr(13));
	} else if(command.find("delete_comment") != std::string::npos) {
		deleteComment(command.substr(15));
	} else if(command.find("display_comment") != std::string::npos) {
		displayComment(result, command.substr(16));
	} else {

		// guard for bad input file
		std::cerr << "Command \"" << command << "\" not recognized." << std::endl;
		exit(1);
	}

	return result.str();
}

/*
 * likeComment() takes a comment id as input and
 * iterates the likecount of the comment in the
 * current container's tree that matches the given id
 * by 1
 */
bool Container::likeComment(std::string comment_id) {

	// search for comment matching provided id and utilize like method on it
	TreeNode* searchresult = search(comment_id);
	if(searchresult!=NULL) {
		searchresult->like();
		return true;
	}
	return false;
}

/*
 * replyComment() takes a parent comment id, comment id, author,
 * and some data as a string and replies to the parent comment and
 * iterates the parents comments replies by 1
 */
bool Container::replyComment(std::string parent_comment_id, std::string comment_id, std::string author, std::string comment) {

	// search for comment matching provided id and utilize reply method on it
	TreeNode* searchresult = search(parent_comment_id);
	if(searchresult!=NULL) {
		searchresult->reply(comment_id, author, comment, video_owner);
		return true;
	}
	return false;
}

/*
 * displayComment() takes in a stringstream and a comment id
 * and streams all data relevant to the provided
 * comment id into the stringstream
 */
bool Container::displayComment(std::stringstream& stream, std::string comment_id) {

	// search for comment matching provided id and stream it into input
	TreeNode* searchresult = search(comment_id);
	if(searchresult!=NULL) {
		stream << *searchresult;
		return true;
	}
	return false;
}

/*
 * deleteComment() takes a comment id as input and removes
 * that comment from its parents' kids list and also
 * calls the delete keyword to release all memory
 */
bool Container::deleteComment(std::string comment_id) {

	// search for comment matching provided id 
	TreeNode* searchresult = search(comment_id);
	if(searchresult!=NULL) {
		// delete the thing

		// case of no parenst/parents
		TreeNode* parent = searchresult->getParent();
		if (parent != NULL) {
			// has parent, find parent and delete from parents' list
			for (std::vector<TreeNode*>::iterator itr = parent->editChildren()->begin(); itr != parent->editChildren()->end(); itr++) {
				if ((*itr) == searchresult) {
					parent->editChildren()->erase(itr);
					parent->setReplyCount(parent->getData().getReplyCount()-1);
					break;
				}
			}
		} else {
			// no parents, delete directly from container class
			for (std::vector<TreeNode*>::iterator itr = children.begin(); itr != children.end(); itr++) {
				if ((*itr) == searchresult) {
					children.erase(itr);
					break;
				}
			}
		}
		delete searchresult;
		return true;
	}
	return false;
}

/*
 * parseJSON() takes a filename as input and parses
 * the provided file and appends all comments
 * that were parsed from the information
 */
void Container::parseJSON(std::string filename) {

	// assume inputFile is a std::string, containing the file name of the input file.
	std::ifstream jsonFile(filename);
	if (!jsonFile.is_open()) {
			std::cerr << "Failed to open the JSON file." << std::endl;
			exit(1);
	}

	std::string json_content;
	std::string line;
	while (std::getline(jsonFile, line)) {
			json_content += line;
	}
	// don't need this json file anymore, as the content is read into json_content.
	jsonFile.close();

	std::string cur_data;

	// relevant data for creatiung a comment object
	std::string video_id;
	std::string author;
	std::string comment_id;
	unsigned int like_count;
	unsigned int reply_count;
	bool is_reply;
	std::string parent_comment_id;
	std::string published_date;
	std::string crawled_date;
	bool is_video_owner;
	std::string comment;

	// Find the starting position of each comment
	size_t startPos = 0;
	size_t endPos;

	while ((endPos = json_content.find("}", startPos)) != std::string::npos) {

		// go line by line in json_content
		std::string commentStr = json_content.substr(startPos, endPos - startPos + 1);
		startPos = endPos + 1;

		size_t pos = 0;

		// parse for all keywords individually
		pos = commentStr.find("{\"video_id\":", pos) + 14;
		video_id = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"author\":", pos) + 11;
		author = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"comment_id\":", pos)+15;
		comment_id = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"like_count\":", pos) + 14;
		like_count = std::stoi(commentStr.substr(pos, commentStr.find(",", pos)-pos));

		pos = commentStr.find("\"reply_count\":", pos)+15;
		reply_count = std::stoi(commentStr.substr(pos, commentStr.find(",", pos)-pos));

		pos = commentStr.find("\"is_reply\":", pos)+12;
		if (commentStr.substr(pos, commentStr.find(",", pos)-pos) == "true")
			is_reply = true;
		else
			is_reply = false;

		pos = commentStr.find("\"parent_comment_id\":", pos)+22;
		parent_comment_id = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"published_date\":", pos)+19;
		published_date = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"crawled_date\":", pos)+17;
		crawled_date = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		pos = commentStr.find("\"is_video_owner\":", pos)+18;
		if (commentStr.substr(pos, commentStr.find(",", pos)-pos) == "true") {
			is_video_owner = true;
			video_owner = author;
		} else
			is_video_owner = false;

		pos = commentStr.find("\"comment\":", pos)+12;
		comment = commentStr.substr(pos, commentStr.find("\"", pos)-pos);

		// after all data is found, create comment and append
		Comment cmt(video_id, author, comment_id, like_count,
			reply_count, is_reply, parent_comment_id, published_date,
			crawled_date, is_video_owner, comment);
		appendComment(cmt);
	}
}

/*
 * appendComment() takes a comment's data and appends the provided
 * comment to a treenode which has the parent comment. if the provided
 * comment does not have a parent, append it to the video.
 */
void Container::appendComment(Comment data) {
	if (data.isReply()) {
		// what to do if has parent
		search(data.getParentCommentID())->append(data);
	} else {
		// if no parent, append to self with no parent ocmment listed
		children.push_back(new TreeNode(NULL, data));
	}
}

/*
 * search() takes in a comment id as input and returns a poitner
 * to the treenode which contains the comment with the matching id
 * this function utilizes the recursive search() function implemented
 * in the TreeNode class
 */
TreeNode* Container::search(std::string id) {

	TreeNode* searchresult;
	for(TreeNode* i : children) {

		searchresult = i->search(id);
		if (searchresult != NULL)
			return searchresult;
	}
	return NULL;
}