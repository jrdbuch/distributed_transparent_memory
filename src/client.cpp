
int main(){
	
	int client_choice; 
	std::vector<sockaddr> server_addrs = load_server_addrs()


	while (1)
	{
		printf("What would you like to do? \n 
				1.) Get stored value \n
				2.) Enter new stored value \n
				3.) Remove stored value \n
				Enter Choice: ");

		cin >> client_choice;

		//create request message 
		msg_t msg;

		switch client_choice{
			case 1:

				msg.header.msg_type;
				msg.header.requet_id = generate_unique_request_id();
				msg.header.client_addr = load_client_addr();

				printf("\nEnter the key (an int) for the stored value of interest: ")
				cin >> client_choice;
				msg.payload.key = (int)client_choice; 

				printf("\nEnter the server number you would like to send the GET request to: ")
				cin >> server_number;
				msg.header.servaddr = server_addrs[(int)client_choice];

			case 2:
				break;
			case 3:
				break;
			default:
				break; 
		}
	} 

}

sockaddr_in load_client_addr()
{	
	//load config file with client address
}

int generate_unique_request_id()
{
	//
}

int send_UDP_msg(const msg_t msg){
	int sockfd;
	int success = 1; 

	//create UDP client socket 
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        printf("[ERROR] Failure Creating UDP Client Socket");
        success = 0;
    }

    //send msg
    if (sendto(sockfd, (const msg_t *) &msg, sizeof(msg_t), 0,
    	(const struct sockaddr *) &msg.header.servaddr, sizeof(servaddr)))
    {
    	printf("[ERROR] Failure sending UDP message");
    	success = 0;
    }

    return success; 
}

void send_TCP_msg(const msg_t msg, const int master_socket_udp){
	int sockfd; 
	int success = 1; 

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("[ERROR] Failure Creating TCP Client Socket");
        success = 0;
	} 

	//Connect client socket to server TCP socket
	if (connect(sockfd, (const struct sockaddr *) &msg.header.servaddr, sizeof(msg.header.servaddr))) { 
        printf("[ERROR] Failed to Connect to TCP Server\n"); 
        success = 0; 
    } 
    else{
        printf("[INFO] Connected to TCP Server\n"); 
	}

	//Send Message to Server 
	if (sendto(sockfd, (const msg_t *) &msg, sizeof(msg_t), 0,
    	(const struct sockaddr *) &msg.header.servaddr, sizeof(servaddr)))
    {
    	printf("[ERROR] Failure sending UDP message");
    	success = 0;
    }

    //Close TCP connection 
	close();

	return success; 
}