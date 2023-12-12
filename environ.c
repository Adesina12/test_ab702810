#include "main.h"

/**
 * _ourenviron - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int _ourenviron(info_t *info)
{
	print_list_string(info->env);
	return (0);
}

/**
 * _getenvrn - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *_getenvrn(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = stts_wh(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _oursetenv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _oursetenv(info_t *info)
{
	if (info->argc != 3)
	{
		_inputstr("Incorrect number of arguements\n");
		return (1);
	}
	if (_setevn(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _ourunsetenviron - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int _ourunsetenviron(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		_inputstr("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		_unsetern(info, info->argv[i]);

	return (0);
}

/**
 * populate_environ_list - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int populate_environ_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_nod_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
