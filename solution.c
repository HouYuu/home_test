/*
 * File:   solution.c for the home testing 
 * Author: yu hou
 * Email:  yu.hou@email.com
 */

#define MAX_LEN 1000
#define BASE    'A'
#define ERROR   -1

#define MAX_STOP   0
#define EXACT_STOP 1

#ifndef NULL
#define NULL 0
#endif


static int get_distance(char src, char dst, int **graph, int town_nums) {
    if (0 > town_nums || NULL == graph) {
        return ERROR;
    }

    int src_index = (int)(src - BASE);
    int dst_index = (int)(dst - BASE);

    if (src_index >= town_nums || dst_index >= town_nums) {
        return ERROR;
    }
    return graph[src_index][dst_index];
}

int GetDistance(char *input, int len, int **in_map, int town_nums) {
    int distance = 0;
    int tmp_value = 0;
    for (int i = 0; i < len - 1; ++i) {
        tmp_value = get_distance(input[i], input[i + 1], in_map, town_nums);
        if (0 == tmp_value) {
            return ERROR;
        } else if (0 < tmp_value) {
            distance += tmp_value;
        } else {
            return ERROR;
        }
    }
    return distance;
}

static int get_trips_number(
    char src, char dst, int stops, int **graph, int town_nums, int flag) {
    int numbers = 0;
    int src_index = (int)(src - BASE);
    int dst_index = (int)(dst - BASE);

    int tmp_queue[MAX_LEN] = { 0 };

    int queue_index = 0;
    tmp_queue[queue_index] = src_index;
    queue_index++;

    int steps = 0;
    int visit_index = 0;

    int size = queue_index - visit_index;
    while (size > 0) {
        steps++;
        if (steps > stops) {
            return numbers;
        }
        for (int i = visit_index; i < visit_index + size; ++i) {
            src_index = tmp_queue[i];
            for (int j = 0; j < town_nums; ++j) {
                if (0 == graph[src_index][j]) {
                    continue;
                }
                
                if (j != dst_index) {
                    tmp_queue[queue_index] = j;
                    queue_index++;
                } else {
                    if (MAX_STOP == flag) {
                        numbers++;
                    } else if (EXACT_STOP == flag) {
                        if (steps == stops) {
                            numbers++;
                        } else {
                            tmp_queue[queue_index] = j;
                            queue_index++;
                        }
                    } else {
                        return ERROR;
                    }
                }
            }
        }
        visit_index += size;
        size = queue_index - visit_index;
    }
    return numbers;
}

int GetNumberOfTripsWithMaxStops(
    char src, char dst, int stops, int **graph, int town_nums) {
    if (0 > stops || 0 > town_nums || NULL == graph) {
        return ERROR;
    }
    return get_trips_number(src, dst, stops, graph, town_nums, 0);
}

int GetNumberOfTripsWithExactStops(
    char src, char dst, int stops, int **graph, int town_nums) {
    if (0 > stops || 0 > town_nums || NULL == graph) {
        return ERROR;
    }
    return get_trips_number(src, dst, stops, graph, town_nums, 1);
}

static int get_shortest_route(
    char src, char dst, int **graph, int town_nums, int distance) {
    if (src == dst && 0 != distance) {
        return distance;
    }
    
    int src_index = (int)(src - BASE);
    int dst_index = (int)(dst - BASE);
    
    if (-2 == graph[src_index][dst_index]) {
        return 0;
    }

    int ret = 0;
    for (int i = 0; i < town_nums; ++i) {
        if (graph[src_index][i] > 0) {
            int tmp = graph[src_index][i];
            graph[src_index][i] = -2;

            char tmp_src = (char)(i + BASE);
            int tmp_value = get_shortest_route(
                tmp_src, dst, graph, town_nums, distance + tmp);
            if (0 == ret) {
                ret = tmp_value;
            } else {
                if (0 < tmp_value && tmp_value < ret) {
                    ret = tmp_value;
                }
            }
            graph[src_index][i] = tmp;
        }
    }

    return ret;
}

int GetLengthOfShortestRoute(char src, char dst, int **graph, int town_nums) {
    if (0 > town_nums || NULL == graph) {
        return ERROR;
    }
    return get_shortest_route(src, dst, graph, town_nums, 0);
}

static int get_route_numbers(char src, char dst,
    int **graph, int town_nums, int max_distance, int current_distance) {
    int ret = 0;
    if (current_distance >= max_distance) {
        return ret;
    }

    if (src == dst &&
        current_distance < max_distance && current_distance > 0) {
        ret++;
    }

    int src_index = (int)(src - BASE);
    for (int i = 0; i < town_nums; ++i) {
        if (graph[src_index][i] > 0) {
            ret += get_route_numbers((char)(i + BASE), dst, graph, town_nums,
                max_distance, current_distance + graph[src_index][i]);
        }
    }
    return ret;
    
}

int GetNumberOfRoutes(char src, char dst,
    int distance, int **graph, int town_nums) {
    if (0 > town_nums || NULL == graph) {
        return ERROR;
    }
    return get_route_numbers(src, dst, graph, town_nums, distance, 0);
}
