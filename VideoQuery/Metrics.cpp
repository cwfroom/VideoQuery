#include "Metrics.h"

Metrics::Metrics() : dominant_color_frames(NULL)
{

}

Metrics::~Metrics()
{
	if (dominant_color_frames != NULL) {
		// delete dominant_color_frames;
	}
}

void Metrics::Compute(Video^ video) {
	// for each frame, compute all metrics
	ComputeColorMetric(video);
	ComputeMotionMetric(video);
	ComputeAudioMetric(video);
}

void Metrics::ComputeColorMetric(Video^ video) {
	/*
		For each frame, compute dominant color

		Dominant color is found by:
			- Converting BGR image to HSV
			- Calculating histogram of H values
			- Finding max H value of histogram (after Gaussian filter across bins)
			- Find expected S and V values with this particular H value
			- Convert this dominant HSV value to BGR

		Dominant color is saved as Eigen array: `this->dominant_color_frames`
	*/

	std::vector<float> dominant_color(video->GetFrameCount() * 3);
	for (int f = 0; f < video->GetFrameCount(); f++) {
		Bitmap^ image = video->GetImage(f);

		/* https://stackoverflow.com/questions/29018442/systemdrawingbitmap-to-cvmat-opencv-c-cli */
		// Convert bitmap to OpenCV Mat
		System::Drawing::Rectangle blank = System::Drawing::Rectangle(0, 0, image->Width, image->Height);
		System::Drawing::Imaging::BitmapData^ bmpdata = image->LockBits(blank, System::Drawing::Imaging::ImageLockMode::ReadWrite, image->PixelFormat);
		cv::Mat bgr(cv::Size(image->Width, image->Height), CV_8UC4, bmpdata->Scan0.ToPointer(), cv::Mat::AUTO_STEP);
		image->UnlockBits(bmpdata);

		// Convert to HSV
		cv::Mat hsv;
		cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV);
		
		// H is in [0, 179]
		int HhistSize = 180;
		float Hrange[] = { 0, 180 };
		const float* HhistRange = { Hrange };

		// S,V are in [0, 255]
		int SVhistSize = 256;
		float SVrange[] = { 0, 256 };
		const float* SVhistRange = { SVrange };

		// Channel identifiers
		int Hchannels[] = { 0 };
		int HSchannels[] = { 0, 1 };
		int HVchannels[] = { 0, 2 };

		// Compute histograms per HSV channel
		cv::Mat H_hist;
		
		// Note histograms have value in each row (1xSIZE matrix)
		calcHist(&hsv, 1, Hchannels, cv::Mat(), H_hist, 1, &HhistSize, &HhistRange);
		
		// Make wrap border for H (H is circular)
		int gaussian_filtersize = 41;
		cv::copyMakeBorder(H_hist, H_hist, gaussian_filtersize, gaussian_filtersize, 0, 0, cv::BORDER_WRAP);

		// Gaussian blur
		cv::GaussianBlur(H_hist, H_hist, cv::Size(1, gaussian_filtersize), 0, 0);
		
		// Crop away borders
		H_hist = H_hist(cv::Rect(0, gaussian_filtersize, 1, HhistSize));
		
		// Find max positions (This is dominant H value)
		cv::Point max_loc;
		cv::minMaxLoc(H_hist, 0, 0, &cv::Point(), &max_loc);
		float dominant_H_value = max_loc.y;

		// Find expected S and V for this dominant H value by calculating histogram on HS, HV
		cv::Mat hisths;
		cv::Mat histhv;

		int HSHVsizes[] = { HhistSize, SVhistSize };
		const float* HSHVranges[] = { HhistRange, SVhistRange };

		cv::calcHist(&hsv, 1, HSchannels, cv::Mat(), hisths, 2, HSHVsizes, HSHVranges);
		cv::calcHist(&hsv, 1, HVchannels, cv::Mat(), histhv, 2, HSHVsizes, HSHVranges);

		// Filter around H value
		cv::GaussianBlur(hisths, hisths, cv::Size(gaussian_filtersize, 1), 0, 0);
		cv::GaussianBlur(histhv, histhv, cv::Size(gaussian_filtersize, 1), 0, 0);

		// Pull row of dominant H value (to find saturation and value of particular H)
		cv::Mat hsvector(cv::Size(SVhistSize, 1), CV_32F, hisths.ptr<float>(dominant_H_value));
		cv::Mat	hvvector(cv::Size(SVhistSize, 1), CV_32F, histhv.ptr<float>(dominant_H_value));

		// Get expected value of value bin
		float bin;
		cv::MatIterator_<float> hvit, hvend;
		float value_EV = 0.0;
		for (hvit = hvvector.begin<float>(), hvend = hvvector.end<float>(), bin = 0.0; hvit != hvend; ++hvit, ++bin) {
			value_EV += (*hvit) * bin;
		}
		cv::Scalar sumv = cv::sum(hvvector);
		value_EV /= sumv[0];

		// Get expected value of saturation bin
		cv::MatIterator_<float> hsit, hsend;
		float saturation_EV = 0.0;
		for (hsit = hsvector.begin<float>(), hsend = hsvector.end<float>(), bin = 0.0; hsit != hsend; ++hsit, ++bin) {
			saturation_EV += (*hsit) * bin;
		}
		cv::Scalar sums = cv::sum(hsvector);
		saturation_EV /= sums[0];

		// Save as BGR
		cv::Mat dominant(cv::Size(1, 1), CV_8UC3, cv::Scalar(dominant_H_value, saturation_EV, value_EV));
		cv::cvtColor(dominant, dominant, cv::COLOR_HSV2BGR);
		dominant_color[f * 3 + 0] = dominant.at<uchar>(0, 0);
		dominant_color[f * 3 + 1] = dominant.at<uchar>(0, 1);
		dominant_color[f * 3 + 2] = dominant.at<uchar>(0, 2);
	}

	// Save dominant colors as Eigen array
	Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> dominant_color_map(
		&dominant_color[0], video->GetFrameCount(), 3);
	dominant_color_frames = new Eigen::ArrayXXf(dominant_color_map);
}

void Metrics::ComputeMotionMetric(Video^ video) {
	// for pairs of frames, compute motion metric

}

void Metrics::ComputeAudioMetric(Video^ video) {
	// for pairs of frames, compute audio metric

}

void Metrics::Accuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Compute weighted accuracy between this and other for frame
	Eigen::ArrayXXf coloracc;
	ColorAccuracy(other, coloracc);
	acc = coloracc;
	return;
}
 
void Metrics::ColorAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// dist = sqrt( sum over (mychannel-theirchannel)^2 )
	acc = (*(this->dominant_color_frames) - *(other->dominant_color_frames)).cwiseAbs2().rowwise().sum().cwiseSqrt();
	// 1 - dist / 441.67   --  NOTE: 441.67 ~= sqrt(3) * 255
	acc = 1.0 - acc / 441.67;
	return;
}

void Metrics::MotionAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Given other and frame, compute motion accuracy, within [0, 1]
	return;
}

void Metrics::AudioAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Given other and frame, compute audio accuracy, within [0, 1]
	return;
}