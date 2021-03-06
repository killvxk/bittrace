/*
 *
 * Copyright 2010 JiJie Shi
 *
 * This file is part of bittrace.
 *
 * bittrace is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bittrace is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bittrace.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
 
 #include "common_func.h"
#include "pipe_line.h"

#define KXEPIPE_WAIT_PIPE_INTERVAL 100	// WaitNamedPipe 时间间隔（毫秒）
#define KXEPIPE_MAX_CONNECT_TIMES 5		// 尝试连接指定的管道服务端的最大次数

LRESULT create_name_pipe( LPCTSTR pipe_name, HANDLE *pipe_out )
{
	LRESULT ret = ERROR_SUCCESS; 
	BOOL _ret; 
	HANDLE _pipe = INVALID_HANDLE_VALUE; 
	INT32 retry_time = 0; 

	ASSERT( pipe_out != NULL ); 
	
	*pipe_out = NULL; 

	//"\\\\.\\Pipe\\Test"
	_pipe = CreateNamedPipe( pipe_name, 
		PIPE_ACCESS_DUPLEX, 
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 
		1, 
		0, 
		0, 
		1000, 
		NULL ); 
	
	if( _pipe == INVALID_HANDLE_VALUE )
	{
		ret = GetLastError(); 
		log_trace( ( DBG_MSG_AND_ERROR_OUT, "create name pipe failed \n" ) ); 
		goto _return; 
	}
	else
	{
		log_trace( ( DBG_MSG_AND_ERROR_OUT, "create name pipe successfully\n" ) ); 
	}

	for( ; ; )
	{
		_ret = ConnectNamedPipe( _pipe, NULL ); 

		if( _ret = FALSE )
		{
			ret = GetLastError(); 

			if( ret == ERROR_PIPE_CONNECTED )
			{
				ret = ERROR_SUCCESS; 	
			}
			else
			{
				ASSERT( ret != ERROR_SUCCESS ); 

				retry_time ++; 
				if( retry_time == KXEPIPE_MAX_CONNECT_TIMES )
				{
					goto _return; 
				}

				Sleep( 300 ); 
			}
		}
		else
		{
			break; 
		}
	}

	*pipe_out = _pipe; 

_return: 
	if( ret != ERROR_SUCCESS )
	{
		if( _pipe != INVALID_HANDLE_VALUE )
		{
			CloseHandle( _pipe ); 
		}
	}

	return ret; 
}

UINT read_from_name_pipe( HANDLE _pipe, CHAR *data, ULONG data_len, ULONG *readed )
{
	LRESULT ret = ERROR_SUCCESS;
	INT32 _ret; 

	_ret = ConnectNamedPipe( _pipe, NULL ); 

	if( _ret == FALSE ) 
	{
		CloseHandle( _pipe ); 
		ret = GetLastError(); 
		goto _return; 
	}

	_ret = ReadFile( _pipe, data, data_len, readed, NULL ); 
	if( _ret == FALSE )
	{
		ret = GetLastError(); 
		goto _return; 
	}

_return:
	return ret; 
}

LRESULT disconnect_name_pipe( HANDLE pipe )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 

	_ret = DisconnectNamedPipe( pipe ) ; 
	if( _ret == FALSE )
	{
		ret = GetLastError(); 
		log_trace( ( DBG_MSG_AND_ERROR_OUT, "disconnect the pipe failed\n" ) ); 
		goto _return; 
	}

_return:
	return ret; 
}

INLINE LRESULT flush_name_pipe( HANDLE pipe )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 

	ASSERT( pipe != NULL && pipe != INVALID_HANDLE_VALUE ); 

	_ret = FlushFileBuffers( pipe ) ; 
	if( _ret == FALSE )
	{
		ret = GetLastError(); 
		log_trace( ( DBG_MSG_AND_ERROR_OUT, "disconnect the pipe failed\n" ) ); 
		goto _return; 
	}

_return:
	return ret; 
}

LRESULT write_to_name_pipe( LPCWSTR pipe_name, CHAR* data, ULONG data_len, ULONG *ret_len )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 _ret; 
	HANDLE pipe; 

	ASSERT( pipe_name != NULL ); 
	ASSERT( ret_len != NULL ); 

	_ret = WaitNamedPipe( pipe_name, NMPWAIT_WAIT_FOREVER ); 
	if( _ret == FALSE )
	{
		ret = GetLastError(); 
		goto _return; 
	}
	
	pipe = CreateFile( pipe_name, 
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL ); 

	if( pipe == INVALID_HANDLE_VALUE )
	{

		ret = GetLastError(); 
		goto _return; 
	} 

	_ret = WriteFile( pipe, data, data_len, ret_len, NULL ); 
	if( _ret == FALSE )
	{
		ret = GetLastError(); 
		goto _return; 
	}

_return: 
	if( pipe != NULL )
	{
		CloseHandle( pipe ); // 关闭管道句柄 
	}

	return ret; 
}

LRESULT create_non_name_pipe( HANDLE *read_pipe, HANDLE *write_pipe )
{
	LRESULT ret = ERROR_SUCCESS; 
	BOOL _ret; 
	
	ASSERT( read_pipe != NULL ); 
	ASSERT( write_pipe != NULL ); 

	_ret = CreatePipe( read_pipe, write_pipe, NULL, 0 ); // 创建匿名管道
	if( _ret == TRUE )
	{
		ret = GetLastError(); 

		log_trace( ( DBG_MSG_AND_ERROR_OUT, "create none name pipe failed\n" ) );
	}

	return ret; 
}

LRESULT none_name_pipe_test()
{
	LRESULT ret = ERROR_SUCCESS; 
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char ReadBuf[ 100 ];
	DWORD ReadNum;
	HANDLE hRead; // 管道读句柄
	HANDLE hWrite; // 管道写句柄
	HANDLE hTemp; 
	BOOL bRet = CreatePipe(&hRead, &hWrite, NULL, 0); // 创建匿名管道
	if (bRet == TRUE)
		printf("成功创建匿名管道!\n");
	else
		printf("创建匿名管道失败,错误代码:%d\n", GetLastError());
	// 得到本进程的当前标准输出
	hTemp = GetStdHandle(STD_OUTPUT_HANDLE);
	// 设置标准输出到匿名管道
	SetStdHandle(STD_OUTPUT_HANDLE, hWrite);
	GetStartupInfo(&si); // 获取本进程的STARTUPINFO结构信息

	bRet = CreateProcess( NULL, 
		_T( "Client.exe" ), 
		NULL, 
		NULL, 
		TRUE, 
		0, 
		NULL, 
		NULL, 
		&si, 
		&pi); // 创建子进程

	SetStdHandle( STD_OUTPUT_HANDLE, hTemp ); // 恢复本进程的标准输出

	if (bRet == TRUE) // 输入信息
		printf("成功创建子进程!\n");
	else
		printf("创建子进程失败,错误代码:%d\n", GetLastError());

	CloseHandle(hWrite); // 关闭写句柄

	// 读管道直至管道关闭
	while (ReadFile(hRead, ReadBuf, 100, &ReadNum, NULL))
	{
		ReadBuf[ReadNum] = '\0';
		printf("从管道[%s]读取%d字节数据\n", ReadBuf, ReadNum);
	}

	if( GetLastError() == ERROR_BROKEN_PIPE ) // 输出信息
		printf("管道被子进程关闭\n");
	else
		printf("读数据错误,错误代码:%d\n", GetLastError());
	return ret; 
}

LRESULT init_pipe_point( pipe_ipc_point *point, LPCWSTR pipe_name, ULONG flags, ULONG try_time )
{
	LRESULT ret = ERROR_SUCCESS; 
	ULONG name_len; 
	INT32 retry_time = 0; 

	ASSERT( point != NULL ); 
	
	point->lock = NULL; 
	point->pipe = NULL; 
	point->pipe_name = NULL; 

	ret = init_mutex( &point->lock ); 
	if( ERROR_SUCCESS != ret )
	{ 
		goto _return; 
	}

	if( pipe_name != NULL )
	{
		name_len = wcslen( pipe_name ); 
		point->pipe_name = malloc( ( name_len + 1 ) * sizeof( WCHAR ) ); 
		if( point->pipe_name == NULL )
		{
			ret = ERROR_NOT_ENOUGH_MEMORY; 
			goto _return; 
		}

		wcscpy( point->pipe_name, pipe_name ); 

		if( try_time > MAX_PIPE_RETRY_TIME )
		{
			try_time = MAX_PIPE_RETRY_TIME; 
		}

		if( SERVER_PIPE == flags )
		{
			for( ; ; )
			{
				ret = create_name_pipe( pipe_name, &point->pipe ); 
				if( ret != ERROR_SUCCESS )
				{
					retry_time ++; 
					if( retry_time == try_time )
					{
						goto _return; 
					}

					Sleep( 1000 ); 
				}
				else
				{
					break; 
				}
			}
		}
		else if( CLIENT_PIPE == flags )
		{
			for( ; ; )
			{
				ret = connect_to_pipe( pipe_name, &point->pipe ); 
				if( ret != ERROR_SUCCESS )
				{
					retry_time ++; 
					if( retry_time == try_time )
					{
						goto _return; 
					}

					Sleep( 1000 ); 
				}
				else
				{
					break; 
				}
			}
		}
		else
		{
			ASSERT( FALSE ); 
		}
	}
	else
	{
		ret = ERROR_INVALID_PARAMETER; 
		goto _return; 
		//point->pipe_name = NULL; 
		//ret = create_non_name_pipe( &point->pipe ); 
	}


_return:
	if( ERROR_SUCCESS != ret )
	{
		if( point->lock != NULL )
		{
			uninit_mutex( point->lock ); 
		}

		if( point->pipe != NULL )
		{
			CloseHandle( point->pipe ); 
		}

		if( point->pipe_name != NULL )
		{
			free( point->pipe_name ); 
		}
	}

	return ret; 
}

LRESULT __stdcall _exec_cmd_from_pipe( pipe_ipc_point *point, 
									  const BYTE* method,	// 如果它为空，客户端会返回一个 “参数错误” 消息
									  const BYTE* data,			// 如果它为空，客户端会返回一个 “参数错误” 消息
									  ULONG data_len,
									  VOID** data_ret,
									  ULONG* data_ret_len
									  )

{
	LRESULT ret = ERROR_SUCCESS; 
	ULONG data_send_len; 
	ULONG method_len; 
	CHAR *send_buf = NULL; 
	ULONG offset; 
	CHAR *_data_ret = NULL; 
	ULONG _data_ret_len; 
	INT32 held_lock = FALSE; 

	//1.... 检查参数正确性
	if( NULL == point ||NULL == method ||
		NULL == data || 0 == data_len )
	{
		ret = ERROR_INVALID_PARAMETER; ;
		goto _return; 
	}

	ASSERT( data_ret != NULL ? data_ret_len != NULL : data_ret_len == NULL ); 

	// 将out参数置空后再返回

	if( data_ret != NULL )
	{
		*data_ret = NULL;
		*data_ret_len = 0;
	}

	// 检查管道是否存在
	ret= check_name_pipe_exist( point->pipe_name );
	if( ERROR_SUCCESS != ret )
	{
		goto _return; 
	}

	lock_mutex( point->lock ); 
	held_lock = TRUE; 
		
	if( NULL == point->pipe )	// 防止重复建立连接
	{
		ret = connect_to_pipe( point->pipe_name, &point->pipe ); 

		if( ERROR_SUCCESS != ret )	// 如果失败，则m_hConnectPipe一定为NULL
		{
			goto _return; 
		}
	}

	method_len = strlen( method ) + 1; 
	data_send_len = method_len + data_len; 

	send_buf = ( CHAR* )malloc( data_send_len + sizeof( ULONG ) );
	if( NULL == send_buf )
	{
		ret = ERROR_NOT_ENOUGH_MEMORY; 
		goto _return; 
	}

	memcpy( send_buf, &data_send_len, sizeof( ULONG ) ); 

	offset = sizeof( ULONG ); 
	memcpy( send_buf + offset, method, method_len ); 
	offset += method_len; 

	memcpy( send_buf + offset, data, data_len ); 

	ret = write_pipe_sync( point, send_buf, data_send_len + sizeof( ULONG ) ); 
	if( ERROR_SUCCESS != ret )
	{
		goto _return; 
	}

	/**
	* 只要 WritePipeSynchronously 成功，即成功地发送出了数据，
	* 则认为CallProductPipe成功，所以接着等待返回数据的到来
	*/

	//4.... 接收客户端的返回数据

	if( data_ret == NULL )
	{
		goto _return; 
	}

	// 先读取返回数据的长度
	ret = read_pipe_sync( point, ( CHAR* )&_data_ret_len, sizeof( ULONG ) );
	if( ERROR_SUCCESS != ret )
	{
		goto _return; 
	}

	if ( 0 == _data_ret_len)
	{
		// 将out参数置空，释放动态分配的发送缓冲区，关闭管道句柄
		ret = ERROR_ERRORS_ENCOUNTERED; 
		goto _return; 
	}

	// 再读取返回数据
	_data_ret = ( CHAR* )malloc( _data_ret_len );
	if( NULL == _data_ret )
	{
		ret = ERROR_NOT_ENOUGH_MEMORY; 
		goto _return; 
	}

	ret = read_pipe_sync(
		point, 
		( CHAR* )_data_ret, 
		_data_ret_len ); 

	if( ERROR_SUCCESS != ret )
	{
		goto _return; 
	}

	*data_ret = _data_ret; 
	*data_ret_len = _data_ret_len; 

