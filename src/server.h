#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "int_dict.h"
#include "msg.h"
#include <vector>
#include "<assert.h>"
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 128


typedef struct action_t
{
	/*
	When a client initiates a request, a unique request ID is generated. 

	An action is outgoing message paired with requirements (ie server replies) that must be satisfied 
	before sending the action msg. If we recieve all of the required replies containing the request ID of interest, 
	then action.msg will be sent, and the action will then be deleted.  
	*/


	//required server replies before action.msg is sent 
	std::map<int,bool> required_replies; //key=server_id, val=is this server's response outstanding?  

	//when outstanding_responses are satisifed send a message in response to the request 
	msg_t msg; //make multiple request_response_msgs


	//default constructor 
	void action_t()
	{
		error = 0;
		msg = msg();
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
	//Queue of linked actions. When action 1 in the queue is completed then
	//action 2 is unblocked, allowing us to perform this next action. This allows
	//us to chain time dependent actions together, where each action is depenent on replies
	//from other servers (ie allows chaining time depedent messages together)
	
	int request_id; //unique request_id linked to all actions in a queue
	std::queue<action_t> queue;

	void action_queue_t(int request_id_arg)
	{
		request_id = request_id_arg;
	}

	bool operator == (const action_queue_t a, const action_queue_t b)
	{
		//does queu_1 have the same request_id as queue_2?
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