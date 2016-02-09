#Protocol:
During the first use, or when the chiplet changes environments a “NETWORK” call is expected to initialize the wifi parameters.

Every time a serial connection is established we will expect a “INIT” call after which any subsequent calls can be made, until the connection is closed. 

In the following examples we use $ to represent variables. For example $Host represents your firebase host.

##Response Format Byte
All responses will be prefixed with one of the following bytes signifying the response type.
```
  + If response is ok and a raw string value.
  * If response is ok and a raw string value prefixed by count of bytes in response then new line.
  # If response is ok and a integer value.
  . If response is ok and a float value.
  $ If response is ok and a boolean value.
  & If response is ok and json formatted and prefixed by count of bytes in response then new line.
  - If response is an error
```
##NETWORK
Only needs to be called when the chiplet is in a new environment and needs to connect to a new network. This setting will be stored by the chiplet and persisted through power cycles.
###Usage
	NETWORK $SSID
	NETWORK $SSID $PASSWORD
###Response
	CONNECTED - When connected to new network
	UNABLE_TO_CONNECT - When unable to connect
###Examples
	>> NETWORK home-guest
	<< +CONNECTED
	>> NETWORK home-private MySecretPassword
	<< +CONNECTED
	>> NETWORK home-guest
	<< -UNABLE_TO_CONNECT

##INIT
Must be called after creating a Serial connection, it can take either just a host for accessing public variables or you may also provide a secret for accessing protected variables in the database.
###Usage
	INIT $Host
	INIT $Host $Secret
###Response
	OK - Accepted initialization parameters
###Examples
	>> INIT https://samplechat.firebaseio-demo.com
	<< +OK
	>> INIT https://samplechat.firebaseio-demo.com nnz...sdf
	<< +OK
##GET
Fetches the value at $Path and returns it on the serial line. If $PATH points to a leaf node you will get the raw value back, if it points to an internal node you will get a JSON string with all children.
###Usage
	GET $PATH
###Response
	$DATA_AT_PATH
	$JSON_DATA_BYTE_COUNT \n\r $JSON_DATA
###Examples
	>>GET /user/aturing/first
	<<+Alan
	>>GET /user/aturing
	<<&39
	<<{ "first" : "Alan", "last" : "Turing" }
	
##GET{+,*,#,.,?,$}
Same as GET but will either return the value in the format specified (by the format byte) or return an error.
###Usage
	GET+ $PATH
	GET* $PATH
	GET# $PATH
	GET. $PATH
	GET? $PATH
	GET$ $PATH
###Response
	$FORMATED_RESPONSE
###Examples
	>>GET? /user/aturing/was_human
	<<?true
	>>GET? /user/aturing/first
	<<-ERROR_INCORRECT_FORMAT
##Set
##Push
##Remove
##Stream
