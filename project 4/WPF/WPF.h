#ifndef WPF_H
#define WPF_H
////////////////////////////////////////////////////////////////////
// WPF.h - C++\CLI implementation of WPF Application              //
//       - Demo for CSE 687 Project #4                            //
// ver 2.2                                                        //
// Language:    C++, Visual Studio 2015 Community                 //
// Platform:	Dell Inspiron 13-7352, Windows 10 Education		  //
// Application: Object Oriented Design, Spring 2016               //
// Author:		Yilan Li, Syracuse University, (315)382-9302      //
//				yli41@syr.edu									  //
// Source:      Jim Fawcett, Syracuse University, CST 4-187       //
//              jfawcett@twcny.rr.com                             //
////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that you may need for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.  So this Demo simulates
*  operations you will need to execute for Project #4.
*
*  Required Files:
*  ---------------
*  WPF.h, WPF.cpp, MochChannel.h
*  HTTPMessage.h HTTPMessage.xpp
*
*  Build Command:
*  --------------
*  devenv Project_4.sln /rebuild debug
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 2.2 : 30 Apr 2016 by Yilan Li
*  - added hUploadFolderBrowseButton to UploadView
*  - added hDownloadFolderBrowseButton to DownloadView
*  - added hAddDepFolderBrowseButton and hAddDepFolderBrowseButtonParent to AddDependencyView
*  - added hDisplayFolderBrowseButton to DisplayDependencyView
*  ver 2.1 : 27 Apr 2016 by Yilan Li
*  - added Upload, Download, Add Dependency and Display Dependency tabs
*  - added functions to set up difernet views
		void setUploadView();
		void setDownloadView();
		void setAddDependencyView();
		void setDisplayDependencyView();