_return:
	if( held_lock == TRUE )
	{
		unlock_mutex( point->lock ); 
	}

	if( ret != ERROR_SUCCESS )
	{
		if( _data_ret != NULL )
		{
			free( _data_ret ); 
		}

		if( send_buf != NULL )
		{
			free( send_buf ); 
		}

		if( point->pipe != NULL )
		{
			CloseHandle( point->pipe );
			point->pipe = NULL;
		}
	}
	else
	{
		ASSERT( send_buf != NULL ); 
		free( send_buf ); 
	}

	return ret;
}

/**
* @brief 建立与服务端命名管道服务器的连接
* @return  0 成功，其他为失败错误码
*/
LRESULT __stdcall connect_to_pipe( LPCWSTR pipe_name, HANDLE *pipe_out )
{
	LRESULT ret; 
	HANDLE _pipe = INVALID_HANDLE_VALUE; 
	INT32 retry_time = 0; 

	ASSERT( pipe_name != NULL ); 
	ASSERT( pipe_out != NULL ); 

	*pipe_out = NULL; 

	ret = check_name_pipe_exist( pipe_name ); 
	if( ERROR_SUCCESS != ret )
	{
		goto _return; 
	}

	do 
	{
		_pipe = CreateFile( pipe_name,		// 命名管道名
			GENERIC_READ | GENERIC_WRITE,	// 管道读写模式：双向
			0, 
			( LPSECURITY_ATTRIBUTES )NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL |
			SECURITY_SQOS_PRESENT |	// 允许服务器在本地系统上识别和模仿该客户
			SECURITY_IMPERSONATION,
			( HANDLE )NULL 
			); 

		if( INVALID_HANDLE_VALUE != _pipe )
		{
			//ret = GetLastError(); 
			break;	// 成功，则 nCount 一定小于 KXEPIPE_MAX_CONNECT_TIMES
		}
		else
		{
			ret = GetLastError(); 
			log_trace( ( DBG_MSG_AND_ERROR_OUT, "connect to " ) ); 
			retry_time ++; 
			Sleep( 1000 ); 
		}

	} while( retry_time != KXEPIPE_MAX_CONNECT_TIMES ); 

	if( retry_time == KXEPIPE_MAX_CONNECT_TIMES )
	{
		if( ret == ERROR_SUCCESS )
		{
			ASSERT( FALSE ); 
			ret = ERROR_ERRORS_ENCOUNTERED; 
		}
		goto _return; 
	}

	*pipe_out = _pipe; 

_return:

#ifdef _DEBUG
	if( ret != ERROR_SUCCESS )
	{
		ASSERT( _pipe == INVALID_HANDLE_VALUE ); 
	}
#endif //_DEBUG
	
	return ret; 
}

