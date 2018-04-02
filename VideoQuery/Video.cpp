#include "video.h"
using namespace VideoQuery;
using namespace System::Runtime::InteropServices;

Video::Video(PictureBox^ display,Label^ label){
	this->display = display;
	this->label = label;
}


void Video::SetVideo(int frameCount, String^ folder, String^ name) {
	this->frameCount = frameCount;
	this->folder = folder;
	this->name = name;
}

void Video::LoadVideo() {
	images.Clear();
	SetLabelVisibility(true);
	String^ path = folder + "\\" + name;
	Console::WriteLine("Loading " + path);
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

	Console::WriteLine("Finished loading " + path);

	display->Image = dynamic_cast<Image^>(images[0]);
	SetLabelVisibility(false);
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

void Video::SetLabelVisibility(bool visible) {
	if (label->InvokeRequired) {
		BoolDelegate^ delegate = gcnew BoolDelegate(this, &Video::SetLabelVisibility);
		label->Invoke(delegate, visible);
	}
	else {
		label->Visible = visible;
	}
}



