#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "int_dict.h"
#include "msg.h"
#include <vector>

#define BUFFER_SIZE 128


typedef struct action_t
{
	//state info 
	std::map<int,bool> required_replies; //key=server_id, val=is this server's response outstanding?  

	//when outstanding_responses are satisifed send a message in response to the request 
	msg_t msg; //make multiple request_response_msgs

	//constructor for an action object
	void action_t(std::vector<int> server_ids, bool is_reply_required)
	{
		error = 0;
		msg = {0};
		init_required_replies(server_ids, is_reply_required)
	}

	//method to intialize vector of required replies 
	void init_required_replies(std::vector<int> server_ids, bool is_reply_required)
	{
		for (std::vector<int>::iterator it = server_ids.begin(), it != server_ids.end(), ++it)
		{
			required_replies[it*] = is_reply_required;
		}
	}

	bool all_replies_recv()
	{
		//return True if all required server replies for an action to be performed have been recieved
		//else return False
		bool all_recv = true; 

		for (std:map<int,bool>::iterator it = required_replies.begin(); it != required_replies.end() ++it)
		{
			if (!it->second)
			{
				all_recv = false;
			}
		}

		return all_recv;
	}

	bool operator == (const &action_t1, const &action_t2)
	{

	}


} action_t;

typedef struct 
{
	int request_id;
	std::queue<action_t> queue;

	bool operator == (const action_queue_t a, const action_queue_t b)
	{
		if (a.request_id == b.request_id)
		{
			return true;
		}

		else
		{
			return false; 
		}
	}

} action_queue_t;