#include "Metrics.h"

Metrics::Metrics() :
	dominant_color_frames(NULL),
	optical_flow_x_frames(NULL),
	optical_flow_y_frames(NULL)
{

}

Metrics::~Metrics()
{
	if (dominant_color_frames != NULL) {
		delete dominant_color_frames;
	}
	if (optical_flow_x_frames != NULL) {
		delete optical_flow_x_frames;
	}
	if (optical_flow_y_frames != NULL) {
		delete optical_flow_y_frames;
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
		cv::Mat bgr;
		BGRFromBitmap(image, bgr);

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
	/*
	For each frame, compute motion metric

	BGR image is converted to grayscale and downsized *.25
	Motion metric is taken from Farneback dense optical flow

	Motion metric is saved as Eigen arrays: `this->optical_flow_x_frames` and `this->optical_flow_y_frames`

	*/

	int width, height, wh, reductionscale = 4;
	if (video->GetFrameCount() > 1) {
		Bitmap^ prev = video->GetImage(0);
		width = prev->Width / reductionscale;
		height = prev->Height / reductionscale;
		wh = width * height;

		if (optical_flow_x_frames != NULL) {
			delete optical_flow_x_frames;
		}
		optical_flow_x_frames = new Eigen::ArrayXXf(video->GetFrameCount() - 1, wh);

		if (optical_flow_y_frames != NULL) {
			delete optical_flow_y_frames;
		}
		optical_flow_y_frames = new Eigen::ArrayXXf(video->GetFrameCount() - 1, wh);
	}

	// Set up first previous image
	Bitmap^ prev = video->GetImage(0);
	cv::Mat bgr_prev, gray_prev;
	BGRFromBitmap(prev, bgr_prev);
	cv::cvtColor(bgr_prev, gray_prev, cv::COLOR_BGRA2GRAY);
	cv::resize(gray_prev, gray_prev, cv::Size(width, height), 0, 0, cv::INTER_AREA);

	// Retain flow calculations for cv::OPTFLOW_USE_INITIAL_FLOW as initial flow approx.
	cv::Mat flow;
	for (int f = 0; f < video->GetFrameCount()-1; f++) {
		// Grab next frame
		Bitmap^ next = video->GetImage(f+1);
		cv::Mat bgr_next,  gray_next;
		BGRFromBitmap(next, bgr_next);
		cv::cvtColor(bgr_next, gray_next, cv::COLOR_BGRA2GRAY);
		cv::resize(gray_next, gray_next, cv::Size(width, height), 0, 0, cv::INTER_AREA);

		// cv::namedWindow("Gray");
		// cv::imshow("Gray", gray_next);
		// cv::waitKey(0);

		// Compute optical flow
		cv::calcOpticalFlowFarneback(gray_prev, gray_next, flow, 0.8, 2, 8, 2, 7, 1.5,
			f != 0 ? cv::OPTFLOW_USE_INITIAL_FLOW : cv::OPTFLOW_FARNEBACK_GAUSSIAN);
		
		// Split flow components to x-y
		std::vector<cv::Mat> flow_split;
		cv::split(flow, flow_split);

		Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> x_map(
			flow_split[0].ptr<float>(), 1, wh);
		(*optical_flow_x_frames).row(f) = x_map.row(0);

		Eigen::Map<Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>> y_map(
			flow_split[1].ptr<float>(), 1, wh);
		(*optical_flow_y_frames).row(f) = y_map.row(0);

		// Retain previous gray value
		gray_prev = gray_next;
	}
}

void Metrics::BGRFromBitmap(Bitmap^ bitmap, cv::Mat& bgr) {
	/* https://stackoverflow.com/questions/29018442/systemdrawingbitmap-to-cvmat-opencv-c-cli */
	// Convert bitmap to OpenCV Mat
	System::Drawing::Rectangle blank = System::Drawing::Rectangle(0, 0, bitmap->Width, bitmap->Height);
	System::Drawing::Imaging::BitmapData^ bmpdata = bitmap->LockBits(blank, System::Drawing::Imaging::ImageLockMode::ReadWrite, bitmap->PixelFormat);
	bgr = cv::Mat(cv::Size(bitmap->Width, bitmap->Height), CV_8UC4, bmpdata->Scan0.ToPointer(), cv::Mat::AUTO_STEP);
	bitmap->UnlockBits(bmpdata);
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

float clampToZero(float x) { return (x > 0.0) ? x : 0.0; }

void Metrics::ColorAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Color accuracy measured as inverse distance between the two dominant colors of corresponding frames
	// dist = sqrt( sum over (mychannel-theirchannel)^2 )
	acc = (*(this->dominant_color_frames) - *(other->dominant_color_frames)).cwiseAbs2().rowwise().sum().cwiseSqrt();
	// Invert using 255.0 (largest langth along one axis)
	acc = 1.0 - acc / 255.0;
	// If distance between the two colors is larger than 255.0 (negative acc), then zero accuracy
	acc = acc.unaryExpr(std::ptr_fun(clampToZero));
	return;
}

void Metrics::MotionAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Motion accuracy measured as inverse mean of motion magnitude squared differences
	acc = (*(this->optical_flow_x_frames) - *(other->optical_flow_x_frames)).cwiseAbs2().rowwise().mean().square() +
		(*(this->optical_flow_y_frames) - *(other->optical_flow_y_frames)).cwiseAbs2().rowwise().mean().square();
	// Because flow
	acc = 1.0 - acc;
	acc = acc.unaryExpr(std::ptr_fun(clampToZero));
	return;
}

void Metrics::AudioAccuracy(Metrics^ other, Eigen::ArrayXXf& acc) {
	// Given other and frame, compute audio accuracy, within [0, 1]
	return;
}