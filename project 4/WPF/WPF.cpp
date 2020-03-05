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
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/

#include "WPF.h"
using namespace CppCliWindows;

//----< Class WPFCppCliDemo constructor >--------------------------------------

WPFCppCliDemo::WPFCppCliDemo()
{
	// set up channel
	ObjectFactory* pObjFact = new ObjectFactory;
	pSendr_ = pObjFact->createSendr();
	pRecvr_ = pObjFact->createRecvr();
	pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_);
	pChann_->start();
	delete pObjFact;

	// client's receive thread
	recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
	recvThread->Start();

	// set event handlers
	this->Loaded +=
		gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
	this->Closing +=
		gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
	
	//set buttons
	SetButtons();

	// set Window properties
	this->Title = "Dependency-Based Remote Code Repository ***  Yilan Li ";
	this->Width = 800;
	this->Height = 600;

	// attach dock panel to Window
	this->Content = hDockPanel;
	hDockPanel->Children->Add(hStatusBar);
	hDockPanel->SetDock(hStatusBar, Dock::Bottom);
	hDockPanel->Children->Add(hGrid);

	// setup Window controls and views
	SetWindowControlsandViews();
}
//----< helper function to set buttons >---------------------------------------

void WPFCppCliDemo::SetButtons()
{
	//set buttons
	hUploadFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForUploadFolder);
	hUploadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendUploadMessage);
	hUploadClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::uploadclear);

	hDownloadFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForDownloadFolder);
	hDownloadButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendDownloadMessage);
	hDownloadClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::downloadclear);

	hAddDepFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForAddDependencyFolder);
	hAddDepFolderBrowseButtonParent->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForAddDependencyParentFolder);
	hAddDependencyButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendAddDependencyMessage);
	hAddDependencyClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::adddependencyclear);

	hDisplayFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForDisplayDependencyFolder);
	hDisplayDependencyButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendShowDependencyMessage);
	hDisplayDependencyClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::displaydependencyclear);

	hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
}
//----< helper function to set window control and views >----------------------

void WPFCppCliDemo::SetWindowControlsandViews()
{
	// setup Window controls and views
	setUpTabControl();
	setUpStatusBar();
	setUploadView();
	setDownloadView();
	setAddDependencyView();
	setDisplayDependencyView();
	setUpFileListView();
	setUpConnectionView();
}
//----< Class WPFCppCliDemo destructor >---------------------------------------

WPFCppCliDemo::~WPFCppCliDemo()
{
	delete pChann_;
	delete pSendr_;
	delete pRecvr_;
}
//----< set ustatus bar properties >-------------------------------------------

void WPFCppCliDemo::setUpStatusBar()
{
	hStatusBar->Items->Add(hStatusBarItem);
	hStatus->Text = "very important messages will appear here";
	hStatusBarItem->Content = hStatus;
	hStatusBar->Padding = Thickness(10, 2, 10, 2);
}
//----< set tab control properties >-------------------------------------------

void WPFCppCliDemo::setUpTabControl()
{
	hGrid->Children->Add(hTabControl);
	hUploadFileTab->Header = "Upload";
	hDownloadFileTab->Header = "Dowanload";
	hAddDependencyTab->Header = "Add Dependency";
	hDisplayDependencyTab->Header = "Display Dependency";
	hFileListTab->Header = "File List";
	hConnectTab->Header = "Connect";
	hTabControl->Items->Add(hUploadFileTab);
	hTabControl->Items->Add(hDownloadFileTab);
	hTabControl->Items->Add(hAddDependencyTab);
	hTabControl->Items->Add(hDisplayDependencyTab);
	hTabControl->Items->Add(hFileListTab);
	hTabControl->Items->Add(hConnectTab);
}
//----< set upload view >------------------------------------------------------

void WPFCppCliDemo::setUploadView()
{
	Console::Write("\n  setting up upload view");
	hUploadSendMessageGrid->Margin = Thickness(20);
	hUploadFileTab->Content = hUploadSendMessageGrid;
	setUploadLocalHostProperties();
	setUploadRemoteHostProperties();
	setUploadFileNameTextBlockProperties();
	setUploadTextBoxProperties();
	setUploadButtonProperties();
}
//----< set upload local properties >-----------------------------------------

void WPFCppCliDemo::setUploadLocalHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hUploadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createUploadLocalHostBlock();
	Border^ hBorder2 = createUploadLocalPortBlock();
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadTextBoxFrom;
	hUploadTextBoxFrom->Text = "localhost";
	hUploadTextBoxFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBoxFrom->FontWeight = FontWeights::Bold;
	hUploadTextBoxFrom->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hUploadTextBoxLocalPort;
	hUploadTextBoxLocalPort->Text = "6060";
	hUploadTextBoxLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBoxLocalPort->FontWeight = FontWeights::Bold;
	hUploadTextBoxLocalPort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hUploadStackPanelLocal->Children->Add(hBorder1);
	hUploadStackPanelLocal->Children->Add(hBorder3);
	hUploadStackPanelLocal->Children->Add(hSpace);
	hUploadStackPanelLocal->Children->Add(hBorder2);
	hUploadStackPanelLocal->Children->Add(hBorder4);
	hUploadStackPanelLocal->Orientation = Orientation::Horizontal;
	hUploadStackPanelLocal->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hUploadSendMessageGrid->SetRow(hUploadStackPanelLocal, 0);
	hUploadSendMessageGrid->Children->Add(hUploadStackPanelLocal);
}
//----< helper function to  create local port block >--------------------------

Border^ WPFCppCliDemo::createUploadLocalPortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadTextBlockLocalPort;
	hUploadTextBlockLocalPort->Text = "Local Port";
	hUploadTextBlockLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBlockLocalPort->FontWeight = FontWeights::Bold;
	hUploadTextBlockLocalPort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create local host block >---------------------------

Border^ WPFCppCliDemo::createUploadLocalHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hUploadTextBlockFrom;
	hUploadTextBlockFrom->Text = "Local Host";
	hUploadTextBlockFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBlockFrom->FontWeight = FontWeights::Bold;
	hUploadTextBlockFrom->FontSize = 12;
	return hBorder1;
}
//----< set upload remote properties >-----------------------------------------

