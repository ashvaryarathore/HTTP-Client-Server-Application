#include "CServerSocket.h"

int CServerSocket::nThreadCounter = 0;
SOCKET CServerSocket::SocketFileDiscriptor = INVALID_SOCKET;

/************************************************************
Default Constructor.
Purpose : The constructor would initialize the data members.
Return Type : void.
Parameters : None.			
**************************************************************/
CServerSocket::CServerSocket()
{
	nRequestedResourceLength1 = 0;
	nRequestedResourceDataLength1 = 0;
	
}


/************************************************************
Funcation name: InitializeWinsock
Purpose :	The function will call the WSAStartup() function
			which will load the appropriate version of the 
			Winsock DLL.
Return Type : void.
Parameters : None.			
**************************************************************/
void CServerSocket::InitializeWinsock()
{
	
	WSADATA wsaData;//predefined stucture. WSAStartup() sets its fields.
	int nCheckInitializationResult;
 
	// Initialize Winsock
	nCheckInitializationResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	
	//Function will return 0 on success.
	if (nCheckInitializationResult != 0) 
	{
	  printf("WSAStartup failed with error %ld\n", WSAGetLastError());
	  //exit(EXIT_FAILURE);
  	}

	else
	{
		printf("\n WSAStartup Successful.\n");
	}
}


/************************************************************
Funcation name: CleanWinsock
Purpose :	The function  will call WSACleanup(), which allows 
			Winsock to free up any resources allocated by Winsock and 
			cancel any pending Winsock calls that your application made.
Return Type : void.
Parameters : None.			
**************************************************************/
void CServerSocket::CleanWinsock()
{
	//Socket Error is predefined in the library. (-1)
	if (WSACleanup() == SOCKET_ERROR)
	{
      printf("WSACleanup failed with error %d\n", WSAGetLastError());
	  //exit(EXIT_FAILURE);
	}

	else
	{
		printf("\n Cleanup Successful.\n");
	}
}





/************************************************************
Funcation name: SetupServerSocket
Purpose :	The function  will invoke the basic socket(), bind(), 
			listen() functions, needed to start up a
			server socket.
Return Type : void.
Parameters : None.			
**************************************************************/
void CServerSocket::SetupServerSocket() 
{


	SOCKET SocketFileDiscriptor = INVALID_SOCKET;
	boolean bOptionValue = TRUE;// Required to be passed as an argument in the setsockopt method.
	struct sockaddr_in server_addr,client_addr;
	int sin_size;
	int nCheckParameter;
	

	//Function socket returns a socket file discriptor. It is an predefined function.
	SocketFileDiscriptor = socket(AF_INET, //The address family specification. 
									SOCK_STREAM, //The type specification for the new socket.
									0);//The protocol to be used. in 0 service provider will choose the protocol to use. 
										

	
    if ((SocketFileDiscriptor) == INVALID_SOCKET)
	{
           printf("\n Server: Error in function: socket");
           perror("\n socket");
		   //exit(EXIT_FAILURE);
  
	}
	else
	{
		printf("\n Server: Successful function: socket ");
	}

	
	//Function setsockopt is used to set options at desired levels.
	nCheckParameter = setsockopt(SocketFileDiscriptor,SOL_SOCKET,SO_REUSEADDR,(char *)&bOptionValue,sizeof(int));
	
	if ( nCheckParameter == SOCKET_ERROR) 
	{
		printf("\n Server: Error in function: setsockopt");
        perror("\n setsockopt");
    }
	
	else
	{
		printf("\n Server: Successful function: setsockopt ");
	}



	/***************************************************************
	Code to assign the values to the struct sockaddr_in server_addr
	****************************************************************/
	server_addr.sin_family = AF_INET;	//The sin_family field must be set to AF_INET, which tells Winsock we are using the IP address family.      
    server_addr.sin_port = htons(8080); //Assigning the server the port 8080.    
    server_addr.sin_addr.s_addr = (INADDR_ANY); //INADDR_ANY is used because we dont wish to bind a socket to a specific IP.


	//Loop to assign the padding bits to the sin_zero character array of size 8.
	for(int nLoopVariable = 0; nLoopVariable < 8 ; nLoopVariable++)
	{
		server_addr.sin_zero[nLoopVariable] = 0;
	}



	//Code to bind a local address with a socket.
	nCheckParameter = bind(SocketFileDiscriptor,			//A descriptor identifying an unbound socket.
							(struct sockaddr *)&server_addr,//A pointer to a sockaddr structure of the local address to assign to the bound socket
							sizeof(struct sockaddr));		//The length, in bytes, of the value pointed to by the name parameter
							

	if (nCheckParameter == SOCKET_ERROR) 
    {
            printf("\n Server: Error in function: bind ");
			perror("\n bind");
    }
	
	else
	{
		printf("\n Server: Successful function: bind ");
	}


	//Code to set a socket in a state in which it is listening for an incoming connection.
	nCheckParameter = listen(SocketFileDiscriptor,	//A descriptor identifying a bound, unconnected socket
							5);						//The maximum length of the queue of pending connections. 
							
	
	//function returns 0 on successful listening.
	 if ( nCheckParameter == SOCKET_ERROR) 
	 {
           printf("\n Server: Error in function: listen ");
		   perror("\n listen");

			// Exit with error
			//exit(EXIT_FAILURE);
     }

	else
	{
		printf("\n MyHTTPServer waiting on port 8080.");

		
	}

	 
	 printf("\n Server: accept() is ready.");


	//Set the socket.
	SetSocket(SocketFileDiscriptor);

}



