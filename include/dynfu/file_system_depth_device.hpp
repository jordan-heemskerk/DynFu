/** 
 *	\file
 */


#pragma once


#include <dynfu/depth_device.hpp>
#include <dynfu/filesystem.hpp>
#include <Eigen/Dense>
#include <cstddef>
#include <stdexcept>
#include <vector>


namespace dynfu {
	
	
	/**
	 *	An abstract base class which provides an interface for
	 *	filtering files for a \ref file_system_depth_device.
	 *
	 *	\sa file_system_depth_device
	 */
	class file_system_depth_device_filter {
		
		
		public:
		
		
			file_system_depth_device_filter () = default;
			file_system_depth_device_filter (const file_system_depth_device_filter &) = delete;
			file_system_depth_device_filter (file_system_depth_device_filter &&) = delete;
			file_system_depth_device_filter & operator = (const file_system_depth_device_filter &) = delete;
			file_system_depth_device_filter & operator = (file_system_depth_device_filter &&) = delete;
			
			
			/**
			 *	Allows derived classes to be cleaned up through
			 *	pointer or reference to base.
			 */
			virtual ~file_system_depth_device_filter () noexcept;
			
			
			/**
			 *	Determines whether the file at \em path should be
			 *	included or excluded.
			 *
			 *	\param [in] path
			 *		The path of the file-in-question.
			 *
			 *	\return
			 *		\em true to include the file at \em path,
			 *		\em false otherwise.
			 */
			virtual bool operator () (const filesystem::path & path) const = 0;
		
		
	};
	
	
	/**
	 *	A \ref file_system_depth_device_filter which includes
	 *	all files.
	 */
	class null_file_system_depth_device_filter final : public file_system_depth_device_filter {
		
		
		public:
		
		
			virtual bool operator () (const filesystem::path &) const override;
		
		
	};
	
	
	/**
	 *	An abstract base class which provides an interface for
	 *	sorting files for a \ref file_system_depth_device.
	 *
	 *	\sa file_system_depth_device
	 */
	class file_system_depth_device_comparer {
		
		
		public:
		
		
			file_system_depth_device_comparer () = default;
			file_system_depth_device_comparer (const file_system_depth_device_comparer &) = delete;
			file_system_depth_device_comparer (file_system_depth_device_comparer &&) = delete;
			file_system_depth_device_comparer & operator = (const file_system_depth_device_comparer &) = delete;
			file_system_depth_device_comparer & operator = (file_system_depth_device_comparer &&) = delete;
			
			
			/**
			 *	Allows derived classes to be cleaned up through
			 *	pointer or reference to base.
			 */
			virtual ~file_system_depth_device_comparer () noexcept;
			
			
			/**
			 *	Determines whether \em a orders before \em b.
			 *
			 *	\param [in] a
			 *		A path.
			 *	\param [in] b
			 *		A path.
			 *
			 *	\return
			 *		\em true if \em a orders before \em b, \em false
			 *		otherwise.
			 */
			virtual bool operator () (const filesystem::path & a, const filesystem::path & b) const = 0;
		
		
	};
	
	
	/**
	 *	A \ref file_system_depth_device_comparer which simply compares
	 *	paths using std::less.
	 */
	class null_file_system_depth_device_comparer final : public file_system_depth_device_comparer {
		
		
		public:
		
		
			virtual bool operator () (const filesystem::path &, const filesystem::path &) const override;
		
		
	};
	
	
	/**
	 *	An abstract base class which provides an interface for
	 *	loading files from the file system as a depth buffer.
	 *
	 *	\sa file_system_depth_device
	 */
	class file_system_depth_device_frame_factory {
		
		
		public:
		
		
			using buffer_type=depth_device::buffer_type;
			using value_type=depth_device::value_type;
			
			
			file_system_depth_device_frame_factory () = default;
			file_system_depth_device_frame_factory (const file_system_depth_device_frame_factory &) = delete;
			file_system_depth_device_frame_factory (file_system_depth_device_frame_factory &&) = delete;
			file_system_depth_device_frame_factory & operator = (const file_system_depth_device_frame_factory &) = delete;
			file_system_depth_device_frame_factory & operator = (file_system_depth_device_frame_factory &&) = delete;
			
			
			/**
			 *	Allows derived classes to be cleaned up through
			 *	pointer or reference to base.
			 */
			virtual ~file_system_depth_device_frame_factory () noexcept;
			
			
			/**
			 *	Generates a depth frame from a file in the file system.
			 *
			 *	\param [in] path
			 *		The path to the file.
			 *	\param [in] v
			 *		A pointer to a pipeline value representing a depth
			 *		buffer which may be used to restore the return value.
			 *
			 *	\return
			 *		A pipeline value representing a depth buffer.
			 */
			virtual value_type operator () (const filesystem::path & path, value_type v) = 0;
			/**
			 *	Retrieves the width of the frames generated by this
			 *	factory.
			 *
			 *	\return
			 *		The width.
			 */
			virtual std::size_t width () const noexcept = 0;
			/**
			 *	Retrieves the height of the frames generated by this
			 *	factory.
			 *
			 *	\return
			 *		The height.
			 */
			virtual std::size_t height () const noexcept = 0;
			/**
			 *	Retrieves the deprojection matrix of the frames generated
			 *	by this factory.
			 *
			 *	\return
			 *		The deprojection matrix.
			 */
			virtual Eigen::Matrix3f k () const noexcept = 0;
		
		
	};
	
	
	/**
	 *	Acts as a source of pre-existing depth information located in
	 *	the file system.
	 */
	class file_system_depth_device final : public depth_device {
		
		
		public:
		
		
			/**
			 *	Since the \ref depth_device interface assumes a physical
			 *	device there's no way to signal the end of the stream.
			 *
			 *	An exception of this type is thrown once all depth frames
			 *	have been loaded and returned from the file system.
			 */
			class end : public std::runtime_error {
				
				
				public:
				
				
					end ();
				
				
			};
			
			
			virtual value_type operator () (value_type v=value_type{}) override;
			virtual std::size_t width () const noexcept override;
			virtual std::size_t height () const noexcept override;
			virtual Eigen::Matrix3f k () const noexcept override;
			
			
			/**
			 * Creates a new file_system_depth_device.
			 *
			 *	\param [in] path
			 *		A path to the directory where a collection of
			 *		depth frame files resides.
			 *	\param [in] factory
			 *		A \ref file_system_depth_device_frame_factory
			 *		which shall be used to load a frame from a file.
			 *		The lifetime of this object must extend until
			 *		after the last invocation of the newly created
			 *		object has returned or the behaviour is undefined.
			 *	\param [in] filter
			 *		A pointer to a \ref file_system_depth_device_filter
			 *		which shall be used to exclude certain paths from the
			 *		enumerated collection.  Defaults to \em nullptr in
			 *		which case a null filter (which includes all files)
			 *		shall be used.  If a pointer to an object is provided
			 *		the lifetime of that object must extend until after
			 *		the last invocation of the newly created object has
			 *		returned or the behaviour is undefined.
			 *	\param [in] comparer
			 *		A pointer to a \ref file_system_depth_device_comparer
			 *		which shall be used to order the files to determine in
			 *		which order they shall be loaded and transformed into
			 *		depth frames.  Defaults to \em nullptr in which case
			 *		std::less shall be used.  If a pointer to an object is
			 *		provided the lifetime of that object must extend until
			 *		after the last invocation of the newly created object
			 *		has returned or the behaviour is undefined.
			 */
			file_system_depth_device (filesystem::path path, file_system_depth_device_frame_factory & factory, const file_system_depth_device_filter * filter=nullptr, const file_system_depth_device_comparer * comparer=nullptr);
			
            
		private:
		
		
			file_system_depth_device_frame_factory & factory_;
			std::vector<filesystem::path> files_;
		
		
	};
	
	
}
