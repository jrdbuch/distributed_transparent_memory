#include "server.h"



namespace
{
	//load config file 
	std::vector<sockaddr_in> server_addrs = load_server_addrs();

	//init storage dicts
	std::map<int,int> local_dict; //empty storage dict, defualt constructor???

	//list of expected responses (ie the msg headers we expect to see for a response)
	std::vector<request_t> outstanding_requests; 

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

		//blocking call until one of more file descriptors are ready, ADD LOOP 
		int nfds_ready = select(nfds + 1, &readfds); 

		//loop through available fd

		if (nfds)
		{
			//error handling
		}

		//Incoming UDP msg
		else if (FD_ISSET(master_socket_udp, &readfds))
		{
			memset(&buffer, 0, sizeof(buffer)); // clear buffer
			memset(&client_addr, 0, sizeof(client_addr)) //clear client address

			//intialize client addr
			recvfrom_bytes = recvfrom(master_socket_udp, &buffer, sizeof(buffer), NULL, (struct sockaddr *)&client_addr);

			//check on recv_frombytes 

			//
		}

		//incoming TCP connection request and msg 
		else if (FD_ISSET(master_socket_tcp, &readfds))
		{
			memset(&buffer, 0, sizeof(buffer)); //clear buffer
			memset(&client_addr, 0, sizeof(client_addr)) //clear client address

			//intialize client address
			client_fd = accept(master_socket_tcp, (struct sockaddr *)&client_addr, sizeof(client_addr));
			recvfrom_bytes = recvfrom(master_socket_tcp, &buffer, sizeof(buffer), NULL, (struct sockaddr *)&client_addr); 

			//check on recv_from_bytes 
			
			//close TCP connection immediately, no need to keep open  
			//I think from a clients perspective we might want to keep a connection open for a get_request 
			close(client_fd);
		}

		//maybe keep connections open for those we exepct a response from ? 

		//process received message and get response messages(s)
		std::vector<msg_t> response_msgs = process_incoming_msg(&buffer, this_server_id);

