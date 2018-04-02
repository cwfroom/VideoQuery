#pragma once
#ifndef DATA_H
#define DATA_H
#include "Video.h"

namespace VideoQuery{
	using namespace System::Threading;

	ref class Data {
		public:
			Data();
			void LoadDataVideo(int frameCount, String^ folder, String^ name);
			void LoadQueryVideo(int frameCount, String^ folder, String^ name);

		public:
			array<String^>^ dataVideoList;

			Video^ queryVideo;
			Video^ dataVideo;
	};
}


#endif // !DATA_H
