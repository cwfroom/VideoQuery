#include "Data.h"
using namespace VideoQuery;

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