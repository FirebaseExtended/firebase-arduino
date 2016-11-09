1. [General Protocol](#general-protocol)
2. [Database Protocol](#database-protocol)
3. [Messaging Protocol](#messaging-protocol)

#General Protocol:
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
	
#Database Protocol

##BEGIN_DB
Called to start communicating with the Firebase database, it can take either just a host for accessing public variables or you may also provide a secret for accessing protected variables in the database.
###Usage
	BEGIN_DB %Host%
	BEGIN_DB %Host% %Secret%
###Response
	+OK - Accepted initialization parameters
###Examples
	>> BEGIN_DB https://samplechat.firebaseio-demo.com
	<< +OK
	>> BEGIN_DB https://samplechat.firebaseio-demo.com nnz...sdf
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

#Messaging Protocol

## BEGIN_MSG 
Called to start communicating with Firebase Cloud Messaging, it requires a server key. You can also optionally supply any of the arguments listed on the MSG command to make them the defaults for this session. If you set them as defaults you don't need to specify them when calling MSG.

###Usage
	BEGIN_MSG %Server_Key% %Argument% %Argument% ...
###Response
	+OK - Accepted initialization parameters
	-ERROR_MISSING_SERVER_KEY
	-ERROR_PARSING_ARGUMENTS
###Examples
	>> BEGIN_MSG AIzaSyCk4GtbBa-XMQbc5TyT5oe1KnH71M-2lAZ
	<< +OK
	>> BEGIN_MSG AIzaSyCk4GtbBa-XMQbc5TyT5oe1KnH71M-2lAZ topic=iot_updates high_priority=true time_to_live=3600
	<< +OK
	>> BEGIN_MSG 
	<< +ERROR_MISSING_SERVER_KEY
	>> BEGIN_MSG AIzaSyCk4GtbBa-XMQbc5TyT5oe1KnH71M-2lAZ topic = iot_updates
	<< +ERROR_PARSING_ARGUMENTS // Can't have spaces in arguments.

## MSG
Called to start composing a message. It is followed by a list of named arguments, all of these arguments may be listed on BEGIN_MSG as well to set them as defaults for the session. Even if they are listed on BEGIN_MSG they can be overriden when calling MSG.

Named arguments are specified by " %name%=%value% " where neither names nor values will have spaces in them.

### Arguments
#### registration_ids
This is a comma separated list of device registrations you wish to send the message to. The list can have between 1 and 100 ids in it so you can send to a single device this way too.
##### Examples
registration_ids=fQCLfBOGdh0...9k0,fQCLfBOGdh0...5j1

#### registration_ids
This is a comma separated list of device registrations you wish to send the message to. The list can have between 1 and 100 ids in it so you can send to a single device this way too.
##### Examples
registration_ids=fQCLfBOGdh0...9k0,fQCLfBOGdh0...5j1

#### topic
This is the name of a topic you wish to send the message to.
##### Examples
topic=news

#### collapse_key
Sets a collapse key on the message. When the server recieves a message with a collapse key it will discard any pending messages it has with the same key. So the client will only see the most recent one when it checks in.
##### Examples
collapse_key=LatestUpdate

#### high_priority
Can be "true" or "false". If set to true the user's device will wake up to receive the message. This is used when the message is high priority but a cost it paid in battery life for the user.

##### Examples
high_priority=true

#### delay_while_idle
Can be "true" or "false". If set to true the message will not be delivered until the user's device is active. This is used when the message can wait to be seen until the user is available.

##### Examples
delay_while_idle=false
	
#### time_to_live
The message will expire after this amount of time in seconds. The default (and max value) is four weeks. Can be between 0 and 2,419,200.

##### Examples
time_to_live=500

##Usage
	MSG %Argument% %Argument% ...
###Response
	+OK - Ready to specify rest of message.
	-ERROR_MISSING_TARGET - You didn't specify either registration_ids or topic.
	-ERROR_PARSING_ARGUMENTS - There was a syntax error in your arguments.
	-ERROR_INVALID_ARGUMENT - You specified an argument we don't support.
###Examples
	>> MSG
	<< +OK // Only works if you specified a default registration_ids or topic on BEGIN_MSG.
	>> MSG 
	<< +ERROR_MISSING_TARGET // If you didn't specify a default registration_ids or topic on BEGIN_MSG.
	>> MSG registration_ids=fQCLfBOGdh0...9k0,fQCLfBOGdh0...5j1 delay_while_idle=true
	<< +OK
	>> MSG registration_ids=fQCLfBOGdh0...9k0, fQCLfBOGdh0...5j1 delay_while_idle=true
	<< +ERROR_PARSING_ARGUMENTS // There is a space in the list of registration ids.
	>> MSG some_random_thing=true
	<< +ERROR_INVALID_ARGUMENT // some_random_thing is not a valid argument.

## NOTIFICATION
Notification to display to the user, is made up of a title and a body. The title is specified on the first line and the first line ends with a count of bytes in the body. We will then read the next %Body_Byte_Count% bytes (after the line break) and interpret them as the body. If there is no body specify 0 for the byte count. The count must be the last thing on the line, it cannot end in whitespace.
##Usage
	NOTIFICATION %Title% %Body_Byte_Count%
	%Body%
###Response
	+OK - Ready to specify rest of message.
	-ERROR_INCORRECT_FORMAT - The message format was incorrect.
	-ERROR_NOT_ENOUGH_DATA_FOR_BODY - We did not receive the specified amount of data for the body.
###Examples
	>> NOTIFICATION This is a test. 0
	<< +OK
	>> NOTIFICATION This is a test.
	<< -ERROR_INCORRECT_FORMAT
	>> NOTIFICATION This is a test with a body. 12
	>> Hello World!
	<< +OK
	>> NOTIFICATION This is a test with a body. 12
	>> Hello Worl
	<< -ERROR_NOT_ENOUGH_DATA_FOR_BODY

## ADD_DATA
Data to deliver to client application. This is comprised of Key->Value pairs. The Key is specified on the first line and the first line ends with a count of bytes in the value. We will then read the next %Value_Byte_Count% bytes (after the line break) and interpret them as the value. If there is no value specify 0 forthe byte count. The count must be the last thing on the line, it cannot end in whitespace.

##Usage
	ADD_DATA %KEY% %VALUE_BYTE_COUNT%
	%VALUE%
###Response
	+OK - Ready to specify rest of message.
	-ERROR_INCORRECT_FORMAT - The message format was incorrect.
	-ERROR_NOT_ENOUGH_DATA_FOR_BODY - We did not receive the specified amount of data for the body.
###Examples
	>> ADD_DATA Temperature_Ready 0
	<< +OK
	>> ADD_DATA Temperature_Ready
	<< -ERROR_INCORRECT_FORMAT
	>> ADD_DATA Temperature 3
	>> 104
	<< +OK
	>> ADD_DATA Temperature 3
	>> 10
	<< -ERROR_NOT_ENOUGH_DATA_FOR_BODY

## SEND_MSG
Send the message we have been preparing.
##Usage
	SEND_MSG
###Response
	+OK - Message Sent.
	-FAIL - Failed to send message, we still have the message prepared so you may retry.
###Examples
	>> SEND_MSG
	<< +OK
	>> SEND_MSG
	<< -FAIL
