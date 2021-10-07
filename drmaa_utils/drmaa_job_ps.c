/* $Id$ */
/*
 * HPC-BASH - part of the DRMAA utilities library
 * Poznan Supercomputing and Networking Center Copyright (C) 2011
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <drmaa_utils/drmaa.h>
#include <drmaa_utils/drmaa_util.h>
#include <drmaa_utils/logging.h>
#include <drmaa_utils/exception.h>
#include <drmaa_utils/xmalloc.h>

#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define DRMAA_LIBRARY_PATH "DRMAA_LIBRARY_PATH"

typedef int (*drmaa_init_function_t)(const char *, char *, size_t );
typedef int (*drmaa_exit_function_t)(char *, size_t );
typedef int (*drmaa_allocate_job_template_function_t)(drmaa_job_template_t **, char *, size_t);
typedef int (*drmaa_delete_job_template_function_t)(drmaa_job_template_t *, char *, size_t);
typedef int (*drmaa_set_attribute_function_t)(drmaa_job_template_t *, const char *, const char *, char *, size_t);
typedef int (*drmaa_get_attribute_function_t)(drmaa_job_template_t *, const char *, char *, size_t , char *, size_t);
typedef int (*drmaa_set_vector_attribute_function_t)(drmaa_job_template_t *, const char *, const char *[], char *, size_t);
typedef int (*drmaa_get_vector_attribute_function_t)(drmaa_job_template_t *, const char *, drmaa_attr_values_t **, char *, size_t);
typedef int (*drmaa_run_job_function_t)(char *, size_t, const drmaa_job_template_t *, char *, size_t);
typedef int (*drmaa_control_function_t)(const char *, int, char *, size_t);
typedef int (*drmaa_job_ps_function_t)(const char *, int *, char *, size_t);
typedef int (*drmaa_wait_function_t)(const char *, char *, size_t, int *, signed long, drmaa_attr_values_t **, char *, size_t);
typedef int (*drmaa_wifexited_function_t)(int *, int, char *, size_t);
typedef int (*drmaa_wexitstatus_function_t)(int *exit_status, int, char *, size_t);
typedef int (*drmaa_wifsignaled_function_t)(int *signaled, int, char *, size_t);
typedef int (*drmaa_wtermsig_function_t)(char *signal, size_t signal_len, int, char *, size_t);
typedef int (*drmaa_wcoredump_function_t)(int *core_dumped, int, char *, size_t);
typedef int (*drmaa_wifaborted_function_t)(int *aborted, int, char *, size_t);
typedef char (*drmaa_strerror_function_t)(int);
typedef int (*drmaa_get_contact_function_t)(char *, size_t , char *, size_t);
typedef int (*drmaa_version_function_t)(unsigned int *, unsigned int *, char *, size_t);
typedef int (*drmaa_get_DRM_system_function_t)(char *, size_t, char *, size_t);
typedef int (*drmaa_get_DRMAA_implementation_function_t)(char *, size_t, char *, size_t);

typedef struct
{
	drmaa_init_function_t init;
	drmaa_exit_function_t exit;
	drmaa_allocate_job_template_function_t allocate_job_template;
	drmaa_delete_job_template_function_t delete_job_template;
	drmaa_set_attribute_function_t set_attribute;
	drmaa_get_attribute_function_t get_attribute;
	drmaa_set_vector_attribute_function_t set_vector_attribute;
	drmaa_get_vector_attribute_function_t get_vector_attribute;
	drmaa_run_job_function_t run_job;
	drmaa_control_function_t control;
	drmaa_job_ps_function_t job_ps;
	drmaa_wait_function_t wait;
	drmaa_wifexited_function_t wifexited;
	drmaa_wexitstatus_function_t wexitstatus;
	drmaa_wifsignaled_function_t wifsignaled;
	drmaa_wtermsig_function_t wtermsig;
	drmaa_wcoredump_function_t wcoredump;
	drmaa_wifaborted_function_t wifaborted;
	drmaa_strerror_function_t strerror;
	drmaa_get_contact_function_t get_contact;
	drmaa_version_function_t version;
	drmaa_get_DRM_system_function_t get_DRM_system;
	drmaa_get_DRMAA_implementation_function_t get_DRMAA_implementation;
	void *handle;
} fsd_drmaa_api_t;

typedef struct
{
	char *jobid;
} fsd_drmaa_ps_opt_t;


static fsd_drmaa_api_t load_drmaa();
static void unload_drmaa(fsd_drmaa_api_t *drmaa_api);

static fsd_drmaa_ps_opt_t parse_args(int argc, char **argv);

static int ps(fsd_drmaa_api_t drmaa_api, fsd_drmaa_ps_opt_t ps_opt);

int main(int argc, char **argv)
{
	fsd_drmaa_api_t drmaa_api = { .handle = NULL };
	fsd_drmaa_ps_opt_t ps_opt;
	volatile int status = -1;

	fsd_log_enter(("(argc=%d)", argc));

	TRY
	 {
		drmaa_api = load_drmaa();
		ps_opt = parse_args(argc,argv);
		status = ps(drmaa_api, ps_opt);
	 }
	EXCEPT_DEFAULT
	 {
		fsd_log_fatal(("Error"));
	 }
	FINALLY
	 {
		unload_drmaa(&drmaa_api);
	 }
	END_TRY

	exit(status);
}


fsd_drmaa_api_t load_drmaa()
{
	fsd_drmaa_api_t api;
	const char *path_to_drmaa = getenv(DRMAA_LIBRARY_PATH);

	fsd_log_enter(("(path=%s)", path_to_drmaa));

	memset(&api, 0, sizeof(api));

	if (!path_to_drmaa) {
#ifdef __APPLE__
		path_to_drmaa = DRMAA_DIR_PREFIX"/lib/libdrmaa.dylib";
#else
		path_to_drmaa = DRMAA_DIR_PREFIX"/lib/libdrmaa.so";
#endif
	}

	api.handle = dlopen(path_to_drmaa, RTLD_LAZY | RTLD_GLOBAL);

	if (!api.handle) {
		const char *msg = dlerror();

		if (!msg)
			fsd_log_fatal(("Could not load DRMAA library: %s (DRMAA_LIBRARY_PATH=%s)\n", msg, path_to_drmaa));
		else
			fsd_log_fatal(("Could not load DRMAA library (DRMAA_LIBRARY_PATH=%s)\n", path_to_drmaa));

		fsd_exc_raise_code(FSD_ERRNO_INVALID_VALUE);
	}

#if defined __GNUC__ && (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4))
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
	if ((api.init = (drmaa_init_function_t)dlsym(api.handle, "drmaa_init")) == 0)
		goto fault;
	if ((api.exit = (drmaa_exit_function_t)dlsym(api.handle, "drmaa_exit")) == 0)
		goto fault;
	if ((api.allocate_job_template = (drmaa_allocate_job_template_function_t)dlsym(api.handle, "drmaa_allocate_job_template")) == 0)
		goto fault;
	if ((api.delete_job_template = (drmaa_delete_job_template_function_t)dlsym(api.handle, "drmaa_delete_job_template")) == 0)
		goto fault;
	if ((api.set_attribute = (drmaa_set_attribute_function_t)dlsym(api.handle, "drmaa_set_attribute")) == 0)
		goto fault;
	if ((api.get_attribute = (drmaa_get_attribute_function_t)dlsym(api.handle, "drmaa_get_attribute")) == 0)
		goto fault;
	if ((api.set_vector_attribute = (drmaa_set_vector_attribute_function_t)dlsym(api.handle, "drmaa_set_vector_attribute")) == 0)
		goto fault;
	if ((api.get_vector_attribute = (drmaa_get_vector_attribute_function_t)dlsym(api.handle, "drmaa_get_vector_attribute")) == 0)
		goto fault;
	if ((api.run_job = (drmaa_run_job_function_t)dlsym(api.handle, "drmaa_run_job")) == 0)
		goto fault;
	if ((api.control = (drmaa_control_function_t)dlsym(api.handle, "drmaa_control")) == 0)
		goto fault;
	if ((api.job_ps = (drmaa_job_ps_function_t)dlsym(api.handle, "drmaa_job_ps")) == 0)
		goto fault;
	if ((api.wait = (drmaa_wait_function_t)dlsym(api.handle, "drmaa_wait")) == 0)
		goto fault;
	if ((api.wifexited = (drmaa_wifexited_function_t)dlsym(api.handle, "drmaa_wifexited")) == 0)
		goto fault;
	if ((api.wexitstatus = (drmaa_wexitstatus_function_t)dlsym(api.handle, "drmaa_wexitstatus")) == 0)
		goto fault;
	if ((api.wifsignaled = (drmaa_wifsignaled_function_t)dlsym(api.handle, "drmaa_wifsignaled")) == 0)
		goto fault;
	if ((api.wtermsig = (drmaa_wtermsig_function_t)dlsym(api.handle, "drmaa_wtermsig")) == 0)
		goto fault;
	if ((api.wcoredump = (drmaa_wcoredump_function_t)dlsym(api.handle, "drmaa_wcoredump")) == 0)
		goto fault;
	if ((api.wifaborted = (drmaa_wifaborted_function_t)dlsym(api.handle, "drmaa_wifaborted")) == 0)
		goto fault;
	if ((api.strerror = (drmaa_strerror_function_t)dlsym(api.handle, "drmaa_strerror")) == 0)
		goto fault;
	if ((api.get_contact = (drmaa_get_contact_function_t)dlsym(api.handle, "drmaa_get_contact")) == 0)
		goto fault;
	if ((api.version = (drmaa_version_function_t)dlsym(api.handle, "drmaa_version")) == 0)
		goto fault;
	if ((api.get_DRM_system = (drmaa_get_DRM_system_function_t)dlsym(api.handle, "drmaa_get_DRM_system")) == 0)
		goto fault;
	if ((api.get_DRMAA_implementation = (drmaa_get_DRMAA_implementation_function_t)dlsym(api.handle, "drmaa_get_DRMAA_implementation")) == 0)
		goto fault;
#if defined __GNUC__ && (((__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)) || (__GNUC__ > 4))
#pragma GCC diagnostic pop
#endif

	return api;

fault:
	fsd_log_fatal(("Failed to dlsym DRMAA function"));

	if (api.handle)
		dlclose(api.handle);

	/*make invalid */
	memset(&api, 0, sizeof(api));

	return api;
}

