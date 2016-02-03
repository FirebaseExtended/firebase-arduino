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
Fetches the value at $Path and returns it on the serial line. If $PATH points to a leaf node you will get the raw value back, if it points to an internal node you will get a JSON string with all children.
###Usage
	GET $PATH
###Response
	$DATA_BYTE_COUNT $DATA
###Examples
	>>GET_VALUE /user/aturing/first
	<<4 Alan
	>>GET_VALUE /user/aturing/last
	<<7 Turing
	>>GET_VALUE /user/aturing
	<<39 { "first" : "Alan", "last" : "Turing" }
	
##Set
##Push
##Remove
##Stream
