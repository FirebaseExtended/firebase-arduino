#Protocol:
During the first use, or when the chiplet changes environments a “NETWORK” call is expected to initialize the wifi parameters.

Every time a serial connection is established we will expect a “BEGIN” call after which any subsequent calls can be made, until the connection is closed. 

In the following examples we use %% to represent variables. For example %Host% represents your firebase host.

##Response Format Byte
All responses will be prefixed with one of the following bytes signifying the response type.
```
  + If response is ok and a raw string value.
  $ If response is ok, raw string value will be json formatted and prefixed by the byte count and a new line.
  : If response is ok and a number, this could be float or int.
  ? If response is ok and a boolean value.
  - If response is an error
```
##NETWORK
Only needs to be called when the chiplet is in a new environment and needs to connect to a new network. This setting will be stored by the chiplet and persisted through power cycles.
###Usage
	NETWORK %SSID%
	NETWORK %SSID% %PASSWORD%
###Response
	+CONNECTED - When connected to new network
	-UNABLE_TO_CONNECT - When unable to connect
###Examples
	>> NETWORK home-guest
	<< +CONNECTED
	>> NETWORK home-private MySecretPassword
	<< +CONNECTED
	>> NETWORK home-guest
	<< -UNABLE_TO_CONNECT

##BEGIN
Must be called after creating a Serial connection, it can take either just a host for accessing public variables or you may also provide a secret for accessing protected variables in the database.
###Usage
	BEGIN %Host%
	BEGIN %Host% %Secret%
###Response
	+OK - Accepted initialization parameters
###Examples
	>> BEGIN https://samplechat.firebaseio-demo.com
	<< +OK
	>> BEGIN https://samplechat.firebaseio-demo.com nnz...sdf
	<< +OK
##GET
Fetches the value at %PATH% and returns it on the serial line. If %PATH% points to a leaf node you will get the raw value back, if it points to an internal node you will get a JSON string with all children.
###Usage
	GET %PATH%
###Response
	%DATA_AT_PATH%
	$%JSON_DATA_BYTE_COUNT% \r\n %JSON_DATA
###Examples
	>>GET /user/aturing/first
	<<+Alan
	>>GET /user/aturing
	<<$39
	<<{ "first" : "Alan", "last" : "Turing" }
	
##GET{+,*,#,.,?,$}
Same as GET but will either return the value in the format specified (by the format byte) or return an error.
###Usage
	GET+ %PATH%
	GET: %PATH%
	GET? %PATH%
	GET$ %PATH%
###Response
	%FORMATED_RESPONSE%
###Examples
	>>GET? /user/aturing/was_human
	<<?true
	>>GET? /user/aturing/first
	<<-ERROR_INCORRECT_FORMAT
##SET
Store the data provided at the path provided. This method should be used for simple strings and will assume the first newline is the end of the data.
###Usage
	SET %PATH% %DATA%
###Response
	+OK
	-FAIL
###Examples
	>>SET /user/aturing/first Alan
	<<+OK
##SET$
Similar to SET above but used to write multiline strings or raw binary data. Data format is similar to the batch string ($) return type, we specify the $DATA_BYTE_COUNT on the same line as SET$ then a newline and all data. However which the batch string ($) return type returns data json escaped and quoted you may provide raw data and we will handle the escaping.

Receiver will wait until a timeout for client to send %DATA_BYTE_COUNT% worth of data before becoming responsive again.
###Usage
	SET$ %PATH%
	%DATA_BYTE_COUNT%
	%DATA%
###Response
	+OK
	-FAIL
	-FAIL_TIMEOUT
###Examples
	>>SET /user/aturing/address 
	>>24
	>>78 High Street,
	>>Hampton 
	<<+OK
	
##SET{+,*,#,.,?}
Same as SET but will force the value to be stored in the given type or return an error if we cannot parse it as that type.
###Usage
	SET+ %PATH% %VALUE%
	SET: %PATH% %VALUE%
	SET? %PATH% %VALUE%
###Response
	+OK
	-INCORRECT_TYPE
###Examples
	>>SET? /user/aturing/was_human true
	<<+OK
	>>SET? /user/aturing/was_human He was not a computer.
	<<-INCORRECT_TYPE

##REMOVE
Deletes the value located at the path provided.
###Usage
	REMOVE %PATH%
###Response
	+OK
	-FAIL
###Examples
	>>REMOVE /user/aturing
	<<+OK

##PUSH
Adds a value to the list located at the path provided and returns the key at which the new object is located.
###Usage
	PUSH %PATH% %DATA%
###Response
	%KEY%
###Examples
	>>PUSH /user/aturing/login_timestamps 1455052043
	<<+-K94eLnB0rAAvfkh_WC2

##PUSH$
Similar to PUSH but used to write multiline strings or raw binary data. Data format is similar to the batch string ($) return type, we specify the $DATA_BYTE_COUNT on the same line as SET$ then a newline and all data. However you are not required to json escape all of your data, that will be handled for you.

Receiver will wait until a timeout for client to send $DATA_BYTE_COUNT worth of data before becoming responsive again.
###Usage
	PUSH$ %PATH% %DATA_BYTE_COUNT%
	%DATA%
###Response
	%KEY%
###Examples
	>>PUSH$ /user/aturing/quotes 91
	>>We can only see a short distance ahead,
	>>but we can see plenty there that needs to be done.
	<<+-K94eLnB0rAAvfkh_WC3

##BEGIN_STREAM
Used to register to receive a stream of events that occur to the object at the provided path.

After registering you will start receiving events on the response line. They will be formatted as one line with the event type {PUT,PATCH,etc..} followed by the sub_path that changed and the other line with the data associated with that event type. This data will be formatted similar to GET results and can have multi-line batch strings (*) or json strings (&).

The event stream will continue until you send END_STREAM.

When there is an ongoing event stream no other commands can be processed until you call END_STREAM as the event stream owns the return line. 
###Usage
	BEGIN_STREAM %PATH%
###Response
	%EVENT_NAME% %SUB_PATH%
	%DATA%
	+OK
###Examples
	>>BEGIN_STREAM /user/aturing
	<<+PUT /last_login
	<<:1455052043
	<<+PUT /address
	<<$24
	<<"78 High Street,\r\nHampton"
	
##END_STREAM
Used to stop listening to events at a given path. This must be the same path provided to a previous BEGIN_STREAM call.

###Usage
	END_STREAM %PATH%
###Response
	+OK
	-NOT_STREAMING_PATH
###Examples
	>>END_STREAM /user/aturing
	<<-NOT_STREAMING_PATH
	>>BEGIN_STREAM /user/aturing
	>>END_STREAM /user/aturing
	<<+OK