void unload_drmaa(fsd_drmaa_api_t *drmaa_api_handle)
{
	fsd_log_enter(("()"));
	
	if (drmaa_api_handle->handle)
		dlclose(drmaa_api_handle->handle);
}

static fsd_drmaa_ps_opt_t parse_args(int argc, char **argv)
{
	fsd_drmaa_ps_opt_t options;

	memset(&options, 0, sizeof(options));

	argv++;
	argc--;

	if (argc != 1) {
		fsd_log_fatal(("syntax error\ndrmaa-job-ps {job-id}"));
		exit(1);
	}

	options.jobid = argv[0];

	return options;
}

int ps(fsd_drmaa_api_t api, fsd_drmaa_ps_opt_t ps_opt)
{
	char errbuf[DRMAA_ERROR_STRING_BUFFER] = "";
	int status;


	if ((api.init(NULL, errbuf, sizeof(errbuf) - 1) != DRMAA_ERRNO_SUCCESS))
		goto fault;

	/* job ps */
	if (api.job_ps(ps_opt.jobid, &status, errbuf, sizeof(errbuf) - 1) != DRMAA_ERRNO_SUCCESS) {
		fsd_log_fatal(("Failed to ps a job: %s ", errbuf));
		exit(2); /* TODO exception */
	}

	printf("Status of job %s is (%d): %s\n", ps_opt.jobid, status, drmaa_job_ps_to_str(status));

	return 0;
fault:
	fsd_log_fatal(("Error"));
	return 1;
}

