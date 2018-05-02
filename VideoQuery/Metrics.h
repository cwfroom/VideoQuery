#pragma once
#ifndef  METRICS_H
#define METRICS_H

/*
 * Metrics class has metric information for an associated video.
 * Use Accuracy(other, frame) to compute accuracy with other Metrics
 */
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <Eigen/Core>
#include "Video.h"


using namespace VideoQuery;

struct WAVHeader {
	char chunkID[4]; //"RIFF"
	unsigned long chunkSize; // File size
	char format[4]; // "WAVE"
	char subchunk1ID[4]; // "fmt "
	unsigned long subchunk1Size; // Length of above data
	unsigned short audioFormat; // Format type (1: PCM)
	unsigned short numChannels; // Number of channels
	unsigned long sampleRate; // Sample Rate = Number of Samples per second, or Hertz.
	unsigned long byteRate; // (Sample Rate * BitsPerSample * Channels) / 8.
	unsigned short blockAlign;
	unsigned short bitsPerSample;
	unsigned char data_chunk_header[4];
	unsigned int data_size;
};

void DumpArray(const char* filename, Eigen::ArrayXXf& arr);
void ReadArray(const char* filename, Eigen::ArrayXXf& arr);

ref class Metrics
{
private:
	Eigen::ArrayXXf* dominant_color_frames;   // rows: frame, col: BGR
	Eigen::ArrayXXf* optical_flow_x_frames;   // rows: frame-pair, col: flow x-component
	Eigen::ArrayXXf* optical_flow_y_frames;   // rows: frame-pair, col: flow y-component
	Eigen::VectorXf* audio_samples;			  // vector of mono audio samples
	Video^ video;
	Metrics^ query;

	// To be filled in by Accuracy()
	Eigen::ArrayXXf* perFrameAccuracy;
	int maxStartFrame;
	float maxAccuracy;

	void ComputeColorMetric();
	void ComputeMotionMetric();
	void ComputeAudioMetric();
	void ColorAccuracy(Metrics^ query, Eigen::ArrayXXf& acc);
	void MotionAccuracy(Metrics^ query, Eigen::ArrayXXf& acc);
	void AudioAccuracy(Metrics^ query, Eigen::ArrayXXf& acc);


public:
	int audio_sample_rate;
	int frames;

	Metrics();
	~Metrics();
	void SetVideo(Video^ video);
	void Dump();
	void Read();
	void Compute();
	void ComputeDump() { Compute(); Dump(); }

	// Call accuracy functions with this as the database video, other as query video
	void SetQuery(Metrics^ query);
	void Accuracy();

	void BGRFromBitmap(Bitmap^ bitmap, cv::Mat& bgr);

	int GetMaxStartFrame() { return maxStartFrame; }
	float GetMaxAccuracy() { return maxAccuracy; }
	Eigen::ArrayXXf* GetPerFrameAccuracy() { return perFrameAccuracy; }
};

#endif // ! METRICS_H