/**
 *	\file
 */


#pragma once


#include <dynfu/depth_device.hpp>
#include <dynfu/measurement_pipeline_block.hpp>
#include <dynfu/pose_estimation_pipeline_block.hpp>
#include <dynfu/surface_prediction_pipeline_block.hpp>
#include <dynfu/timer.hpp>
#include <dynfu/update_reconstruction_pipeline_block.hpp>
#include <Eigen/Dense>
#include <cstddef>


namespace dynfu {
	
	
	/**
	 *	Represents the KinectFusion processing pipeline.
	 *
	 *	\sa <a href="http://homes.cs.washington.edu/~newcombe/papers/newcombe_etal_ismar2011.pdf">KinectFusion: Real-Time Dense Surface Mapping and Tracking</a>
	 */
	class kinect_fusion {
		
		
		public:
		
		
			/**
			 *	The timer that the pipeline shall use for
			 *	benchmarking.
			 */
			using timer=dynfu::timer;
		
		
		private:
		
		
			dynfu::depth_device * dd_;
			timer::duration ddt_;
			Eigen::Matrix3f k_;
			std::size_t width_;
			std::size_t height_;
			dynfu::measurement_pipeline_block * mpb_;
			timer::duration mpbt_;
			dynfu::pose_estimation_pipeline_block * pepb_;
			timer::duration pepbt_;
			dynfu::update_reconstruction_pipeline_block * urpb_;
			timer::duration urpbt_;
			dynfu::surface_prediction_pipeline_block * sppb_;
			timer::duration sppbt_;
			dynfu::depth_device::value_type frame_;
			dynfu::measurement_pipeline_block::vertex_value_type v_;
			dynfu::measurement_pipeline_block::normal_value_type n_;
			dynfu::pose_estimation_pipeline_block::value_type t_g_k_;
			dynfu::update_reconstruction_pipeline_block::value_type tsdf_;
			dynfu::surface_prediction_pipeline_block::vertex_value_type prev_v_;
			dynfu::surface_prediction_pipeline_block::normal_value_type prev_n_;
			
			
			void check_pipeline () const;
			void get_frame ();
			void get_vertex_and_normal_maps ();
			void get_tgk ();
			void get_tsdf ();
			void get_prediction ();
		
		
		public:
		
		
			kinect_fusion (const kinect_fusion &) = delete;
			kinect_fusion (kinect_fusion &&) = delete;
			kinect_fusion & operator = (const kinect_fusion &) = delete;
			kinect_fusion & operator = (kinect_fusion &&) = delete;
			
			
			/**
			 *	Creates a kinect_fusion object with no
			 *	pipeline blocks or depth device.
			 */
			kinect_fusion () noexcept;
			
			
			/**
			 *	Sets the depth device.
			 *
			 *	\param [in] dd
			 *		A reference to the depth device this
			 *		kinect_fusion shall use.  This reference
			 *		must remain valid as long as this object
			 *		is in use or the behaviour is undefined.
			 */
			void depth_device (dynfu::depth_device & dd) noexcept;
			/**
			 *	Sets the measurement pipeline block.
			 *
			 *	\param [in] mpb
			 *		A reference to the measurement pipeline
			 *		block this kinect_fusion shall use.  This
			 *		reference must remain valid as long as this
			 *		object is in use or the behaviour is undefined.
			 */
			void measurement_pipeline_block (dynfu::measurement_pipeline_block & mpb) noexcept;
			/**
			 *	Sets the pose estimation pipeline block.
			 *
			 *	\param [in] pepb
			 *		A reference to the pose estimation pipeline
			 *		block this kinect_fusion shall use.  This
			 *		reference must remain valid as long as this
			 *		object is in use or the behaviour is undefined.
			 */
			void pose_estimation_pipeline_block (dynfu::pose_estimation_pipeline_block & pepb) noexcept;
			/**
			 *	Sets the update reconstruction pipeline block.
			 *
			 *	\param [in] urpb
			 *		A reference to the update reconstruction
			 *		pipeline block this kinect_fusion shall use.
			 *		This reference must remain valid as long as
			 *		this object is in use or the behaviour is
			 *		undefined.
			 */
			void update_reconstruction_pipeline_block (dynfu::update_reconstruction_pipeline_block & urpb) noexcept;
			/**
			 *	Sets the surface prediction pipeline block.
			 *
			 *	\param [in] sppb
			 *		A reference to the surface prediction
			 *		pipeline block this kinect_fusion shall use.
			 *		This reference must remain valid as long as
			 *		this object is in use or the behaviour is
			 *		undefined.
			 */
			void surface_prediction_pipeline_block (dynfu::surface_prediction_pipeline_block & sppb) noexcept;
			
			
			/**
			 *	Retrieves the next frame from the depth device
			 *	and updates the internal state of this object
			 *	accordingly.
			 */
			void operator () ();
			
			
			/**
			 *	Retrieves the amount of time the pipeline spent
			 *	waiting for the depth frame on the last invocation.
			 *
			 *	\return
			 *		The amount of time.
			 */
			timer::duration depth_device_elapsed () const;
			/**
			 *	Retrieves the amount of time the pipeline spent
			 *	waiting for the measurement pipeline block on the
			 *	last invocation.
			 *
			 *	\return
			 *		The amount of time.
			 */
			timer::duration measurement_pipeline_block_elapsed () const;
			/**
			 *	Retrieves the amount of time the pipeline spent
			 *	waiting for the pose estimation pipeline block on the
			 *	last invocation.
			 *
			 *	\return
			 *		The amount of time.
			 */
			timer::duration pose_estimation_pipeline_block_elapsed () const;
			/**
			 *	Retrieves the amount of time the pipeline spent
			 *	waiting for the update reconstruction pipeline block
			 *	on the last invocation.
			 *
			 *	\return
			 *		The amount of time.
			 */
			timer::duration update_reconstruction_pipeline_block_elapsed () const;
			/**
			 *	Retrieves the amount of time the pipeline spent
			 *	waiting for the surface preduction pipeline block on
			 *	the last invocation.
			 *
			 *	\return
			 *		The amount of time.
			 */
			timer::duration surface_prediction_pipeline_block_elapsed () const;


