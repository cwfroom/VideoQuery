#include "MainWindow.h"
#include "guicon.h"
#include "SDL.h"
#include "SDL_mixer.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace VideoQuery;

#define queryFrames 150
#define dataFrames 600
#define LOAD_DATABASE_METRICS 1	// 1 to read metrics from disk; 0 to generate and dump

[STAThreadAttribute]
void Main(array<String^>^ args) {
	if (SDL_Init(SDL_INIT_AUDIO) < 0) return;
	if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 4096) < 0) return;
	//Attach a console for debugging
	RedirectIOToConsole();
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	VideoQuery::MainWindow form;
	Application::Run(%form);
	SDL_Quit();
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
	if (LOAD_DATABASE_METRICS) {
		data.LoadDatabaseMetrics(dataFrames);
	} else {
		data.GenerateDatabaseMetrics(dataFrames);
	}
}


System::Void MainWindow::dataVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (data.dataVideo->IsLoaded()) {
		if (dataVideoPlayButton->Text == "Play") {
			data.dataVideo->PlayVideo();
		}
		else {
			data.dataVideo->PauseVideo();
		}
	}
}

System::Void  MainWindow::queryVideoTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	data.queryVideo->SeekVideo(queryVideoTrackBar->Value);
}

System::Void MainWindow::dataVideoTrackBar_Scroll(System::Object^  sender, System::EventArgs^  e) {
	data.dataVideo->SeekVideo(dataVideoTrackBar->Value);
}

System::Void MainWindow::queryVideoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (data.queryVideo->IsLoaded()) {
		if (queryVideoPlayButton->Text == "Play") {
			data.queryVideo->PlayVideo();
		}
		else {
			data.queryVideo->PauseVideo();
		}
	}
}

System::Void MainWindow::queryVideoLoadButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (queryVideoNameText->Text != "") {
		dataVideoListBox->Items->Clear();
		queryVideoLabel->Text = "Computing Accuracies ....";
		data.LoadQueryVideo(queryFrames, "query_videos\\query", queryVideoNameText->Text);
		data.queryVideo->SetUnderscore(underscoreCheck->Checked);
		array<String^>^ accstrs = data.GetSortedAccuracyStrings();
		for (int i = 0; i < accstrs->Length; i++) {
			dataVideoListBox->Items->Add(accstrs[i]);
		}
		dataVideoListBox->SelectedIndex = 0;
	}
}


System::Void MainWindow::playBothButton_Click(System::Object^  sender, System::EventArgs^  e) {
	if (playBothButton->Text == "Play Both") {
		playBothButton->Text == "Pause Both";
	}
	else {
		playBothButton->Text = "Play Both";
	}
	queryVideoPlayButton_Click(sender, e);
	dataVideoPlayButton_Click(sender, e);
}

System::Void MainWindow::dataVideoLoadButton_Click(System::Object^  sender, System::EventArgs^  e) {
	// dataVideoListBox loads and has indices of dataVideoListBox
	data.SwapDataVideo(dataVideoListBox->SelectedIndex);
	dataVideoLabel->Text = dataVideoListBox->SelectedItem->ToString();
	perFrameBox->Refresh();
	colorAccuracyBox->Refresh();
	motionAccuracyBox->Refresh();
	audioAccuracyBox->Refresh();
}

System::Void MainWindow::paintHelper(Graphics^ g, Eigen::VectorXf vec) {
	// Clear
	g->Clear(Color::SlateGray);

	// Draw the visualization
	if (vec.rows() > 1) {
		int prev_y = vec(0);
		for (int x = 1; x < vec.rows(); x++) {
			int y = vec(x) * 100.0;
			// (0,0) is top left
			g->DrawLine(System::Drawing::Pens::Red,
				(x - 1) * 2, 100 - prev_y,
				x * 2, 100 - y);
			prev_y = y;
		}
	}
}

System::Void MainWindow::perFrameBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	paintHelper(e->Graphics, data.getPerFrameAccuracy());
}

System::Void MainWindow::colorAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	paintHelper(e->Graphics, data.getColorAccuracy());
}

System::Void MainWindow::motionAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	paintHelper(e->Graphics, data.getMotionAccuracy());
}

System::Void MainWindow::audioAccuracyBox_Paint(Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	paintHelper(e->Graphics, data.getAudioAccuracy());
}

System::Void MainWindow::setMetricsBox(int* arr,int size){

}