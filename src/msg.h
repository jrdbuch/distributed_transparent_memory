#include <sys/types.h>
#include <sys/socket.h>

typedef enum {
	GET, 
	GET_FORWARD,
	GET_FORWARD_RETURN,
	GET_RETURN,
	PUT,
	PUT_FORWARD, 
	PUT_RETURN,
	REMOVE,
	REMOVE_FORWARD
} msg_type_t;


//msg header
typedef struct //think of a way to make this more elegant instead of throwing all fields in one struct
{
	msg_types msg_type;  
	sockaddr_in client_addr;
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
	msg_payload_t data;
} msg_t;