/************************************************************
Funcation name: StartServerSocket
Purpose :	The function will invoke the accept() function, and listen 
			for clients.
Return Type : void.
Parameters : None.			
**************************************************************/
void CServerSocket::StartServerSocket()
{
	 //**********************************************************************************************

	printf("\nStarting Thread %d \n", GetCurrentThreadId());
	
	char chReceiveDataBuffer[1024];
	SOCKET NewConnectionSocketFileDiscriptor = INVALID_SOCKET;
	//boolean bOptionValue = TRUE;// Required to be passed as an argument in the setsockopt method.
	struct sockaddr_in client_addr;
	int sin_size;
	int nCheckParameter;
	
	unsigned int nRecievedBits = 0;
	int nflag = 0;
	SOCKET SocketFileDiscriptorTemp = INVALID_SOCKET;
	SocketFileDiscriptorTemp = GetSocket();

	//Calcluating the sin size, to pass as an argument later.
	sin_size = sizeof(struct sockaddr_in);
  
	
	 /********************************************************************************************/
	

	 // Loop to Accept a new connection when one arrives.
	 //NewConnectionSocketFileDiscriptor = SOCKET_ERROR;
	 while(NewConnectionSocketFileDiscriptor == SOCKET_ERROR)
	 {
		//printf(" \n IN CONNECTION WHILE LOOP.......\n");
        NewConnectionSocketFileDiscriptor = accept(SocketFileDiscriptorTemp, (struct sockaddr *)&client_addr, &sin_size);
	 }

	//printf("\n Server: Client connected, ready for receiving and sending data.");

	//printf("\n------------------"); 

	
	if(nThreadCounter == 0)
	{
		nThreadCounter = nThreadCounter + 1;
		printf("\nThread Going to sleep.. %d \n", GetCurrentThreadId());
		Sleep(100000);
	}


	//array to hold the file name;
	char chFileName[200];

	//loop variable to monitor the request when > buffer size;
	int nDoLoop = 0;

	//variable to store the request type.
	int nRequestType ;

	//Variable to hold requested resource.
	char *chRequestedResource;
	char *chRequestedResourceFull;
	char chEndOfDataBoundary [200];
	unsigned int unBytesWritten = 0;
	unsigned int unTotalBytesRecieved = 0;
	unsigned int unContentLength = 0;
	unsigned int unSentBytes = 0;
	strcpy(chEndOfDataBoundary, "\r\n");
	
	//Code to recieve full request.
	do
	{
		
		nRecievedBits = recv(NewConnectionSocketFileDiscriptor,		//The descriptor that identifies a connected socket.
							chReceiveDataBuffer,		//A pointer to the buffer to receive the incoming data.
							1024,						//The length, in bytes, of the buffer pointed to by the buf parameter.
							0);					//A set of flags that influences the behavior of this function. 
		

		//Case when recieve bits are not zero.
		
		unTotalBytesRecieved = unTotalBytesRecieved + nRecievedBits;

		//The total bytes print.
		//printf("\n Total bytes recieved : %d", unTotalBytesRecieved);

		//Printing the recieve data.
		//printf("%s", chReceiveDataBuffer);
		
		//recieve bits equeals 0 when the client has no more data to be sent.
		if (nRecievedBits == 0) 
		{
			//printf("Connection closing...\n");
			break;
		} 
		
		//Case when error occurs in recieving.
		if (nRecievedBits == -1)  
		{
			//printf("\n\n recieved bits :: %d",nRecievedBits);
			//printf("\n\n else line 693 recv failed with error: %d\n", WSAGetLastError());
			//printf("\n recievecompleted. Sending send data..... \n\n");
			break;
		}

		 if (nRecievedBits > 0) 
		 {
			//Code to get the request type.
			nRequestType = GetRequestType(chReceiveDataBuffer);

			 if(nDoLoop == 0)
			 {
				//printf("\n\nBytes received: %d\n", nRecievedBits);

				//printf("\n\n Request data : \n ");

				//Printing the recieve data.
				//printf("%s", chReceiveDataBuffer);
				

				//printf("\n------------------");

				unsigned int nRequestedResourceLength;

				switch(nRequestType)
				{
					//The request is simple get request, code can be proceeded.
					case 1:
						printf("\n it is a simple get request to display data on the browswer. Thread inside %d \n", GetCurrentThreadId());
						printf("\n\n\nThread inside %d \n", GetCurrentThreadId());
						//Code to obtain the requested Resource from client request.
						chRequestedResource = GetRequestedResource(chReceiveDataBuffer, 1);
						
						//Obtaining the length of the requested resource.
						nRequestedResourceLength = GetRequestedResourceLength();
	
						//Marking End of string.
						chRequestedResource[nRequestedResourceLength] = '\0';
						break;
				


					//The request is a download request, code needs to be altered.
					case 2:
						printf("\n it is a download request. Thread inside %d \n", GetCurrentThreadId());
						//Code to obtain the requested Resource from client request.
						chRequestedResource = GetRequestedResource(chReceiveDataBuffer, 2);
						
						break;


					//The request is a upload request. 
					case 3:
						printf("\n The request is a upload request Thread inside %d \n", GetCurrentThreadId());
						printf("\n\n\nThread inside %d \n", GetCurrentThreadId());
						//Code to obtain the requested Resource from client request.
						chRequestedResourceFull = GetRequestedResource(chReceiveDataBuffer,3);
						
						//Obtaining the file name.
						chRequestedResource = GetFileName();

						/****************************************
						code to get the end of data boundary string.
						******************************************/
						
						char chTemp[5];
						//Concatrnation of the \r\n with boundary.
						strcat(chEndOfDataBoundary,GetBoundary());
						
						//assigning the chTemp value to the array.
						strcpy(chTemp, "\r\n");

						//Concatrnation of the \r\n with boundary.
						strcat(chEndOfDataBoundary,chTemp);

						//printf("\n the end boundary:::: %s", chEndOfDataBoundary);

						//Obtain the content length
						unContentLength = GetRequestedResourceContentLength();
						
						//Case Break;
						break;
				}


				//printf("\n Requested resource :  ");
			
				//printf(" %s \n", chRequestedResource);

				//printf("\n------------------");
	
				/*****************************************************
				Code to check the resource requested is valid or not.
				******************************************************/
					
				//assigning the folder name to the array.
				strcpy(chFileName, "D:/");


				//printf("\n Source Folder: %s",chFileName);

				//Concatrnation of the folder name and resource name.
				strcat(chFileName,chRequestedResource);
	
				//printf("\n The requested resource on server : %s",chFileName);





				if((nRequestType == 1) || (nRequestType == 2))
				{
					//checking if the file is present on the folder or not.
					int nValidityCheck = GetRequestedResourceValidity(chFileName);
	
					unsigned int unSentBytes;
					//If the requested resource is valid and present on the system.
					if(nValidityCheck == 1)
					{
						
						//printf("\n the requested resource is valid.\n\n");
				
						//File pointer
						FILE *file;
	
						if((file = fopen(chFileName, "r+b")) != NULL)
						{
							//Seek the end of file.
							fseek(file, 0, SEEK_END);

							//printf("file opened....");

							//Setting Resource data length.
							SetRequestedResourceDataLength(ftell(file));
					
							//Seek begining of the file.
							 fseek(file, SEEK_SET, 0);

							//Obtaining the requested resource Data length.
							unsigned int nRequestedResourceDataLength = GetRequestedResourceDataLength();

							//array to store the content length.
							char chContentLengthString[30];

							unsigned int nRequestedResourceDataLengthTemp = nRequestedResourceDataLength;

							//Predefined function in C Library. 10 indicates decimal conversion.
							itoa(nRequestedResourceDataLengthTemp,chContentLengthString,10);

							/************************************		
							Code to Send reply to client.
							************************************/
				
							//Size of the total bytes to be sent.(initially)
							unsigned int unSentbytesleft = nRequestedResourceDataLength;
					
							//Http response Header.

							char chSendDataBuffer[1024]= "HTTP/1.1 200 OK\r\n";
							char chSendDataBufferViewRequest[]= "Content-Type:text/html";
							switch(nRequestType)
							{
								//The http header for get simple display request.
								case 1:
																		
									//Concatenation of header with the chSendDataBufferViewRequest
									strcat(chSendDataBuffer,chSendDataBufferViewRequest);
									nflag = 1;
									break;
				
								//The http header for download request.
								case 2:
									//Adding the Content-Disposition :attachement in the http header for download.
									char chSendDataBufferDownloadRequest[]= "Content-Type:Content-Type:application/octet-stream\r\nContent-Disposition:attachment;filename=";
									
									
									//Concatenation of content with the chSendDataBufferDownloadRequest.
									strcat(chSendDataBuffer,chSendDataBufferDownloadRequest);


									//Concatenation of content with the file name.
									strcat(chSendDataBuffer,GetFileName());
									nflag = 1;
									break;
					
							}
							


							char chSendDataBuffer1[]= "\r\nContent-Length:";
					
							char chSendData2[]="\r\n\r\n";
		
							//Concatenation of content with the chSendDataBuffer1.
							strcat(chSendDataBuffer,chSendDataBuffer1);

							//Concatenation of content with the centent length.
							strcat(chSendDataBuffer,chContentLengthString);

							//Concatenation of content with the sendData2.
							strcat(chSendDataBuffer,chSendData2);
				
							//Sending initial header bits data to client.
							unSentBytes = send(NewConnectionSocketFileDiscriptor,	//The descriptor that identifies a connected socket.
												(chSendDataBuffer),		//A pointer to the buffer to receive the incoming data.
												strlen(chSendDataBuffer),					//The length, in bytes, of the buffer pointed to by the buf parameter.
												0);	


							//printf("\n sent bytes : %d",unSentBytes);

							unSentBytes = 0;								
				
							unsigned int unBytesLeft = nRequestedResourceDataLength;
							unsigned int unTotalBytes = 0; 
				
							//Seek start of file.
							fseek( file, unTotalBytes , SEEK_SET );

							//Printing the file position.
							//printf("\n the file pointer is at:: %d", unTotalBytes);

							//Loop to send all the data.
							while( unTotalBytes < nRequestedResourceDataLength)
							{
								printf("\nThread Going to sleep.. %d \n", GetCurrentThreadId());
								Sleep(1000);

								if( unBytesLeft > 1024)
								{		
										//printf("\n\nin if loop\n");
										unsigned int unBytesReadFromFile = fread(chSendDataBuffer, sizeof(char),1024, file);
							
										unSentBytes = send(NewConnectionSocketFileDiscriptor,				//The descriptor that identifies a connected socket.
															chSendDataBuffer,					//A pointer to the buffer to receive the incoming data.
															1024,								//The length, in bytes, of the buffer pointed to by the buf parameter.
															0);

										if( unBytesReadFromFile != unBytesLeft)
										{
											//printf("\n error in reading......");
											//printf("\n the bytes read: ");
										}

										unsigned int len = strlen(chSendDataBuffer);

										//printf("\n sent bytes : %d",unSentBytes);
										unBytesLeft = unBytesLeft - 1024;	
										unTotalBytes = unTotalBytes + unSentBytes;
								
										//Seek to the next bytes.
										fseek( file, unTotalBytes , SEEK_SET );
								
										//Printing the file position.
										//printf("\n the file pointer is at:: %d", unTotalBytes);
								}
							

								else
								{								
									
									//printf("\n\nin else loop\n");
									//printf("\n the unsent bytes now : %d,\n",unBytesLeft);
									unsigned int unBytesReadFromFile = fread(chSendDataBuffer, sizeof(char), unBytesLeft, file);
							
									if( unBytesReadFromFile != unBytesLeft)
									{
										//printf("\n error in reading......");
									}
									
							
									unSentBytes = send(NewConnectionSocketFileDiscriptor,	//The descriptor that identifies a connected socket.
															chSendDataBuffer,				//A pointer to the buffer to receive the incoming data.
															unBytesLeft,					//The length, in bytes, of the buffer pointed to by the buf parameter.
															0);
						
									unTotalBytes = unTotalBytes + unSentBytes;
									//printf("\n sent bytes : %d", unTotalBytes);
									//printf("\n total bytes were: %d", nRequestedResourceDataLength);

									fseek( file, unBytesReadFromFile , SEEK_SET );
						
									//Printing the file position.
									//printf("\n the file pointer is at:: %d", unTotalBytes);
									
									//closing the file
									fclose(file);
					
								}

								
							}//While closed.
				
							//printf("\n STATUS:%d",unTotalBytes);
      
							//printf("\n --------------");
				
						}
					
				

					}

					//Case when requested resource is not valid.
					else
					{
						printf("\n the requested resource is not valid.\n\n");
						char chSendDataBuffer[200]= "HTTP/1.1 200 OK\r\nContent-Length:46\r\nContent-Type: text/html\r\n\r\nRequested resource is not valid. Kindly check.";

						//Sending data to the client.
						unSentBytes = send(NewConnectionSocketFileDiscriptor,	//The descriptor that identifies a connected socket.
									chSendDataBuffer,				//A pointer to the buffer to receive the incoming data.
									strlen(chSendDataBuffer),		//The length, in bytes, of the buffer pointed to by the buf parameter.
									0);	

					}

				}//Case 1 and 2 if closed.

				
			}
			
			//To check for the upload request.
			if((nRequestType == 3))
			{	
				//unContentLength = unContentLength - strlen(chEndOfDataBoundary);
				if( unBytesWritten < unContentLength)
				{
					
					//File pointer
					FILE *file;
					
					if((nDoLoop != 0 ))
					{
						char * chReceiveDataBufferTemp = chReceiveDataBuffer;

						nDoLoop = nDoLoop + 1; 
						//Case when data cannot be recieved in one time.
						if((file = fopen(chFileName, "a+b")) != NULL)
						{
							int i = 1;
							while(i <= nRecievedBits)
							{
								if(*(chReceiveDataBufferTemp) == *(chEndOfDataBoundary))
								{
									if(*(chReceiveDataBufferTemp + 1) == *(chEndOfDataBoundary + 1))
									{
										if(*(chReceiveDataBufferTemp + 2) == *(chEndOfDataBoundary + 2))
										{
											//printf("\n\n\n\n\n BOUNDARY FOUND...");
											//printf("\n recievecompleted. Sending send data..... \n\n");
											nflag = 1;
											break;
										}
									}

								}


								//code to write the buffer data to file.
								fprintf(file,"%c", *(chReceiveDataBufferTemp));
								//printf("\nvalue: %c\n",*(chReceiveDataBufferTemp));
								chReceiveDataBufferTemp = chReceiveDataBufferTemp + 1;
								unBytesWritten = unBytesWritten + 1;
								i = i + 1;

							}

							fclose(file);
						
					
						}//if closed.

					}// if closed do loop.

					
					//To check if the header is presented or just plain data
					//In case of plain data skip the step.
					if((nDoLoop == 0))
					{
						//printf("\nFirst iteration.");

						//Increment to indicate first iteration .
						nDoLoop = nDoLoop + 1;
				
						if((file = fopen(chFileName, "w+b")) != NULL)
						{
							
							unsigned int unPostionTemp = GetPostion();

							char * chReceiveDataBufferTemp = &(chReceiveDataBuffer[unPostionTemp]);

							//printf("\n about to write.....\n");
							//Loop to check the end of content data.

							unsigned int unPosition = GetPostion();

							unPosition = 1024 - unPosition;

							while(unBytesWritten <= unPosition)
							{
								if(*(chReceiveDataBufferTemp) == *(chEndOfDataBoundary))
								{
									if(*(chReceiveDataBufferTemp + 1) == *(chEndOfDataBoundary + 1))
									{
										if(*(chReceiveDataBufferTemp + 2) == *(chEndOfDataBoundary + 2))
										{
											//printf("\n\n\n\n\n BOUNDARY FOUND...");
											//printf("\n recievecompleted. Sending send data..... \n\n");
											nflag = 1;
											break;
										}
									}

								}

								//code to write the buffer data to file.
								fprintf(file,"%c", *(chReceiveDataBufferTemp));
								//printf("\nvalue: %c\n",*(chReceiveDataBufferTemp));
								chReceiveDataBufferTemp = chReceiveDataBufferTemp + 1;
								unBytesWritten = unBytesWritten + 1;

							}

							fclose(file);
							
						
							//printf("\nFirst iteration COMPLETED.");
						}//Inner if closed
												
					
					}//if of nDoloop = 0, closed/
												
				}
								
			}// request type 3 if.

		}

		//printf("\n\nValur of receievd bits: %d" , nRecievedBits);
		
		if(nflag == 1)
		{
			char chSendDataBuffer[200]= "HTTP/1.1 200 OK\r\nContent-Length:19\r\nContent-Type:text/html\r\n\r\n Upload successful.";

			//Sending data to the client.
			unSentBytes = send(NewConnectionSocketFileDiscriptor,	//The descriptor that identifies a connected socket.
						chSendDataBuffer,				//A pointer to the buffer to receive the incoming data.
						strlen(chSendDataBuffer),		//The length, in bytes, of the buffer pointed to by the buf parameter.
						0);	

			//printf("Breaking the while loop....");
			
			break;

		}

	if(nflag == 1)
	{
		break;
	}

	}while( nRecievedBits > 0);

	//printf("value of recieved bits == %d",nRecievedBits);
	
	
	
	//printf("\n vfter cleanup");
	
	//closesocket(nSocketFileDiscriptor);
	printf("\n REQUEST PROCESSED...Thread inside %d \n", GetCurrentThreadId());
	shutdown(NewConnectionSocketFileDiscriptor, SD_SEND);	
}




