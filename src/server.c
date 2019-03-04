#include "server.h"



namespace
{
	//load config file 
	const static std::vector<sockaddr_in> server_addrs = load_server_addrs();

	//init storage dicts
	static std::map<int,int> local_dict; //empty storage dict, defualt constructor???

	//actions queus
	static std::vector<action_queue_t> action_queues;

}


int main(int argc, char *argv[])
{
	//get this server's id from command line input 
	const int this_server_id = (int)*argv[1]; //does this work

	//client address (could be another server or the client who is trying to use the mem)
	sockaddr_in client_addr, server_addr;

	//inits
	fd_set readfds; //set of file descriptors being monitored 
	int master_socket_upd, master_socket_tcp; 
	int recvfrom_bytes = 0; 
	char buffer[BUFFER_SIZE] = 0;

	//create master sockets 
	master_socket_udp = socket(AF_INET, SOCK_STREAM, 0);
	master_socket_tcp = socket(AF_INET, SOCK_DGRAM, 0);
	int nfds = master_socket_tcp > master_socket_udp ? master_socket_tcp : master_socket_udp; 

	//bind sockets to server ports 
	bind(master_socket_upd, (const struct sockaddr *)&server_addr, sizeof(server_addr));
	bind(master_socket_tcp, (const struct sockaddr *)&server_addr, sizeof(server_addr));

	//begin listening on ports 
	listen(master_socket_udp, 10);
	listen(master_socket_tcp, 10);

	while(1)
	{
		//reset readfds, and only monitor master sockets 
		FD_ZERO(&readfds); //set all fd bits to 0
		FD_SET(master_socket_tcp, &readfds); //WHY????
		FD_SET(master_socket_udp, &readfds);

		//blocking call until one or more file descriptors are ready, ADD LOOP 
		int nfds_ready = select(nfds + 1, &readfds); 

		if (nfds < 1)
		{
			//error handling
		}

		//Incoming UDP msg
		if (FD_ISSET(master_socket_udp, &readfds))
		{
			memset(&buffer, 0, sizeof(buffer)); // clear buffer
			memset(&client_addr, 0, sizeof(client_addr)) //clear client address

			//intialize client addr
			recvfrom_bytes = recvfrom(master_socket_udp, &buffer, sizeof(buffer), NULL, (struct sockaddr *)&client_addr);

			//check on recv_frombytes 
		}

		//incoming TCP connection request and msg 
		if (FD_ISSET(master_socket_tcp, &readfds))
		{
			memset(&buffer, 0, sizeof(buffer)); //clear buffer
			memset(&client_addr, 0, sizeof(client_addr)) //clear client address

			//intialize client address
			client_fd = accept(master_socket_tcp, (struct sockaddr *)&client_addr, sizeof(client_addr));
			recvfrom_bytes = recvfrom(master_socket_tcp, &buffer, sizeof(buffer), NULL, (struct sockaddr *)&client_addr); 

			//check on recv_from_bytes 
			
			//close TCP connection immediately, no need to keep open   
			close(client_fd);
		} 

		//process received message and get response messages(s)
		process_incoming_msg(&buffer, this_server_id);

		std:vector<msg_t> outgoing_msgs = get_outgoing_msgs();

		//send response messages
		for (std::vector<msg_t>::iterator it =  outgoing_msgs.begin(); it != outgoing_msgs.end(); ++it)
		{
			//send UDP message

			//send TCP message 

		}
	}
}

std::vector<sockaddr_in> load_server_addrs()
{	
	//load config file with server numbers and address and store each in vector 
}

