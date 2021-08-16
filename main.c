#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1
#define MAX_INPUT_LEN 100

#define DISTANCE_OF_ROUTE          0
#define NUMBER_OF_TRIPS_MAX_STOP   1
#define NUMBER_OF_TRIPS_EXACT_STOP 2
#define SHORTEST_ROUTE             3
#define NUMBER_OF_DIFF_ROUTES      4

/**
* @brief get the distance of the route
* @param[in]  input          towns
* @param[in]  len            the number of towns
* @param[in]  **graph        town graph
* @param[in]  town_nums      the number of towns
* @return
* - the distance from the first to last town
* - ERROR        "NO SUCH ROUTE"
*/
extern int GetDistance(char *input, int len, int **in_map, int town_nums);

/**
* @brief get the number of trips starting at src and ending at dst with a maximum of N stops
* @param[in]  src            source town
* @param[in]  dst            destination town
* @param[in]  stops          max value of stops
* @param[in]  **graph        town graph
* @param[in]  town_nums      the number of towns
* @return
* - the number of trips
*/
extern int GetNumberOfTripsWithMaxStops(char src, char dst, int stops, int **graph, int town_nums);

/**
* @brief get the number of trips starting at src and ending at dst with exactly N stops
* @param[in]  src            source town
* @param[in]  dst            destination town
* @param[in]  stops          max value of stops
* @param[in]  **graph        town graph
* @param[in]  town_nums      the number of towns
* @return
* - the number of trips
*/
extern int GetNumberOfTripsWithExactStops(char src, char dst, int stops, int **graph, int town_nums);

/**
* @brief get the length of the shortest route from src to dst
* @param[in]  src            source town
* @param[in]  dst            destination town
* @param[in]  **graph        town graph
* @param[in]  town_nums      the number of towns
* @return
* - the shortest route
*/
extern int GetLengthOfShortestRoute(char src, char dst, int **graph, int town_nums);

/**
* @brief get the number of different routes from C to C with a distance of less than N
* @param[in]  src            source town
* @param[in]  dst            destination town
* @param[in]  distance       limit distance
* @param[in]  **graph        town graph
* @param[in]  town_nums      the number of towns
* @return
* - the number of different routes
*/
extern int GetNumberOfRoutes(char src, char dst, int distance, int **graph, int town_nums);

static int** init(int len) {
    int input_nums = len;
    int **town_graph = (int **)malloc(sizeof(int *) * input_nums);

    for (int i = 0; i < input_nums; ++i) {
        town_graph[i] = (int *)malloc(sizeof(int) * input_nums);
        memset(town_graph[i], 0, sizeof(int) * input_nums);
        if (NULL == town_graph[i]) {
            return NULL;
        }
    }
    return town_graph;
}

static void deinit(int **town_graph, int len) {
    int input_nums = len;
    for (int i = 0; i < input_nums; ++i) {
        if (NULL != town_graph[i]) {
            free(town_graph[i]);
            town_graph[i] = NULL;
        }
    }

    if (NULL != town_graph) {
        free(town_graph);
        town_graph = NULL;
    }
}

static void fill_map(int **in_map, int map_rows, char data[], int data_nums) {
    if (NULL == in_map) {
        return;
    }

    int row = 0;
    int col = 0;
    int value = -1;
    
    int i = 0;
    while (data[i] != '\0' || data[i] != '\n') {
        row = (int)(data[i] - 'A');
        col = (int)(data[i + 1] - 'A');
        value = atoi(&(data[i + 2]));

        in_map[row][col] = value;

        if (data[i + 3] == '\0' || data[i + 3] == '\n') {
            break;
        }
        i += 4;
    }
}

static int g_index = 1;
static void run(int case_value, char *str, int str_len, char src, char dst, int **graph, int town_nums, int limit_value) {
    int ret = 0;
    switch(case_value) {
    case DISTANCE_OF_ROUTE:
        ret = GetDistance(str, str_len, graph, town_nums);
        if (ERROR == ret) {
             printf("Output #%d: %s\n", g_index, "NO SUCH ROUTE");
             return;
        }
        break;
    case NUMBER_OF_TRIPS_MAX_STOP:
        ret = GetNumberOfTripsWithMaxStops(src, dst, limit_value, graph, town_nums);
        break;
    case NUMBER_OF_TRIPS_EXACT_STOP:
        ret = GetNumberOfTripsWithExactStops(src, dst, limit_value, graph, town_nums);
        break;
    case SHORTEST_ROUTE:
        ret = GetLengthOfShortestRoute(src, dst, graph, town_nums);
        break;
    case NUMBER_OF_DIFF_ROUTES:
        ret = GetNumberOfRoutes(src, dst, limit_value, graph, town_nums);
        break;
    default:
        printf("Error input: #%d\n", g_index);
        break;
    }
    printf("Output #%d: %d\n", g_index, ret);
}


static int** test_case(char *input, int charater_nums, int edge_nums) {
    int **map = init(charater_nums);
    if (NULL == map) {
        deinit(map, charater_nums);
    }
    fill_map(map, charater_nums, input, edge_nums);
    return map;
}

int main() {
    FILE *fp = NULL;
    char str[MAX_INPUT_LEN] = { 0 };
 
    fp = fopen("test_case_input.txt","r");
    if (fp == NULL) {
        puts("Open test_case_input.txt failed!");
        return 0;
    }
   
    fgets(str, MAX_INPUT_LEN, fp);
    
    int nums[3] = { 0 };
    int index = 0;
    char *p = strtok(str, " ");
    if (p) {
        nums[index] = atoi(p);
        index++;
    }
 
    while (p = strtok(NULL, " ")) {
        nums[index] = atoi(p);
        index++;
        if (3 == index) {
            break;
        }
    }

    int tmp_idx = 0;
    int test_count = nums[tmp_idx++];
    int town_count = nums[tmp_idx++];
    int edge_count = nums[tmp_idx++];

    fgets(str, MAX_INPUT_LEN, fp);
    int **graph = test_case(str, town_count, edge_count);

    g_index = 1;
    for (int i = 0; i < nums[0]; ++i) {
        fgets(str, MAX_INPUT_LEN, fp);
        char *find = strchr(str, '\n');
        if (find) {
            *find = '\0';
        }

        int case_index = atoi(str);
        char tmp_str0[MAX_INPUT_LEN] = { 0 };
        if (0 == case_index) {
            strcpy(tmp_str0, &(str[2]));
        }

        int limit = 0;
        if (str[4] != '\0') {
            limit = atoi(&str[5]);
        }

        run(case_index, tmp_str0, strlen(tmp_str0), str[2], str[3], graph, town_count, limit);
        g_index++;
    }

    deinit(graph, town_count);
    fclose(fp);
    return 0;
}