		//send response messages
		for (std::vector<msg_t>::iterator it =  response_msgs.begin(); it != response_msgs.end(); ++it)
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

std::vector<msg_t> response_msgs process_incoming_msg(char (*msg_buffer_p)[], const int this_server_id)
{
	msg_t msg_recv = {0};
	std::vector<msg_t> response_msgs;
	memcpy(&msg_recv, msg_buffer_p, sizeof(msg_t));

	switch(msg_recv.header.msg_type)
	{
		case GET:
			
			
			std::map<int,int>::iterator search_it = local_dict.find(msg_recv.data.key); 

			// key/val pair stored locally 
			if (search_it != local_dict.end())
			{
				//key,val pair exists in local dict, return this data to client
				msg_t msg_send = {0};
				msg_send.header.msg_type = GET_RETURN;
				msg_send.header.init_client_addr = msg_recv.header.init_client_addr;
				msg_send.header.client_addr = server_addrs[this_server_id];
				msg_send.header.server_addr = *client_addr;
				msg_send.payload.key = msg_recv.data.key; 
				msg_send.payload.val = *search_it;
				response_msgs.push_back(msg_send);
			}

			// key/val pair does not exist locally 
			else
			{
				//Data does not exist in local dict, send GET_FORWARD request to all other servers 
				//and create an outstanding request entry. 
				generate_request_obj();

				//key,val pair does not exist in local dict, send GET_FORWARD to all other servers 
				for (int server_id = 0; server_id < server_addrs.size(); ++server_id) //change this to loop through server_ids
				{
					if (server_id != this_server_id) //exclude current server
					{
						//generate GET_FORWARD message
						msg_t msg_send = {0};
						msg_send.header.msg_type = GET_FORWARD;
						msg_send.header.init_client_addr = msg_recv.header.init_client_addr;
						msg_send.header.client_addr = server_addrs[this_server_id];
						msg_send.header.server_addr = *server_it;
						msg_send.payload.key = msg_recv.data.key;
						msg_send.payload.val = *search_it;
						response_msgs.push_back(msg_send);
					}
				}
			}


			break;

		case GET_FORWARD:
			//send a GET_FORWARD_RETURN to sender. return key/val if it exists, otherwise return error. 

			std::map<int,int>::iterator search_it = local_dict.find(msg_recv.data.key);

			msg_t msg_send = {0};
			msg_send.header.msg_type = GET_FORWARD_RETURN;
			msg_send.header.init_client_addr = msg_recv.header.init_client_addr;
			msg_send.header.client_addr = server_addrs[this_server_id];
			msg_send.header.server_addr = msg_recv.header.client_addr;
			msg_send.payload.key = msg_recv.data.key;

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

			response_msgs.push_back(msg_send);

			break;

		case GET_FORWARD_RETURN:
			//if key_val pair in local_dict return to server who received initial GET
			//key,val,error 

			request_it = std::find(outstanding_request.begin(), outstanding_requests.end(), msg_recv.header)
			
			//there is a record of an outstanding request dependent on this GET_FORWARD_RETURN response
			if (request_it != outstanding_requests.end())
			{
				if (!msg_recv.payload.error)
				{
					//the GET_FORWARD_RETURN has produced the kev/val pair of interest from original GET request 
					//generate GET_RETURN response 
					msg_t msg_send = {0};
					msg_send.header.msg_type = GET_RETURN;
					msg_send.header.init_client_addr = msg_recv.header.init_client_addr;
					msg_send.header.client_addr = server_addrs[this_server_id];
					msg_send.header.server_addr = msg_recv.header.init_client_addr;
					msg_send.payload.key = msg_recv.data.key; 
					msg_send.payload.val = msg_recv.data.val;
					response_msgs.push_back(msg_send);

					//remove request record 
					outstanding_requests.erase(request_it);
				}
				else
				{
					//server does not have key/val pair stored locally, remove it from the list of outstanding responses
					request_t->outstanding_responses[server_id] = 0; //will fail 

					if (request_t->outstanding_responses == all 0)
					{
						//remove request_t
						//generate get_return with error 
					}
			}


			break;

		case GET_RETURN:
			//search for requests linked to a GET_RETURN
			//both REMOVE and PUT initiate a GET to search for key/val pairs 

			request_it = std::find(outstanding_requests.begin(), outstanding_requests.end(), msg_recv.header);
			response_msgs.push_back(request_it->request_response_msg);

			// if request_t == PUT
				//if GET_RETURN has error
					//randomly choose server and generate PUT_FORWARD
				//else
					//generate PUT_RETURN with error 

			//delete request


			break;

		case PUT:
			//put this key,val pair into storage
			//send a GET to self which will trigger a global search for the key_val pair
			//upon recieving response from all servers, either return error if key,val exists 
			//or store in random server with PUT_FORWARD 
			//open up two pending requests (GET request for self, PUT request for client)
 

			break;

		case PUT_FORWARD:
			//using mem table, choose which server has the most space, and forward PUT to 
			//key,val
			break;

		case PUT_RETURN:

			break; 

		case REMOVE:
			//remove key,val pair
			//key
			break;

		case REMOVE_FORWARD:
			//key
			break;
	}
}



generate_request_obj()
{
	request_t outstanding_request;
				outstanding_request.init_client_addr = msg_recv.header.init_client_addr;
				outstanding_request.init_timestamp = ; //TO DO
				outstanding_request.msg_type = GET_FORWARD_RETURN;
				outstanding_request.error = 1;
				outstanding_request.outstanding_responses = {1};
				outstanding_request.outstanding_responses[this_server_id] = 0;
				outstanding_requests.push_back(outstanding_request);
}



// readfds -> one bit for each fd, initilized to a huge array of 0s, one for eveyr possible fd? 
// set fd to 1 if we want to monitor on input 
// select then modifies in readfds place upon return, 0 if no read avaiable, 1 if read avaiable 


//why don't we add 