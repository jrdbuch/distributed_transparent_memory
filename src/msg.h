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
	REMOVE_RETURN
} msg_type_t;

typedef struct //think of a way to make this more elegant instead of throwing all fields in one struct
{
	msg_type_t msg_type;
	int request_id; 
	sockaddr_in client_addr;
	sockaddr_in server_addr;
} msg_header_t;

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

	void msg_t()
	{
		//default constructor 
	}
} msg_t;