void WPFCppCliDemo::setUploadRemoteHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hUploadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createUploadRemoteHostBlock();
	Border^ hBorder2 = createUploadRemotePortBlock();
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadTextBoxTo;
	hUploadTextBoxTo->Text = "localhost";
	hUploadTextBoxTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBoxTo->FontWeight = FontWeights::Bold;
	hUploadTextBoxTo->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hUploadTextBoxRemotePort;
	hUploadTextBoxRemotePort->Text = "8080";
	hUploadTextBoxRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBoxRemotePort->FontWeight = FontWeights::Bold;
	hUploadTextBoxRemotePort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hUploadStackPanelRemote->Children->Add(hBorder1);
	hUploadStackPanelRemote->Children->Add(hBorder3);
	hUploadStackPanelRemote->Children->Add(hSpace);
	hUploadStackPanelRemote->Children->Add(hBorder2);
	hUploadStackPanelRemote->Children->Add(hBorder4);
	hUploadStackPanelRemote->Orientation = Orientation::Horizontal;
	hUploadStackPanelRemote->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hUploadSendMessageGrid->SetRow(hUploadStackPanelRemote, 1);
	hUploadSendMessageGrid->Children->Add(hUploadStackPanelRemote);
}
//----< helper function to create remote port block >--------------------------

Border^ WPFCppCliDemo::createUploadRemotePortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadTextBlockRemotePort;
	hUploadTextBlockRemotePort->Text = "Remote Port";
	hUploadTextBlockRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBlockRemotePort->FontWeight = FontWeights::Bold;
	hUploadTextBlockRemotePort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create remote host block >--------------------------

Border^ WPFCppCliDemo::createUploadRemoteHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hUploadTextBlockTo;
	hUploadTextBlockTo->Padding = Thickness(0);
	hUploadTextBlockTo->Text = "Remote Host";
	hUploadTextBlockTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBlockTo->FontWeight = FontWeights::Bold;
	hUploadTextBlockTo->FontSize = 12;
	return hBorder1;
}
//----< set upload file properties >-------------------------------------------

void WPFCppCliDemo::setUploadFileNameTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hUploadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 80;
	hBorder1->Height = 40;
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hUploadTextBlockFileName;
	hUploadTextBlockFileName->Text = "File Name";
	hUploadTextBlockFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextBlockFileName->FontWeight = FontWeights::Bold;
	hUploadTextBlockFileName->FontSize = 12;
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 420;
	hBorder2->Height = 40;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadListBox;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 18;
	hUploadStackPanelFile->Children->Add(hBorder1);
	hUploadStackPanelFile->Children->Add(hSpace);
	hUploadStackPanelFile->Children->Add(hBorder2);
	TextBlock^ hSpace1 = gcnew TextBlock();
	hSpace1->Width = 15;
	hUploadStackPanelFile->Children->Add(hSpace1);
	hUploadFolderBrowseButton->Content = "Select File";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 100;
	hBorder3->Height = 40;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadFolderBrowseButton;
	hUploadStackPanelFile->Children->Add(hBorder3);
	TextBlock^ hSpace2 = gcnew TextBlock();
	hSpace2->Width = 8;
	hUploadStackPanelFile->Children->Add(hSpace2);	
	Border^ hBorder4 = createCheckinBox();
	hUploadStackPanelFile->Children->Add(hBorder4);
	hUploadStackPanelFile->Orientation = Orientation::Horizontal;
	hUploadStackPanelFile->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hUploadSendMessageGrid->SetRow(hUploadStackPanelFile, 2);
	hUploadSendMessageGrid->Children->Add(hUploadStackPanelFile);
}
//----< helper function to create upload checkin box >-------------------------

Border^ WPFCppCliDemo::createCheckinBox()
{
	hUploadCheckinBox->Content = "CheckinDone";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 92;
	hBorder4->Height = 20;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hUploadCheckinBox;
	return hBorder4;
}
//----< set upload reply message properties >----------------------------------

void WPFCppCliDemo::setUploadTextBoxProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hUploadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder = gcnew Border();
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hUploadTextlock;
	hUploadTextlock->Padding = Thickness(15);
	std::string c = toStdString(" Please select one file from ../Test/ClientTest/ "
		+ "\n\n CheckinDone: Selected when this is the last file that will be checked in"
		+ "\n\n Saved Metadata is shown in Server Console"
		+ "\n\n Client send HTTP mesage is shown in Client Console"
		+"\n\n Server reply HTTP message is shown in Client Console");
	hUploadTextlock->Text = toSystemString(c);
	hUploadTextlock->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hUploadTextlock->FontWeight = FontWeights::Bold;
	hUploadTextlock->FontSize = 12;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder;
	hUploadSendMessageGrid->SetRow(hScrollViewer1, 3);
	hUploadSendMessageGrid->Children->Add(hScrollViewer1);
}
//----< set upload button properties >-----------------------------------------

void WPFCppCliDemo::setUploadButtonProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hUploadSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hUploadButton->Content = "UPLOAD";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 110;
	hBorder2->Height = 50;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hUploadButton;
	hUploadClearButton->Content = "CLEAR";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 110;
	hBorder3->Height = 50;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadClearButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 210;
	hUploadStackPanelUpload->Children->Add(hBorder2);
	hUploadStackPanelUpload->Children->Add(hSpacer);
	hUploadStackPanelUpload->Children->Add(hBorder3);
	hUploadStackPanelUpload->Orientation = Orientation::Horizontal;
	hUploadStackPanelUpload->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadSendMessageGrid->SetRow(hUploadStackPanelUpload, 4);
	hUploadSendMessageGrid->Children->Add(hUploadStackPanelUpload);
}
//----< set download view >----------------------------------------------------

void WPFCppCliDemo::setDownloadView()
{
	Console::Write("\n  setting up download view");
	hDownloadSendMessageGrid->Margin = Thickness(20);
	hDownloadFileTab->Content = hDownloadSendMessageGrid;
	setDownloadLocalHostProperties();
	setDownloadRemoteHostProperties();
	setDownloadServerFilelistProperties();
	setDownloadFileResultBlockProperties();
	setDownloadFileResultProperties();
	setDownloadButtonProperties();
}
//----< set download local properties >----------------------------------------

