// author: Andre Silva
// email: andreluizeng@yahoo.com.br

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#define CL_ERROR !CL_SUCCESS

#define TRUE 	1
#define FALSE 	0

using namespace std;

// support struct to load kernel source from an external file
struct kernel_src_str
{
	char *src;
	size_t size;
};

cl_platform_id 	platform_id;
cl_device_id 	device_id;
cl_context 	context;
cl_command_queue cq;
cl_program 	program;
cl_kernel 	kernel;
cl_mem helloworld_mem_input = NULL;
cl_mem helloworld_mem_output = NULL;


cl_int BuildProgram (cl_program *program, cl_device_id *device_id, cl_context context, kernel_src_str *kernel);
cl_int PrintInfo (cl_platform_id platform_id, cl_device_id device_id);
cl_int Init (cl_platform_id *platform_id, cl_device_id *device_id, cl_context *context, cl_command_queue *cq);
cl_int LoadKernelSource (char *filename, kernel_src_str *kernel_src);

int main (int argc, char **argv)
{

	struct kernel_src_str kernel_str;

	// one dimensional work-items
	int dimension = 1;

	// our problem size
	size_t global = 512;

	// preferred work-group size
	size_t local = 16;

	int size;

	// input data buffer - random values for the helloworld sample
	char *input_data_buffer;

	// output data_buffer for results
	char *output_data_buffer;

	cl_int ret;

	size = global;

	input_data_buffer = (char *) malloc (sizeof (char) * size);
	if (! input_data_buffer)
	{
		printf ("\nFailed to allocate input data buffer memory\n");
		return 0;
	}

	output_data_buffer = (char *) malloc (sizeof (char) * size);
	if (! output_data_buffer)
	{
		printf ("\nFailed to allocate output data buffer memory\n");
		return 0;
	}


	// populate data_buffer with random values
	for (int i = 0; i < size; i++)
	{
		input_data_buffer[i] = rand () % 255;
	}

	printf ("\nInitializing OpenCL:");
	ret = Init (&platform_id, &device_id, &context, &cq);

	if (ret != CL_SUCCESS)
	{
		printf ("\nFailed Initializing OpenCL\n");
		exit (0);
	}
	else
		printf (" Ok\n");

	ret = PrintInfo (platform_id, device_id);

	if (ret != CL_SUCCESS)
		printf ("\nFailed reading OpenCL Info\n");

	printf ("\nLoading CL programs: hello_world");

	ret = LoadKernelSource ((char *)"hello_world.cl", &kernel_str);

	if (ret != CL_SUCCESS)
	{
		printf ("\nFailed loading hello_world kernel\n");
		exit (0);
	}
	else
		printf (" Ok\n");

	printf ("\nBuilding hello_world kernel: ");

	ret = BuildProgram (&program, &device_id, context, &kernel_str);
	if (ret != CL_SUCCESS)
	{
		 printf ("Failed Building hello_world kernel\n");
		 exit (0);
	}
	else
		printf (" Ok\n");

	printf ("\nCreating CL kernel...");
	kernel = clCreateKernel (program, "hello_world", &ret);

	if (ret != CL_SUCCESS)
	{
		 printf ("Failed Creating hellow_world program\n");
		 exit (0);
	}
	else
		printf (" Ok\n");

	printf ("\nAllocating buffers...");

	//---------------------------
	// cl_mem clCreateBuffer (cl_context context, cl_mem_flags flags, size_t size, void *host_ptr, cl_int *errcode_ret)
	//
	// context
	//	A valid OpenCL context used to create the buffer object.
	//
	// flags
	//	A bit-field that is used to specify allocation and usage information such as the memory arena that should be used to allocate the buffer object and how it will be used. See Khronos documentation for more information
	//
	// size
	//	The size in bytes of the buffer memory object to be allocated.
	//
	// host_ptr
	//	A pointer to the buffer data that may already be allocated by the application. The size of the buffer that host_ptr points to must be greater than or equal to the size bytes.
	//
	// errcode_ret
	//	Returns an appropriate error code. If errcode_ret is NULL, no error code is returned.
	//----------------------------
	helloworld_mem_input = clCreateBuffer (context, CL_MEM_READ_ONLY, size, NULL, &ret);

	if (ret != CL_SUCCESS)
	{
		 printf ("Failed Allocation hello_world input buffer\n");
		 exit (0);
	}

	helloworld_mem_output = clCreateBuffer (context, CL_MEM_WRITE_ONLY, size, NULL, &ret);

	if (ret != CL_SUCCESS)
	{
		 printf ("Failed Allocation hello_world output buffer\n");
		 exit (0);
	}
	else
		printf (" Ok\n");

	//-----------------------------
	// cl_int clSetKernelArg (cl_kernel kernel, cl_uint arg_index, size_t arg_size, const void *arg_value)
	//
	// kernel
	//	A valid kernel object.
	//
	// arg_index
	//	The argument index. Arguments to the kernel are referred by indices that go from 0 for the leftmost argument to n - 1, where n is the total number of arguments declared by a kernel.
	//
	// arg_value
	//	A pointer to data that should be used as the argument value for argument specified by arg_index. The argument data pointed to by arg_value is copied and the arg_value pointer can therefore be reused by the application after clSetKernelArg returns. The argument value specified is the value used by all API calls that enqueue kernel (clEnqueueNDRangeKernel and clEnqueueTask) until the argument value is changed by a call to clSetKernelArg for kernel.
	//	If the argument is a memory object (buffer or image), the arg_value entry will be a pointer to the appropriate buffer or image object. The memory object must be created with the context associated with the kernel object. A NULL value can also be specified if the argument is a buffer object in which case a NULL value will be used as the value for the argument declared as a pointer to __global or __constant memory in the kernel. If the argument is declared with the __local qualifier, the arg_value entry must be NULL. If the argument is of type sampler_t, the arg_value entry must be a pointer to the sampler object. For all other kernel arguments, the arg_value entry must be a pointer to the actual data to be used as argument value.
	//	The memory object specified as argument value must be a buffer object (or NULL) if the argument is declared to be a pointer of a built-in or user defined type with the __global or __constant qualifier. If the argument is declared with the __constant qualifier, the size in bytes of the memory object cannot exceed CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE and the number of arguments declared with the __constant qualifier cannot exceed CL_DEVICE_MAX_CONSTANT_ARGS.
	//	The memory object specified as argument value must be a 2D image object if the argument is declared to be of type image2d_t. The memory object specified as argument value must be a 3D image object if argument is declared to be of type image3d_t.
	//
	// arg_size
	//	Specifies the size of the argument value. If the argument is a memory object, the size is the size of the buffer or image object type. For arguments declared with the __local qualifier, the size specified will be the size in bytes of the buffer that must be allocated for the __local argument. If the argument is of type sampler_t, the arg_size value must be equal to sizeof(cl_sampler). For all other arguments, the size will be the size of argument type.
	//-------------------------------
	clSetKernelArg (kernel, 0, sizeof(cl_mem), &helloworld_mem_input);
	clSetKernelArg (kernel, 1, sizeof(cl_mem), &helloworld_mem_output);

	//-------------------------------
	// cl_int clEnqueueWriteBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_write, size_t offset, size_t cb, const void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event)
	//
	//command_queue
	//	Refers to the command-queue in which the write command will be queued. command_queue and buffer must be created with the same OpenCL context.
	//
	// buffer
	//	Refers to a valid buffer object.
	//
	// blocking_write
	//	Indicates if the write operations are blocking or nonblocking.
	//	If blocking_write is CL_TRUE, the OpenCL implementation copies the data referred to by ptr and enqueues the write operation in the command-queue. The memory pointed to by ptr can be reused by the application after the clEnqueueWriteBuffer call returns.
	//	If blocking_write is CL_FALSE, the OpenCL implementation will use ptr to perform a nonblocking write. As the write is non-blocking the implementation can return immediately. The memory pointed to by ptr cannot be reused by the application after the call returns. The event argument returns an event object which can be used to query the execution status of the write command. When the write command has completed, the memory pointed to by ptr can then be reused by the application.
	//
	// offset
	//	The offset in bytes in the buffer object to write to.
	//
	// cb
	//	The size in bytes of data being written.
	//
	// ptr
	//	The pointer to buffer in host memory where data is to be written from.
	//
	// event_wait_list , num_events_in_wait_list
	//	event_wait_list and num_events_in_wait_list specify events that need to complete before this particular command can be executed. If event_wait_list is NULL, then this particular command does not wait on any event to complete. If event_wait_list is NULL, num_events_in_wait_list must be 0. If event_wait_list is not NULL, the list of events pointed to by event_wait_list must be valid and num_events_in_wait_list must be greater than 0. The events specified in event_wait_list act as synchronization points. The context associated with events in event_wait_list and command_queue must be the same.
	//
	// event
	//	Returns an event object that identifies this particular write command and can be used to query or queue a wait for this particular command to complete. event can be NULL in which case it will not be possible for the application to query the status of this command or queue a wait for this command to complete.
	//---------------------------------
	ret = clEnqueueWriteBuffer(cq, helloworld_mem_input, CL_TRUE, 0, size, input_data_buffer, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		printf ("\nError writing input buffer\n");

	//-------------------------------
	// cl_int clEnqueueNDRangeKernel (cl_command_queue command_queue, cl_kernel kernel, cl_uint work_dim, const size_t *global_work_offset, const size_t *global_work_size, const size_t *local_work_size, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event)
	//
	// command_queue
	//	A valid command-queue. The kernel will be queued for execution on the device associated with command_queue.
	//
	// kernel
	//	A valid kernel object. The OpenCL context associated with kernel and command_queue must be the same.
	//
	// work_dim
	// 	The number of dimensions used to specify the global work-items and work-items in the work-group. work_dim must be greater than zero and less than or equal to three.
	//
	// global_work_offset
	//	Must currently be a NULL value. In a future revision of OpenCL, global_work_offset can be used to specify an array of work_dim unsigned values that describe the offset used to calculate the global ID of a work-item instead of having the global IDs always start at offset (0, 0,... 0).
	//
	// global_work_size
	//	Points to an array of work_dim unsigned values that describe the number of global work-items in work_dim dimensions that will execute the kernel function. The total number of global work-items is computed as global_work_size[0] *...* global_work_size[work_dim - 1].
	//	The values specified in global_work_size cannot exceed the range given by the sizeof(size_t) for the device on which the kernel execution will be enqueued. The sizeof(size_t) for a device can be determined using CL_DEVICE_ADDRESS_BITS in the table of OpenCL Device Queries for clGetDeviceInfo. If, for example, CL_DEVICE_ADDRESS_BITS = 32, i.e. the device uses a 32-bit address space, size_t is a 32-bit unsigned integer and global_work_size values must be in the range 1 .. 2^32 - 1. Values outside this range return a CL_OUT_OF_RESOURCES error.
	//
	// local_work_size
	//	Points to an array of work_dim unsigned values that describe the number of work-items that make up a work-group (also referred to as the size of the work-group) that will execute the kernel specified by kernel. The total number of work-items in a work-group is computed as local_work_size[0] *... * local_work_size[work_dim - 1]. The total number of work-items in the work-group must be less than or equal to the CL_DEVICE_MAX_WORK_GROUP_SIZE value specified in table of OpenCL Device Queries for clGetDeviceInfo and the number of work-items specified in local_work_size[0],... local_work_size[work_dim - 1] must be less than or equal to the corresponding values specified by CL_DEVICE_MAX_WORK_ITEM_SIZES[0],.... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim - 1]. The explicitly specified local_work_size will be used to determine how to break the global work-items specified by global_work_size into appropriate work-group instances. If local_work_size is specified, the values specified in global_work_size[0],... global_work_size[work_dim - 1] must be evenly divisable by the corresponding values specified in local_work_size[0],... local_work_size[work_dim - 1].
	//	The work-group size to be used for kernel can also be specified in the program source using the __attribute__((reqd_work_group_size(X, Y, Z)))qualifier. In this case the size of work group specified by local_work_size must match the value specified by the reqd_work_group_size __attribute__ qualifier.
	//	local_work_size can also be a NULL value in which case the OpenCL implementation will determine how to be break the global work-items into appropriate work-group instances.
	//	event_wait_list and num_events_in_wait_list
	// 	Specify events that need to complete before this particular command can be executed. If event_wait_list is NULL, then this particular command does not wait on any event to complete. If event_wait_list is NULL, num_events_in_wait_list must be 0. If event_wait_list is not NULL, the list of events pointed to by event_wait_list must be valid and num_events_in_wait_list must be greater than 0. The events specified in event_wait_list act as synchronization points. The context associated with events in event_wait_list and command_queue must be the same.
	//
	// event
	//	Returns an event object that identifies this particular kernel execution instance. Event objects are unique and can be used to identify a particular kernel execution instance later on. If event is NULL, no event will be created for this kernel execution instance and therefore it will not be possible for the application to query or queue a wait for this particular kernel execution instance.
	//---------------------------------
	ret = clEnqueueNDRangeKernel (cq, kernel, dimension, NULL, &global, &local, 0, NULL, NULL);
	if  (ret == CL_SUCCESS)
	{
		//------------------------------------
		// cl_int clEnqueueReadBuffer (cl_command_queue command_queue, cl_mem buffer, cl_bool blocking_read, size_t offset, size_t cb, void *ptr, cl_uint num_events_in_wait_list, const cl_event *event_wait_list, cl_event *event)
		//
		// command_queue
		//	Refers to the command-queue in which the read command will be queued. command_queue and buffer must be created with the same OpenCL context.
		//
		// buffer
		//	Refers to a valid buffer object.
		//
		// blocking_read
		//	Indicates if the read operations are blocking or non-blocking. If blocking_read is CL_TRUE i.e. the read command is blocking, clEnqueueReadBuffer does not return until the buffer data has been read and copied into memory pointed to by ptr.
		//	If blocking_read is CL_FALSE i.e. the read command is non-blocking, clEnqueueReadBuffer queues a non-blocking read command and returns. The contents of the buffer that ptr points to cannot be used until the read command has completed. The event argument returns an event object which can be used to query the execution status of the read command. When the read command has completed, the contents of the buffer that ptr points to can be used by the application.
		//
		// offset
		//	The offset in bytes in the buffer object to read from.
		//
		// cb
		//	The size in bytes of data being read.
		//
		// ptr
		//	The pointer to buffer in host memory where data is to be read into.
		//
		// event_wait_list , num_events_in_wait_list
		//	event_wait_list and num_events_in_wait_list specify events that need to complete before this particular command can be executed. If event_wait_list is NULL, then this particular command does not wait on any event to complete. If event_wait_list is NULL, num_events_in_wait_list must be 0. If event_wait_list is not NULL, the list of events pointed to by event_wait_list must be valid and num_events_in_wait_list must be greater than 0. The events specified in event_wait_list act as synchronization points. The context associated with events in event_wait_list and command_queue must be the same.
		//
		// event
		//	Returns an event object that identifies this particular read command and can be used to query or queue a wait for this particular command to complete. event can be NULL in which case it will not be possible for the application to query the status of this command or queue a wait for this command to complete.
		//----------------------------------------
		ret = clEnqueueReadBuffer(cq, helloworld_mem_output, CL_TRUE, 0, size, output_data_buffer, 0, NULL, NULL);
	}
	else
		printf ("\nError reading output buffer\n");


	// just a debug for some values
	bool flag = 0;
	for (int i = 0; i < size; i++)
	{
		if (input_data_buffer[i] != output_data_buffer[i])
			flag = 1;
	}

	if (flag)
		printf ("\noutput buffer is different from input\n");
	else
		printf ("\nAll values successfully copied\n");

	free (input_data_buffer);
	free (output_data_buffer);

	clFlush( cq);
	clFinish(cq);

	printf ("\nFinishing OpenCL...");
	clReleaseContext(context);
	clReleaseProgram(program);
	clReleaseCommandQueue(cq);
	clReleaseKernel (kernel);
	clReleaseMemObject (helloworld_mem_input);
	clReleaseMemObject (helloworld_mem_output);
	printf ("OK\n\n");

	return 0;
}

cl_int Init (cl_platform_id *platform_id, cl_device_id *device_id, cl_context *context, cl_command_queue *cq)
{
	cl_uint  platforms, devices;
	cl_int error;

	//-------------------------------------------
	// cl_int clGetPlatformIDs (cl_uint num_entries, cl_platform_id *platforms, cl_uint *num_platforms)
	//
	// num_entries
	//	The number of cl_platform_id entries that can be added to platforms. If platforms is not NULL, the num_entries must be greater than zero.
	//
	// platforms
	//	Returns a list of OpenCL platforms found. The cl_platform_id values returned in platforms can be used to identify a specific OpenCL platform. If platforms argument is NULL, this argument is ignored. The number of OpenCL platforms returned is the mininum of the value specified by num_entries or the number of OpenCL platforms available.
	//
	// num_platforms
	//	Returns the number of OpenCL platforms available. If num_platforms is NULL, this argument is ignored.
	//--------------------------------------------
	error = clGetPlatformIDs (1, platform_id, &platforms);
	if (error != CL_SUCCESS)
		return CL_ERROR;

	//--------------------------------------------
	// cl_int clGetDeviceIDs (cl_platform_id platform, cl_device_type device_type, cl_uint num_entries, cl_device_id *devices, cl_uint *num_devices)
	//
	// platform
	//	Refers to the platform ID returned by clGetPlatformIDs or can be NULL. If platform is NULL, the behavior is implementation-defined.
	//
	// device_type
	//	A bitfield that identifies the type of OpenCL device. The device_type can be used to query specific OpenCL devices or all OpenCL devices available. The valid values for device_type are specified as:
	//	CL_DEVICE_TYPE_CPU, CL_DEVICE_TYPE_GPU, GL_DEVICE_TYPE_ACCELERATOR, CL_DEVICE_TYPE_DEFAULT, GL_DEVICE_TYPE_ALL
	//
	// num_entries
	//	The number of cl_device entries that can be added to devices. If devices is not NULL, the num_entries must be greater than zero.
	//
	// devices
	//	A list of OpenCL devices found. The cl_device_id values returned in devices can be used to identify a specific OpenCL device. If devices argument is NULL, this argument is ignored. The number of OpenCL devices returned is the mininum of the value specified by num_entries or the number of OpenCL devices whose type matches device_type.
	//
	// num_devices
	//	The number of OpenCL devices available that match device_type. If num_devices is NULL, this argument is ignored.
	//--------------------------------------------
	error = clGetDeviceIDs ((* platform_id), CL_DEVICE_TYPE_GPU, 1, device_id, &devices);
	if (error != CL_SUCCESS)
		return CL_ERROR;

	//--------------------------------------------
	// cl_context clCreateContext (cl_context_properties *properties, cl_uint num_devices, const cl_device_id *devices, void *pfn_notify (const char *errinfo, const void *private_info, size_t cb, void *user_data), 	void *user_data, cl_int *errcode_ret)
	//
	//properties
	//	Specifies a list of context property names and their corresponding values. Each property name is immediately followed by the corresponding desired value. The list is terminated with 0. properties can be NULL in which case the platform that is selected is implementation-defined.
	//
	// num_devices
	//	The number of devices specified in the devices argument.
	//
	// devices
	//	A pointer to a list of unique devices returned by clGetDeviceIDs for a platform.
	//
	// pfn_notify
	//	A callback function that can be registered by the application. This callback function will be used by the OpenCL implementation to report information on errors that occur in this context. This callback function may be called asynchronously by the OpenCL implementation. It is the application's responsibility to ensure that the callback function is thread-safe. If pfn_notify is NULL, no callback function is registered. The parameters to this callback function are:
	//	errinfo is a pointer to an error string.
	//	private_info and cb represent a pointer to binary data that is returned by the OpenCL implementation that can be used to log additional information helpful in debugging the error.
	//	user_data is a pointer to user supplied data.
	//
	// user_data
	//	Passed as the user_data argument when pfn_notify is called. user_data can be NULL.
	//
	// errcode_ret
	//	Returns an appropriate error code. If errcode_ret is NULL, no error code is returned.
	//----------------------------------------------
	cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)(* platform_id), 0};
	(* context) = clCreateContext (properties, 1, device_id, NULL, NULL, &error);

	if (error != CL_SUCCESS)
		return CL_ERROR;

	//----------------------------------------------
	// cl_command_queue clCreateCommandQueue (cl_context context, cl_device_id device, cl_command_queue_properties properties, cl_int *errcode_ret)
	//
	// context
	//	Must be a valid OpenCL context.
	//
	// device
	//	Must be a device associated with context. It can either be in the list of devices specified when context is created using clCreateContext or have the same device type as the device type specified when the context is created using clCreateContextFromType.
	//
	// properties
	//	Specifies a list of properties for the command-queue. This is a bit-field. Only command-queue properties specified below can be set in properties; otherwise the value specified in properties is considered to be not valid.
	//	CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, CL_QUEUE_PROFILING_ENABLE
	//-----------------------------------------------
	(* cq) = clCreateCommandQueue ((* context), (* device_id), 0, &error);
	if (error != CL_SUCCESS)
		return CL_ERROR;

	return CL_SUCCESS;
}