std::vector<msg_t> process_incoming_msg(char (*msg_buffer_p)[], const int this_server_id)
{
	msg_t msg_recv = {0};
	std::vector<msg_t> response_msgs;
	memcpy(&msg_recv, msg_buffer_p, sizeof(msg_t));

	//returns do no create actions 

	switch(msg_recv.header.msg_type)
	{
		case GET:
			
			//request from client, create action_queue
			action_queue_t action_queue = create_action_queue(msg_recv.header.request_id);

			//Send GET_FORWARD to all servers
			for (int server_id = 0; server_id < server_addrs.size(); ++server_id)
			{
				action_t action = {0};

				//generate GET_FORWARD message
				//no required replies, send immediately 
				action.msg_send.header.msg_type = GET_FORWARD;
				action.msg_send.header.request_id = msg_recv.header.request_id;
				action.msg_send.header.client_addr = server_addrs[this_server_id];
				action.msg_send.header.server_addr = *server_addrs[server_id];
				action.msg_send.payload.key = msg_recv.data.key;

				action_queue.push(action)
			}

			//add GET_RETURN action to action queue
			//enact when we hear back from all outgoing GET_FORWARDS
			action_t action = {0};
			action.msg_send.header.msg_type = GET_RETURN;
			action.msg_send.header.request_id = msg_recv.header.request_id;
			action.msg_send.header.client_addr = server_addrs[this_server_id];
			action.msg_send.header.server_addr = msg_recv.header.client_addr;
			action.msg_send.payload.key = msg_recv.data.key;

			action_queue.push(action) //add GET_RETURN action to queue
			action_queues.push_back(action_queue) //add this action queue to set of all action queues 

			break;

		case GET_FORWARD:
			//send a GET_FORWARD_RETURN to sender. return key/val if it exists, otherwise return error. 


			//create action queue for client request 
			action_queue_t action_queue = create_action_queue(msg_recv.header.request_id);

			//search for key/val pair in local storage
			std::map<int,int>::iterator search_it = local_dict.find(msg_recv.data.key);

			//create action for queue (only 1 action in this case)
			action_t action = {0};
			action.msg_send.header.msg_type = GET_FORWARD_RETURN;
			action.msg_send.header.request_id = msg_recv.header.request_id;
			action.msg_send.header.client_addr = server_addrs[this_server_id];
			action.msg_send.header.server_addr = msg_recv.header.client_addr;
			action.msg_send.payload.key = msg_recv.data.key;

			//key/val pair in local dict, return key/val pair
			if (search_it != local_dict.end())
			{
				msg_send.payload.error = 0;
				msg_send.payload.val = *search_it;
			}

			//no key/val pair in local dict, return error  
			else
			{
				msg_send.payload.error = 1;
				msg_send.payload.val = 0;
			}

			action_queue.push(action)
			action_queues.push_back(action_queue) //add action queue to set of all action queues 

			break;

		case GET_FORWARD_RETURN:
			//find action queue associate with a desired request_id 
			type queue_it = std::find(action_queues.begin(), action_queues.end(), msg_recv.header.request_id);
			action_t &get_return_action = queue_it->front();
			server_id = ;
			//error handling 

			//find GET_RETURN action dependent on this reply msg, mark reply as satisfied, and adjust action msg
			//if GET_FORWARD_RETURN has returned desired val sore in GET_RETURN payload 
			if (get_return_action.msg.header.type == GET_RETURN)
			{
				if (!msg_recv.payload.error)
				{
					get_return_action.msg.payload.error = 1;
					get_return_action.msg.payload.val = msg_recv.payload.val;
				}

				get_return_action.required_replies[server_id] = 0; //reply from this server is no longer required 
			}

			break;

		case GET_RETURN:
			//find action queue with desired request_id 
			request_it = std::find(action_queues.begin(), outstanding_requests.end(), msg_recv.header.request_id)

			//if GET_RETURN and next message PUT_RETURN
				//if GET_RETURN has error 
					//mark PUT_FORWARD as ready to send
				//if GET_RETURN no error
					//delete PUT_FORWARD and mark PUT_RETURN as error 

			//elif REMOVE 

		case PUT:

			action_queue_t action_queue = create_action_queue();

			//send GET to self 
			action_t action = {0};
			action.msg_send.header.msg_type = GET;
			action.msg_send.header.request_id = msg_recv.header.request_id;
			action.msg_send.header.client_addr = server_addrs[this_server_id];
			action.msg_send.header.server_addr = server_addrs[this_server_id];
			action.msg_send.payload.key = msg_recv.data.key; 
			action.msg_send.payload.val = msg_recv.data.val;
			action_queue.push(action);

			//randomly choose server to store key/val pair in and generate PUT_FORWARD msg
			//do not send put_forward untill GET_RETURN has been recv with msg.error = 1 (ie key does not exist)
			//else delete PUT_FORWARD msg 
			random_server_id =;
			action_t action = {0};
			action.msg_send.header.msg_type = PUT_FORWARD;
			action.msg_send.header.request_id = msg_recv.header.request_id;
			action.msg_send.header.client_addr = server_addrs[this_server_id];
			action.msg_send.header.server_addr = server_addrs[random_server_id];
			action.init_required_replies(server_addrs[this_server_id], true); //require a GET_RETURN response from self  
			action.msg_send.payload.key = msg_recv.data.key;
			action.msg_send.payload.val = msg_recv.data.val;
			action_queue.push(action);

			//add PUT_RETURN into queue (will return success or fail to client)
			action_t action = {0};
			action.msg_send.header.msg_type = PUT_RETURN;
			action.msg_send.header.request_id = msg_recv.header.request_id;
			action.msg_send.header.client_addr = server_addrs[this_server_id];
			action.msg_send.header.server_addr = msg_recv.header.client_addr;
			action.msg_send.payload.key = msg_recv.data.key; 
			action.msg_send.payload.val = msg_recv.data.val;
			action_queue.push(action);

			action_queues.push_back(action_queue)

			break;

		case PUT_FORWARD:
			//double check to see if key exists already

			//store key/val pair locally 
			local_dict[msg_recv.data.key] = msg_recv.data.val;

			break;

		case REMOVE:
			//remove key,val pair
			//very similar to PUT workflow
			break;

		case REMOVE_FORWARD:
			break;

		case REMOVE_RETURN:
			break;
	}
}


void get_outgoing_msgs(){
	//go through each action queue and pop action msgs that are ready to be sent 
	//delete actions that have the error flag enabled 
	

	//delete empty action queues 


}