void WPFCppCliDemo::setDownloadLocalHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createDownloadLocalHostBlock();
	Border^ hBorder2 = createDownloadLocalPortBlock();
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadTextBoxFrom;
	hDownloadTextBoxFrom->Text = "localhost";
	hDownloadTextBoxFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxFrom->FontWeight = FontWeights::Bold;
	hDownloadTextBoxFrom->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDownloadTextBoxLocalPort;
	hDownloadTextBoxLocalPort->Text = "6060";
	hDownloadTextBoxLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxLocalPort->FontWeight = FontWeights::Bold;
	hDownloadTextBoxLocalPort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hDownloadStackPanelLocal->Children->Add(hBorder1);
	hDownloadStackPanelLocal->Children->Add(hBorder3);
	hDownloadStackPanelLocal->Children->Add(hSpace);
	hDownloadStackPanelLocal->Children->Add(hBorder2);
	hDownloadStackPanelLocal->Children->Add(hBorder4);
	hDownloadStackPanelLocal->Orientation = Orientation::Horizontal;
	hDownloadStackPanelLocal->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadSendMessageGrid->SetRow(hDownloadStackPanelLocal, 0);
	hDownloadSendMessageGrid->Children->Add(hDownloadStackPanelLocal);
}
//----< helper function to create download local port block >------------------

Border^ WPFCppCliDemo::createDownloadLocalPortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadTextBlockLocalPort;
	hDownloadTextBlockLocalPort->Text = "Local Port";
	hDownloadTextBlockLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockLocalPort->FontWeight = FontWeights::Bold;
	hDownloadTextBlockLocalPort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create download local host block >------------------

Border^ WPFCppCliDemo::createDownloadLocalHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBlockFrom;
	hDownloadTextBlockFrom->Text = "Local Host";
	hDownloadTextBlockFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockFrom->FontWeight = FontWeights::Bold;
	hDownloadTextBlockFrom->FontSize = 12;
	return hBorder1;
}
//----< set download remote properties >---------------------------------------

void WPFCppCliDemo::setDownloadRemoteHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createDownloadRemoteHostBlock();
	Border^ hBorder2 = createDownloadRemotePortBlock();
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadTextBoxTo;
	hDownloadTextBoxTo->Text = "localhost";
	hDownloadTextBoxTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxTo->FontWeight = FontWeights::Bold;
	hDownloadTextBoxTo->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDownloadTextBoxRemotePort;
	hDownloadTextBoxRemotePort->Text = "8080";
	hDownloadTextBoxRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBoxRemotePort->FontWeight = FontWeights::Bold;
	hDownloadTextBoxRemotePort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hDownloadStackPanelRemote->Children->Add(hBorder1);
	hDownloadStackPanelRemote->Children->Add(hBorder3);
	hDownloadStackPanelRemote->Children->Add(hSpace);
	hDownloadStackPanelRemote->Children->Add(hBorder2);
	hDownloadStackPanelRemote->Children->Add(hBorder4);
	hDownloadStackPanelRemote->Orientation = Orientation::Horizontal;
	hDownloadStackPanelRemote->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadSendMessageGrid->SetRow(hDownloadStackPanelRemote, 1);
	hDownloadSendMessageGrid->Children->Add(hDownloadStackPanelRemote);
}
//----< helper function to create download remote port block >-----------------

Border^ WPFCppCliDemo::createDownloadRemotePortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadTextBlockRemotePort;
	hDownloadTextBlockRemotePort->Text = "Remote Port";
	hDownloadTextBlockRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockRemotePort->FontWeight = FontWeights::Bold;
	hDownloadTextBlockRemotePort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create download remote host block >-----------------

Border^ WPFCppCliDemo::createDownloadRemoteHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBlockTo;
	hDownloadTextBlockTo->Padding = Thickness(0);
	hDownloadTextBlockTo->Text = "Remote Host";
	hDownloadTextBlockTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockTo->FontWeight = FontWeights::Bold;
	hDownloadTextBlockTo->FontSize = 12;
	return hBorder1;
}
//----< set download file properties >-----------------------------------------

void WPFCppCliDemo::setDownloadServerFilelistProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBlockFile1;
	hDownloadTextBlockFile1->Text = "File Name";
	hDownloadTextBlockFile1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockFile1->FontWeight = FontWeights::Bold;
	hDownloadTextBlockFile1->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 18;
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 420;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadListBox;
	hDownloadStackPanelFile->Children->Add(hBorder1);
	hDownloadStackPanelFile->Children->Add(hSpace);
	hDownloadStackPanelFile->Children->Add(hBorder2);
	TextBlock^ hSpace1 = gcnew TextBlock();
	hSpace1->Width = 18;
	hDownloadStackPanelFile->Children->Add(hSpace1);
	hDownloadFolderBrowseButton->Content = "Select File";
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadFolderBrowseButton;
	hDownloadStackPanelFile->Children->Add(hBorder3);	
	hDownloadStackPanelFile->Orientation = Orientation::Horizontal;
	hDownloadStackPanelFile->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadSendMessageGrid->SetRow(hDownloadStackPanelFile, 2);
	hDownloadSendMessageGrid->Children->Add(hDownloadStackPanelFile);
}
//----< set download result properties >---------------------------------------

void WPFCppCliDemo::setDownloadFileResultBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 120;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBlockResult1;
	hDownloadTextBlockResult1->Text = "Download";
	hDownloadTextBlockResult1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockResult1->FontWeight = FontWeights::Bold;
	hDownloadTextBlockResult1->FontSize = 12;
	hDownloadStackPanelResult->Children->Add(hBorder1);
	hDownloadStackPanelResult->Orientation = Orientation::Horizontal;
	hDownloadStackPanelResult->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDownloadSendMessageGrid->SetRow(hDownloadStackPanelResult, 3);
	hDownloadSendMessageGrid->Children->Add(hDownloadStackPanelResult);
}
//----< set download reply message properties >--------------------------------

void WPFCppCliDemo::setDownloadFileResultProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDownloadTextBlockResult;
	std::string c = toStdString(" Please download one file from ../Test/ServerTest/"
		+ "\n\n Selected file will be downloaded to ../Test/ClientTest/"
		+ "\n\n Client send HTTP mesage is shown in Client Console"
		+ "\n\n Server reply HTTP message is shown in Client Console");
	hDownloadTextBlockResult->Text = toSystemString(c);
	hDownloadTextBlockResult->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDownloadTextBlockResult->FontWeight = FontWeights::Bold;
	hDownloadTextBlockResult->FontSize = 12;
	hDownloadScrollViewer2->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hDownloadScrollViewer2->Content = hBorder1;
	hDownloadSendMessageGrid->SetRow(hDownloadScrollViewer2, 4);
	hDownloadSendMessageGrid->Children->Add(hDownloadScrollViewer2);
}
//----< set download button properties >---------------------------------------

