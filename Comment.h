#ifndef COMMENT_H_
#define COMMENT_H_

#include <iostream>

class Comment {
public:

	// default constructor
	Comment(std::string video_id, std::string author, std::string comment_id, unsigned int like_count,
		unsigned int reply_count, bool is_reply, std::string parent_comment_id, std::string published_date,
		std::string crawled_date, bool is_video_owner, std::string comment);

	// safe accessors
	std::string getVideoID() const { return video_id; };
	std::string getAuthor() const { return author; };
	std::string getCommentID() const { return comment_id; };
	unsigned int getLikeCount() const { return like_count; };
	unsigned int getReplyCount() const { return reply_count; };
	bool isReply() const { return is_reply; };
	std::string getParentCommentID() const { return parent_comment_id; };
	std::string getPublishedDate() const { return published_date; };
	std::string getCrawledDate() const { return crawled_date; };
	bool isOwner() const { return is_video_owner; };
	std::string getComment() const { return comment; };
	unsigned int getDepth() const { return depth; };

	// setter methods
	void setLikeCount(unsigned int val) { like_count = val; };
	void setReplyCount(unsigned int val) { reply_count = val; };
	void setDepth(unsigned int val) { depth = val; };

	// all overloads
	friend std::ostream& operator<<(std::ostream& out, const Comment& c);
	friend bool operator==(const Comment& c1, const Comment& c2) { return c1.getCommentID() == c2.getCommentID(); };
	friend bool operator!=(const Comment& c1, const Comment& c2) { return !(c1 == c2); };
private:

	// main comment info vars
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

	// var used in operator<< overload
	unsigned int depth;
};

#endif