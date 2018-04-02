#include "MainWindow.h"
#include "guicon.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace VideoQuery;

[STAThreadAttribute]
void Main(array<String^>^ args) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	VideoQuery::MainWindow form;
	//Attach a console for debugging
	RedirectIOToConsole();
	Application::Run(%form);
}

MainWindow::MainWindow(void)
{
	InitializeComponent();
	//Initialize data object
	data.dataVideo = gcnew Video(dataVideoImage);
	data.queryVideo = gcnew Video(queryVideoImage);
}


System::Void MainWindow::dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	//Loading all 600 frames takes about 90 seconds
	//Set to a smaller value for testing
	data.LoadDataVideo(10, "database_videos", "flowers");
}

System::Void MainWindow::queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	data.LoadQueryVideo(10, "query_videos\\query", "first");
}