void WPFCppCliDemo::setDownloadButtonProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDownloadSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hDownloadButton->Content = "DOWNLOAD";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 110;
	hBorder2->Height = 50;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownloadButton;
	hDownloadClearButton->Content = "CLEAR";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 110;
	hBorder3->Height = 50;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDownloadClearButton;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 210;
	hDownloadStackPanel->Children->Add(hBorder2);
	hDownloadStackPanel->Children->Add(hSpacer2);
	hDownloadStackPanel->Children->Add(hBorder3);
	hDownloadStackPanel->Orientation = Orientation::Horizontal;
	hDownloadStackPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDownloadSendMessageGrid->SetRow(hDownloadStackPanel, 5);
	hDownloadSendMessageGrid->Children->Add(hDownloadStackPanel);
}
//----< set add dependency view >----------------------------------------------

void WPFCppCliDemo::setAddDependencyView()
{
	Console::Write("\n  setting up add dependency view");
	hAddDependencyGrid->Margin = Thickness(20);
	hAddDependencyTab->Content = hAddDependencyGrid;
	setAddDependencyLocalHostProperties();
	setAddDependencyRemoteHostProperties();
	setAddDependencyFileProperties();
	setAddDependencyParentFileProperties();
	setAddDependencyPackageProperties();
	setAddDependencyPackageListProperties();
	setAddDependencyButtonProperties();
}
//----< set add dependency local properties >----------------------------------

void WPFCppCliDemo::setAddDependencyLocalHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createAddDependencyLocalHostBlock();
	Border^ hBorder2 = createAddDependencyLocalPortBlock();	
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddDependencyTextBoxFrom;
	hAddDependencyTextBoxFrom->Text = "localhost";
	hAddDependencyTextBoxFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBoxFrom->FontWeight = FontWeights::Bold;
	hAddDependencyTextBoxFrom->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hAddDependencyTextBoxLocalPort;
	hAddDependencyTextBoxLocalPort->Text = "6060";
	hAddDependencyTextBoxLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBoxLocalPort->FontWeight = FontWeights::Bold;
	hAddDependencyTextBoxLocalPort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hAddDependencyStackPanelLocal->Children->Add(hBorder1);
	hAddDependencyStackPanelLocal->Children->Add(hBorder3);
	hAddDependencyStackPanelLocal->Children->Add(hSpace);
	hAddDependencyStackPanelLocal->Children->Add(hBorder2);
	hAddDependencyStackPanelLocal->Children->Add(hBorder4);
	hAddDependencyStackPanelLocal->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanelLocal->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanelLocal, 0);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanelLocal);
}
//----< helper function to create add dependency local port >------------------

Border^ WPFCppCliDemo::createAddDependencyLocalPortBlock()
{
	Border^ hBorder2 = gcnew Border();		//local port block
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hAddDependencyTextBlockLocalPort;
	hAddDependencyTextBlockLocalPort->Text = "Local Port";
	hAddDependencyTextBlockLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBlockLocalPort->FontWeight = FontWeights::Bold;
	hAddDependencyTextBlockLocalPort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create add dependency local host >------------------

Border^ WPFCppCliDemo::createAddDependencyLocalHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyTextBlockFrom;
	hAddDependencyTextBlockFrom->Text = "Local Host";
	hAddDependencyTextBlockFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBlockFrom->FontWeight = FontWeights::Bold;
	hAddDependencyTextBlockFrom->FontSize = 12;
	return hBorder1;
}
//----< set add dependency remote properties >---------------------------------

void WPFCppCliDemo::setAddDependencyRemoteHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createAddDependencyRemoteHostBlock();
	Border^ hBorder2 = createAddDependencyRemotePortBlock();	
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddDependencyTextBoxTo;
	hAddDependencyTextBoxTo->Text = "localhost";
	hAddDependencyTextBoxTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBoxTo->FontWeight = FontWeights::Bold;
	hAddDependencyTextBoxTo->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hAddDependencyTextBoxRemotePort;
	hAddDependencyTextBoxRemotePort->Text = "8080";
	hAddDependencyTextBoxRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBoxRemotePort->FontWeight = FontWeights::Bold;
	hAddDependencyTextBoxRemotePort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hAddDependencyStackPanelRemote->Children->Add(hBorder1);
	hAddDependencyStackPanelRemote->Children->Add(hBorder3);
	hAddDependencyStackPanelRemote->Children->Add(hSpace);
	hAddDependencyStackPanelRemote->Children->Add(hBorder2);
	hAddDependencyStackPanelRemote->Children->Add(hBorder4);
	hAddDependencyStackPanelRemote->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanelRemote->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanelRemote, 1);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanelRemote);
}
//----< helper function to create add dependecy remote port >------------------

Border^ WPFCppCliDemo::createAddDependencyRemotePortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hAddDependencyTextBlockRemotePort;
	hAddDependencyTextBlockRemotePort->Text = "Remote Port";
	hAddDependencyTextBlockRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBlockRemotePort->FontWeight = FontWeights::Bold;
	hAddDependencyTextBlockRemotePort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create add dependecy remote host >------------------

Border^ WPFCppCliDemo::createAddDependencyRemoteHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyTextBlockTo;
	hAddDependencyTextBlockTo->Padding = Thickness(0);
	hAddDependencyTextBlockTo->Text = "Remote Host";
	hAddDependencyTextBlockTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyTextBlockTo->FontWeight = FontWeights::Bold;
	hAddDependencyTextBlockTo->FontSize = 12;
	return hBorder1;
}
//----< set add dependency child file properties >-----------------------------