/**
* @brief 向管道同步地写入指定数目的数据
* @param[in] pszSendBuffer 发送缓冲区首址
* @param[in] dwSendDataLength 要发送的数据长度
* @return  0 成功，其他为错误码
*/
LRESULT __stdcall write_pipe_sync( 
								  pipe_ipc_point *point, 
								  const BYTE* data, 
								  const ULONG data_len
								  )
{
	LRESULT ret = ERROR_SUCCESS; 
	ULONG writed_once = 0;
	ULONG transfer_point = 0;
	INT32 _ret = FALSE;

	ASSERT( point != NULL ); 
	ASSERT( NULL != point->pipe );

	ASSERT( point != NULL ); 
	ASSERT( point->pipe != NULL ); 

	while( transfer_point < data_len )
	{
		writed_once = 0;
		_ret = WriteFile( point->pipe, 
			( LPCVOID )( data + transfer_point ),	// 每次循环都要调整文件指针
			data_len - transfer_point,
			&writed_once,
			NULL	// 同步方式 
			);

		if( _ret == FALSE )
		{
			ret = GetLastError(); 
			goto _return; 
		}
		else
		{
			transfer_point += writed_once; 
		}
	}

_return:
	return ret;
}

/**
* @brief 从管道同步地读取指定数目的数据
* @param[in, out] pszReceiveBuffer 接收缓冲区首址
* @param[in] dwReceiveDataLength 要接收的数据长度
* @return  0 成功，其他为错误码
*/
LRESULT __stdcall read_pipe_sync(
								 pipe_ipc_point *point, 
								 BYTE* data, 
								 const ULONG data_len )
{
	LRESULT ret = ERROR_SUCCESS;
	DWORD readed_once = 0;
	DWORD transfer_point = 0;
	BOOL _ret; 

	ASSERT( point != NULL ); 
	ASSERT( point->pipe != NULL ); 

	while( transfer_point < data_len )
	{
		readed_once = 0; 

		_ret = ReadFile( 
			point->pipe,
			( LPVOID )( data + transfer_point ),
			data_len - transfer_point,
			&readed_once,
			NULL
			);
		if( _ret == FALSE )
		{
			ret = GetLastError(); 
			goto _return; 
		}
		else
		{
			transfer_point += readed_once; 
		}
	}

_return:
	return ret;
}