cl_int PrintInfo (cl_platform_id platform_id, cl_device_id device_id)
{
	uint i;
	char buffer[10240];
	cl_int ret;

	//---------------------------------------
	//cl_int clGetPlatformInfo (cl_platform_id platform, cl_platform_info param_name, size_t param_value_size, void *param_value, size_t *param_value_size_ret)
	//
	// platform
	// 	The platform ID returned by clGetPlatformIDs or can be NULL. If platform is NULL, the behavior is implementation-defined.
	//
	// param_name
	//	An enumeration constant that identifies the platform information being queried. See Khronos documentation for more information.
	//
	// param_value_size
	//	Specifies the size in bytes of memory pointed to by param_value. This size in bytes must be greater than or equal to size of return type specified in the table below.
	//
	// param_value
	//	A pointer to memory location where appropriate values for a given param_value will be returned. Acceptable param_value values are listed in the table below. If param_value is NULL, it is ignored.
	//
	// param_value_size_ret
	//	Returns the actual size in bytes of data being queried by param_value. If param_value_size_ret is NULL, it is ignored
	//-----------------------------------------

	printf ("\n-=-=-=- Platform Information -=-=-=-\n\n");
	ret = clGetPlatformInfo (platform_id, CL_PLATFORM_NAME, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Platform Name: %s\n", buffer);

	ret = clGetPlatformInfo (platform_id, CL_PLATFORM_PROFILE, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Platform Profile: %s\n", buffer);

	ret = clGetPlatformInfo (platform_id, CL_PLATFORM_VERSION, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Platform Version: %s\n", buffer);

	ret = clGetPlatformInfo (platform_id, CL_PLATFORM_VENDOR, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Platform Vendor: %s\n", buffer);

	printf ("\n-=-=-=- Device Information -=-=-=-\n\n");
	ret = clGetDeviceInfo( device_id, CL_DEVICE_NAME, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Device Name: %s\n", buffer);

	//-------------------------------------------
	// cl_int clGetDeviceInfo (cl_device_id device, cl_device_info param_name, size_t param_value_size, void *param_value, size_t *param_value_size_ret)
	//
	// device
	//	Refers to the device returned by clGetDeviceIDs.
	//
	// param_name
	//	An enumeration constant that identifies the device information being queried. See Khronos documentation for more information
	//
	// param_value
	//	A pointer to memory location where appropriate values for a given param_name as specified in the table below will be returned. If param_value is NULL, it is ignored.
	//
	// param_value_size
	//	Specifies the size in bytes of memory pointed to by param_value. This size in bytes must be greater than or equal to size of return type specified in the table below.
	//
	// param_value_size_ret
	//	Returns the actual size in bytes of data being queried by param_value. If param_value_size_ret is NULL, it is ignored
	//--------------------------------------------
	ret = clGetDeviceInfo (device_id, CL_DEVICE_PROFILE, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;

	printf ("Device Profile: %s\n", buffer);

	ret = clGetDeviceInfo (device_id, CL_DEVICE_VERSION, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Device Version: %s\n", buffer);

	ret = clGetDeviceInfo (device_id, CL_DEVICE_VENDOR, 10240, buffer, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Device Vendor: %s\n", buffer);

	ret = clGetDeviceInfo (device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof (uint), &i, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Device Max Work Item Dimensions: %u-D\n", i);

	ret = clGetDeviceInfo (device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof (uint), &i, NULL);
	if (ret != CL_SUCCESS)
		return CL_ERROR;
	printf ("Device Max Work Group Size: %u\n", i);

//	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof (uint), &i, NULL);
//	if (ret != CL_SUCCESS)
//		return 0;
//	printf ("Device Max Work Item Sizes: %u\n", i);

//	ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof (uint), &i, NULL);
//	if (ret != CL_SUCCESS)
//		return 0;
//	printf ("Device Global Memory Size: %u\n", i);
	printf ("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");

	return CL_SUCCESS;
}

cl_int LoadKernelSource (char *filename, kernel_src_str *kernel_src)
{

	FILE *fp = NULL;

	fp = fopen (filename, "rb");

	if (fp == NULL)
	{
		printf ("\nFailed to open: %s\n", filename);
		return CL_ERROR;
	}

	fseek (fp, 0, SEEK_END);
	kernel_src->size = ftell (fp);
	rewind (fp);

	// prevent re-allocation
	//if (kernel->src) free (kernel->src);
	kernel_src->src =  (char *) malloc (sizeof (char) * kernel_src->size);
	if (! kernel_src->src)
	{
		printf ("\nError Allocating memory to load CL program");
		return CL_ERROR;
	}
	fread (kernel_src->src, 1, kernel_src->size, fp);

	kernel_src->src[kernel_src->size] = '\0';
	fclose (fp);

	return CL_SUCCESS;
}

cl_int BuildProgram (cl_program *program, cl_device_id *device_id, cl_context context, kernel_src_str *kernel)
{
	cl_int error = CL_SUCCESS;

	//----------------------------------------------
	// cl_program clCreateProgramWithSource (cl_context context, cl_uint count, const char **strings, const size_t *lengths, cl_int *errcode_ret)
	//
	// Parameters
	//	context
	//		Must be a valid OpenCL context.
	//
	//	strings
	//		An array of count pointers to optionally null-terminated character strings that make up the source code.
	//
	//	lengths
	//		An array with the number of chars in each string (the string length). If an element in lengths is zero, its accompanying string is null-terminated. If lengths is NULL, all strings in the strings argument are considered null-terminated. Any length value passed in that is greater than zero excludes the null terminator in its count.
	//
	//	errcode_ret
	//		Returns an appropriate error code. If errcode_ret is NULL, no error code is returned.
	//------------------------------------------------
	(* program) = clCreateProgramWithSource (context, 1, (const char **)&kernel->src, &kernel->size, &error);
	if (error != CL_SUCCESS)
	{
		return CL_ERROR;
	}

	//------------------------------------------------
	// cl_int clBuildProgram (cl_program program, cl_uint num_devices, const cl_device_id *device_list, const char *options, void (*pfn_notify)(cl_program, void *user_data), void *user_data)
	//
	// Parameters
	//	program
	//		The program object
	//
	//	device_list
	//		A pointer to a list of devices that are in program. If device_list is NULL value, the program executable is built for all devices associated with program for which a source or binary has been loaded. If device_list is a non-NULL value, the program executable is built for devices specified in this list for which a source or binary has been loaded.
	//
	//	num_devices
	//		The number of devices listed in device_list.
	//
	//	options
	//		A pointer to a string that describes the build options to be used for building the program executable. The list of supported options is described in "Build Options" below.
	//
	//	pfn_notify
	//		A function pointer to a notification routine. The notification routine is a callback function that an application can register and which will be called when the program executable has been built (successfully or unsuccessfully). If pfn_notify is not NULL, clBuildProgram does not need to wait for the build to complete and can return immediately. If pfn_notify is NULL, clBuildProgram does not return until the build has completed. This callback function may be called asynchronously by the OpenCL implementation. It is the application's responsibility to ensure that the callback function is thread-safe.
	//
	//	user_data
	//		Passed as an argument when pfn_notify is called. user_data can be NULL.
	//-------------------------------------------------
	error = clBuildProgram ((* program), 1, device_id, "", NULL, NULL);
	if (error < 0)
	{
		//---------------------------------------------------
		// cl_int clGetProgramBuildInfo ( cl_program  program, cl_device_id  device, cl_program_build_info  param_name, size_t  param_value_size, void  *param_value, size_t  *param_value_size_ret)
		// program
		//	Specifies the program object being queried.
		//
		// device
		//	Specifies the device for which build information is being queried. device must be a valid device associated with program.
		//
		// param_name
		//	Specifies the information to query. The list of supported param_name is: CL_PROGRAM_BUILD_STATUS, CL_PROGRAM_BUILD_OPTIONS, CL_PROGRAM_BUILD_LOG
		//
		// param_value
		//	A pointer to memory where the appropriate result being queried is returned. If param_value is NULL, it is ignored.
		//
		// param_value_size
		//	Specifies the size in bytes of memory pointed to by param_value. This size must be greater than or equal to the size of return type as described in the table above.
		//
		// param_value_size_ret
		//	Returns the actual size in bytes of data copied to param_value. If param_value_size_ret is NULL, it is ignored.
		//---------------------------------------------------
		clGetProgramBuildInfo((* program), (* device_id), CL_PROGRAM_BUILD_LOG, kernel->size, kernel->src, NULL);
		printf ("\n%s", kernel->src);
	}

	return error;
}
