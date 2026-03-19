/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathCompute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:05:10 by mbirou            #+#    #+#             */
/*   Updated: 2026/03/19 19:36:41 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem-in.h>

/*typedef struct s_bestPred
{
	t_room				**paths;
	float				antsPerCycle;
	struct s_bestPred	next;
}						t_bestPred;*/

t_room	***incrListList(t_room ***list, t_room **elem)
{
	t_room	***nList;
	int		len;
	int		i;

	len = 0;
	while (list[len])
		len ++;
	nList = malloc(sizeof(*list) * (len + 2));
	if (!nList)
		return (NULL);
	nList[len + 1] = NULL;
	nList[len] = elem;
	i = 0;
	while (i < len)
	{
		nList[i] = list[i];
		i ++;
	}
	free(list);
	return (nList);
}

t_room	**incrList(t_room **list, t_room *elem)
{
	t_room	**nList;
	int		len;
	int		i;

	len = 0;
	while (list[len])
		len ++;
	nList = malloc(sizeof(*list) * (len + 2));
	if (!nList)
		return (NULL);
	nList[len + 1] = NULL;
	nList[len] = elem;
	i = 0;
	while (i < len)
	{
		nList[i] = list[i];
		i ++;
	}
	free(list);
	return (nList);
}

float	getbestPred(t_AntFarm *antFarm, t_bestPred *bestPred, t_room *node, float depth, int pathID, bool editPath)
{
	int		i;
	float	bestAPC; // APC = AntsPerCycle
	float	predAPC;
	t_room	*bestNeighbour;

	// if (editPath)
	// 	printf("node:%s; \tdepth:%.0f;\tnbNeighs%d\n", node->ID, depth, node->nbNeighbours);

	if (node == antFarm->end)
	{
		// if (editPath)
		// 	printf("path finished, trying to start a new one\n");

		bestPred->paths = incrListList(bestPred->paths, NULL);
		bestPred->paths[pathID + 1] = malloc(sizeof(*(bestPred->paths[pathID + 1])));
		bestPred->paths[pathID + 1][0] = NULL;

		return (1.f / depth + getbestPred(antFarm, bestPred, antFarm->start, 0, pathID + 1, true));
	}
	node->isUsed = true;
	i = 0;
	bestAPC = 0;
	bestNeighbour = NULL;
	while (node->neighbours[i])
	{
		if (!node->neighbours[i]->isUsed)
		{
			predAPC = getbestPred(antFarm, bestPred, node->neighbours[i], depth + 1, pathID, false);
			if (predAPC > bestAPC)
			{
				bestAPC = predAPC;
				bestNeighbour = node->neighbours[i];
			}
		}
		i ++;
	}
	if (editPath && bestAPC)
	{
		getbestPred(antFarm, bestPred, bestNeighbour, depth + 1, pathID, true);
		bestPred->paths[pathID] = incrList(bestPred->paths[pathID], bestNeighbour);
	}
	node->isUsed = false;

	return (bestAPC);
}

t_room	**computePaths(t_AntFarm *antFarm)
{
	t_room		**bestPath;
	t_bestPred	*bestPred;

	bestPred = malloc(sizeof(t_bestPred));
	bestPred->antsPerCycle = 0;
	bestPred->paths = malloc(sizeof(*bestPred->paths) * 2);
	bestPred->paths[0] = malloc(sizeof(*(bestPred->paths[0])));
	bestPred->paths[0][0] = NULL;
	bestPred->paths[1] = NULL;
	bestPred->next = NULL;

	bestPred->antsPerCycle = getbestPred(antFarm, bestPred, antFarm->start, 0, 0, true);
	bestPath = NULL;


	int i = 0;
	int ii = 0;
	int predID = 0;
	while (bestPred)
	{
		printf("pred %d(%f):", predID, bestPred->antsPerCycle);
		i = 0;
		while (bestPred->paths[i])
		{
			printf("\n\t");
			ii = 0;
			while (bestPred->paths[i][ii])
			{
				printf("(%s), ", bestPred->paths[i][ii]->ID);
				ii ++;
			}
			i ++;
		}

		bestPred = bestPred->next;
	}

	return (bestPath);
}

