#include <sys/types.h>
#include <sys/socket.h>
#include <map>
#include "int_dict.h"
#include "msg.h"
#include <vector>

#define BUFFER_SIZE 128


typedef struct 
{
	int request_id; //unique request id 
	msg_type_t request_type; 

	//state info 
	std::map<int,bool> outstanding_responses; //key=server_id, val=is this server's response outstanding? 
	bool error;

	//when outstanding_responses are satisifed send a message in response to the request 
	msg_t request_response_msg;

	//overload the equality operator so we can compare msg_headers with request_ts and request_ts with request_ts

	//add constructor so a msg_header_t will construct an instance 
} request_t;