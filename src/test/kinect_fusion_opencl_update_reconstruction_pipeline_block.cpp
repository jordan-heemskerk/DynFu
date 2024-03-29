#include <dynfu/kinect_fusion_opencl_update_reconstruction_pipeline_block.hpp>
#include <dynfu/msrc_file_system_depth_device.hpp>
#include <dynfu/file_system_depth_device.hpp>
#include <dynfu/file_system_opencl_program_factory.hpp>
#include <dynfu/opencl_depth_device.hpp>
#include <boost/compute.hpp>
#include <dynfu/cpu_pipeline_value.hpp>
#include <dynfu/filesystem.hpp>
#include <dynfu/path.hpp>
#include <dynfu/file_system_opencl_program_factory.hpp>
#include <Eigen/Dense>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>


#include <catch.hpp>


namespace {


	class fixture {

		private:

			static dynfu::filesystem::path cl_path () {

				dynfu::filesystem::path retr(dynfu::current_executable_parent_path());
				retr/="..";
				retr/="cl";

				return retr;

			}

		protected:

			boost::compute::device dev;
			boost::compute::context ctx;
			boost::compute::command_queue q;
			std::size_t width;
			std::size_t height;
			Eigen::Matrix3f k;
			dynfu::cpu_pipeline_value<Eigen::Matrix4f> t_g_k;
			dynfu::file_system_opencl_program_factory fsopf;
			std::size_t tsdf_width;
			std::size_t tsdf_height;
			std::size_t tsdf_depth;

		public:

		fixture () : dev(boost::compute::system::default_device()), ctx(dev), q(ctx,dev), width(640), height(480), fsopf(cl_path(),ctx), tsdf_width(256), tsdf_height(256), tsdf_depth(256) {

				//needs to be a valid k matrix for a meaningful test
				k << 585.0f, 0.0f, 320.0f,
					 0.0f, 585.0f, 240.0f,
					 0.0f, 0.0f, 1.0f;

				//needs to be a valid t_g_k for meaningful test (?)
				Eigen::Matrix4f t_g_k(Eigen::Matrix4f::Identity());
				t_g_k(0,3) = 1.5f;
				t_g_k(1,3) = 1.5f;
				t_g_k(2,3) = 1.5f;
				this->t_g_k.emplace(std::move(t_g_k));

			}

	};


}


SCENARIO_METHOD(fixture, "A dynfu::kinect_fusion_opencl_update_reconstruction_pipeline_block implements the update_reconstruction phase of the kinect fusion pipeline on the GPU using OpenCL","[dynfu][update_reconstruction_pipeline_block][kinect_fusion_opencl_update_reconstruction_pipeline_block]") {

	GIVEN("A dynfu::kinect_fusion_opencl_update_reconstruction_pipeline_block") {


		dynfu::kinect_fusion_opencl_update_reconstruction_pipeline_block kfourpb(q, fsopf, 5.0f, tsdf_width, tsdf_height, tsdf_depth);
		dynfu::cpu_pipeline_value<std::vector<float>> pv;

		dynfu::filesystem::path pp(dynfu::current_executable_parent_path());

		auto d=boost::compute::system::default_device();
		boost::compute::context ctx(d);
		boost::compute::command_queue q(ctx,d);

		dynfu::msrc_file_system_depth_device_frame_factory ff;
		dynfu::msrc_file_system_depth_device_filter f;
		dynfu::file_system_depth_device ddi(pp/".."/"data/test/msrc_file_system_depth_device",ff,&f);
		dynfu::opencl_depth_device dd(ddi,q);

		std::vector<float> v = dd()->get();

		pv.emplace(std::move(v));

		THEN("Invoking it and downloading the produced TSDF does not fail") {

			auto f = [&](){

				auto && t=kfourpb(pv, width, height, k, t_g_k);
				auto && ts = t.buffer->get();

#ifdef GENERATE_GROUND_TRUTH
				std::size_t i(0);
				for (auto && t_ : ts) {

					std::cout << t_ <<",";

					i++;
					if ( i > 10) {
						std::cout << std::endl;
						i = 0;
					}
				}
#endif

#ifdef SLICE_VIEW

				for (unsigned int z = 0; z < tsdf_width; z++) {

					std::cout << "Slice " << z << std::endl;

					for (unsigned int y = 0; y < tsdf_height; y++) {

						for (unsigned int x = 0; x < tsdf_depth; x++) {

							std::cout << "(" << x << "," << y << "," << z << ") ";
							std::cout << ts.at(x + (tsdf_width)*y + ((tsdf_width) * (tsdf_height))*z) << ", ";

						}
						std::cout << std::endl;
					}

					std::cout << std::endl;

				}
#endif
			};

			CHECK_NOTHROW(f());

		}

	}

}