			/**
			 *	Retrieves the depth frame from the last invocation.
			 *
			 *	If this object has not yet been successfully invoked
			 *	the behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last depth frame.
			 */
			dynfu::depth_device::value_type::element_type & frame () const noexcept;
			/**
			 *	Retrieves the vertex map calculated from the depth frame
			 *	during the last invocation.
			 *
			 *	If this object has not yet been successfully invoked
			 *	the behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last vertex map.
			 */
			dynfu::measurement_pipeline_block::vertex_value_type::element_type & vertex_map () const noexcept;
			/**
			 *	Retrieves the normal map calculated from the depth frame
			 *	during the last invocation.
			 *
			 *	If this object has not yet been successfully invoked
			 *	the behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last normal map.
			 */
			dynfu::measurement_pipeline_block::normal_value_type::element_type & normal_map () const noexcept;
			/**
			 *	Retrieves the pose estimation calculated during the last
			 *	invocation.
			 *
			 *	If this object has not yet been successfully invoked
			 *	the behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last pose estimation.
			 */
			dynfu::pose_estimation_pipeline_block::value_type::element_type & pose_estimation () const noexcept;
			/**
			 *	Retrieves the vertex map which was obtained as a result
			 *	of raycasting the truncated signed distance function during
			 *	the last invocation.
			 *
			 *	If this object has not yet been successfully invoked the
			 *	behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last predicted vertex map.
			 */
			dynfu::surface_prediction_pipeline_block::vertex_value_type::element_type & predicted_vertex_map () const noexcept;
			/**
			 *	Retrieves the normal map which was obtained as a result
			 *	of raycasting the truncated signed distance function during
			 *	the last invocation.
			 *
			 *	If this object has not yet been successfully invoked the
			 *	behaviour is undefined.
			 *
			 *	\return
			 *		A reference to a \ref pipeline_value object which
			 *		represents the last predicted normal map.
			 */
			dynfu::surface_prediction_pipeline_block::normal_value_type::element_type & predicted_normal_map () const noexcept;
		
		
	};
	
	
}
