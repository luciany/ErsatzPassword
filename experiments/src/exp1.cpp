#include <security/pam_appl.h>
#include <security/openpam.h>
#include <security/pam_mod_misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

using namespace std;

static struct pam_conv conv = {openpam_ttyconv, NULL};


//compile with g++48 -lpam -o exp1 exp1.cpp
int main(int argc, char *argv[]) 
{
	if(argc < 2)
	{
		printf("Experiment 1 - Enrollment Latency\n");
		printf("usage: %s <username> <new_password> \n", argv[0]);
		return 0; 
	}
	pam_handle_t *pamh = NULL;  /** PAM data structure **/
	int retval;

	timeval t1, t2;				/* hi-res timer www.songho.ca/misc/timer/timer.html */
	double elp_time, sum = 0.0;
	/** Creating and initializing a PAM session **/
	gettimeofday(&t1, NULL);
	retval = pam_start("passwd", argv[1], &conv, &pamh);
	pam_set_item(pamh, PAM_AUTHTOK, argv[2]);
	/** Change password **/
	retval = pam_chauthtok(pamh, 0);
	if(retval != PAM_SUCCESS)
		printf("Error!\n");

	/** Destroy the PAM session **/
	pam_end(pamh, retval);
	gettimeofday(&t2, NULL);
	elp_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elp_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
	sum += elp_time;
	printf("%f\n", elp_time);
	if (retval != PAM_SUCCESS) 
		fprintf(stderr, "stderr: %s\n", pam_strerror(pamh, retval));
}
