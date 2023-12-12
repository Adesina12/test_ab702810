#include "main.h"

/**
 * cde - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int cde(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clr_info(info);
		if (intratv(info))
			_puts("$ ");
		_inputcha(BUF_FLUSH);
		r = write_input(info);
		if (r != -1)
		{
			set_information(info, av);
			builtin_ret = fd_built(info);
			if (builtin_ret == -1)
				fd_cmd(info);
		}
		else if (intratv(info))
			_putchar('\n');
		free_information(info, 0);
	}
	 write_hty(info);
	 free_information(info, 1);
	if (!intratv(info) && info->status)
		exit(info->status);
	if (builtin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (builtin_ret);
}

/**
 * fd_built - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int fd_built(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit",  _ourexits},
		{"env", _ourenviron},
		{"help",  _ourhelps},
		{"history", _ourhrr},
		{"setenv", _oursetenv},
		{"unsetenv", _ourunsetenviron},
		{"cd", _outcds},
		{"alias",  _ourals},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcompare(info->argv[0], builtintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * fd_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fd_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delimi(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = fd_path(info, _getenvrn(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		 fk_cmd(info);
	}
	else
	{
		if ((intratv(info) || _getenvrn(info, "PATH=")
			|| info->argv[0][0] == '/') && is_cod(info, info->argv[0]))
			fk_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			print_err(info, "not found\n");
		}
	}
}

/**
 * fk_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fk_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_envi(info)) == -1)
		{
			free_information(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WEXITSTATUS(info->status);
			if (info->status == 126)
				print_err(info, "Permission denied\n");
		}
	}
}
