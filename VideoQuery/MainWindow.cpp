#include "MainWindow.h"
#include "guicon.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace VideoQuery;

#define queryFrames 60
#define dataFrames 60

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
	//Hook UI to video objects
	data.dataVideo->SetUI(dataVideoImage, dataVideoLabel, dataVideoTrackBar,dataVideoPlayButton);
	data.queryVideo->SetUI(queryVideoImage, queryVideoLabel, queryVideoTrackBar,queryVideoPlayButton);
	for (int i = 0; i < data.dataVideoList->Length;i++) {
		dataVideoListBox->Items->Add(data.dataVideoList[i]);
	}
	dataVideoListBox->SelectedIndex = 0;
	dataVideoTrackBar->Maximum = dataFrames - 1;
	queryVideoTrackBar->Maximum = queryFrames - 1;
}


System::Void MainWindow::dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (dataVideoPlayButton->Text == "Play") {
		data.dataVideo->PlayVideo();
	}
	else {
		data.dataVideo->PauseVideo();
	}

}
System::Void MainWindow::dataVideoTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	data.dataVideo->SeekVideo(dataVideoTrackBar->Value);
}

System::Void MainWindow::queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	data.LoadQueryVideo(queryFrames, "query_videos\\query", "first");
}

System::Void MainWindow::dataVideoLoadButton_Click(System::Object^  sender, System::EventArgs^  e) {
	data.LoadDataVideo(dataFrames, "database_videos", static_cast<String^>(dataVideoListBox->SelectedItem));
}