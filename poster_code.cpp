if (argc != 7) {
	return EXIT_FAILURE;
}

platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices);
	
std::ifstream bin_file(xclbinFilename, std::ifstream::binary);
bin_file.seekg(0, bin_file.end);
unsigned nb = bin_file.tellg();
bin_file.seekg(0, bin_file.beg);
char* buf = new char[nb];
bin_file.read(buf, nb);

context = cl::Context(device, nullptr, nullptr, nullptr, &err);
q = cl::CommandQueue(context, device, CL_QUEUE_PROFILING_ENABLE, &err);

program = cl::Program(context, {device}, bins, NULL, &err);
krnl = cl::Kernel(program, "mmult_top", &err);

cl::Buffer buffer_array(context, CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR , buffer_size, NULL, &err);

krnl.setArg(narg++, arg);

array = q.enqueueMapBuffer(buffer_array, CL_TRUE, CL_MAP_WRITE, 0, size, nullptr, nullptr, &err));

init_arrays(array_b, array_c_sw, array_c);

q.enqueueMigrateMemObjects({buffer_array_a, buffer_array_b}, 0);

q.enqueueTask(krnl);

q.enqueueMigrateMemObjects({buffer_array_c}, CL_MIGRATE_MEM_OBJECT_HOST);

mmult_golden_byte(array_a, array_b, array_c_sw);

if (result_check(array_c, array_c_sw))
	return 1;

q.enqueueUnmapMemObject(buffer_array, array);
