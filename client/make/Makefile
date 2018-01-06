#server.makefile
VPATH=../:../../hpsocket:../../hpsocket/common:../../global

objects=client.o helper.o TcpClient.o FuncHelper.o BufferPool.o Event.o IODispatcher.o RWLock.o SysHelper.o SocketHelper.o  FileHelper.o MiscHelper.o PollHelper.o

client.out:$(objects)
	g++ -std=c++1y -o client.out -pthread $(objects)
client.o:client.cpp helper.h TcpClient.h
	g++ -std=c++1y -c $< -I../
helper.o:helper.cpp helper.h FileHelper.h
	g++ -std=c++1y -c $< -I../../global
TcpClient.o:TcpClient.cpp TcpClient.h FileHelper.h GeneralHelper.h SocketHelper.h
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
PollHelper.o:PollHelper.cpp PollHelper.h FuncHelper.h GlobalDef.h 
	g++ -std=c++1y -c $< -I../../hpsocket/common
.PHONY:clean
clean:
	-rm -rf $(objects) client.out
