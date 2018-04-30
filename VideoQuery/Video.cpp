#include "video.h"
using namespace VideoQuery;
using namespace System::Runtime::InteropServices;

Video::Video(){
	audioPlayer = new Audio();
	playbackTimer = gcnew Timers::Timer(interval);
	playbackTimer->Elapsed += gcnew ElapsedEventHandler(this,&Video::UpdateFrame);
}

void Video::SetUI(PictureBox^ display, Label^ label, TrackBar^ trackBar, Button^ button) {
	this->display = display;
	this->label = label;
	this->trackBar = trackBar;
	this->button = button;
}


void Video::SetVideo(int frameCount, String^ folder, String^ name) {
	this->frameCount = frameCount;
	this->folder = folder;
	this->name = name;
	currentFrame = 0;
}

void Video::LoadVideo() {
	images.Clear();
	playbackTimer->Stop();
	String^ path = folder + "\\" + name;
	
	for (int i = 1; i <= frameCount; i++) {
		char numchar[4];
		snprintf(numchar, 4, "%03d", i);
		String^ num = gcnew String(numchar);
		String^ fileName = path + "\\" + name + num + ".rgb";
		UpdateLabel("Loading \"" + name + "\" " + "frame " +  i + "/" + frameCount);
		MyImage rawImage;
		rawImage.setImagePath(static_cast<char*>(Marshal::StringToHGlobalAnsi(fileName).ToPointer()));
		rawImage.ReadImage();
		char* rawImageData = rawImage.getImageData();

		if (!rawImageData) {
			MessageBox::Show("Failed to load " + fileName, "Error", MessageBoxButtons::OK);
			return;
		}

		if (rawImageData) {
			Bitmap^ bitmap = gcnew Bitmap(rawImage.getWidth(), rawImage.getHeight());
			for (int i = 0; i < bitmap->Height; i++) {
				for (int j = 0; j < bitmap->Width; j++) {
					unsigned char b = rawImageData[bitmap->Width * i * 3 + 3 * j];
					unsigned char g = rawImageData[bitmap->Width * i * 3 + 3 * j + 1];
					unsigned char r = rawImageData[bitmap->Width * i * 3 + 3 * j + 2];
					bitmap->SetPixel(j, i, Drawing::Color::FromArgb(255, r, g, b));
				}
			}
			images.Add(bitmap);
		}
	}
	UpdateLabel("Loading \"" + name + "\" " + "sound");
	String^ audioFilePath = path + "\\" + name + ".wav";
	audioPlayer->LoadAudio(static_cast<char*>(Marshal::StringToHGlobalAnsi(audioFilePath).ToPointer()));
	UpdateLabel(name + " loaded");
	display->Image = dynamic_cast<Image^>(images[0]);
	
}

void Video::PlayVideo() {
	UpdateButton("Pause");
	Console::WriteLine("Playing from frame " + currentFrame);
	audioPlayer->PlayAudio(currentFrame);
	if (images.Count > 0) {
		playbackTimer->Start();
	}
}

void Video::PauseVideo() {
	playbackTimer->Stop();
	audioPlayer->PauseAudio();
	UpdateButton("Play");
}

void Video::SeekVideo(int frame) {
	PauseVideo();
	currentFrame = frame;
	display->Image = dynamic_cast<Image^>(images[frame]);
}

void Video::UpdateFrame(Object^ sender, ElapsedEventArgs^ e) {
	if (currentFrame >= frameCount) {
		PauseVideo();
	}
	else {
		display->Image = dynamic_cast<Image^>(images[currentFrame]);
		SetTrackBarValue(currentFrame);
		currentFrame++;
	}
	
}

void Video::UpdateLabel(String^ text) {
	if (label->InvokeRequired) {
		StringDelegate^ delegate = gcnew StringDelegate(this, &Video::UpdateLabel);
		label->Invoke(delegate, text);
	}
	else {
		label->Text = text;
	}
}

void Video::UpdateButton(String^ text) {
	if (button->InvokeRequired) {
		StringDelegate^ delegate = gcnew StringDelegate(this, &Video::UpdateButton);
		button->Invoke(delegate, text);
	}
	else {
		button->Text = text;
	}
}

void Video::SetLabelVisibility(bool visible) {
	if (label->InvokeRequired) {
		BoolDelegate^ delegate = gcnew BoolDelegate(this, &Video::SetLabelVisibility);
		label->Invoke(delegate, visible);
	}
	else {
		label->Visible = visible;
	}
}

void Video::SetTrackBarValue(int value) {
	if (trackBar->InvokeRequired) {
		IntDelegate^ delegate = gcnew IntDelegate(this, &Video::SetTrackBarValue);
		trackBar->Invoke(delegate, value);
	}
	else {
		trackBar->Value = value;
	}

}

