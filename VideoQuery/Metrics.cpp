#include "Metrics.h"

Metrics::Metrics() :
	dominant_color_frames(NULL),
	optical_flow_x_frames(NULL),
	optical_flow_y_frames(NULL),
	audio_samples(NULL),
	perFrameAccuracy(NULL)
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
	if (audio_samples != NULL) {
		delete audio_samples;
	}
	if (perFrameAccuracy != NULL) {
		delete perFrameAccuracy;
	}
}

void Metrics::SetVideo(Video^ video) {
	this->video = video;
}

void DumpArray(const char* filename, Eigen::ArrayXXf& arr) {
	FILE * ptr;
	ptr = fopen(filename, "wb");
	unsigned int cols = arr.cols();
	unsigned int rows = arr.rows();
	fwrite(&rows, sizeof(unsigned int), 1, ptr);
	fwrite(&cols, sizeof(unsigned int), 1, ptr);
	fwrite(arr.data(), sizeof(float) * cols * rows, 1, ptr);
	fclose(ptr);
}

void ReadArray(const char* filename, Eigen::ArrayXXf& arr) {
	FILE * ptr;
	ptr = fopen(filename, "rb");
	unsigned int rows;
	unsigned int cols;
	fread(&rows, sizeof(unsigned int), 1, ptr);
	fread(&cols, sizeof(unsigned int), 1, ptr);
	std::vector<float> mat_data(cols * rows);
	fread(&mat_data[0], sizeof(float) * cols * rows, 1, ptr);
	fclose(ptr);
	arr = Eigen::Map<Eigen::ArrayXXf>(&mat_data[0], rows, cols);
}

void Metrics::Dump() {
	/*
	* Dumps metric information into file
	* Assumes that this->Compute(..) has been called
	*
	* Prepends metric type to filename
	*
	*/
	std::string filenamestr(
		static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(video->GetName()).ToPointer()));

	/* Dominant Colors (ArrayXXf) */
	DumpArray((std::string("dominant_colors.") + filenamestr).c_str(), *(this->dominant_color_frames));

	/* Motion Values (ArrayXXf) */
	DumpArray((std::string("motion_vector_x.") + filenamestr).c_str(), *(this->optical_flow_x_frames));
	DumpArray((std::string("motion_vector_y.") + filenamestr).c_str(), *(this->optical_flow_y_frames));

	/* Audio Samples (VectorXf) */
	// Don't dump - Just "compute" from WAV file (basically reading WAV, we don't alter it)
}

void Metrics::Read() {
	/*
	* Read metric information from file
	*
	* Prepends metric type to filename to read
	* Deletes old metrics
	*
	*/
	std::string filenamestr(
		static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(video->GetName()).ToPointer()));

	// Just make sure we have this ... Not the best design ..
	this->frames = video->GetFrameCount();

	/* Dominant Colors (MatrixXf) */
	Eigen::ArrayXXf dominant_colors_arr;
	ReadArray((std::string("dominant_colors.") + filenamestr).c_str(), dominant_colors_arr);
	if (this->dominant_color_frames != NULL) {
		delete this->dominant_color_frames;
	}
	this->dominant_color_frames = new Eigen::ArrayXXf(dominant_colors_arr);

	/* Motion Vectors(MatrixXf) */
	Eigen::ArrayXXf optical_flow_x_frames_arr;
	Eigen::ArrayXXf optical_flow_y_frames_arr;
	ReadArray((std::string("motion_vector_x.") + filenamestr).c_str(), optical_flow_x_frames_arr);
	ReadArray((std::string("motion_vector_y.") + filenamestr).c_str(), optical_flow_y_frames_arr);
	if (this->optical_flow_x_frames != NULL) {
		delete this->optical_flow_x_frames;
	}
	if (this->optical_flow_y_frames != NULL) {
		delete this->optical_flow_y_frames;
	}
	this->optical_flow_x_frames = new Eigen::ArrayXXf(optical_flow_x_frames_arr);
	this->optical_flow_y_frames = new Eigen::ArrayXXf(optical_flow_y_frames_arr);

	/* Audio Samples */
	// We aren't changing anything .. just re-read the WAV
	ComputeAudioMetric();
}

void Metrics::Compute() {
	while (!video->IsLoaded()) {
		// Hack for serial processing, only compute metric if video is loaded...
		// Maybe just don't have video depend on this to prevent deadlock x_x
		Thread::Sleep(100);
	}
	// for each frame, compute all metrics
	this->frames = video->GetFrameCount();

	ComputeColorMetric();
	ComputeAudioMetric();
	ComputeMotionMetric();
}

void Metrics::ComputeColorMetric() {
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
	if (this->dominant_color_frames != NULL) {
		delete this->dominant_color_frames;
	}

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
	Eigen::Map<Eigen::ArrayXXf> dominant_color_map(
		&dominant_color[0], 3, video->GetFrameCount());
	dominant_color_frames = new Eigen::ArrayXXf(dominant_color_map);
}

