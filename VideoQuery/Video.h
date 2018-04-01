#pragma once
#ifndef  VIDEO_H
#define VIDEO_H

#include "Image.h"

namespace VideoQuery {
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;

	ref class Video {
		public:
			Video(PictureBox^ display);
			void LoadVideo(int frameCount, String^ folder, String^ name);

		private:


		public:
			int frameCount;

		private:
			List<Bitmap^> images;
			PictureBox^ display;
	};

}


#endif // ! VIDEO_H
