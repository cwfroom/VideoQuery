#include "Data.h"
using namespace VideoQuery;

VideoQuery::Data::Data() {
	dataVideo = gcnew Video();
	queryVideo = gcnew Video();
	dataVideoList = gcnew array<String^>{
			"flowers",
			"interview",
			"movie",
			"musicvideo",
			"sports",
			"starcraft",
			"traffic"
	};
	dataMetrics = gcnew array<Metrics^>(dataVideoList->Length);
	dataVideos = gcnew array<Video^>(dataVideoList->Length);

	lastSelectedIndex = -1;
	dataStartFrames = gcnew array<int>(dataVideoList->Length);
	dataMaxAccuracy = gcnew array<float>(dataVideoList->Length);
	dataPerFrameAccuracy = gcnew array<Eigen::ArrayXXf*>(dataVideoList->Length);
	dataColorAccuracy = gcnew array<Eigen::ArrayXXf*>(dataVideoList->Length);
	dataMotionAccuracy = gcnew array<Eigen::ArrayXXf*>(dataVideoList->Length);
	dataAudioAccuracy = gcnew array<Eigen::ArrayXXf*>(dataVideoList->Length);
	displayToVideoListMapping = gcnew array<int>(dataVideoList->Length);
	for (int i = 0; i < dataVideoList->Length; i++) {
		dataStartFrames[i] = 0;
		displayToVideoListMapping[i] = i;
		dataPerFrameAccuracy[i] = NULL;
		dataColorAccuracy[i] = NULL;
		dataMotionAccuracy[i] = NULL;
		dataAudioAccuracy[i] = NULL;
	}
}

void VideoQuery::Data::LoadDataVideo(int frameCount, String^ folder, String^ name) {
	dataVideo->SetVideo(frameCount, folder, name);
	ThreadStart^ dataVideoLoadthreadDelegate = gcnew ThreadStart(dataVideo, &Video::LoadVideo);
	Thread^ dataVideoLoadThread = gcnew Thread(dataVideoLoadthreadDelegate);
	dataVideoLoadThread->Start();
}

void VideoQuery::Data::LoadQueryVideo(int frameCount, String^ folder, String^ name) {	
	queryVideo->SetVideo(frameCount, folder, name);
	queryVideo->LoadVideo();
	if (queryVideo->IsLoaded()) {
		ComputeAccuracy();
	}
}

array<String^>^ VideoQuery::Data::GetSortedAccuracyStrings() {
	array<String^>^ strs = gcnew array<String^>(dataVideoList->Length);
	for (int displayid = 0; displayid < dataVideoList->Length; displayid++) {
		int originalid = displayToVideoListMapping[displayid];
		strs[displayid] = dataVideoList[originalid] + " (" + (dataMaxAccuracy[originalid] * 100.0).ToString("0.0000") + ")";
	}
	return strs;
}

Eigen::VectorXf VideoQuery::Data::getAccuracyHelper(array<Eigen::ArrayXXf*>^ arr) {
	if (lastSelectedIndex != -1 && arr[lastSelectedIndex] != NULL) {
		Eigen::VectorXf accuracy = Eigen::Map<Eigen::VectorXf>(
			arr[lastSelectedIndex]->data(),
			arr[lastSelectedIndex]->rows() * arr[lastSelectedIndex]->cols());
		return accuracy;
	}
	else {
		return Eigen::VectorXf::Zero(1);
	}
}

