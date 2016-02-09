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
  ? If response is ok and a boolean value.
  $ If response is ok and json formatted and prefixed by count of bytes in response then new line.
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
##SET
Store the data provided at the path provided. This method should be used for simple strings and will assume the first newline is the end of the data.
###Usage
	SET $PATH $DATA
###Response
	+OK
	-FAIL
###Examples
	>>SET /user/aturning/first Alan
	<<+OK
##SET_BULK
Similar to SET above but used to write multiline strings or raw binary data.

Receiver will wait until a timeout for client to send $DATA_BYTE_COUNT worth of data before becoming responsive again.
###Usage
	SET_BULK $PATH $DATA_BYTE_COUNT $DATA
###Response
	+OK
	-FAIL
	-FAIL_TIMEOUT
###Examples
	>>SET /user/aturning/address 24 78 High Street,
	>>Hampton 
	<<+OK

##REMOVE
Deletes the value located at the path provided.
###Usage
	REMOVE $PATH
###Response
	+OK
	-FAIL
###Examples
	>>REMOVE /user/aturning
	<<+OK

##PUSH
Adds a value to the list located at the path provided and returns the key at which the new object is located.
###Usage
	PUSH $PATH $DATA
###Response
	$KEY
###Examples
	>>PUSH /user/aturning/login_timestamps 1455052043
	<<+-K94eLnB0rAAvfkh_WC2

##PUSH_BULK
Similar to PUSH but used to write multiline strings or raw binary data.

Receiver will wait until a timeout for client to send $DATA_BYTE_COUNT worth of data before becoming responsive again.
###Usage
	PUSH_BULK $PATH $DATA_BYTE_COUNT $DATA
###Response
	$KEY
###Examples
	>>PUSH /user/aturning/quotes 91 We can only see a short distance ahead,
	>>but we can see plenty there that needs to be done.
	<<+-K94eLnB0rAAvfkh_WC3

##STREAM
Used to register to receive a stream of events that occur to the object at the provided path.

After registering you will start receiving events on the response line. They will be formatted as one line with the event type {PUT,PATCH,etc..} followed by the sub_path that changed and the other line with the data associated with that event type. This data will be formatted similar to GET results and can have multi-line batch strings (*) or json strings (&).

The event stream will continue until you send CANCEL_STREAM.
###Usage
	STREAM $PATH
	CANCEL_STREAM
###Response
	$EVENT_NAME $SUB_PATH
	$DATA
	+OK
###Examples
	>>STREAM /user/aturning
	<<+PUT /last_login
	<<#1455052043
	<<+PUT /address
	<<*24
	<<78 High Street,
	<<Hampton
	>>CANCEL_STREAM
	<<+OK