void WPFCppCliDemo::setAddDependencyFileProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyBlockChildFile1;
	hAddDependencyBlockChildFile1->Text = "File Name";
	hAddDependencyBlockChildFile1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyBlockChildFile1->FontWeight = FontWeights::Bold;
	hAddDependencyBlockChildFile1->FontSize = 12;
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 420;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hAddDepListBox;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 18;
	hAddDependencyStackPanelChildFile->Children->Add(hBorder1);
	hAddDependencyStackPanelChildFile->Children->Add(hSpace);
	hAddDependencyStackPanelChildFile->Children->Add(hBorder2);
	TextBlock^ hSpace1 = gcnew TextBlock();
	hSpace1->Width = 18;
	hAddDependencyStackPanelChildFile->Children->Add(hSpace1);
	hAddDepFolderBrowseButton->Content = "Select File";		
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddDepFolderBrowseButton;
	hAddDependencyStackPanelChildFile->Children->Add(hBorder3);
	hAddDependencyStackPanelChildFile->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanelChildFile->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanelChildFile, 2);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanelChildFile);
}
//----< set add dependency parent file properties >----------------------------

void WPFCppCliDemo::setAddDependencyParentFileProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyBlockParentFile1;
	hAddDependencyBlockParentFile1->Text = "Dependency";
	hAddDependencyBlockParentFile1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyBlockParentFile1->FontWeight = FontWeights::Bold;
	hAddDependencyBlockParentFile1->FontSize = 12;
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 420;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hAddDepParentListBox;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 18;
	hAddDependencyStackPanelParentFile->Children->Add(hBorder1);
	hAddDependencyStackPanelParentFile->Children->Add(hSpace);
	hAddDependencyStackPanelParentFile->Children->Add(hBorder2);
	TextBlock^ hSpace1 = gcnew TextBlock();
	hSpace1->Width = 18;
	hAddDependencyStackPanelParentFile->Children->Add(hSpace1);
	hAddDepFolderBrowseButtonParent->Content = "Select File";		
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddDepFolderBrowseButtonParent;
	hAddDependencyStackPanelParentFile->Children->Add(hBorder3);
	hAddDependencyStackPanelParentFile->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanelParentFile->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanelParentFile, 3);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanelParentFile);
}
//----< set add dependency parent file name properties >-----------------------

void WPFCppCliDemo::setAddDependencyPackageProperties() 
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->Width = 200;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyBlockResult1;
	hAddDependencyBlockResult1->Text = "Add Dependency";
	hAddDependencyBlockResult1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyBlockResult1->FontWeight = FontWeights::Bold;
	hAddDependencyBlockResult1->FontSize = 12;
	hAddDependencyStackPanelResult->Children->Add(hBorder1);
	hAddDependencyStackPanelResult->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanelResult->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanelResult, 4);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanelResult);
}
//----< set add dependency files properties >----------------------------------

void WPFCppCliDemo::setAddDependencyPackageListProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hAddDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hAddDependencyBlockResult;
	std::string c = toStdString(" Please select one file in ../Test/ServerTest/ after uploding from .. / Test / ClientTest /"
		+ "\n\n Please upload files first and then add dependencies"
		+ "\n\n Client send HTTP mesage is shown in Client Console");
	hAddDependencyBlockResult->Text =toSystemString(c);
	hAddDependencyBlockResult->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hAddDependencyBlockResult->FontWeight = FontWeights::Bold;
	hAddDependencyBlockResult->FontSize = 12;
	hAddDependencyScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hAddDependencyScrollViewer1->Content = hBorder1;
	hAddDependencyGrid->SetRow(hAddDependencyScrollViewer1, 5);
	hAddDependencyGrid->Children->Add(hAddDependencyScrollViewer1);
}
//----< set add dependency button properties >---------------------------------

void WPFCppCliDemo::setAddDependencyButtonProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hAddDependencyGrid->RowDefinitions->Add(hRow2Def);
	hAddDependencyButton->Content = "ADD DEPENDENCY";
	Border^ hBorder0 = gcnew Border();
	hBorder0->Width = 110;
	hBorder0->Height = 50;
	hBorder0->BorderThickness = Thickness(1);
	hBorder0->BorderBrush = Brushes::Black;
	hBorder0->Child = hAddDependencyButton;
	hAddDependencyClearButton->Content = "CLEAR";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 110;
	hBorder3->Height = 50;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddDependencyClearButton;
	TextBlock^ hSpacer2 = gcnew TextBlock();
	hSpacer2->Width = 210;
	hAddDependencyStackPanel->Children->Add(hBorder0);
	hAddDependencyStackPanel->Children->Add(hSpacer2);
	hAddDependencyStackPanel->Children->Add(hBorder3);
	hAddDependencyStackPanel->Orientation = Orientation::Horizontal;
	hAddDependencyStackPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hAddDependencyGrid->SetRow(hAddDependencyStackPanel, 6);
	hAddDependencyGrid->Children->Add(hAddDependencyStackPanel);
}
//----< set display dependency view >------------------------------------------

void WPFCppCliDemo::setDisplayDependencyView()
{
	Console::Write("\n  setting up display dependency view");
	hDisplayDependencyGrid->Margin = Thickness(20);
	hDisplayDependencyTab->Content = hDisplayDependencyGrid;
	setDisplayDependencyLocalHostProperties();
	setDisplayDependencyRemoteHostProperties(); 
	setDisplayDependencyFileNameProperties();
	setDisplayDependencyResultProperties();
	setDisplayDependencyButtonProperties();
}
//----< set display dependency local properties >------------------------------

void WPFCppCliDemo::setDisplayDependencyLocalHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDisplayDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createDisplayDependencyLocalHostBlock();
	Border^ hBorder2 = createDisplayDependencyLocalPortBlock();
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDisplayDependencyTextBoxFrom;
	hDisplayDependencyTextBoxFrom->Text = "localhost";
	hDisplayDependencyTextBoxFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBoxFrom->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBoxFrom->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDisplayDependencyTextBoxLocalPort;
	hDisplayDependencyTextBoxLocalPort->Text = "6060";
	hDisplayDependencyTextBoxLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBoxLocalPort->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBoxLocalPort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hDisplayDependencyStackPanelLocal->Children->Add(hBorder1);
	hDisplayDependencyStackPanelLocal->Children->Add(hBorder3);
	hDisplayDependencyStackPanelLocal->Children->Add(hSpace);
	hDisplayDependencyStackPanelLocal->Children->Add(hBorder2);
	hDisplayDependencyStackPanelLocal->Children->Add(hBorder4);
	hDisplayDependencyStackPanelLocal->Orientation = Orientation::Horizontal;
	hDisplayDependencyStackPanelLocal->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDisplayDependencyGrid->SetRow(hDisplayDependencyStackPanelLocal, 0);
	hDisplayDependencyGrid->Children->Add(hDisplayDependencyStackPanelLocal);
}
//----< helper function to create display dependency local port >--------------