LRESULT exec_cmd_on_pipe( LPCWSTR pipe_name, 
						BYTE* cmd, 
						ULONG cmd_len, 
						BYTE **resp, 
						ULONG *resp_len )
{
	LRESULT ret = ERROR_SUCCESS; 
	INT32 i; 
	HANDLE service_pipe = NULL; 
	pipe_ipc_point point = { 0 }; 
	//ULONG data_len; 
	//CHAR *data = NULL; 
	BYTE *ret_data = NULL; 
	ULONG ret_data_len;  

	log_trace( ( MSG_INFO, "enter %s \n", __FUNCTION__ ) );

	ASSERT( pipe_name != NULL ); 
	ASSERT( cmd != NULL ); 
	ASSERT( cmd_len > 0 ); 
	ASSERT( resp != NULL ); 
	ASSERT( resp_len != NULL ); 

	*resp = NULL; 
	*resp_len = 0; 

	ret = init_pipe_point( &point, pipe_name, CLIENT_PIPE, MAX_PIPE_RETRY_TIME ); 
	if( ret != ERROR_SUCCESS )
	{
		goto _return; 
	}

	//data = ( CHAR* )malloc( MAX_PIPE_DATA_LEN ); 
	//if( data == NULL )
	//{
	//	ret = ERROR_NOT_ENOUGH_MEMORY; 
	//	goto _return; 
	//}

	ret = write_pipe_sync( &point, ( CHAR* )&cmd_len, sizeof( cmd_len ) ); 
	if( ret != ERROR_SUCCESS )
	{
		goto _return; 
	}

	ret = write_pipe_sync( &point, cmd, cmd_len ); 
	if( ret != ERROR_SUCCESS )
	{
		goto _return; 
	}

	ret = read_pipe_sync( &point, ( CHAR* )&ret_data_len, sizeof( ULONG ) ); 
	if( ret != ERROR_SUCCESS )
	{
		goto _return; 
	}

	if( ret_data_len == 0 )
	{
		ret = ERROR_ERRORS_ENCOUNTERED; 
		goto _return; 
	}

	ret_data = ( CHAR* )malloc( ret_data_len ); 
	if( ret_data == NULL )
	{
		ret = ERROR_NOT_ENOUGH_MEMORY; 
		goto _return; 
	}

	ret = read_pipe_sync( &point, ret_data, ret_data_len ); 
	if( ret != ERROR_SUCCESS )
	{
		goto _return; 
	}

	*resp = ret_data; 
	*resp_len = ret_data_len; 

_return:
	if( ret != ERROR_SUCCESS )
	{
		if( ret_data != NULL )
		{
			free( ret_data ); 
		}
	}

	if( point.pipe != NULL )
	{
		uninit_pipe_point( &point ); 
	}

	log_trace( ( MSG_INFO, "leave %s return 0x%0.8xs\n", __FUNCTION__, ret ) ); 
	return ret;  
}

