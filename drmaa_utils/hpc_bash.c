/* $Id: $ */
/*
 * HPC-BASH - part of the DRMAA utilities library
 * Poznan Supercomputing and Networking Center Copyright (C) 2010
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define HPC_BASH_EXIT_OK (0)
#define HPC_BASH_EXIT_ERROR (5)

static const char *translate_hpc_bash_script(const char *orginal_script_name);

int main(int argc, char **argv)
{
	char *tmp_script_file_name = NULL;
	int child_pid = -1;

	if (argc < 2) {
		fprintf(stderr, "Missing script file\n");
		exit(HPC_BASH_EXIT_ERROR);
	}

	if ((tmp_script_file_name = translate_hpc_bash_script(argv[1])) == NULL) {
		exit(HPC_BASH_EXIT_ERROR);
	}

	if ((child_pid = fork()) > 0) {
		int status = -1;

		/* parent process*/
		if (waitpid(child_pid, &status, 0) == -1) {
			perror("waitpid() failed");
			exit(HPC_BASH_EXIT_ERROR);
		}

		/* remove temporary script file */
		unlink(tmp_script_file_name);

		if (WIFEXITED(status)) {
			exit(WEXITSTATUS(status));
		} else if (WIFSIGNALED(status)) {
			exit(128 + WTERMSIG(status));
		} else {
			fprintf(stderr, "Ilegall wait staus = %d\n", status);
			exit(HPC_BASH_EXIT_ERROR);
		}

	} else if (child_pid == 0) {
		/* child process */

		/*shift arguments by one */
		argv++;
		argc--;
		execv(tmp_scrpt_file_name, argv);
		perror("execv() failed");
		exit(127);
	} else {
		perror("fork() failed");
	}

	return 1;
}

char *
translate_hpc_bash_script(const char *orginal_script_name)
{
	char script_template[32] = ".hpc-bash.XXXXXX";
	char *tmp_script_file = NULL;
	char line_buf[1024] = "";
	int out_fd = -1;
	int line_counter = 1;
	FILE *in_file = NULL;

	if ((out_fd = mkstemp(script_template)) == -1) {
		perror("mkstemp() failed");
		goto out;
	}

	if ((in_file = fopen(orginal_script_name, "r")) == NULL) {
		perror("fopen() failed");
		goto out;
	}

	while (fgets(line_buf, sizeof(line_buf), in_file) != NULL) {
		int line_length = strlen(line_buf);
		if (line_length > 0 && line_length[line_length - 1] != '\n')
			fprintf("line %d: line to long", line_counter);
	}


	tmp_script_file = strdup(script_template);
out:
	if (out_fd != -1) {
		close(out_fd);
		out_fd = -1;
	}

	if (in_file) {
		fclose(in_file);
		in_file = NULL;
	}

	return tmp_scrip_file;
}
