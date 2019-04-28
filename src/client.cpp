//TO DO add includes


int main(){
	
	int client_choice; 
	std::vector<sockaddr> server_addrs = load_server_addrs()


	//TO DO: add setup dialogue, connect to only a single server

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

		//TO DO: add error handling for bad user entries 

		switch (int)client_choice {
			case 1:
				//Send GET request for a specified key
				msg.header.msg_type;
				msg.header.requet_id = generate_unique_request_id();
				msg.header.client_addr = load_client_addr();

				printf("\nEnter the key (an int) for the stored value of interest: ")
				cin >> client_choice;
				msg.payload.key = (int)client_choice; 

				printf("\nEnter the server number you would like to send the GET request to: ");
				cin >> server_number;
				msg.header.servaddr = server_addrs[(int)client_choice];

			case 2:
				//Send POST request to server
				break;
			case 3:
				//TO DO: add REMOVE support
				break;
			default:
				printf("Bad Choice Entered")
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
	//TO DO: add random int generator
}

int send_UDP_msg(const msg_t msg){
	//Send UDP message to server address specified in msg header
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

int send_TCP_msg(const msg_t msg){
	//send TCP message to server address specified in msg header 
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
    	printf("[ERROR] Failure sending TCP message");
    	success = 0;
    }

    //Close TCP connection 
	close();

	return success; 
}
