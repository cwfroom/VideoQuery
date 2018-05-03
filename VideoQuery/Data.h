#pragma once
#ifndef DATA_H
#define DATA_H
#include <algorithm>
#include "Video.h"
#include "Metrics.h"

namespace VideoQuery{
	using namespace System::Threading;

	struct Pair {
		float acc;
		int id;

		bool operator<(const Pair& a) const
		{
			return this->acc < a.acc;
		}
	};

	ref class Data {
		public:
			Data();
			void LoadDataVideo(int frameCount, String^ folder, String^ name);
			void LoadQueryVideo(int frameCount, String^ folder, String^ name);
			void SwapDataVideo(int index);
			void LoadDatabaseMetrics(int dataFrameCount);
			void GenerateDatabaseMetrics(int dataFrameCount);
			void ComputeAccuracy();
			array<String^>^ GetSortedAccuracyStrings();
			Eigen::VectorXf getAccuracyHelper(array<Eigen::ArrayXXf*>^);
			Eigen::VectorXf getPerFrameAccuracy() { return getAccuracyHelper(dataPerFrameAccuracy); }
			Eigen::VectorXf getColorAccuracy() { return getAccuracyHelper(dataColorAccuracy); }
			Eigen::VectorXf getMotionAccuracy() { return getAccuracyHelper(dataMotionAccuracy); }
			Eigen::VectorXf getAudioAccuracy() { return getAccuracyHelper(dataAudioAccuracy); }


		public:
			array<String^>^ dataVideoList;
			array<Metrics^>^ dataMetrics;   // Aligned with dataVideoList
			array<Video^>^ dataVideos;      // Aligned with dataVideoList

			array<int>^ displayToVideoListMapping; // this[index] => index of dataVideoList
			array<int>^ videoListToDisplayMapping; // this[index] => index of being displayed
			array<int>^ dataStartFrames;
			array<float>^ dataMaxAccuracy;
			array<Eigen::ArrayXXf*>^ dataPerFrameAccuracy;
			array<Eigen::ArrayXXf*>^ dataColorAccuracy;
			array<Eigen::ArrayXXf*>^ dataMotionAccuracy;
			array<Eigen::ArrayXXf*>^ dataAudioAccuracy;

			Metrics^ queryMetric;
			int lastSelectedIndex;

			Video^ queryVideo;
			Video^ dataVideo;
	};
}


#endif // !DATA_H
