#include "main.h"

/**
 * clr_info - initializes info_t struct
 * @info: struct address
 */
void clr_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_information - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_information(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strgtoww(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strduplicate(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		repe_alias(info);
		repe_vars(info);
	}
}

/**
 * free_information - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_information(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			free_listt(&(info->env));
		if (info->history)
			free_listt(&(info->history));
		if (info->alias)
			free_listt(&(info->alias));
		ffree(info->environ);
			info->environ = NULL;
		bfree((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		_putchar(BUF_FLUSH);
	}
}
