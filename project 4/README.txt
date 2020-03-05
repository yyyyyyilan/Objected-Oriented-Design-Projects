This is CSE687 Project 4 Spring 2016: Dependency-Based Remote Code Repository.

This is declaration of Requirement ONE.
=======================================
Project #4   Dependency-Based Remote Code Repository
Yilan Li     Syracuse University
yli41@syr.edu
Platform: Dell Inspiron 13-7352, Windows 10 Education
Language: C++, Visual Studio 2015 Community


This is declaration of Requirement TWO.
=======================================
No use of the C language I/O, e.g. printf, scanf, etc, nor the C memory management,
e.g. calloc, malloc, or free.


This is descriptions of all packages.
=====================================
------------------
Packages included:
------------------
- HTTPMessage: Contains class HTTPMessage to provide methods to manipulate HTTP-style request
	       or response transactions.
	       HTTPMessage.h HTTPMessage.cpp
- Channel: Contains class ChannelRecvr, class ChannelSender and class Channel to provide a 
	   message-passing communication system. It can be used to access the Repository's
           functionality from another process or machine.
 	   Channel.h Channel.cpp
- Client: Contains class ClientSenderHandler to provide different handlings of different 
	  commands when sending and receiving messages, which are operating on client side.
	  Client.h Client.cpp
- Server: Contains class ServerRcvHandler to provide different handlings of different commands
	  when sending and receiving messages, which are operating on Server side.
	  Server.h Server.cpp
- MockChannel: Contains class MockChannel to provide a transaction between client and GUI.
	       It reads from send queue and writes to receive queue.
	       MockChannel.h MockChannel.cpp
- WPF:Contains class WPFCppCliDemo to build a C++\CLI WPF application.
      WPF.h WPF.cpp
- XmlDocument: Contains class AbstractXmlElement, class DocElement, class TextElement, class
	       TaggedElement, class CommentElement, class ProcInstrElement, class XmlDeclarElement.
	       It's wrapper around an AST used to contain the results of parsing SML makrup.
	       XmlDocument.h XmlDocument.cpp
- BlockingQueue: Contains a thread-safe class: BlockingQueue<T> to support sending messages 
	         between threads.
                 BlockingQueue.h BlockingQueue.cpp
- FileMgr: Contains class Block, class File, class FileInfo, class Path and class Directory.
	   It's used to support file and directory operations. 
	   FileSystem.h FileSystem.cpp IFileMgr.h FileMgr.h FileMgr.cpp
- Sockets: Contains class Socket, class SocketConnecter, class SocketListener and class SocketSystem.
	   They'are used as a wrapper for win32 socket API.
	   Logger.h Logger.cpp Socket.h Socket.cpp SocketsUtilities.h SocketsUtilities.cpp
	   WindowsHelpers.h WindowsHelpers.cpp
- RemoteCodeRepositoryExecutive: This is an automated unit test suite to demonstrate that all requirements are
			implemented. 
 			RemoteCodeRepositoryExecutive.h RemoteCodeRepositoryExecutive.cpp
- ServerExecutive: This is server test suit.

-------------
Build Command
-------------
devenv Project_4.sln /rebuild debug

-----------
Run Command
-----------
start .\Debug\WPF.exe 8080
start .\Debug\ServerEexcutive.exe 8080

----------
Reference
----------
http://ecs.syr.edu/faculty/fawcett/handouts/CSE687/Lectures/Project4-S2016.htm


This is declaration of Requirement THREE and Five.
==================================================
When in checkin process, metadata of .one file is created firstly.
The created metadata is shown below.


This is declaration of Requirement FOUR.
========================================
The dependency is added manually.


This is declaration of Requirement SIX.
=======================================
A boolean checkin_ private data memeber in class ServerRcvHandler is used to control the checkin
property. When file is the checkin process and checkin_ = open, file can be added to
the content. Once it's finishes checkin process and checkin_ = false, changes can't be made to the
contents.


This is declaration of Requirement SEVEN.
=========================================
This is done by DisplayDependencyHandling. If option = true, packages with all depenndencies 
will be extracted. If option = false, only files in this package will be eatracted.


This is declaration of Requirement EIGHT & NINE.
================================================
This message-passing communication system is created by class Channel. It passes HTTP style messages.
ClientOne sends an HTTPMessage one and ClientTwo sends an HTTPMessage two. Both two HTTPMessages are
sent to Server Test.


This is declaration of Requirement TEN.
=======================================
Client Ten sends file ClientReuqirment10.cpp to server and download file from server.
It used to demonstrate thecommunication system also supports sending and receiving streams of bytes.


This is declaration of Requirement ELEVEN.
==========================================
Set RemoteCodeRepositoryExecutive and ServerExecutive both as starting projects.