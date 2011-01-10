
#include "drmaa.h"

#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

extern char **environ;


#define DRMAA_PATH "DRMAA_PATH"
#define DRMAA_DEBUG "DRMAA_DEBUG"


int main(int argc, char **argv)
{
	char *drmaa_path = getenv(DRMAA_PATH);
    void *handle = NULL;
	char *command = NULL;
	char **command_args = NULL;
	char working_directory[1024] = ".";
    drmaa_job_template_t *jt = NULL;
    char errbuf[DRMAA_ERROR_STRING_BUFFER] = "";
    char stdout_name[1048] = "stdout";
    char stderr_name[1048] = "stderr";
    char jobid[DRMAA_JOBNAME_BUFFER] = "";

	printf("DRMAA run (DRMAA_PATH=%s)\n", drmaa_path);

	if (!drmaa_path)
	{
		fprintf(stderr, DRMAA_PATH " not set!\n");
		exit(1);
	}

	if (argc <= 1)
	{
		fprintf(stderr, "Insufficient number of arguments \n");
		exit(1);
	}

    handle = dlopen(drmaa_path, RTLD_NOW | RTLD_GLOBAL);

    if (!handle)
    {
        const char *msg = dlerror();

		if (!msg)
			fprintf(stderr, "Could not load DRMAA library: %s (DRMAA_PATH=%s)", msg, drmaa_path);
		else
			fprintf(stderr, "Could not load DRMAA library: %s", drmaa_path);

		exit(1);
    }

    assert(drmaa_allocate_job_template(&jt, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);

	/*  command */
    command = argv[1];

    assert(drmaa_set_attribute(jt, DRMAA_REMOTE_COMMAND, command, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);

	/*  args */
    if (argc > 2)
    {
    	char **args_vector = NULL;
    	int i;

    	command_args = &(argv[2]);
    	argc-=2;

    	args_vector = calloc(argc + 1, sizeof(char *));
    	/* TODO: check */

    	for (i=0; i < argc; i++)
    	{
    		args_vector[i] = command_args[i];
    	}

        assert(drmaa_set_vector_attribute(jt, DRMAA_V_ARGV, (const char **)args_vector, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);
    }

	/*  environment */
    {
    	int i = 0;

    	while (environ[i]) {
    		printf("environ[%d]=%s\n", i, environ[i]);
    	}

        assert(drmaa_set_vector_attribute(jt, DRMAA_V_ENV, (const char **)environ, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);
    }


	/*  working directory */
    assert( getcwd(working_directory, sizeof(working_directory)) );
    assert( drmaa_set_attribute(jt, DRMAA_WD, working_directory, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);

    /* read stdin */

	/* stdout.PID stderr.PID */
    sprintf(stdout_name, ":%s/stdout.%u", working_directory, (unsigned int)getpid());
    sprintf(stderr_name, ":%s/stderr.%u", working_directory, (unsigned int)getpid());

    assert( drmaa_set_attribute(jt, DRMAA_OUTPUT_PATH, stdout_name, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);
    assert( drmaa_set_attribute(jt, DRMAA_ERROR_PATH, stderr_name, errbuf, sizeof(errbuf) - 1) == DRMAA_ERRNO_SUCCESS);

    /* run */
    if (drmaa_run_job(jobid, sizeof(jobid) - 1, jt, errbuf, sizeof(errbuf) - 1) != DRMAA_ERRNO_SUCCESS)
    {
		fprintf(stderr, "Failed to submit a job: %s \n", errbuf);
		exit(2);
    }

	/* wait */

	/*  print stdout and stderr */

	/* 7 - exit */


    return 0;
}
