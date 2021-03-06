/** @file process.h
 *  @brief Lauch and pipe processes
 *
 *  This file contains the function signatures for lauching commands in new
 *  processes, get their pid, wait for a process to stop, get status,
 *  pipe processes.
 *
 *  @author Tom Gouville (tom.gouville@telecomnancy.eu)
 *  @author Amélie Ferstler (amelie.ferstler@telecomnancy.eu)
 */
#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h> 
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

/** @brief A struct for managing a process
 * 
 *  Allow to launch a command in a process, get its status, pid, and to make pipes on
 *  stdin or stdout.
 */
typedef struct _process {
	int fd_in[2];	///< a file descriptor which can be piped to stdin
	int fd_out[2];	///< a file descriptor which can be piped to stdout
	int status;		///< the execution status of the process
	bool isPiped;	///< if the output of the process is piped
	pid_t pid;		///< the pid of the process
} process;


/** @brief New process struct
 * 
 *  Create a new process struct and initialize a pipe on process.fd_out
 * 
 *  @return a pointer to a process
 */
process* new_process();

/** @brief New process struct with stdin piped on another process
 * 
 *  Create a new process struct and pipe stdin on the stdout of
 *  the process given in parameters.
 * 
 *  @param p1 the process piped
 *  @return a pointer to a process
 */
process* piped_process(process* p1);

/** @brief New process struct with stdin piped on a file 
 * 
 *  Create a new process struct and pipe stdin on the content of
 *  the file given in parameters.
 * 
 *  @param filename the name of the file piped
 *  @return a pointer to a process
 */
process* piped_process_from_file(char* filename);


/** @brief Lauch a command in a new process
 * 
 *  Launch a command (an array of arguments) in a new fork, manage pipes on
 *  stdin and stdout and store the pid of the process.
 *  If the command fails, the process exit with the error code of exec.
 * 
 *  @param p the process
 *  @param args the command to launch (as an array of arguments, terminating with NULL)
 *  @param pipeOutput pipe stdout on process.fd_out (true) or print stdout on terminal (false)
 *  @return the pid of the process
 */
int launch_process(process* p, char* args[], bool pipeOutput);

/** @brief Lauch a command in a new process and print result to terminal
 * 
 *  Wrapper for `launch_process` with `pipeOutput` = `false`
 * 
 *  @param p the process
 *  @param args the command to launch (as an array of arguments, terminating with NULL)
 *  @return the pid of the process
 */
int launch_and_print(process* p, char* args[]);

/** @brief Lauch a command in a new process and redirect result to a pipe
 * 
 *  Wrapper for `launch_process` with `pipeOutput` = `true`
 * 
 *  @param p the process
 *  @param args the command to launch (as an array of arguments, terminating with NULL)
 *  @return the pid of the process
 */
int launch_and_pipe(process* p, char* args[]);

/** @brief Lauch a process and pipe its content to a file
 * 
 *  Clear process from memory.
 * 
 *  @param p the process
 *  @param args the command to launch (as an array of arguments, terminating with NULL)
 *  @param filename the name of the file to write to
 *  @param append if true the new content is put at the end of the file, if not the actual
 *  content is deleted
 *  @return the pid of the process
 */
int pipe_to_file(process* p, char* args[], char* filename, bool append);

/** @brief Wait for a process to finish and get its status
 * 
 *  Wait for a process to finish and store its status
 * 
 *  @param p the process
 *  @return the status of the process
 */
int wait_status(process* p);

/** @brief Free a process
 * 
 *  Clear process from memory.
 * 
 *  @param p the process
 *  @return void
 */
void free_process(process* p);
