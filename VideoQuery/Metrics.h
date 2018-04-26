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
#include <Eigen/Core>
#include "Video.h"


using namespace VideoQuery;

ref class Metrics
{
private:
	Eigen::ArrayXXf* dominant_color_frames; // rows: frame, col: BGR

public:
	Metrics();
	~Metrics();
	void Compute(Video^ video);
	void ComputeColorMetric(Video^ video);
	void ComputeMotionMetric(Video^ video);
	void ComputeAudioMetric(Video^ video);
	void Accuracy(Metrics^ other, Eigen::ArrayXXf& acc);
	void ColorAccuracy(Metrics^ other, Eigen::ArrayXXf& acc);
	void MotionAccuracy(Metrics^ other, Eigen::ArrayXXf& acc);
	void AudioAccuracy(Metrics^ other, Eigen::ArrayXXf& acc);
};

#endif // METRICS_H