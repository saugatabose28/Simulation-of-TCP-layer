# Simulation-of-TCP-layer
In this project, I have written a C program to simulate the workings of TCP. Here I have assumed 212 bytes frame size.

Networking assignment.
Implementing Data link layer functionality.

Group 13:

	Sandesh Bhujbal 	(12203)
	Saugata Bose 		(12204)


Steps to run:

1) First run frame_server.c by
	gcc frame_server.c -lm
	./a.out

2) Give any string as input and press enter.
3) This will display frames by converting the string into binary format and wait for the client to connect.
4) Then on another terminal run frame_client.c by 
	gcc frame_client.c
	./a.out
5) After this client will wait for the frames coming from server.
6) On the server side enter 1/0 depending on whether you want error in the data in the frame or not and press enter.
7) The frames will be sent to the client and client will check for errors and give the msg. 

