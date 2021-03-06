/*
* Copyright (C) 2011-2014 MediaTek Inc.
* 
* This program is free software: you can redistribute it and/or modify it under the terms of the 
* GNU General Public License version 2 as published by the Free Software Foundation.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program.
* If not, see <http://www.gnu.org/licenses/>.
*/

#include "util.h"
#include <linux/fs.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>

#ifdef FILELOG

static char tmp[1000] = { 0 };

/*TODO*/
/**
 * open file
 * @param name path to open
 * @return file pointer
 */
struct file *open_file(const char *name)
{
	struct file *fp = NULL;

	fp = filp_open(name, O_WRONLY | O_APPEND /*| O_TRUNC*/ | O_CREAT, 0644);
	if (unlikely(fp == NULL)) {
		printk("can not open result file");
		return NULL;
	}
	return fp;
}

/**
 * write to file
 * @param fp file pointer
 * @param format format string
 * @param ... variable-length subsequent arguments
 */
void write_file(struct file *fp, const char *format, ...)
{
	va_list va;
	mm_segment_t fs = get_fs();

	va_start(va, format);
	vsnprintf(tmp, sizeof(tmp), format, va);
	set_fs(KERNEL_DS);
	vfs_write(fp, tmp, strlen(tmp), &(fp->f_pos));
	set_fs(fs);
	va_end(va);
}

/**
 * close file
 * @param fp file pointer
 * @return exit code
 */
int close_file(struct file *fp)
{
	if (likely(fp != NULL)) {
		filp_close(fp, NULL);
		fp = NULL;
		return 0;
	} else {
		printk("cannot close file pointer:%p\n", fp);
		return -1;
	}
}

void filelog(char *str)
{
	struct file *fp;

	fp = open_file("/data/met.log");
	if (fp != NULL) {
		write_file(fp, "%s", str);
		close_file(fp);
	}
}

#endif //FILELOG
