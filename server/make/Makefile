#server.makefile
VPATH=../:../../hpsocket:../../hpsocket/common:../../global

objects=server.o helper.o TcpServer.o BufferPool.o Event.o IODispatcher.o RWLock.o SysHelper.o SocketHelper.o  FileHelper.o MiscHelper.o FuncHelper.o

server.out:$(objects)
	g++ -std=c++1y -o server.out -pthread $(objects)
server.o:server.cpp helper.h TcpServer.h
	g++ -std=c++1y -c $< -I../
helper.o:helper.cpp helper.h FileHelper.h
	g++ -std=c++1y -c $< -I../../global
TcpServer.o:TcpServer.cpp TcpServer.h FileHelper.h GeneralHelper.h SocketHelper.h
	g++ -std=c++1y -c $< -I../../hpsocket

FuncHelper.o:FuncHelper.cpp FuncHelper.h
	g++ -std=c++1y -c $< -I../../hpsocket/common
BufferPool.o:BufferPool.cpp BufferPool.h
	g++ -std=c++1y -c $< -I../../hpsocket/common 	
Event.o:Event.cpp Event.h
	g++ -std=c++1y -c $< -I../../hpsocket/common
IODispatcher.o:IODispatcher.cpp IODispatcher.h
	g++ -std=c++1y -c $< -I../../hpsocket/common
RWLock.o:RWLock.cpp RWLock.h
	g++ -std=c++1y -c $< -I../../hpsocket/common
SysHelper.o:SysHelper.cpp SysHelper.h
	g++ -std=c++1y -c $< -I../../hpsocket/common


SocketHelper.o:SocketHelper.cpp SocketHelper.h StringT.h
	g++ -std=c++1y -c $< -I../../hpsocket
FileHelper.o:FileHelper.cpp FileHelper.h 
	g++ -std=c++1y -c $< -I../../hpsocket/common
MiscHelper.o:MiscHelper.cpp MiscHelper.h
	g++ -std=c++1y -c $< -I../../hpsocket
.PHONY:clean
clean:
	-rm -rf $(objects) server.out