Border^ WPFCppCliDemo::createDisplayDependencyLocalPortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDisplayDependencyTextBlockLocalPort;
	hDisplayDependencyTextBlockLocalPort->Text = "Local Port";
	hDisplayDependencyTextBlockLocalPort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBlockLocalPort->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBlockLocalPort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create display dependency local host >--------------

Border^ WPFCppCliDemo::createDisplayDependencyLocalHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDisplayDependencyTextBlockFrom;
	hDisplayDependencyTextBlockFrom->Text = "Local Host";
	hDisplayDependencyTextBlockFrom->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBlockFrom->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBlockFrom->FontSize = 12;
	return hBorder1;
}
//----< set display dependency remote properties >-----------------------------

void WPFCppCliDemo::setDisplayDependencyRemoteHostProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDisplayDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = createDisplayDependencyRemoteHostBlock();
	Border^ hBorder2 = createDisplayDependencyRemotePortBlock();	
	Border^ hBorder3 = gcnew Border();		
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDisplayDependencyTextBoxTo;
	hDisplayDependencyTextBoxTo->Text = "localhost";
	hDisplayDependencyTextBoxTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBoxTo->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBoxTo->FontSize = 12;
	Border^ hBorder4 = gcnew Border();		
	hBorder4->Width = 100;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDisplayDependencyTextBoxRemotePort;
	hDisplayDependencyTextBoxRemotePort->Text = "8080";
	hDisplayDependencyTextBoxRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBoxRemotePort->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBoxRemotePort->FontSize = 12;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 200;
	hDisplayDependencyStackPanelRemote->Children->Add(hBorder1);
	hDisplayDependencyStackPanelRemote->Children->Add(hBorder3);
	hDisplayDependencyStackPanelRemote->Children->Add(hSpace);
	hDisplayDependencyStackPanelRemote->Children->Add(hBorder2);
	hDisplayDependencyStackPanelRemote->Children->Add(hBorder4);
	hDisplayDependencyStackPanelRemote->Orientation = Orientation::Horizontal;
	hDisplayDependencyStackPanelRemote->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDisplayDependencyGrid->SetRow(hDisplayDependencyStackPanelRemote, 1);
	hDisplayDependencyGrid->Children->Add(hDisplayDependencyStackPanelRemote);
}
//----< helper function to create display dependency remote port >-------------

Border^ WPFCppCliDemo::createDisplayDependencyRemotePortBlock()
{
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 100;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(0);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDisplayDependencyTextBlockRemotePort;
	hDisplayDependencyTextBlockRemotePort->Text = "Remote Port";
	hDisplayDependencyTextBlockRemotePort->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBlockRemotePort->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBlockRemotePort->FontSize = 12;
	return hBorder2;
}
//----< helper function to create display dependency remote host >-------------

Border^ WPFCppCliDemo::createDisplayDependencyRemoteHostBlock()
{
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 100;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDisplayDependencyTextBlockTo;
	hDisplayDependencyTextBlockTo->Padding = Thickness(0);
	hDisplayDependencyTextBlockTo->Text = "Remote Host";
	hDisplayDependencyTextBlockTo->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBlockTo->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBlockTo->FontSize = 12;
	return hBorder1;
}
//----< set display dependency file name properties >--------------------------

void WPFCppCliDemo::setDisplayDependencyFileNameProperties() 
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hDisplayDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();		
	hBorder1->Width = 80;
	hBorder1->Height = 30;
	hBorder1->BorderThickness = Thickness(0);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hDisplayDependencyTextBlockFileName;
	hDisplayDependencyTextBlockFileName->Padding = Thickness(0);
	hDisplayDependencyTextBlockFileName->Text = "File Name";
	hDisplayDependencyTextBlockFileName->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyTextBlockFileName->FontWeight = FontWeights::Bold;
	hDisplayDependencyTextBlockFileName->FontSize = 12;
	Border^ hBorder2 = gcnew Border();		
	hBorder2->Width = 420;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDisplayListBox;
	TextBlock^ hSpace = gcnew TextBlock();
	hSpace->Width = 18;
	hDisplayDependencyStackPanelFile->Children->Add(hBorder1);
	hDisplayDependencyStackPanelFile->Children->Add(hSpace);
	hDisplayDependencyStackPanelFile->Children->Add(hBorder2);
	TextBlock^ hSpace1 = gcnew TextBlock();
	hSpace1->Width = 18;
	hDisplayDependencyStackPanelFile->Children->Add(hSpace1);
	hDisplayFolderBrowseButton->Content = "Select File";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 100;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDisplayFolderBrowseButton;
	TextBlock^ hSpace2 = gcnew TextBlock();
	hSpace2->Width = 12;
	hDisplayDependencyStackPanelFile->Children->Add(hBorder3);
	hDisplayDependencyStackPanelFile->Children->Add(hSpace2);
	Border^ hBorder4 = createWithDepCheckBox();
	hDisplayDependencyStackPanelFile->Children->Add(hBorder4);
	hDisplayDependencyStackPanelFile->Orientation = Orientation::Horizontal;
	hDisplayDependencyStackPanelFile->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hDisplayDependencyGrid->SetRow(hDisplayDependencyStackPanelFile, 2);
	hDisplayDependencyGrid->Children->Add(hDisplayDependencyStackPanelFile);
}
//----helper function to create with dependency check box >--------------------

Border^ WPFCppCliDemo::createWithDepCheckBox()
{
	hDisplayWithDepBox->Content = "WithDep";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 80;
	hBorder4->Height = 20;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hDisplayWithDepBox;
	return hBorder4;
}
//----< set display dependency result properties >-----------------------------

