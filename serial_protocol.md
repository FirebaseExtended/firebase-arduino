#Protocol:
During the first use, or when the chiplet changes environments a “NETWORK” call is expected to initialize the wifi parameters.

Every time a serial connection is established we will expect a “INIT” call after which any subsequent calls can be made, until the connection is closed. 

In the following examples we use $ to represent variables. For example $Host represents your firebase host.
##NETWORK
Only needs to be called when the chiplet is in a new environment and needs to connect to a new network. This setting will be stored by the chiplet and persisted through power cycles.
###Usage
	NETWORK $SSID
	NETWORK $SSID $PASSWORD
###Response
	OK - When connected to new network
	FAIL - When unable to connect
###Examples
	>> NETWORK home-guest
	<< OK
	>> NETWORK home-private MySecretPassword
	<< OK

##INIT
Must be called after creating a Serial connection, it can take either just a host for accessing public variables or you may also provide a secret for accessing protected variables in the database.
###Usage
	INIT $Host
	INIT $Host $Secret
###Response
	OK - Accepted initialization parameters
###Examples
	>> INIT https://samplechat.firebaseio-demo.com
	<< OK
	>> INIT https://samplechat.firebaseio-demo.com nnz...sdf
	<< OK
##GET
Fetches the value, in json, at $Path and returns it on the serial line.
###Usage
	GET $PATH
###Response
	$DATA_BYTE_COUNT $DATA
###Examples
	>>GET /user/aturing
	<<39 { "first" : "Alan", "last" : "Turing" }
	
##GET_VALUE
Fetches the value stored $Path and returns it on the serial line. $PATH must point to a leaf node in the tree as this call returns one value but does so without json wrapper.
###Usage
	GET_VALUE $PATH
###Response
	$DATA_BYTE_COUNT $DATA
	ERROR_NOT_LEAF_NODE
###Examples
	>>GET_VALUE /user/aturing/first
	<<4 Alan
	>>GET_VALUE /user/aturing/last
	<<7 Turing
	>>GET_VALUE /user/aturing
	<<ERROR_NOT_LEAF_NODE
	
##Set
##Push
##Remove
##Stream
