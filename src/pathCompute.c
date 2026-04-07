/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathCompute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 15:05:10 by mbirou            #+#    #+#             */
/*   Updated: 2026/04/07 23:12:12 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <lem_in.h>

t_path	**incrPath(t_path **list, t_path *elem)
{
	t_path	**nList;
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

t_pairRoomFloat	*makePair(t_pairRoomFloat value)
{
	t_pairRoomFloat	*pair;

	pair = malloc(sizeof(*pair));
	pair->first = value.first;
	pair->second = value.second;
	return (pair);
}

t_pairRoomFloat	**fillMap(t_room **keys, int defaultValue)
{
	t_pairRoomFloat	**map;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (keys[len])
		len ++;
	map = malloc(sizeof(*map) * (len + 1));
	map[len] = NULL;
	while (i < len)
	{
		map[i] = malloc(sizeof(*(map[i])));
		map[i]->first = keys[i];
		map[i]->second = defaultValue;
		i ++;
	}
	return (map);
}

void	mapSet(t_pairRoomFloat **map, t_pairRoomFloat value)
{
	int	i;

	i = 0;
	while (map[i])
	{
		if (map[i]->first == value.first)
		{
			map[i]->second = value.second;
			return ;
		}
		i ++;
	}
}

t_pairRoomFloat	**mapPop(t_pairRoomFloat **map, t_room *key)
{
	t_pairRoomFloat	**nMap;
	int		i;
	int		len;
	bool	passedKey;

	i = 0;
	len = 0;
	if (!map || !map[0])
	{
		if (map)
			free(map);
		return (NULL);
	}
	while (map[len])
		len ++;
	nMap = malloc(sizeof(*nMap) * (len));
	nMap[len - 1] = NULL;
	passedKey = false;
	while (i < len && map[i])
	{
		if (map[i]->first == key)
		{
			passedKey = true;
			free(map[i]);
		}
		else
			nMap[i - passedKey] = map[i];
		i ++;
	}
	free(map);
	return (nMap);
}

t_pairRoomFloat	**mapInsert(t_pairRoomFloat **map, t_pairRoomFloat value)
{
	t_pairRoomFloat	**nMap;
	int		i;
	int		len;
	bool	inserted;

	i = 0;
	len = 0;
	while (map && map[len])
		len ++;
	nMap = malloc(sizeof(*nMap) * (len + 2));
	nMap[len + 1] = NULL;
	inserted = false;
	if (!len)
		nMap[0] = makePair(value);
	while (len && i < len + 1)
	{
		if (!inserted && (!map[i] || map[i]->second >= value.second))
		{
			nMap[i] = makePair(value);
			inserted = true;
		}
		else
			nMap[i] = map[i - inserted];
		i ++;
	}
	free(map);
	return (nMap);
}

t_room	**popListValue(t_room **list, t_room *key)
{
	t_room	**nList;
	int		i;
	int		len;
	bool	passedKey;

	i = 0;
	len = 0;
	while (list[len])
		len ++;
	nList = malloc(sizeof(*nList) * (len));
	nList[len - 1] = NULL;
	passedKey = false;
	while (i < len)
	{
		if (list[i] == key && !passedKey)
		{
			passedKey = true;
			free(list[i]);
			continue;
		}
		nList[i - passedKey] = list[i];
		i ++;
	}
	free(list);
	return (nList);
}

float	mapFind(t_pairRoomFloat **map, t_room *key)
{
	int	i;

	i = 0;
	while (map && map[i])
	{
		if (map[i]->first == key)
			return (map[i]->second);
		i ++;
	}
	return (-1);
}

t_pathPair	**pathInsert(t_pathPair **path, t_pathPair nNode)
{
	t_pathPair	**nPath;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (path[len])
		len ++;
	nPath = malloc(sizeof(*nPath) * (len + 2));
	nPath[len + 1] = NULL;
	nPath[len] = malloc(sizeof(*nPath[len]));
	nPath[len]->first = nNode.first;
	nPath[len]->second = nNode.second;
	while (i < len)
	{
		nPath[i] = path[i];
		i ++;
	}
	free(path);
	return (nPath);
}


t_pathPair	*pathFind(t_pathPair **path, t_room *key)
{
	int	i;

	i = 0;
	while (path[i])
	{
		if (path[i]->first == key)
			return (path[i]);
		i ++;
	}
	return (NULL);
}

static void	freeAll(t_pairRoomFloat **checkList, t_pathPair **path, t_pairRoomFloat **gScore, t_pairRoomFloat **fScore)
{
	int	i;

	i = -1;
	while (checkList && checkList[++i])
		free(checkList[i]);
	if (checkList)
		free(checkList);
	i = -1;
	while (path && path[++i])
		free(path[i]);
	if (path)
		free(path);
	i = -1;
	while (gScore && gScore[++i])
		free(gScore[i]);
	if (gScore)
		free(gScore);
	i = -1;
	while (fScore && fScore[++i])
		free(fScore[i]);
	if (fScore)
		free(fScore);
}

t_path	*makePath(t_AntFarm *antFarm, t_pathPair **paths, t_room *current)
{
	t_path	*path;

	path = malloc(sizeof(*(path)));

	// printf("\n\n== path list (%s; %s) ==\n", antFarm->start->ID, antFarm->end->ID);
	// int	i = 0;
	// while (paths[i])
	// {
	// 	printf("%s, ", paths[i]->first->ID);
	// 	i ++;
	// }
	// printf("\n");
	// printf("==========================\n\n");
	// printf("== remaking the path ==\n");

	path->nodeLenght = 0;
	path->path = malloc(sizeof(*path->path));
	path->path[0] = NULL;
	while (pathFind(paths, current))
	{

		// printf("\tcurrent: %s\n", current->ID);
		
		path->path = incrList(path->path, current);
		current = pathFind(paths, current)->second;
		path->nodeLenght += 1;
	}
	path->nodeLenght --;

	// printf("== finished the path ==\n\n\n\n");

	path->path = incrList(path->path, antFarm->start);
	freeAll(NULL, paths, NULL, NULL);
	return (path);
}

t_path	*AStar(t_AntFarm *antFarm)
{
	t_pairRoomFloat	**checkList;
	t_pathPair		**path;
	t_pairRoomFloat	**gScore;
	t_pairRoomFloat	**fScore;
	t_room			*current;
	t_room			*neightbor;
	float			tryScore;
	int				i;

	checkList = malloc(sizeof(*(checkList)) * 2);
	checkList[0] = makePair((t_pairRoomFloat){antFarm->start, 0});
	checkList[1] = NULL;

	path = malloc(sizeof(*(path)));
	path[0] = NULL;

	gScore = fillMap(antFarm->room, __INT_MAX__);
	mapSet(gScore, (t_pairRoomFloat){antFarm->start, 0});
	fScore = fillMap(antFarm->room, __INT_MAX__);
	mapSet(fScore, (t_pairRoomFloat){antFarm->start, antFarm->start->distToEnd});
	while (checkList && checkList[0])
	{
		current = checkList[0]->first;
		if (current == antFarm->end)
		{
			freeAll(checkList, NULL, gScore, fScore);
			return (makePath(antFarm, path, current));
		}
		checkList = mapPop(checkList, current);
		i = 0;
		while (((t_room*)current)->neighbours[i])
		{
			neightbor = ((t_room*)current)->neighbours[i];
			i ++;
			if (neightbor->isUsed)
				continue ;
			neightbor->isUsed = true;
			tryScore = mapFind(gScore, current) + neightbor->distToEnd;
			if (tryScore < mapFind(gScore, neightbor))
			{
				if (pathFind(path, neightbor))
					pathFind(path, neightbor)->second = current;
				else
					path = pathInsert(path, (t_pathPair){neightbor, current});
				mapSet(gScore, (t_pairRoomFloat){neightbor, tryScore});
				mapSet(fScore, (t_pairRoomFloat){neightbor, tryScore + neightbor->distToEnd});
				if (mapFind(checkList, neightbor) == -1)
					checkList = mapInsert(checkList, (t_pairRoomFloat){neightbor, mapFind(fScore, neightbor)});
			}
		}
	}
	freeAll(checkList, path, gScore, fScore);
	return (NULL);
}

void	resetRooms(t_AntFarm *antFarm, t_path **bestPaths)
{
	int i;
	int	ii;

	i = 0;
	while (antFarm->room[i])
	{
		antFarm->room[i]->isUsed = false;
		i ++;
	}
	i = 0;
	while (bestPaths[i])
	{
		ii = 0;
		while (bestPaths[i]->path[ii])
		{
			if (bestPaths[i]->path[ii] != antFarm->start && bestPaths[i]->path[ii] != antFarm->end)
				bestPaths[i]->path[ii]->isUsed = true;
			ii ++;
		}
		i ++;
	}
}

t_path **computePaths(t_AntFarm *antFarm)
{
	t_path	**bestPaths;
	t_path	*nPath;
	
	nPath = AStar(antFarm);
	if (nPath)
	{
		bestPaths = malloc(sizeof(*bestPaths));
		bestPaths[0] = NULL;
	}
	else
		return (NULL);
	while (nPath)
	{
		bestPaths = incrPath(bestPaths, nPath);
		resetRooms(antFarm, bestPaths);
		nPath = AStar(antFarm);
	}
	return (bestPaths);
}
