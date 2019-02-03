#include "server.h"

/*
readfds is a sequence of bits, place corresponds to fd int, 0 if inactive, 1 if active 
	need a seperate structure to store client socket fd that have been added

*/



int main(int argc, char *argv[])
{
	//get this server's id
	int server_id = (int)*argv[1]; //does this work

	//load addresses of all servers from config file
	std::vector<sockaddr_in> server_addrs = load_server_addrs();

	//client address (could be another server or the client who is trying to use the mem)
	sockaddr_in client_addr; 

	//inits
	fd_set readfds; //set of file descriptors being monitored 
	int master_socket_upd, master_socket_tcp; 
	int recvfrom_bytes = 0; 
	char buffer[BUFFER_SIZE];
	//need mem table for all servers (how much each server is using)

	//init storage dicts
	std::map<int,int> server_usage = init_server_usage();
	std::map<int,int> local_dict; //empty storage dict, defualt constructor???

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
		FD_ZERO(&readfds);
		FD_SET(master_socket_tcp, &readfds); //WHY????
		FD_SET(master_socket_udp, &readfds);

		//blocking call until one of more file descriptors are ready 
		int nfds_ready = select(nfds + 1, &readfds); 

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
		}

		//incoming TCP connection request and msg 
		else if (FD_ISSET(master_socket_tcp, &readfds))
		{
			memset(&buffer, 0, sizeof(buffer)); //clear buffer
			memset(&client_addr, 0, sizeof(client_addr)) //clear client address

			//intialize client address 
			client_fd = accept(master_socket_tcp, (struct sockaddr *)&client_addr, sizeof(client_addr));
			recvfrom_bytes = recvfrom(master_socket_tcp, &buffer, sizeof(buffer), NULL, (struct sockaddr *)&client_addr); 

			//handle TCP request, generate response message for a given recieved message  
			msg_t response_msg = recv_msg(&buffer, recvfrom_bytes);
			
			//close TCP connection immediately, no need to keep open  
			close(client_fd);
		}

		//send response message


	}
}

std::vector<sockaddr_in> load_server_addrs()
{	
	//load config file with server numbers and address and store each in vector 
}

std::map<int,int> init_server_usage(std::vector<sockaddr_in> &server_addrs)
{
	//iterate through server addresses and build map (key=server_id/position in server_addrs, val=0 memory_used by each)
}


msg_t recv_msg(char (*msg_buffer_p)[BUFFER_SIZE], int recvfrom_bytes)
{
	msg_t msg; 
	memcpy(&msg_header, msg_buffer_p, sizeof(msg_t));

	switch(msg.header.msg_type)
	{
		case GET:
			//does key_val pair exist in local_dict, if yes return (key,val) to client_addr, else GET_FORWARD
			
			//if: key in local dict then return to client_addr with GET_FORWARD_RETURN
			//else: GET_FORWARD to all other servers and wait for return GET_FORWARD_RETURN from each server
				//if: a remote server has key,val return with GET_RETURN 
				//else: return error with GET_RETURN 

			//key

			std::map<int,int>::iterator search_result = local_dict.find(msg.data.key); 
			if (search_result != local_dict.end())
			{

			}
			else
			{

			}


			break;

		case GET_FORWARD:
			//if no key_val pair in local_dict, send GET_FORWARD to all other servers 

			//look in local dict for val, return key,val or error with GET_FORWARD_RETURN

			//key
			break;

		case GET_FORWARD_RETURN:
			//if key_val pair in local_dict return to server who received initial GET
			//key,val,error 
			break;

		case GET_RETURN:
			//return key_val pair to client
			//key, val, error
			break;

		case PUT:
			//put this key,val pair into storage
			//key,val
			break;

		case PUT_FORWARD:
			//using mem table, choose which server has the most space, and forward PUT to 
			//key,val
			break;

		case SYNC_STORAGE:
			//after chaning a local mem table, send this info to all other servers so they can update thier tables
			//memory_usage
			break

		case REMOVE:
			//remove key,val pair
			//key
			break;

		case REMOVE_FORWARD:
			//key
			break;
	}
}

msg_t generate_msg(msg_type_t msg_type, ser)
{

}

void send_msg(msg_t msg){

}

// readfds -> one bit for each fd, initilized to a huge array of 0s, one for eveyr possible fd? 
// set fd to 1 if we want to monitor on input 
// select then modifies in readfds place upon return, 0 if no read avaiable, 1 if read avaiable 
