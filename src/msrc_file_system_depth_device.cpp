#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <seng499/msrc_file_system_depth_device.hpp>
#include <cstdint>
#include <regex>
#include <stdexcept>


namespace seng499 {

	static const std::regex & get_regex() {

		static const std::regex msrc_depth_file_pattern(".*depth.*");
		return msrc_depth_file_pattern;

	}
	
	
	bool msrc_file_system_depth_device_filter::operator () (const boost::filesystem::path & path) const {

		return std::regex_match(path.filename().string(), get_regex());
		
	}
	
	
	std::vector<float> msrc_file_system_depth_device_frame_factory::operator () (const boost::filesystem::path & path, std::vector<float> vec) {
	
		// make sure this is empty
		vec.clear();

		// reserve the size of our image
		vec.reserve(640*480);

		// Load a grayscale depth image. Anydepth is necessary as these are 16 bit ints 
		auto img = cv::imread(path.string(), CV_LOAD_IMAGE_GRAYSCALE | CV_LOAD_IMAGE_ANYDEPTH);

		if (!img.data) throw std::runtime_error("cv::imread failed to read image");

		if (img.rows != 480 || img.cols != 640) throw std::runtime_error("Image is not of the expected size (640x480)");

		// Convert mm to m, cast uint16 to float and push into the vector		
		for (int i=0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j)
				vec.push_back(float(img.at<std::uint16_t>(i,j)/1000.0f));
		

		return vec;
		
	}
	
	
	std::size_t msrc_file_system_depth_device_frame_factory::width () const noexcept {
		
		return 640;
		
	}
	
	std::size_t msrc_file_system_depth_device_frame_factory::height () const noexcept {
		
		return 480;
		
	}
	
	Eigen::Matrix3f msrc_file_system_depth_device_frame_factory::k () const noexcept {
		
		Eigen::Matrix3f k;
		k << 585.0f, 0.0f, 320.0f,
		     0.0f, -585.0f, 240.0f,
		     0.0f, 0.0f, 1.0f;
		
		return k;
	}
	
}