#pragma once
#ifndef  VIDEO_H
#define VIDEO_H

#include "Image.h"
#include "Audio.h"
namespace VideoQuery {
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace System::Threading;
	using namespace System::Timers;

	ref class Video {
		public:
			Video();
			void SetUI(PictureBox^ display, Label^ label, TrackBar^ trackBar, Button^ button);
			void CopyUI(Video^ other);
			void SetVideo(int frameCount, String^ folder, String^ name);
			void LoadVideo();
			void PlayVideo();
			void PauseVideo();
			void SeekVideo(int);
			int GetFrameCount() { return frameCount; }
			Bitmap^ GetImage(int frame) { return images[frame]; }
			String^ GetAudioFilePath() { return folder + "\\" + name + "\\" + name + ".wav";  }
			String^ GetName() { return name; }
			bool IsLoaded() { return loaded; }
			void SetUnderscore(bool);

		private:
			delegate void StringDelegate(String^);
			void UpdateLabel(String^ text);
			void UpdateButton(String^ text);
			delegate void BoolDelegate(bool);
			void SetLabelVisibility(bool visible);
			delegate void IntDelegate(int);
			void SetTrackBarValue(int value);
			void UpdateFrame(Object^ sender, ElapsedEventArgs^ e);
			bool loaded;
		private:
			const float interval = 31;
			int frameCount;
			int currentFrame;
			String^ folder;
			String^ name;
			List<Bitmap^> images;
			PictureBox^ display;
			Label^ label;
			TrackBar^ trackBar;
			Button^ button;
			Timers::Timer^ playbackTimer;
			bool hasUnderscore;
			Audio* audioPlayer;
	};

}


#endif // ! VIDEO_H