*  ver 2.0 : 15 Apr 2015 by Jim Fawcett
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015 by Jim Fawcett
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     MockChannel
*     System
*     System.Core
*     System.Windows.Presentation
*     System.Windows.Forms
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
	ref class WPFCppCliDemo : Window
	{
		// MockChannel references
		ISendr* pSendr_;
		IRecvr* pRecvr_;
		IMockChannel* pChann_;

		// Controls for Window
		DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
		Grid^ hGrid = gcnew Grid();
		TabControl^ hTabControl = gcnew TabControl();

		//TabItem^ hSendMessageTab = gcnew TabItem();
		TabItem^ hUploadFileTab = gcnew TabItem();
		TabItem^ hDownloadFileTab = gcnew TabItem();
		TabItem^ hAddDependencyTab = gcnew TabItem();
		TabItem^ hDisplayDependencyTab = gcnew TabItem();

		TabItem^ hFileListTab = gcnew TabItem();
		TabItem^ hConnectTab = gcnew TabItem();
		StatusBar^ hStatusBar = gcnew StatusBar();
		StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
		TextBlock^ hStatus = gcnew TextBlock();

		// Controls for UPLOAD View
		Grid^ hUploadSendMessageGrid = gcnew Grid();
		Button^ hUploadButton = gcnew Button();
		Button^ hUploadClearButton = gcnew Button();
		Forms::OpenFileDialog^ hUploadFileBrowserDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hUploadListBox = gcnew ListBox();
		Button^ hUploadFolderBrowseButton = gcnew Button();
		CheckBox^ hUploadCheckinBox = gcnew CheckBox();
		TextBlock^ hUploadTextlock = gcnew TextBlock();
		TextBlock^ hUploadTextBlockFrom = gcnew TextBlock();			
		TextBox^ hUploadTextBoxFrom = gcnew TextBox();
		TextBlock^ hUploadTextBlockTo = gcnew TextBlock();				
		TextBox^ hUploadTextBoxTo = gcnew TextBox();
		TextBlock^ hUploadTextBlockLocalPort = gcnew TextBlock();		
		TextBox^ hUploadTextBoxLocalPort = gcnew TextBox();
		TextBlock^ hUploadTextBlockRemotePort = gcnew TextBlock();		
		TextBox^ hUploadTextBoxRemotePort = gcnew TextBox();
		TextBlock^ hUploadTextBlockFileName = gcnew TextBlock();		
		TextBox^ hUploadTextBoxFileName = gcnew TextBox();
		ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
		StackPanel^ hUploadStackPanelLocal = gcnew StackPanel();		
		StackPanel^ hUploadStackPanelRemote = gcnew StackPanel();		
		StackPanel^ hUploadStackPanelFile = gcnew StackPanel();			 
		StackPanel^ hUploadStackPanelUpload = gcnew StackPanel();		

		// Controls for DOWNLOAD View
		Grid^ hDownloadSendMessageGrid = gcnew Grid();
		Button^ hDownloadButton = gcnew Button();
		Button^ hDownloadClearButton = gcnew Button();
		Forms::OpenFileDialog^ hDownloadFileBrowserDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hDownloadListBox = gcnew ListBox();
		Button^ hDownloadFolderBrowseButton = gcnew Button();
		TextBlock^ hDownloadTextBlockFile1 = gcnew TextBlock();
		TextBlock^ hDownloadTextBlockFile = gcnew TextBlock();			
		TextBlock^ hDownloadTextBlockResult1 = gcnew TextBlock();
		TextBlock^ hDownloadTextBlockResult = gcnew TextBlock();		
		TextBlock^ hDownloadTextBlockFrom = gcnew TextBlock();			
		TextBox^ hDownloadTextBoxFrom = gcnew TextBox();
		TextBlock^ hDownloadTextBlockTo = gcnew TextBlock();			
		TextBox^ hDownloadTextBoxTo = gcnew TextBox();
		TextBlock^ hDownloadTextBlockLocalPort = gcnew TextBlock();		
		TextBox^ hDownloadTextBoxLocalPort = gcnew TextBox();
		TextBlock^ hDownloadTextBlockRemotePort = gcnew TextBlock();	
		TextBox^ hDownloadTextBoxRemotePort = gcnew TextBox();
		TextBlock^ hDownloadTextBlockFileName = gcnew TextBlock();		
		TextBox^ hDownloadTextBoxFileName = gcnew TextBox();
		ScrollViewer^ hDownloadScrollViewer2 = gcnew ScrollViewer();
		StackPanel^ hDownloadStackPanelLocal = gcnew StackPanel();		
		StackPanel^ hDownloadStackPanelRemote = gcnew StackPanel();		
		StackPanel^ hDownloadStackPanelFile = gcnew StackPanel();		
		StackPanel^ hDownloadStackPanelResult = gcnew StackPanel();		
		StackPanel^ hDownloadStackPanel = gcnew StackPanel();			

		// Controls for ADD DEPENDENCY View
		Grid^ hAddDependencyGrid = gcnew Grid();
		Button^ hAddDependencyPackagesButton = gcnew Button();
		Button^ hAddDependencyButton = gcnew Button();
		Button^ hAddDependencyClearButton = gcnew Button();
		Forms::OpenFileDialog^ hAddDepFileBrowserDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hAddDepListBox = gcnew ListBox();
		Button^ hAddDepFolderBrowseButton = gcnew Button();	
		Forms::OpenFileDialog^ hAddDepFileParentBrowserDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hAddDepParentListBox = gcnew ListBox();
		Button^ hAddDepFolderBrowseButtonParent = gcnew Button();
		TextBlock^ hAddDependencyBlockChildFile1 = gcnew TextBlock();
		TextBlock^ hAddDependencyBlockChildFile = gcnew TextBlock();			
		TextBlock^ hAddDependencyBlockParentFile1 = gcnew TextBlock(); 
		TextBlock^ hAddDependencyBlockParentFile = gcnew TextBlock();			
		TextBlock^ hAddDependencyBlockResult1 = gcnew TextBlock();
		TextBlock^ hAddDependencyBlockResult = gcnew TextBlock();
		TextBlock^ hAddDependencyTextBlockFrom = gcnew TextBlock();				
		TextBox^ hAddDependencyTextBoxFrom = gcnew TextBox();
		TextBlock^ hAddDependencyTextBlockTo = gcnew TextBlock();				
		TextBox^ hAddDependencyTextBoxTo = gcnew TextBox();
		TextBlock^ hAddDependencyTextBlockLocalPort = gcnew TextBlock();		
		TextBox^ hAddDependencyTextBoxLocalPort = gcnew TextBox();
		TextBlock^ hAddDependencyTextBlockRemotePort = gcnew TextBlock();		
		TextBox^ hAddDependencyTextBoxRemotePort = gcnew TextBox();
		ScrollViewer^ hAddDependencyScrollViewer1 = gcnew ScrollViewer();		
		StackPanel^ hAddDependencyStackPanelLocal = gcnew StackPanel();			
		StackPanel^ hAddDependencyStackPanelRemote = gcnew StackPanel();		
		StackPanel^ hAddDependencyStackPanelChildFile = gcnew StackPanel();		 
		StackPanel^ hAddDependencyStackPanelParentFile = gcnew StackPanel();	
		StackPanel^ hAddDependencyStackPanelResult = gcnew StackPanel();
		StackPanel^ hAddDependencyStackPanel = gcnew StackPanel();				 

		// Controls for Display Dependency View
		Grid^ hDisplayDependencyGrid = gcnew Grid();
		Button^ hDisplayDependencyButton = gcnew Button();
		Button^ hDisplayDependencyClearButton = gcnew Button();
		Forms::OpenFileDialog^ hDisplayFileBrowserDialog = gcnew Forms::OpenFileDialog();
		ListBox^ hDisplayListBox = gcnew ListBox();
		Button^ hDisplayFolderBrowseButton = gcnew Button();
		CheckBox^ hDisplayWithDepBox = gcnew CheckBox();
		TextBlock^ hDisplayDependencyTextBlockFrom = gcnew TextBlock();				
		TextBox^ hDisplayDependencyTextBoxFrom = gcnew TextBox();
		TextBlock^ hDisplayDependencyTextBlockTo = gcnew TextBlock();				
		TextBox^ hDisplayDependencyTextBoxTo = gcnew TextBox();
		TextBlock^ hDisplayDependencyTextBlockLocalPort = gcnew TextBlock();		
		TextBox^ hDisplayDependencyTextBoxLocalPort = gcnew TextBox();
		TextBlock^ hDisplayDependencyTextBlockRemotePort = gcnew TextBlock();		
		TextBox^ hDisplayDependencyTextBoxRemotePort = gcnew TextBox();
		TextBlock^ hDisplayDependencyTextBlockFileName = gcnew TextBlock();			
		TextBox^ hDisplayDependencyTextBoxFileName = gcnew TextBox();
		TextBlock^ hDisplayDependencyResult = gcnew TextBlock();
		ScrollViewer^ hDisplayDependencyScrollViewer = gcnew ScrollViewer();
		StackPanel^ hDisplayDependencyStackPanelLocal = gcnew StackPanel();			
		StackPanel^ hDisplayDependencyStackPanelRemote = gcnew StackPanel();		
		StackPanel^ hDisplayDependencyStackPanelFile = gcnew StackPanel();			 
		StackPanel^ hDisplayDependencyStackPanelFile1 = gcnew StackPanel();			
		StackPanel^ hDisplayDependencyStackPanel = gcnew StackPanel();

		// Controls for FileListView View
		Grid^ hFileListGrid = gcnew Grid();
		Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
		ListBox^ hListBox = gcnew ListBox();
		Button^ hFolderBrowseButton = gcnew Button();
		Grid^ hGrid2 = gcnew Grid();

		// receive thread
		Thread^ recvThread;
	public:
		WPFCppCliDemo();
		~WPFCppCliDemo();
		void setUpStatusBar();
		void setUpTabControl();
		void setUploadView();
		void setDownloadView();
		void setAddDependencyView();
		void setDisplayDependencyView();
		void setUpFileListView();
		void setUpConnectionView();
		//upload
		void sendUploadMessage(Object^ obj, RoutedEventArgs^ args);
		void browseForUploadFolder(Object^ sender, RoutedEventArgs^ args);
		void uploadclear(Object^ sender, RoutedEventArgs^ args);
		//download
		void sendDownloadMessage(Object^ obj, RoutedEventArgs^ args);
		void browseForDownloadFolder(Object^ sender, RoutedEventArgs^ args);
		void downloadclear(Object^ sender, RoutedEventArgs^ args);
		//add dependency
		void sendAddDependencyMessage(Object^ sender, RoutedEventArgs^ args);
		void browseForAddDependencyFolder(Object^ sender, RoutedEventArgs^ args);
		void browseForAddDependencyParentFolder(Object^ sender, RoutedEventArgs^ args);
		void adddependencyclear(Object^ sender, RoutedEventArgs^ args);
		//display dependency
		void sendShowDependencyMessage(Object^ sender, RoutedEventArgs^ args);
		void browseForDisplayDependencyFolder(Object^ sender, RoutedEventArgs^ args);
		void displaydependencyclear(Object^ sender, RoutedEventArgs^ args);

		void addUploadText(String^ msg);
		void addDownloadText(String^ msg);
		void addAddDependencyText(String^ msg);
		void addDisplayDependencyText(String^ msg);
		void getMessage();
		void browseForFolder(Object^ sender, RoutedEventArgs^ args);
		void OnLoaded(Object^ sender, RoutedEventArgs^ args);
		void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);
	private:
		std::string toStdString(String^ pStr);
		String^ toSystemString(std::string& str);
		List<String^>^ vServerInfo = gcnew List < String^ >;
		List<String^ > ^ vFileInfo = gcnew List<String^>;
		String^ getFilename(String^ name);

		//upload
		void setUploadLocalHostProperties();
		Border^ createUploadLocalPortBlock();
		Border^ createUploadLocalHostBlock();
		void setUploadRemoteHostProperties();
		Border^ createUploadRemotePortBlock();
		Border^ createUploadRemoteHostBlock();
		void setUploadFileNameTextBlockProperties();
		Border^ createCheckinBox();
		void setUploadTextBoxProperties();
		void setUploadButtonProperties();

		//download
		void setDownloadLocalHostProperties();
		Border^ createDownloadLocalPortBlock();
		Border^ createDownloadLocalHostBlock();
		void setDownloadRemoteHostProperties();
		Border^ createDownloadRemotePortBlock();
		Border^ createDownloadRemoteHostBlock();
		void setDownloadFileResultProperties();
		void setDownloadFileResultBlockProperties();
		void setDownloadServerFilelistProperties();
		void setDownloadButtonProperties();

		//add dependency
		void setAddDependencyLocalHostProperties();
		Border^ createAddDependencyLocalPortBlock();
		Border^ createAddDependencyLocalHostBlock();
		void setAddDependencyRemoteHostProperties();
		Border^ createAddDependencyRemotePortBlock();
		Border^ createAddDependencyRemoteHostBlock();
		void setAddDependencyFileProperties();
		void setAddDependencyParentFileProperties();
		void setAddDependencyPackageProperties();
		void setAddDependencyPackageListProperties();
		void setAddDependencyButtonProperties();

		//display dependency
		void setDisplayDependencyLocalHostProperties();
		Border^ createDisplayDependencyLocalPortBlock();
		Border^ createDisplayDependencyLocalHostBlock();
		void setDisplayDependencyRemoteHostProperties();
		Border^ createDisplayDependencyRemotePortBlock();
		Border^ createDisplayDependencyRemoteHostBlock();
		void setDisplayDependencyFileNameProperties();
		Border^ createWithDepCheckBox();
		void setDisplayDependencyResultProperties();
		void setDisplayDependencyButtonProperties();

		// set buttons
		void SetButtons();
		// setup Window controls and views
		void SetWindowControlsandViews();
	};
}

#endif