//#include <windows.h> 
//#include <stdio.h>
//#include <conio.h>
//#include <tchar.h>
//
//#define BUFSIZE 512
//
//int _tmain(int argc, TCHAR *argv[]) 
//{ 
//	HANDLE hPipe; 
//	LPTSTR lpvMessage=TEXT("Default message from client"); 
//	TCHAR chBuf[BUFSIZE]; 
//	BOOL fSuccess; 
//	DWORD cbRead, cbWritten, dwMode; 
//	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 
//
//	if( argc > 1 )
//		lpvMessage = argv[1];
//
//	// Try to open a named pipe; wait for it, if necessary. 
//
//	while (1) 
//	{ 
//		hPipe = CreateFile( 
//			lpszPipename,   // pipe name 
//			GENERIC_READ |  // read and write access 
//			GENERIC_WRITE, 
//			0,              // no sharing 
//			NULL,           // default security attributes
//			OPEN_EXISTING,  // opens existing pipe 
//			0,              // default attributes 
//			NULL);          // no template file 
//
//		// Break if the pipe handle is valid. 
//
//		if (hPipe != INVALID_HANDLE_VALUE) 
//			break; 
//
//		// Exit if an error other than ERROR_PIPE_BUSY occurs. 
//
//		if (GetLastError() != ERROR_PIPE_BUSY) 
//		{
//			printf("Could not open pipe"); 
//			return 0;
//		}
//
//		// All pipe instances are busy, so wait for 20 seconds. 
//
//		if (!WaitNamedPipe(lpszPipename, 20000)) 
//		{ 
//			printf("Could not open pipe"); 
//			return 0;
//		} 
//	} 
//
//	// The pipe connected; change to message-read mode. 
//
//	dwMode = PIPE_READMODE_MESSAGE; 
//	fSuccess = SetNamedPipeHandleState( 
//		hPipe,    // pipe handle 
//		&dwMode,  // new pipe mode 
//		NULL,     // don't set maximum bytes 
//		NULL);    // don't set maximum time 
//	if (!fSuccess) 
//	{
//		printf("SetNamedPipeHandleState failed"); 
//		return 0;
//	}
//
//	// Send a message to the pipe server. 
//
//	fSuccess = WriteFile( 
//		hPipe,                  // pipe handle 
//		lpvMessage,             // message 
//		(lstrlen(lpvMessage)+1)*sizeof(TCHAR), // message length 
//		&cbWritten,             // bytes written 
//		NULL);                  // not overlapped 
//	if (!fSuccess) 
//	{
//		printf("WriteFile failed"); 
//		return 0;
//	}
//
//	do 
//	{ 
//		// Read from the pipe. 
//
//		fSuccess = ReadFile( 
//			hPipe,    // pipe handle 
//			chBuf,    // buffer to receive reply 
//			BUFSIZE*sizeof(TCHAR),  // size of buffer 
//			&cbRead,  // number of bytes read 
//			NULL);    // not overlapped 
//
//		if (! fSuccess && GetLastError() != ERROR_MORE_DATA) 
//			break; 
//
//		_tprintf( TEXT("%s\n"), chBuf ); 
//	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 
//
//	getch();
//
//	CloseHandle(hPipe); 
//
//	return 0; 
//}