void WPFCppCliDemo::setDisplayDependencyResultProperties() 
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hDisplayDependencyGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder = gcnew Border();
	hBorder->BorderThickness = Thickness(1);
	hBorder->BorderBrush = Brushes::Black;
	hBorder->Child = hDisplayDependencyResult;
	hDisplayDependencyResult->Padding = Thickness(15);
	std::string c = toStdString(" Please select one file from ../Test/ServerTest/ after uploading from ../Test/ClientTest/"
		+ "\n\n Client send HTTP mesage is shown in Client Console"
		+ "\n\n Dependency detail is shown in Server Console"
		+ "\n\n WithDep: selected to extract file with its dependency");
	hDisplayDependencyResult->Text = toSystemString(c);
	hDisplayDependencyResult->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hDisplayDependencyResult->FontWeight = FontWeights::Bold;
	hDisplayDependencyResult->FontSize = 12;
	hDisplayDependencyScrollViewer->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hDisplayDependencyScrollViewer->Content = hBorder;
	hDisplayDependencyGrid->SetRow(hDisplayDependencyScrollViewer, 3);
	hDisplayDependencyGrid->Children->Add(hDisplayDependencyScrollViewer);
}
//----< set display dependency button properties >-----------------------------

void WPFCppCliDemo::setDisplayDependencyButtonProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hDisplayDependencyGrid->RowDefinitions->Add(hRow2Def);
	hDisplayDependencyButton->Content = "DISPLAY";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 110;
	hBorder2->Height = 50;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDisplayDependencyButton;
	hDisplayDependencyClearButton->Content = "CLEAR";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 110;
	hBorder3->Height = 50;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDisplayDependencyClearButton;
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 210;
	hDisplayDependencyStackPanel->Children->Add(hBorder2);
	hDisplayDependencyStackPanel->Children->Add(hSpacer);
	hDisplayDependencyStackPanel->Children->Add(hBorder3);
	hDisplayDependencyStackPanel->Orientation = Orientation::Horizontal;
	hDisplayDependencyStackPanel->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hDisplayDependencyGrid->SetRow(hDisplayDependencyStackPanel, 4);
	hDisplayDependencyGrid->Children->Add(hDisplayDependencyStackPanel);
}
//----< convert to std string >------------------------------------------------

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
	std::string dst;
	for (int i = 0; i < pStr->Length; ++i)
		dst += (char)pStr[i];
	return dst;
}
//----< send upload message >--------------------------------------------------

void WPFCppCliDemo::sendUploadMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Upload File");
	hStatus->Text = "Upload File";
	int count = hUploadListBox->SelectedItems->Count;
	if ( count >= 0)
	{
		std::string fromAddr = toStdString(hUploadTextBoxFrom->Text);
		std::string toAddr = toStdString(hUploadTextBoxTo->Text);
		std::string toPort = toStdString(hUploadTextBoxRemotePort->Text);
		std::string fileName = toStdString(hUploadListBox->Items[0]->ToString());
		std::string command = "UPLOAD";
		std::string mode = "oneway";
		HTTPMessage h;
		h.addAttribute("FromAddr", fromAddr);
		h.addAttribute("Command", command);
		h.addAttribute("Mode", mode);
		h.addAttribute("Content", fileName);
		h.addAttribute("ToAddr", toAddr);
		h.addAttribute("ToPort", toPort);
		bool p = hUploadCheckinBox->IsChecked.Value;
		if (p == false)
			h.addAttribute("Checkin", "Open");
		if (p == true)
			h.addAttribute("Checkin", "Closed");
		pChann_->postMessage(h);
	}
	else
	{
		hStatus->Text = "Invalid Message. ";
	}
}
//----< browse for upload folder >---------------------------------------------

void WPFCppCliDemo::browseForUploadFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hUploadListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hUploadFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ name = hUploadFileBrowserDialog->FileName;
		String^ n = getFilename(name);
		hUploadListBox->Items->Add(n);
	}
}
//----< clear upload reply message >-------------------------------------------
void WPFCppCliDemo::uploadclear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hUploadTextlock->Text = "";
	hUploadListBox->Items->Clear();
	hUploadCheckinBox->IsChecked = false;
}
//----< send download message >------------------------------------------------

void WPFCppCliDemo::sendDownloadMessage(Object^ obj, RoutedEventArgs^ args)
{
	Console::Write("\n  Download File");
	hStatus->Text = "Download File";
	if (hDownloadListBox->SelectedItems->Count >= 0)
	{
		std::string fromAddr = toStdString(hDownloadTextBoxFrom->Text);
		std::string toAddr = toStdString(hDownloadTextBoxTo->Text);
		std::string toPort = toStdString(hDownloadTextBoxRemotePort->Text);
		std::string fileName = toStdString(hDownloadListBox->Items[0]->ToString());
		std::string command = "DOWNLOAD";
		std::string mode = "oneway";
		HTTPMessage h;
		h.addAttribute("FromAddr", fromAddr);
		h.addAttribute("Command", command);
		h.addAttribute("Mode", mode);
		h.addAttribute("Content", fileName);
		h.addAttribute("ToAddr", toAddr);
		h.addAttribute("ToPort", toPort);
		pChann_->postMessage(h);
	}
	else
	{
		hStatus->Text = "Invalid Message";
	}
}
//----< browse for download folder >-------------------------------------------

void WPFCppCliDemo::browseForDownloadFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hDownloadListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hDownloadFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ name = hDownloadFileBrowserDialog->FileName;
		String^ n = getFilename(name);
		hDownloadListBox->Items->Add(n);
	}
}
//----< clear download reply message >-----------------------------------------

void WPFCppCliDemo::downloadclear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hDownloadTextBlockResult->Text = "";
	hDownloadListBox->Items->Clear();
}
//----< send add dependency message >------------------------------------------

void WPFCppCliDemo::sendAddDependencyMessage(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Add Dependency");
	hStatus->Text = "Add Dependency";
	if ((hAddDepListBox->SelectedItems->Count >= 0) && (hAddDepParentListBox->SelectedItems->Count >= 0))
	{
		std::string fromAddr = toStdString(hAddDependencyTextBoxFrom->Text);
		std::string toAddr = toStdString(hAddDependencyTextBoxTo->Text);
		std::string toPort = toStdString(hAddDependencyTextBoxRemotePort->Text);
		std::string fileName = toStdString(hAddDepListBox->Items[0]->ToString());
		std::string dep = toStdString(hAddDepParentListBox->Items[0]->ToString());
		std::string command = "ADD DEPENDENCY";
		std::string mode = "oneway";
		HTTPMessage h;
		h.addAttribute("FromAddr", fromAddr);
		h.addAttribute("Command", command);
		h.addAttribute("Mode", mode);
		h.addAttribute("Content", fileName);
		h.addAttribute("ToAddr", toAddr);
		h.addAttribute("ToPort", toPort);
		h.addAttribute("Dependency", dep);
		pChann_->postMessage(h);
	}
	else
	{
		hStatus->Text = "Invalid Message. ";
	}
}
//----< browse for add dependency folder >-------------------------------------

