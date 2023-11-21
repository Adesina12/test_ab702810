#include "lists.h"

/**
 * print_listint - Entry point
 * @h: a constant
 * Return: number of nodes
 */
size_t print_listint(const listint_t *h)
{
	size_t num;

	num = 0;
	while (h != NULL)
	{
		printf("%d\n", h->n);
		h = h->next;
		num++;
	}
	return (num);
}
