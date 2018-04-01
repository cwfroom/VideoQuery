#include "video.h"

using namespace VideoQuery;
using namespace System::Runtime::InteropServices;

Video::Video(PictureBox^ display){
	this->display = display;
	//images = gcnew List<Image^>();
}
 
void Video::LoadVideo(int frameCount, String^ folder, String^ name) {
	this->frameCount = frameCount;
	String^ path = folder + "\\" + name;
	Console::WriteLine("Loading " + path);
	for (int i = 1; i <= frameCount; i++) {
		char numchar[4];
		snprintf(numchar, 4, "%03d", i);
		String^ num = gcnew String(numchar);
		String^ fileName = path + "\\" + name + num + ".rgb";
		
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
		Console::WriteLine("Loaded frame "  + i + "/" + frameCount);

	}

	Console::WriteLine("Finished loading " + path);

	display->Image = dynamic_cast<Image^>(images[0]);
	
}