void VideoQuery::Data::ComputeAccuracy() {
	/* Compute metric for query video, then compute accuracy against all database metrics */

	// Compute metric for video 
	queryMetric = gcnew Metrics();
	queryMetric->SetVideo(queryVideo);
	queryMetric->Compute();

	array<ThreadStart^>^ threaddeligates = gcnew array<ThreadStart^>(dataVideoList->Length);
	array<Thread^>^ threads = gcnew array<Thread^>(dataVideoList->Length);
	for (int i = 0; i < dataVideoList->Length; i++) {
		// Compute accuracy against all database metrics
		dataMetrics[i]->SetQuery(queryMetric);

		// Note that this can be run in parallel because doesn't depend on Video frames
		threaddeligates[i] = gcnew ThreadStart(dataMetrics[i], &Metrics::Accuracy);
		threads[i] = gcnew Thread(threaddeligates[i]);
		threads[i]->Start();
	}
	for (int i = 0; i < dataVideoList->Length; i++) {
		threads[i]->Join();
	}
	for (int i = 0; i < dataVideoList->Length; i++) {
		dataMaxAccuracy[i] = dataMetrics[i]->GetMaxAccuracy();
		dataStartFrames[i] = dataMetrics[i]->GetMaxStartFrame();
		dataPerFrameAccuracy[i] = dataMetrics[i]->GetPerFrameAccuracy();
		dataColorAccuracy[i] = dataMetrics[i]->GetColorAccuracy();
		dataMotionAccuracy[i] = dataMetrics[i]->GetMotionAccuracy();
		dataAudioAccuracy[i] = dataMetrics[i]->GetAudioAccuracy();
	}

	// Compute mapping between dataVideoList and accuracy list
	std::vector<Pair> pairs(dataVideoList->Length);
	for (int i = 0; i < dataVideoList->Length; i++) {
		pairs[i] = Pair{ dataMaxAccuracy[i], i };
	}
	// Desc sort
	std::sort(pairs.rbegin(), pairs.rend());
	// Update mappings
	for (int i = 0; i < dataVideoList->Length; i++) {
		displayToVideoListMapping[i] = pairs[i].id;
	}
}

void VideoQuery::Data::SwapDataVideo(int index) {
	// index is the display index
	dataVideos[displayToVideoListMapping[index]]->CopyUI(dataVideo);
	dataVideo = dataVideos[displayToVideoListMapping[index]];
	dataVideo->SeekVideo(dataStartFrames[displayToVideoListMapping[index]]);
	lastSelectedIndex = index;
}

void VideoQuery::Data::LoadDatabaseMetrics(int dataFrameCount) {
	array<ThreadStart^>^ threaddeligates = gcnew array<ThreadStart^>(dataVideoList->Length*2);
	array<Thread^>^ threads = gcnew array<Thread^>(dataVideoList->Length*2);
	for (int i = 0; i < dataVideoList->Length; i++) {
		dataVideos[i] = gcnew Video();
		dataVideos[i]->SetVideo(dataFrameCount, "database_videos", dataVideoList[i]);

		threaddeligates[i] = gcnew ThreadStart(dataVideos[i], &Video::LoadVideo);
		threads[i] = gcnew Thread(threaddeligates[i]);
		threads[i]->Start();
	}

	for (int i = 0; i < dataVideoList->Length; i++) {
		dataMetrics[i] = gcnew Metrics();
		dataMetrics[i]->SetVideo(dataVideos[i]);

		// Note that this can be run in parallel because doesn't depend on Video frames
		threaddeligates[i + dataVideoList->Length] = gcnew ThreadStart(dataMetrics[i], &Metrics::Read);
		threads[i + dataVideoList->Length] = gcnew Thread(threaddeligates[i + dataVideoList->Length]);
		threads[i + dataVideoList->Length]->Start();
	}

	// Make sure everything is good because loading ...
	for (int i = 0; i < dataVideoList->Length*2; i++) {
		threads[i]->Join();
	}
}

void VideoQuery::Data::GenerateDatabaseMetrics(int dataFrameCount) {
	array<ThreadStart^>^ threaddeligates = gcnew array<ThreadStart^>(dataVideoList->Length);
	array<Thread^>^ threads = gcnew array<Thread^>(dataVideoList->Length);
	for (int i = 0; i < dataVideoList->Length; i++) {
		dataVideos[i] = gcnew Video();
		dataVideos[i]->SetVideo(dataFrameCount, "database_videos", dataVideoList[i]);

		threaddeligates[i] = gcnew ThreadStart(dataVideos[i], &Video::LoadVideo);
		threads[i] = gcnew Thread(threaddeligates[i]);
		threads[i]->Start();
	}
	for (int i = 0; i < dataVideoList->Length; i++) {
		threads[i]->Join();
	}

	for (int i = 0; i < dataVideoList->Length; i++) {
		dataMetrics[i] = gcnew Metrics();
		dataMetrics[i]->SetVideo(dataVideos[i]);

		threaddeligates[i] = gcnew ThreadStart(dataMetrics[i], &Metrics::ComputeDump);
		threads[i] = gcnew Thread(threaddeligates[i]);
		threads[i]->Start();
	}
	for (int i = 0; i < dataVideoList->Length; i++) {
		threads[i]->Join();
	}
}