void WPFCppCliDemo::browseForAddDependencyFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hAddDepListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hAddDepFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ name = hAddDepFileBrowserDialog->FileName;
		String^ n = getFilename(name);
		hAddDepListBox->Items->Add(n);
	}
}
//----< browse for add parent dependency folder >------------------------------

void WPFCppCliDemo::browseForAddDependencyParentFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hAddDepParentListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hAddDepFileParentBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ name = hAddDepFileParentBrowserDialog->FileName;
		String^ n = getFilename(name);
		hAddDepParentListBox->Items->Add(n);
	}
}

//----< clear add dependecy reply message >------------------------------------

void WPFCppCliDemo::adddependencyclear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hAddDependencyBlockResult->Text = "";
	hAddDepParentListBox->Items->Clear();
}
//----< send display dependency message >--------------------------------------

void WPFCppCliDemo::sendShowDependencyMessage(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Display Dependency");
	hStatus->Text = "Display Dependency";
	if (hDisplayListBox->SelectedItems->Count >= 0)
	{
		std::string fromAddr = toStdString(hDisplayDependencyTextBoxFrom->Text);
		std::string toAddr = toStdString(hDisplayDependencyTextBoxTo->Text);
		std::string toPort = toStdString(hDisplayDependencyTextBoxRemotePort->Text);
		std::string fileName = toStdString(hDisplayListBox->Items[0]->ToString());
		std::string command = "GET FILELISTS";
		std::string mode = "oneway";
		HTTPMessage h;
		h.addAttribute("FromAddr", fromAddr);
		h.addAttribute("Command", command);
		h.addAttribute("Mode", mode);
		h.addAttribute("Content", fileName);
		h.addAttribute("ToAddr", toAddr);
		h.addAttribute("ToPort", toPort);
		bool p = hDisplayWithDepBox->IsChecked.Value;
		if (p == false)
			h.addAttribute("WithDep", "False");
		if (p == true)
			h.addAttribute("WithDep", "True");
		pChann_->postMessage(h);
	}
	else
	{
		hStatus->Text = "Invalid Message. ";
	}
}
//----< browsw for display dependency folder >---------------------------------

void WPFCppCliDemo::browseForDisplayDependencyFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hDisplayListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hDisplayFileBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ name = hDisplayFileBrowserDialog->FileName;
		String^ n = getFilename(name);
		hDisplayListBox->Items->Add(n);
	}
}
//----< clear display dependency reply message >-------------------------------

void WPFCppCliDemo::displaydependencyclear(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hDisplayWithDepBox->IsChecked = false;
	hDisplayDependencyResult->Text = "";
	hDisplayListBox->Items->Clear();
}
//----< convert to system string >---------------------------------------------

String^ WPFCppCliDemo::toSystemString(std::string& str)
{
	StringBuilder^ pStr = gcnew StringBuilder();
	for (size_t i = 0; i < str.size(); ++i)
		pStr->Append((Char)str[i]);
	return pStr->ToString();
}
//----< add upload text >------------------------------------------------------

void WPFCppCliDemo::addUploadText(String^ msg)
{
	hUploadTextlock->Text += msg + "\n\n";
}
//----< add download text >----------------------------------------------------

void WPFCppCliDemo::addDownloadText(String^ msg)
{
	hDownloadTextBlockResult->Text += msg + "\n\n";
}
//----< add add-dependency text >----------------------------------------------------

void WPFCppCliDemo::addAddDependencyText(String^ msg)
{
	hAddDependencyBlockResult->Text += msg + "\n\n";
}
//----< add display-dependency text >----------------------------------------------------

void WPFCppCliDemo::addDisplayDependencyText(String^ msg)
{
	hDisplayDependencyResult->Text += msg + "\n\n";
}
//----< get string format command message >------------------------------------

void WPFCppCliDemo::getMessage()
{
	// recvThread runs this function
	while (true)
	{
		std::cout << "\n  receive thread calling getMessage()";
		HTTPMessage h= pRecvr_->getMessage();
		std::string msg = h.createHeader();
		std::cout << msg;
		String^ sMsg = toSystemString(msg);
		array<String^>^ args = gcnew array<String^>(1);
		args[0] = sMsg;
		std::string command = h.findValue("Command");
		Action<String^>^ act;
		if (command == "UPLOAD")
			act = gcnew Action<String^>(this, &WPFCppCliDemo::addUploadText);
		if (command=="DOWNLOAD")
			act = gcnew Action<String^>(this, &WPFCppCliDemo::addDownloadText);
		if (command == "ADD DEPENDENCY")
			act = gcnew Action<String^>(this, &WPFCppCliDemo::addAddDependencyText);
		if (command == "DISPLAY DEPENDENCY")
			act = gcnew Action<String^>(this, &WPFCppCliDemo::addDisplayDependencyText);
		Dispatcher->Invoke(act, args);		// must call addText on main UI thread
	}
}
//----< set up file list view >------------------------------------------------

void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);
	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}
//----< crowse for folder >----------------------------------------------------

void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	std::string list = " All .xml files are stored in ../Test/ServerTest/xmlDirectory";
	hListBox->Items->Add(toSystemString(list));
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}
//----< set up connection view >-----------------------------------------------

void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
}
//----< onloaded function>-----------------------------------------------------
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
	Console::Write("\n  Window loaded");
}
//----< unloading function>----------------------------------------------------

void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
	Console::Write("\n  Window closing");
}
//----< helper function to get file name >---------------------------

String^ WPFCppCliDemo::getFilename(String^ name)
{
	std::string n = toStdString(name);
	String^ name_;
	for (size_t i = n.size() - 1; i >= 0; i--)
	{
		if (n[i] == '\\')
		{
			std::string temp = n.substr(i+1);
			name_ = toSystemString(temp);
			return name_;
		}
	}
	std::string temp = "nofile";
	name_ = toSystemString(temp);
	return name_;
}
//----< test stub >--------------------------------------------------

#ifdef TEST_WPF

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	app->Run(gcnew WPFCppCliDemo());
	Console::WriteLine(L"\n\n");
}

#endif
