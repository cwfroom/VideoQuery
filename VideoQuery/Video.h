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
			Video(PictureBox^ display,Label^ label);
			void SetVideo(int frameCount, String^ folder, String^ name);
			void LoadVideo();
		private:
			delegate void StringDelegate(String^);
			void UpdateLabel(String^ text);
			delegate void BoolDelegate(bool);
			void SetLabelVisibility(bool visible);

		private:
			int frameCount;
			String^ folder;
			String^ name;

			List<Bitmap^> images;
			PictureBox^ display;
			Label^ label;
	};

}


#endif // ! VIDEO_H
