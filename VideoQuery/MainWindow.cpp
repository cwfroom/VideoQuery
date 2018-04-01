#include "MainWindow.h"
#include "guicon.h"
#include "Image.h"

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


System::Void MainWindow::videoPlayButton_Click(System::Object^  sender, System::EventArgs^  e) {
	MyImage testImage;
	testImage.setImagePath("database_videos/flowers/flowers001.rgb");
	testImage.ReadImage();
	char* rawImageData = testImage.getImageData();
	
	if (!rawImageData) {
		MessageBox::Show("Failed to load image", "Error",MessageBoxButtons::OK);
	}

	if (rawImageData) {
		Bitmap^ bitmap = gcnew Bitmap(testImage.getWidth(), testImage.getHeight());
		for (int i = 0; i < bitmap->Height; i++) {
			for (int j = 0; j < bitmap->Width; j++) {
				unsigned char b = rawImageData[bitmap->Width * i * 3 + 3 * j];
				unsigned char g = rawImageData[bitmap->Width * i * 3 + 3 * j + 1];
				unsigned char r = rawImageData[bitmap->Width * i * 3 + 3 * j + 2];
				bitmap->SetPixel(j, i, Drawing::Color::FromArgb(255, r, g, b));
			}
		}

		dataVideoImage->Image = dynamic_cast<Image^>(bitmap);
	}



}