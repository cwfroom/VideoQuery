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


System::Void MainWindow::dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!data.dataVideo) {
		data.dataVideo = gcnew Video(dataVideoImage);
	}
	//Loading all 600 frames takes about 90 seconds
	//Set to a smaller value for testing
	data.dataVideo->LoadVideo(10,"database_videos","flowers");

}

System::Void MainWindow::queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (!data.queryVideo) {
		data.queryVideo = gcnew Video(queryVideoImage);
	}
	data.queryVideo->LoadVideo(10, "query_videos\\query", "first");

}