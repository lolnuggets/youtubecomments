#include "Comment.h"

/*
 * Comment() fully saturated constructor. sets all releavant information
 * and additionally sets depth to 0 for use in printing
 */
Comment::Comment(std::string video_id, std::string author, std::string comment_id, unsigned int like_count,
	unsigned int reply_count, bool is_reply, std::string parent_comment_id, std::string published_date,
	std::string crawled_date, bool is_video_owner, std::string comment) {

	// main assignment
	this->video_id=video_id;
	this->author=author;
	this->comment_id=comment_id;
	this->like_count=like_count;
	this->reply_count=reply_count;
	this->is_reply=is_reply;
	this->parent_comment_id=parent_comment_id;
	this->published_date=published_date;
	this->crawled_date=crawled_date;
	this->is_video_owner=is_video_owner;
	this->comment=comment;

	depth = 0;
}

/*
 * operator<< overload to print the comment object.
 * the depth determines the amount of space in front
 * of the comment. this is to create the nested comments
 * effect
 */
std::ostream& operator<<(std::ostream& out, const Comment& c) {

	// sets the correct amount of indentation before each comment
	std::string indent = "";
	for (unsigned int i = 0; i < c.getDepth(); i++)
		indent +="    ";

	// utilizes the calculates indentation to create nested effet
	out << indent << c.getAuthor() << " " << c.getPublishedDate() << std::endl;
	out << indent << c.getComment() << std::endl;
	out << indent << "\U0001F44D " << c.getLikeCount() << std::endl;
	if (c.getReplyCount() > 1)
		out << indent << c.getReplyCount() << " replies" << std::endl;
	else if (c.getReplyCount() == 1)
		out << indent <<  c.getReplyCount() << " reply" << std::endl;
	return out;
}