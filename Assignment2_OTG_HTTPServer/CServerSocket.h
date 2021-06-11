#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

#define MAX_THREADS 5

/************************************************************
Class name: CServerSocket
Purpose : The class will have the code to start the server.
Return Type : void.
Parameters : None.			
**************************************************************/

class CServerSocket
{
	unsigned int nRequestedResourceLength1;
	unsigned int nRequestedResourceDataLength1;
	unsigned int usRequestedResourceContentLength;
	char *chBoundary;
	char *chFileName;
	unsigned int unPositionVariable;
	static SOCKET SocketFileDiscriptor;
	static int nThreadCounter;

	public:

	/************************************************************
	Default Constructor.
	Purpose : The constructor would initialize the data members.
	Return Type : void.
	Parameters : None.			
	**************************************************************/
	CServerSocket();


	/************************************************************
	Funcation name: InitializeWinsock
	Purpose : The function will call the WSAStartup() function
			  which will load the appropriate version of the 
			  Winsock DLL.
	Return Type : void.
	Parameters : None.			
	**************************************************************/
	void InitializeWinsock();


	/************************************************************
	Funcation name: CleanWinsock
	Purpose :	The function  will call WSACleanup(), which allows 
				Winsock to free up any resources allocated by Winsock and 
				cancel any pending Winsock calls that your application made.
	Return Type : void.
	Parameters : None.			
	**************************************************************/
	void CleanWinsock();


	/************************************************************
	Funcation name: StartServerSocket
	Purpose :	The function  will invoke the basic socket(), bind(), 
				listen(), accept() functions, needed to start up a
				server socket.
	Return Type : void.
	Parameters : None.			
	**************************************************************/
	void StartServerSocket();



	/************************************************************
	Funcation name: GetRequestedResource
	Purpose :	The function will extract the reqested resource from 
				the character array.
	Return Type : char*.
	Parameters : char chReceiveDataBuffer[],  int nRequestType			
	**************************************************************/
	char* GetRequestedResource(char chReceiveDataBuffer[],  int nRequestType);



	
	/************************************************************
	Funcation name: SetRequestedResourceLength
	Purpose :	The function will set the reqested resource length.
	Return Type : void.
	Parameters : unsigned int nRequestedResourceLength.			
	**************************************************************/
	void SetRequestedResourceLength(unsigned int nRequestedResourceLengthTemp);
	


	/************************************************************
	Funcation name: GetRequestedResourceLength
	Purpose :	The function will return the reqested resource length.
	Return Type : unsigned int.
	Parameters : None.		
	**************************************************************/
	unsigned int GetRequestedResourceLength();



	/************************************************************
	Funcation name: GetRequestedResourceValidity
	Purpose : The function will check the validity of the reqested 
			  resource.
	Return Type : int. 
				  Function will return 1 if file is valid. 0 
				  otherwise.
	Parameters : char *chFileName.		
	**************************************************************/
	int GetRequestedResourceValidity(char *chFileName);
	
	/************************************************************
	Funcation name: GetRequestedResourceData
	Purpose : The function will read the data of the reqested 
			  resource.
	Return Type : char *RequestedResourceData. 
	Parameters : char *chFileName.		
	**************************************************************/
	char * GetRequestedResourceData(char *chFileName);


	
	/************************************************************
	Funcation name: SetRequestedResourceLength
	Purpose :	The function will set the reqested resource length.
	Return Type : void.
	Parameters : unsigned int nRequestedResourceLength.			
	**************************************************************/
	void SetRequestedResourceDataLength(unsigned int nRequestedResourceLengthTemp);
	


	/************************************************************
	Funcation name: GetRequestedResourceLength
	Purpose :	The function will return the reqested resource length.
	Return Type : unsigned int.
	Parameters : None.		
	**************************************************************/
	unsigned int GetRequestedResourceDataLength();



	/************************************************************
	Funcation name: GetRequestType
	Purpose :	The function will return the reqested resource type.
	Return Type : int.
	Parameters : char *chRequestedResource.		
	**************************************************************/
	int GetRequestType(char *chRequestedResource);


	/************************************************************
	Funcation name: SetRequestedResourceContentLength
	Purpose :	The function will set the reqested resource upload 
				content length.
	Return Type : void.
	Parameters : char *chRequestedResourceContentLength, unsigned int unStartPosition		
	**************************************************************/
	void SetRequestedResourceContentLength(char *chRequestedResourceContentLength, unsigned int unStartPosition);

	/************************************************************
	Funcation name: GetRequestedResourceContentLength
	Purpose :	The function will set the return the reqested resource upload 
				content length.
	Return Type : int
	Parameters : None.	
	**************************************************************/
	int GetRequestedResourceContentLength();
	

	/************************************************************
	Funcation name: SetBoundary
	Purpose :	The function will set the boundary parameter.
	Return Type : void.
	Parameters : char *boundary, unsigned int unBoundaryStartAddress	
	**************************************************************/
	void SetBoundary(char *boundary, unsigned int unBoundaryStartAddress);


	/************************************************************
	Funcation name: GetBoundary
	Purpose :	The function will return the boundary parameter.
	Return Type : char *chBoundary.
	Parameters : None.		
	**************************************************************/
	char * GetBoundary();

		
	/************************************************************
	Funcation name: SetFileName
	Purpose :	The function will set the FileName parameter.
	Return Type : void.
	Parameters : char *chFileName, unsigned int unFileNameStartAddress	
	**************************************************************/
	void SetFileName(char *chFileName, unsigned int unFileNameStartAddress);


	/************************************************************
	Funcation name: GetFileName
	Purpose :	The function will return the FileName parameter.
	Return Type : char *chFileName.
	Parameters : None.		
	**************************************************************/
	char * GetFileName();
		
	/************************************************************
	Funcation name: GetPostion
	Purpose :	The function will return the position parameter.
	Return Type : unsigned int.
	Parameters : None.		
	**************************************************************/
	unsigned int GetPostion();
	


	/************************************************************
	Funcation name: SetupServerSocket
	Purpose :	The function  will invoke the basic socket(), bind(), 
				listen() functions, needed to start up a
				server socket.
	Return Type : void.
	Parameters : None.			
	**************************************************************/
	void SetupServerSocket();


	/************************************************************
	Funcation name: SetSocket
	Purpose :	The function will set the socket parameter.
	Return Type : void.
	Parameters : SOCKET SocketFileDiscriptor;	
	**************************************************************/
	void SetSocket(SOCKET SocketFileDiscriptor);


	/************************************************************
	Funcation name: GetSocket
	Purpose :	The function will return the reqested Socket.
	Return Type : SOCKET.
	Parameters : None.		
	**************************************************************/
	SOCKET GetSocket();

};