//#include <windows.h> 
//#include <stdio.h> 
//#include <tchar.h>
//
//#define BUFSIZE 4096
//
//VOID InstanceThread(LPVOID); 
//VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD); 
//
//int _tmain(VOID) 
//{ 
//	BOOL fConnected; 
//	DWORD dwThreadId; 
//	HANDLE hPipe, hThread; 
//	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe"); 
//
//	// The main loop creates an instance of the named pipe and 
//	// then waits for a client to connect to it. When the client 
//	// connects, a thread is created to handle communications 
//	// with that client, and the loop is repeated. 
//
//	for (;;) 
//	{ 
//		hPipe = CreateNamedPipe( 
//			lpszPipename,             // pipe name 
//			PIPE_ACCESS_DUPLEX,       // read/write access 
//			PIPE_TYPE_MESSAGE |       // message type pipe 
//			PIPE_READMODE_MESSAGE |   // message-read mode 
//			PIPE_WAIT,                // blocking mode 
//			PIPE_UNLIMITED_INSTANCES, // max. instances  
//			BUFSIZE,                  // output buffer size 
//			BUFSIZE,                  // input buffer size 
//			NMPWAIT_USE_DEFAULT_WAIT, // client time-out 
//			NULL);                    // default security attribute 
//
//		if (hPipe == INVALID_HANDLE_VALUE) 
//		{
//			printf("CreatePipe failed"); 
//			return 0;
//		}
//
//		// Wait for the client to connect; if it succeeds, 
//		// the function returns a nonzero value. If the function
//		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
//
//		fConnected = ConnectNamedPipe(hPipe, NULL) ? 
//TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
//
//		if (fConnected) 
//		{ 
//			// Create a thread for this client. 
//			hThread = CreateThread( 
//				NULL,              // no security attribute 
//				0,                 // default stack size 
//				(LPTHREAD_START_ROUTINE) InstanceThread, 
//				(LPVOID) hPipe,    // thread parameter 
//				0,                 // not suspended 
//				&dwThreadId);      // returns thread ID 
//
//			if (hThread == NULL) 
//			{
//				printf("CreateThread failed"); 
//				return 0;
//			}
//			else CloseHandle(hThread); 
//		} 
//		else 
//			// The client could not connect, so close the pipe. 
//			CloseHandle(hPipe); 
//	} 
//	return 1; 
//} 
//
//VOID InstanceThread(LPVOID lpvParam) 
//{ 
//	TCHAR chRequest[BUFSIZE]; 
//	TCHAR chReply[BUFSIZE]; 
//	DWORD cbBytesRead, cbReplyBytes, cbWritten; 
//	BOOL fSuccess; 
//	HANDLE hPipe; 
//
//	// The thread's parameter is a handle to a pipe instance. 
//
//	hPipe = (HANDLE) lpvParam; 
//
//	while (1) 
//	{ 
//		// Read client requests from the pipe. 
//		fSuccess = ReadFile( 
//			hPipe,        // handle to pipe 
//			chRequest,    // buffer to receive data 
//			BUFSIZE*sizeof(TCHAR), // size of buffer 
//			&cbBytesRead, // number of bytes read 
//			NULL);        // not overlapped I/O 
//
//		if (! fSuccess || cbBytesRead == 0) 
//			break; 
//		GetAnswerToRequest(chRequest, chReply, &cbReplyBytes); 
//
//		// Write the reply to the pipe. 
//		fSuccess = WriteFile( 
//			hPipe,        // handle to pipe 
//			chReply,      // buffer to write from 
//			cbReplyBytes, // number of bytes to write 
//			&cbWritten,   // number of bytes written 
//			NULL);        // not overlapped I/O 
//
//		if (! fSuccess || cbReplyBytes != cbWritten) break; 
//	} 
//
//	// Flush the pipe to allow the client to read the pipe's contents 
//	// before disconnecting. Then disconnect the pipe, and close the 
//	// handle to this pipe instance. 
//
//	FlushFileBuffers(hPipe); 
//	DisconnectNamedPipe(hPipe); 
//	CloseHandle(hPipe); 
//}
//
//VOID GetAnswerToRequest(LPTSTR chRequest, 
//						LPTSTR chReply, LPDWORD pchBytes)
//{
//	_tprintf( TEXT("%s\n"), chRequest );
//	lstrcpy( chReply, TEXT("Default answer from server") );
//	*pchBytes = (lstrlen(chReply)+1)*sizeof(TCHAR);
//}
