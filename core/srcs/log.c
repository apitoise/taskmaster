#include "../hdrs/includes.h"
#include "../hdrs/prog.h"
#include "../hdrs/glob.h"

void	state_log(prog_t *prog, uint64_t proc_i) {
	char 	*state_str[] = {
		"Stopping", "Stopping...", "Stopped",
		"Starting", "Starting...", "Started",
		"Start failed", "Start retry", "Exited" };

	fprintf(glob.log_file, "[DATE][INFO] - %s %lu/%lu: %s.\n"
		, prog->name, proc_i, prog->procs->sz,
		state_str[((proc_t *)prog->procs->data[proc_i])->state]);
}
