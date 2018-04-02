#pragma once
#ifndef  VIDEO_H
#define VIDEO_H

#include "Image.h"

namespace VideoQuery {
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace System::Threading;

	ref class Video {
		public:
			Video(PictureBox^ display);
			void SetVideo(int frameCount, String^ folder, String^ name);
			void LoadVideo();

		private:
			int frameCount;
			String^ folder;
			String^ name;

			List<Bitmap^> images;
			PictureBox^ display;
	};

}


#endif // ! VIDEO_H