/************************************************************
Funcation name: GetRequestedResource
Purpose :	The function will extract the reqested resource from 
			the character array.
Return Type : char*.
Parameters : char chReceiveDataBuffer[],  int nRequestType			
**************************************************************/
char* CServerSocket::GetRequestedResource(char chReceiveDataBuffer[], int nRequestType)
{

	int nPositionOfRequestedResource = 0;
	char * chRequestedResource;
	int nLoopVariable = 0; 
	unsigned int unStartAddressFileName = 0;


	switch(nRequestType)
	{
		//Request is a simple get request, with browsing a content.	
		case 1:
					
				//loop to find end of request menthod type.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != '/')
				{
					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
				}

				//Aditional increment to reach the next character then '/'
				nPositionOfRequestedResource = nPositionOfRequestedResource + 1;

				//Initializing the requested resource with the address of the first character of requested resource.
				chRequestedResource = &chReceiveDataBuffer[nPositionOfRequestedResource];

				//loop to find the requested resource.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != ' ')
				{
					//Assign value to the variable holding the requesated resource.
					chRequestedResource[nLoopVariable] = chReceiveDataBuffer[nPositionOfRequestedResource];

					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
					nLoopVariable = nLoopVariable + 1;
				}

				break;

		//When request is a download request.
		case 2:
				//loop to find end of request menthod type.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != '?')
				{
					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
				}

				//Aditional increment to reach the next character then '/'
				nPositionOfRequestedResource = nPositionOfRequestedResource + 1;

				//Initializing the requested resource with the address of the first character of requested resource.
				chRequestedResource = &chReceiveDataBuffer[nPositionOfRequestedResource];

				nLoopVariable = 0;
				unStartAddressFileName = nPositionOfRequestedResource;

				//loop to find the requested resource.
				while((chRequestedResource[nLoopVariable]) != ' ')
				{
					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
					nLoopVariable = nLoopVariable + 1;
				}

				//Mark end of resource.
				chRequestedResource[nLoopVariable] = '\0';

				//Set the file name.
				SetFileName(chRequestedResource, unStartAddressFileName);

				break;


		//When the request is upload request.
		case 3:
				//code to process the file name in case of upload.
				char chMatchCaseArray1[] = "Length:";
				char chMatchCaseArray[] = "filename=";
				char chMatchCaseArray2[] = "boundary=";
				unsigned int unStartPosition; 
				unsigned int unLastPosition;
				char *chRequestedResourceContentLength;
				char *boundary;
				char *chContentLength;
				char *chFileName;

				//Initializing the requested resource with the address of the first character of requested resource.
				chRequestedResource = &chReceiveDataBuffer[nPositionOfRequestedResource];

				nLoopVariable = 0;

				//Loop to find boundary.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\0')
				{
					if((chReceiveDataBuffer[nPositionOfRequestedResource]) == (chMatchCaseArray2[nLoopVariable]))
					{
						if((chReceiveDataBuffer[nPositionOfRequestedResource+2]) == (chMatchCaseArray2[nLoopVariable+2]))
						{
							if((chReceiveDataBuffer[nPositionOfRequestedResource+3]) == (chMatchCaseArray2[nLoopVariable+3]))
							{
								//Move pointer after 'boundary='
								nPositionOfRequestedResource = nPositionOfRequestedResource + strlen(chMatchCaseArray2);
								boundary = &chReceiveDataBuffer[nPositionOfRequestedResource];
								unsigned int unBoundaryStartAddress = nPositionOfRequestedResource;

								//printf("\n the boundary is : %s", boundary);
								nLoopVariable = 0;
								while(((boundary[nLoopVariable]) != '\r') && ((boundary[nLoopVariable+1]) != '\n'))
								{
									//printf("\n Value :: %c \n",(boundary[nLoopVariable]));
									nLoopVariable = nLoopVariable + 1;
									nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
								}
								
								boundary[nLoopVariable] = '\0';

								SetBoundary(boundary, unBoundaryStartAddress);

								//Incrementing the position pointer 2 times to include"\r\n' after boundary.
								nPositionOfRequestedResource = nPositionOfRequestedResource + 2;

								//printf("\n the boundary is : %s", boundary);
								
								//break while loop.
								break;
							}
						}
					}

					//Incrementing the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
				}

				//resetting the loop variable.
				nLoopVariable = 0;

				//loop to find the content length.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\0')
				{
					if((chReceiveDataBuffer[nPositionOfRequestedResource]) == (chMatchCaseArray1[nLoopVariable]))
					{
						if((chReceiveDataBuffer[nPositionOfRequestedResource+2]) == (chMatchCaseArray1[nLoopVariable+2]))
						{
							if((chReceiveDataBuffer[nPositionOfRequestedResource+4]) == (chMatchCaseArray1[nLoopVariable+4]))
							{
								//Move pointer after 'Length:'
								nPositionOfRequestedResource = nPositionOfRequestedResource + strlen(chMatchCaseArray1);
								chContentLength = &chReceiveDataBuffer[nPositionOfRequestedResource];
								unsigned int unContentLengthStartAddress = nPositionOfRequestedResource;

								//printf("\n the content length is : %s", chContentLength);
								
								nLoopVariable = 0;
								
								//Loop to reach the end of content length.
								while(((chContentLength[nLoopVariable]) != '\r') && ((chContentLength[nLoopVariable+1]) != '\n'))
								{
									//printf("\n Value :: %c \n",(chContentLength[nLoopVariable]));
									nLoopVariable = nLoopVariable + 1;
									nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
								}
								
								chContentLength[nLoopVariable] = '\0';
								//printf("\n the chContentLength is : %s", chContentLength);
								SetRequestedResourceContentLength(chContentLength, unContentLengthStartAddress);

								//Incrementing the position pointer 2 times to include"\r\n' after boundary.
								nPositionOfRequestedResource = nPositionOfRequestedResource + 2;

								int i = GetRequestedResourceContentLength();

								//printf("\n the integer content length is :: %d \n",i);
								
								
								//break while loop.
								break;													
								
							}											
						}						
					}

					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
				}
				
				nLoopVariable = 0;
				//loop to find file name.
				while((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\0')
				{
					if((chReceiveDataBuffer[nPositionOfRequestedResource]) == (chMatchCaseArray[nLoopVariable]))
					{
						if((chReceiveDataBuffer[nPositionOfRequestedResource+2]) == (chMatchCaseArray[nLoopVariable+2]))
						{
							if((chReceiveDataBuffer[nPositionOfRequestedResource+3]) == (chMatchCaseArray[nLoopVariable+3]))
							{
								
								//Move pointer after 'filename='
								nPositionOfRequestedResource = nPositionOfRequestedResource + strlen(chMatchCaseArray) + 1;
								chFileName = &chReceiveDataBuffer[nPositionOfRequestedResource];
								unsigned int unFileNameStartAddress = nPositionOfRequestedResource;

								//printf("\n the file name is before: %s", chFileName);
								
								nLoopVariable = 0;
								
								//Loop to reach the end of file name.
								while(((chFileName[nLoopVariable]) != '\"'))
								{
									//printf("\n Value :: %c \n",(chFileName[nLoopVariable]));
									nLoopVariable = nLoopVariable + 1;
									nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
								}
								
								chFileName[nLoopVariable] = '\0';
								//printf("\n the file name is : %s", chFileName);
													
								SetFileName(chFileName, unFileNameStartAddress);

								//printf("\n\n req value:: %c", chReceiveDataBuffer[nPositionOfRequestedResource]);
								
								//Incrementing the position pointer 3 times to include""\r\n' 
								nPositionOfRequestedResource = nPositionOfRequestedResource + 3;
															
								//printf("\n\n req value:: %c", chReceiveDataBuffer[nPositionOfRequestedResource]);

								//break while loop.
								break;									
								
							}											
						}						
					}

					//Increment the pointer.
					nPositionOfRequestedResource = nPositionOfRequestedResource + 1;
				}


				//Inrementing the loop to return the content data.
				while(((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\r')&&((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\n'))
				{
					while(((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\r')&&((chReceiveDataBuffer[nPositionOfRequestedResource]) != '\n'))
					{
						//incrementing
						//printf("\n\n req value in inner loop:: %c", chReceiveDataBuffer[nPositionOfRequestedResource]);
						nPositionOfRequestedResource = nPositionOfRequestedResource + 1;			
					}
					//incrementing for the initial /r/n
					//printf("\n\n req value in outer loop:: %c", chReceiveDataBuffer[nPositionOfRequestedResource]);
					nPositionOfRequestedResource = nPositionOfRequestedResource + 2;
					//printf("\n\n req value in outer loop after incerem:: %c", chReceiveDataBuffer[nPositionOfRequestedResource+2]);
				}

				nPositionOfRequestedResource = nPositionOfRequestedResource + 2;

				//printf("\n\n req value after loop:: %c", chReceiveDataBuffer[nPositionOfRequestedResource]);
				

				chReceiveDataBuffer = &(chReceiveDataBuffer[nPositionOfRequestedResource]);
				unPositionVariable = nPositionOfRequestedResource;
				//CaseBreak;
				break;

	}

	if((nRequestType == 1) || (nRequestType == 2))
	{
		//Setting the length of requested resource(ex 1.txt))
		SetRequestedResourceLength(nLoopVariable);
		return chRequestedResource;
	}
	
	if((nRequestType == 3))
	return chReceiveDataBuffer;

}





/************************************************************
Funcation name: SetRequestedResourceLength
Purpose :	The function will set the reqested resource length.
Return Type : int.
Parameters : int nRequestedResourceLength.			
**************************************************************/
void CServerSocket::SetRequestedResourceLength(unsigned int nRequestedResourceLengthTemp)
{
	 nRequestedResourceLength1 = nRequestedResourceLengthTemp;
}



/************************************************************
Funcation name: GetRequestedResourceLength
Purpose :	The function will return the reqested resource length.
Return Type : int.
Parameters : None.		
**************************************************************/
unsigned int CServerSocket::GetRequestedResourceLength()
{
	return nRequestedResourceLength1;
}




/************************************************************
Funcation name: GetRequestedResourceValidity
Purpose : The function will check the validity of the reqested 
		  resource.
Return Type : int. 
			  Function will return 1 if file is valid. 0 
			  otherwise.
Parameters : char *chFileName.		
**************************************************************/
int CServerSocket::GetRequestedResourceValidity(char *chFileName)
{
    FILE *file;
    if (file = fopen(chFileName, "r"))
    {
        fclose(file);
        return 1;
    }
    return 0;
}





/************************************************************
Funcation name: GetRequestedResourceData
Purpose : The function will read the data of the reqested 
		  resource.
Return Type : char *RequestedResourceData. 
Parameters : char *chFileName.		
**************************************************************/
char * CServerSocket::GetRequestedResourceData(char *chFileName)
{
	char RequestedResourceData[8192];

	FILE *file;
	file = fopen(chFileName, "r");
	
    int nLoopVariable = 0;
    char nCharacterRead;

	while((nCharacterRead = fgetc(file)) != EOF)
	{
		RequestedResourceData[nLoopVariable] = nCharacterRead;
		nLoopVariable = nLoopVariable + 1;
	} 

	//getting the total file size.
	unsigned int nLength = ftell(file);

	//closing the resource.
	fclose(file);

	//setting the total file size.
	SetRequestedResourceDataLength(nLength);
	
	return RequestedResourceData;
}




/************************************************************
Funcation name: SetRequestedResourceDataLength
Purpose :	The function will set the reqested resource length.
Return Type : void.
Parameters : unsigned int nRequestedResourceLength.			
**************************************************************/
void CServerSocket::SetRequestedResourceDataLength(unsigned int nRequestedResourceLengthTemp)
{
	 nRequestedResourceDataLength1 = nRequestedResourceLengthTemp;
}
	


/************************************************************
Funcation name: GetRequestedResourceDataLength
Purpose :	The function will return the reqested resource length.
Return Type : unsigned int.
Parameters : None.		
**************************************************************/
unsigned int CServerSocket::GetRequestedResourceDataLength()
{
	return nRequestedResourceDataLength1;
}





/************************************************************
Funcation name: GetRequestType
Purpose :	The function will return the reqested resource type.
Return Type : int.
Parameters : char *chReceiveDataBuffer.		
**************************************************************/
int CServerSocket::GetRequestType(char *chRequestedResource)
{
	//loop to iterate the size 10 array.
	for(int nLoopVariable = 0; nLoopVariable <10 ; nLoopVariable++)
	{
		
		/**************************************
		Code to Check for download request.
		**************************************/
		if(chRequestedResource[nLoopVariable + 6] == 'd')
		{
			if(chRequestedResource[nLoopVariable + 7] == 'o')
			{
				if(chRequestedResource[nLoopVariable + 8] == 'w')
				{
					if(chRequestedResource[nLoopVariable + 9] == 'n')
					{
						if(chRequestedResource[nLoopVariable + 10] == 'l')
						{
							if(chRequestedResource[nLoopVariable + 11] == 'o')
							{
								if(chRequestedResource[nLoopVariable + 12] == 'a')
								{
									if(chRequestedResource[nLoopVariable + 13] == 'd')
									{
										return 2;
									}
								}
							}
						}
					}
				}
			}
		}




		/**************************************
		Code to Check for simple GET request.
		**************************************/
		if(chRequestedResource[nLoopVariable] == 'G')
		{
			if(chRequestedResource[nLoopVariable + 1] == 'E')
			{
				if(chRequestedResource[nLoopVariable + 2] == 'T')
				{
					
					for(int nLoopVariable1 = 0; nLoopVariable1 <10 ; nLoopVariable1++)
					{
							/**************************************
							Code to Check for download request.
							**************************************/
							if(chRequestedResource[nLoopVariable1] == 'd')
							{
								if(chRequestedResource[nLoopVariable1 + 1] == 'o')
								{
									if(chRequestedResource[nLoopVariable1 + 2] == 'w')
									{
										if(chRequestedResource[nLoopVariable1 + 3] == 'n')
										{
											if(chRequestedResource[nLoopVariable1 + 4] == 'l')
											{
												if(chRequestedResource[nLoopVariable1 + 5] == 'o')
												{
													if(chRequestedResource[nLoopVariable1 + 6] == 'a')
													{
														if(chRequestedResource[nLoopVariable1 + 7] == 'd')
														{
															return 2;
														}
													}
												}
											}
										}
									}
								}
							}
					
					}
					
					
					return 1;
								
				}
			}
		}


			
	}//For closed.

	//if case did not match.
	return 3;
		

}




/************************************************************
Funcation name: SetRequestedResourceContentLength
Purpose :	The function will set the reqested resource upload 
			content length.
Return Type : void.
Parameters : char *chRequestedResourceContentLength, unsigned int unStartPosition		
**************************************************************/
void CServerSocket::SetRequestedResourceContentLength(char *chContentLengthTemp, unsigned int unContentLengthStartAddress)
{
	char *chContentLength;

	chContentLength = &(chContentLengthTemp[unContentLengthStartAddress]);
	chContentLength = chContentLengthTemp;

	//Convert string to int.
	usRequestedResourceContentLength = atoi(chContentLength);
	
}



/************************************************************
Funcation name: GetRequestedResourceContentLength
Purpose :	The function will set the return the reqested resource upload 
			content length.
Return Type : int
Parameters : None.	
**************************************************************/
int CServerSocket::GetRequestedResourceContentLength()
{
	return	usRequestedResourceContentLength;
	
}



/************************************************************
Funcation name: SetBoundary
Purpose :	The function will set the boundary parameter.
Return Type : void.
Parameters : char *boundary, unsigned int unBoundaryStartAddress		
**************************************************************/
void CServerSocket::SetBoundary(char *boundary, unsigned int unBoundaryStartAddress)
{
	chBoundary = &(boundary[unBoundaryStartAddress]);
	chBoundary = boundary;
}



/************************************************************
Funcation name: GetBoundary
Purpose :	The function will return the boundary parameter.
Return Type : char *chBoundary.
Parameters : None.		
**************************************************************/
char * CServerSocket::GetBoundary()
{
	return chBoundary;
}




/************************************************************
Funcation name: SetFileName
Purpose :	The function will set the FileName parameter.
Return Type : void.
Parameters : char *chFileName, unsigned int unFileNameStartAddress	
**************************************************************/
void CServerSocket::SetFileName(char *chFileNameTemp, unsigned int unFileNameStartAddress)
{
	chFileName = &(chFileNameTemp[unFileNameStartAddress]);
	chFileName = chFileNameTemp;
}


/************************************************************
Funcation name: GetFileName
Purpose :	The function will return the FileName parameter.
Return Type : char *chFileName.
Parameters : None.		
**************************************************************/
char * CServerSocket::GetFileName()
{
	return chFileName;
}



/************************************************************
Funcation name: GetPostion
Purpose :	The function will return the position parameter.
Return Type : unsigned int.
Parameters : None.		
**************************************************************/
unsigned int CServerSocket::GetPostion()
{
	return unPositionVariable;

}


/************************************************************
Funcation name: SetSocket
Purpose :	The function will set the socket parameter.
Return Type : void.
Parameters : SOCKET SocketFileDiscriptor;	
**************************************************************/
void CServerSocket::SetSocket(SOCKET SocketFileDiscriptorTemp)
{
	SocketFileDiscriptor = SocketFileDiscriptorTemp;
}


/************************************************************
Funcation name: GetSocket
Purpose :	The function will return the reqested Socket.
Return Type : SOCKET.
Parameters : None.		
**************************************************************/
SOCKET CServerSocket::GetSocket()
{
	return SocketFileDiscriptor;
}