void Metrics::ComputeMotionMetric() {
	/*
	For each frame, compute motion metric

	BGR image is converted to grayscale and downsized *.25
	Motion metric is taken from Farneback dense optical flow

	Motion metric is saved as Eigen arrays: `this->optical_flow_x_frames` and `this->optical_flow_y_frames`

	Columns of optical motion vector per frame

	*/

	int width, height, fwidth, fheight, wh, initialreductionscale = 8;
	fwidth = 4;
	fheight = 4;
	if (video->GetFrameCount() > 1) {
		Bitmap^ prev = video->GetImage(0);
		width = prev->Width / initialreductionscale;
		height = prev->Height / initialreductionscale;
		wh = fwidth * fheight;

		if (optical_flow_x_frames != NULL) {
			delete optical_flow_x_frames;
		}
		optical_flow_x_frames = new Eigen::ArrayXXf(wh, video->GetFrameCount() - 1);

		if (optical_flow_y_frames != NULL) {
			delete optical_flow_y_frames;
		}
		optical_flow_y_frames = new Eigen::ArrayXXf(wh, video->GetFrameCount() - 1);
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

		// Compute optical flow
		cv::calcOpticalFlowFarneback(gray_prev, gray_next, flow, 0.8, 2, 8, 2, 7, 1.5,
			f != 0 ? cv::OPTFLOW_USE_INITIAL_FLOW : cv::OPTFLOW_FARNEBACK_GAUSSIAN);
		
		// Reduce and split flow components to x-y
		cv::Mat flowtosave;
		cv::resize(flow, flowtosave, cv::Size(fwidth, fheight), 0, 0, cv::INTER_AREA);
		std::vector<cv::Mat> flow_split;
		cv::split(flow, flow_split);

		Eigen::Map<Eigen::ArrayXXf> x_map(
			flow_split[0].ptr<float>(), wh, 1);
		(*optical_flow_x_frames).col(f) = x_map.col(0);

		Eigen::Map<Eigen::ArrayXXf> y_map(
			flow_split[1].ptr<float>(), wh, 1);
		(*optical_flow_y_frames).col(f) = y_map.col(0);

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

void Metrics::ComputeAudioMetric() {
	if (this->audio_samples != NULL) {
		delete this->audio_samples;
	}

	FILE *ptr;

	ptr = fopen(static_cast<char*>(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(video->GetAudioFilePath()).ToPointer()), "rb");
	if (ptr == NULL) {
		exit(1);
	}

	int read = 0;
	struct WAVHeader header;
	read = fread(&header, sizeof(header), 1, ptr);

	//Number of samples
	int samples_count = header.data_size / header.blockAlign; // this is number of samples PER CHANNEL
	int total_samples_count = samples_count * header.numChannels; // total number of samples
	int sample_size = header.blockAlign / header.numChannels; // sample size per channel
	this->audio_sample_rate = header.sampleRate;

	std::vector<short> value(total_samples_count);

	// Go through data stream, keep channels interleaved
	for (int i = 0; i < total_samples_count; i++)
	{
		fread(&value[i], sample_size, 1, ptr);
	}

	fclose(ptr);

	// Ensure we have enough samples
	while (value.size() < video->GetFrameCount() / 30 * audio_sample_rate * header.numChannels) {
		value.push_back(0);
	}

	// Map data to eigen
	Eigen::Map<Eigen::Matrix<short, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>> wave_map(
		&value[0], header.numChannels, video->GetFrameCount() / 30 * audio_sample_rate);

	// Return mean of the channels
	audio_samples = new Eigen::VectorXf(wave_map.cast<float>().colwise().mean().row(0));
}

void Metrics::SetQuery(Metrics^ v_query) {
	this->query = v_query;
}

void Metrics::Accuracy() {
	/* Returns the start frame with the highest accuracy total accuracy
	 *
	 * acc is loaded with the N vector of per-frame accuracies that has highest total accuracy,
	 * where N is the framecount of the query
	 *
	 * accf is the value of the highest total accuracy
	 */
	float color_weight = 0.0,
		motion_weight = 1.0,
		audio_weight = 0.0;
	
	Eigen::ArrayXXf coloracc;
	ColorAccuracy(query, coloracc);
	Eigen::ArrayXXf coloracc_mean;
	coloracc_mean = coloracc.colwise().mean();
	
	Eigen::ArrayXXf motionacc;
	MotionAccuracy(query, motionacc);
	Eigen::ArrayXXf motionacc_mean;
	motionacc_mean = motionacc.colwise().mean();

	Eigen::ArrayXXf audioacc;
	AudioAccuracy(query, audioacc);
	Eigen::ArrayXXf audioacc_mean;
	audioacc_mean = audioacc.colwise().mean();
	
	// Find weighted mean accuracies
	Eigen::ArrayXXf meanacc =
		coloracc_mean  * color_weight +
		motionacc_mean * motion_weight +
		audioacc_mean  * audio_weight;

	// Get start frame with largest mean accuracy
	Eigen::ArrayXXf::Index max_i, max_j;
	maxAccuracy = meanacc.maxCoeff(&max_i, &max_j);

	// Set per-frame accuracy beginning at start frame
	if (perFrameAccuracy != NULL) {
		delete perFrameAccuracy;
	}
	perFrameAccuracy = new Eigen::ArrayXXf(
		coloracc.col(max_j) * color_weight +
		motionacc.col(max_j) * motion_weight +
		audioacc.col(max_j) * audio_weight);

	// Set max start frame
	maxStartFrame = (int) max_j;
}

float clampToZero(float x) { return (x > 0.0) ? x : 0.0; }

void Metrics::ColorAccuracy(Metrics^ query, Eigen::ArrayXXf& acc) {
	/* Call with this as the database video, other as query video
	 *
	 * Load 2D acc:
	 *  - Cols: database frame offset
	 *  - Rows: Accuracy per query frame (Inverted BGR distance, clamping at 255.0)
	 *
	 * This means that col i gives frame-corresponding accuracy when
	 * query video is matched with database video, beginning at frame i.
	 *
	 * Use col-wise mean to find "total" accuracy when matching with database video, beginning at frame `col`
	 *
	 */
	acc = Eigen::ArrayXXf();
	int total_offset = this->frames - query->frames + 1;
	acc.resize(query->frames-1, total_offset); // Remove last offset

	for (int frame_offset = 0; frame_offset < total_offset; frame_offset++) {
		// dist = sqrt( sum over (mychannel-theirchannel)^2 )
		acc.col(frame_offset) = ((*(this->dominant_color_frames)).block(0, frame_offset, 3, query->frames) - *(query->dominant_color_frames)).cwiseAbs2().colwise().sum().cwiseSqrt().block(0,0,1, query->frames - 1).transpose();
	}

	// Invert using 255.0 (largest langth along one axis)
	acc = 1.0 - acc / 255.0;

	// If distance between the two colors is larger than 255.0 (negative acc), then zero accuracy
	acc = acc.unaryExpr(std::ptr_fun(clampToZero));

	return;
}

void Metrics::MotionAccuracy(Metrics^ query, Eigen::ArrayXXf& acc) {

	/* Call with this as the database video, other as query video
	*
	* Load 2D acc:
	*  - Cols: database frame offset
	*  - Rows: Accuracy per query frame (Inverse mean of motion magnitude, clamping at 1.0)
	*
	* This means that col i gives frame-corresponding accuracy when
	* query video is matched with database video, beginning at frame i.
	*
	* Use col-wise mean to find "total" accuracy when matching with database video, beginning at frame `col`
	*
	*/
	acc = Eigen::ArrayXXf();
	int total_offset = this->frames - query->frames + 1;
	acc.resize(query->frames - 1, total_offset);

	for (int frame_offset = 0; frame_offset < total_offset; frame_offset++) {
		// dist = sqrt( sum over (mychannel-theirchannel)^2 )
		acc.col(frame_offset) = (
			((*(this->optical_flow_x_frames)).block(0, frame_offset, (*(query->optical_flow_x_frames)).rows(), (*(query->optical_flow_x_frames)).cols()) - *(query->optical_flow_x_frames)).cwiseAbs2() +
			((*(this->optical_flow_y_frames)).block(0, frame_offset, (*(query->optical_flow_y_frames)).rows(), (*(query->optical_flow_y_frames)).cols()) - *(query->optical_flow_y_frames)).cwiseAbs2()
			).cwiseSqrt().colwise().mean().transpose();
	}

	// Invert using, clipping at larger than 1.0 (This will change if optical size is changed
	acc = 1.0 - acc;

	// If distance between the two colors is larger than 255.0 (negative acc), then zero accuracy
	acc = acc.unaryExpr(std::ptr_fun(clampToZero));

	return;
}

void Metrics::AudioAccuracy(Metrics^ query, Eigen::ArrayXXf& acc) {
	/* Call with this as the database video, other as query video
	*
	* Load 2D acc:
	*  - Cols: database frame offset
	*  - Rows: Accuracy per query frame (SAD, scaled by 1.0/32768.0)
	*
	* This means that col i gives frame-corresponding accuracy when
	* query video is matched with database video, beginning at frame i.
	*
	* Use col-wise sum to find "total" accuracy when matching with database video, beginning at frame `col`
	*
	*/

	acc = Eigen::ArrayXXf();
	int total_offset = this->frames - query->frames + 1;
	acc.resize(query->frames - 1, total_offset); // Remove last offset

	for (int frame_offset = 0; frame_offset < total_offset; frame_offset++) {
		Eigen::VectorXf sad = ((*this->audio_samples).segment(frame_offset * this->audio_sample_rate / 30, query->audio_sample_rate * query->frames / 30) - (*query->audio_samples)).cwiseAbs();
		Eigen::Map<Eigen::MatrixXf> decimate(sad.data(), query->audio_sample_rate / 30, query->frames);
		acc.col(frame_offset) = decimate.colwise().mean().block(0, 0, 1, query->frames - 1).transpose();
	}

	// Invert using 32768.0 (half of short range)
	acc = 1.0 - acc / 32768.0;

	// If distance between the two colors is larger than 32768.0 (negative acc), then zero accuracy
	acc = acc.unaryExpr(std::ptr_fun(clampToZero));

	return;
}