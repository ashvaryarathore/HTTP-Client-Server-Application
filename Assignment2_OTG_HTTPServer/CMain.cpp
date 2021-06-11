#include "CServerSocket.h"
#include<stdio.h>



//Method declaration.
DWORD WINAPI ThreadFunction( LPVOID lpParam );




/********************************************************
Function name : main
Purpose : The function will be used as the starting point of 
		  execution.
**********************************************************/
int main()
{
	//Initializations for multithreading.
	DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS];
	
	while(1)
	{
		//Object of CServerSocket class.
		CServerSocket *objCServerSocket = new CServerSocket();

		objCServerSocket->InitializeWinsock();		
		
		objCServerSocket->SetupServerSocket();

		// Create MAX_THREADS worker threads.
	    for( int i=0; i<MAX_THREADS; i++ )
		{
       		// Create the thread to begin execution on its own.
			hThreadArray[i] = CreateThread( 
				NULL,                   // default security attributes
				0,                      // use default stack size  
				ThreadFunction,			// thread function name
				NULL,					// argument to thread function 
				0,                      // use default creation flags 
				&dwThreadIdArray[i]);   // returns the thread identifier 
			       
		} // End of main thread creation loop.

				
		// Wait until all threads have terminated.
		WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

		//Close all thread handles and free memory allocations.
		for(int i=0; i<MAX_THREADS; i++)
		{
			CloseHandle(hThreadArray[i]);
		}

		
		//Cleanup Method.
		objCServerSocket->CleanWinsock();
	}

	return 0;
} 




/********************************************************
Function name : main
Purpose : The function will be used as the starting point of 
		  execution.
**********************************************************/
DWORD WINAPI ThreadFunction( LPVOID lpParam ) 
{ 
    HANDLE hStdout;
    DWORD dwChars;
	
	//printf("Hello I am in thread \n");

	//Object of CServerSocket class.
	CServerSocket *objCServerSocket = new CServerSocket();

	//Calling the server socket function.
	objCServerSocket->StartServerSocket();	
	
    return 0; 
} 







