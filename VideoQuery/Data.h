#pragma once
#ifndef DATA_H
#define DATA_H
#include "Video.h"

namespace VideoQuery{
	using namespace System::Threading;

	ref class Data {
		public:
			void LoadDataVideo(int frameCount, String^ folder, String^ name);
			void LoadQueryVideo(int frameCount, String^ folder, String^ name);

		public:
			Video^ queryVideo;
			Video^ dataVideo;
	};
}


#endif // !DATA_H
