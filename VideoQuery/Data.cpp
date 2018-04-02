#include "Data.h"
using namespace VideoQuery;

VideoQuery::Data::Data() {
	dataVideoList = gcnew array<String^>{
			"flowers",
			"interview",
			"movie",
			"musicvideo",
			"sports",
			"starcraft",
			"traffic"
	};

}

void VideoQuery::Data::LoadDataVideo(int frameCount, String^ folder, String^ name) {
	if (dataVideo) {
		dataVideo->SetVideo(frameCount, folder, name);
		ThreadStart^ dataVideoLoadthreadDelegate = gcnew ThreadStart(dataVideo, &Video::LoadVideo);
		Thread^ dataVideoLoadThread = gcnew Thread(dataVideoLoadthreadDelegate);
		dataVideoLoadThread->Start();
	}
}

void VideoQuery::Data::LoadQueryVideo(int frameCount, String^ folder, String^ name) {
	if (queryVideo) {
		queryVideo->SetVideo(frameCount, folder, name);
		ThreadStart^ queryVideoLoadthreadDelegate = gcnew ThreadStart(queryVideo, &Video::LoadVideo);
		Thread^ queryVideoLoadThread = gcnew Thread(queryVideoLoadthreadDelegate);
		queryVideoLoadThread->Start();
	}
}