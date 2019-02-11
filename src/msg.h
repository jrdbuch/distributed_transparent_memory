#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>

typedef enum {
	GET, 
	GET_FORWARD,
	GET_FORWARD_RETURN,
	GET_RETURN,
	PUT,
	PUT_FORWARD, 
	PUT_RETURN,
	REMOVE,
	REMOVE_FORWARD, 
	SYNC_STORAGE
} msg_type_t;


//msg header
typedef struct //think of a way to make this more elegant instead of throwing all fields in one struct
{
	msg_types msg_type;
	time_t init_timestamp; 
	sockaddr_in init_client_addr; // address of client that initiated original request
	sockaddr_in client_addr; //address of sending client (single hop), may not be the original client that initiates request
	sockaddr_in server_addr; //address of recieving server (single hop)
} msg_header_t;

//msg bodies for differnet messages 
typedef struct
{
	int key;
	int val;
	bool error;
} msg_payload_t; 

typedef struct 
{
	msg_header_t header;
	msg_payload_t payload;

	//add clear method

} msg_t;