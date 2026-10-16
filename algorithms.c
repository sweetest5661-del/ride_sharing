/*
 * Beginner-friendly C algorithms for the SmartRide example.
 *
 * This file uses arrays, loops, if/else, functions, structures, pointers,
 * strings, and recursion. It avoids C++ code and advanced shortcuts.
 */

#include <stdio.h>
#include <string.h>

#define C_MAX_LOCATIONS 10

struct CMatchCandidate
{
    char rideId[32];
    char riderId[32];
    char source[64];
    char destination[64];
    int score;
    double distanceKm;
    double originalFare;
    double sharedFare;
    double savings;
};

/* Remove spaces, tabs, and line breaks from the start and end of a string. */
void c_str_trim(char str[])
{
    int length = 0;
    int start = 0;
    int end;
    int newIndex = 0;
    int i;

    if (str == NULL)
    {
        return;
    }

    while (str[length] != '\0')
    {
        length++;
    }

    while (start < length &&
           (str[start] == ' ' || str[start] == '\t' ||
            str[start] == '\n' || str[start] == '\r'))
    {
        start++;
    }

    if (start == length)
    {
        str[0] = '\0';
        return;
    }

    end = length - 1;
    while (end >= start &&
           (str[end] == ' ' || str[end] == '\t' ||
            str[end] == '\n' || str[end] == '\r'))
    {
        end--;
    }

    for (i = start; i <= end; i++)
    {
        str[newIndex] = str[i];
        newIndex++;
    }
    str[newIndex] = '\0';
}

/* Change English lowercase letters to uppercase. */
void c_str_to_upper(char str[])
{
    int i;

    if (str == NULL)
    {
        return;
    }

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= 'a' && str[i] <= 'z')
        {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

/* Compare two strings without treating uppercase and lowercase as different. */
int c_str_case_cmp(const char str1[], const char str2[])
{
    int i = 0;
    char char1;
    char char2;

    if (str1 == NULL && str2 == NULL)
    {
        return 0;
    }
    if (str1 == NULL)
    {
        return -1;
    }
    if (str2 == NULL)
    {
        return 1;
    }

    while (str1[i] != '\0' && str2[i] != '\0')
    {
        char1 = str1[i];
        char2 = str2[i];

        if (char1 >= 'A' && char1 <= 'Z')
        {
            char1 = char1 + ('a' - 'A');
        }
        if (char2 >= 'A' && char2 <= 'Z')
        {
            char2 = char2 + ('a' - 'A');
        }

        if (char1 != char2)
        {
            return (int)char1 - (int)char2;
        }
        i++;
    }

    char1 = str1[i];
    char2 = str2[i];
    if (char1 >= 'A' && char1 <= 'Z')
    {
        char1 = char1 + ('a' - 'A');
    }
    if (char2 >= 'A' && char2 <= 'Z')
    {
        char2 = char2 + ('a' - 'A');
    }

    return (int)char1 - (int)char2;
}

/* Search each array item in order. Return -1 when the value is not found. */
int c_linear_search_int(const int arr[], int size, int target)
{
    int i;

    for (i = 0; i < size; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }
    return -1;
}

/* Search a sorted array by repeatedly checking the middle item. */
int c_binary_search_int(const int arr[], int size, int target)
{
    int low = 0;
    int high = size - 1;
    int middle;

    while (low <= high)
    {
        middle = low + (high - low) / 2;

        if (arr[middle] == target)
        {
            return middle;
        }
        else if (arr[middle] < target)
        {
            low = middle + 1;
        }
        else
        {
            high = middle - 1;
        }
    }
    return -1;
}

/* Swap two candidate structures using a temporary structure. */
void swap_candidates(struct CMatchCandidate *first,
                     struct CMatchCandidate *second)
{
    struct CMatchCandidate temporary;

    temporary = *first;
    *first = *second;
    *second = temporary;
}

/* Put higher score first; for equal scores, put higher savings first. */
int partition_candidates(struct CMatchCandidate arr[], int low, int high)
{
    int pivotScore = arr[high].score;
    double pivotSavings = arr[high].savings;
    int i = low - 1;
    int j;

    for (j = low; j < high; j++)
    {
        if (arr[j].score > pivotScore ||
            (arr[j].score == pivotScore && arr[j].savings > pivotSavings))
        {
            i++;
            swap_candidates(&arr[i], &arr[j]);
        }
    }

    swap_candidates(&arr[i + 1], &arr[high]);
    return i + 1;
}

/* QuickSort uses recursion to sort candidates from highest score to lowest. */
void c_quicksort_candidates(struct CMatchCandidate arr[], int low, int high)
{
    int pivotIndex;

    if (low < high)
    {
        pivotIndex = partition_candidates(arr, low, high);
        c_quicksort_candidates(arr, low, pivotIndex - 1);
        c_quicksort_candidates(arr, pivotIndex + 1, high);
    }
}

/* Copy candidates whose score meets the minimum. outArr needs enough space. */
int c_filter_candidates(const struct CMatchCandidate inArr[], int inCount,
                        struct CMatchCandidate outArr[], int minScore)
{
    int count = 0;
    int i;

    for (i = 0; i < inCount; i++)
    {
        if (inArr[i].score >= minScore)
        {
            outArr[count] = inArr[i];
            count++;
        }
    }
    return count;
}

/* Dijkstra shortest path for nonnegative road distances.
   A zero in graph means there is no direct road between two locations. */
double c_dijkstra(const double graph[][C_MAX_LOCATIONS], int count,
                  int start, int destination)
{
    double distance[C_MAX_LOCATIONS];
    int visited[C_MAX_LOCATIONS];
    int i, j, current;

    if (count <= 0 || count > C_MAX_LOCATIONS || start < 0 ||
        destination < 0 || start >= count || destination >= count)
    {
        return -1;
    }
    for (i = 0; i < count; i++)
    {
        distance[i] = 1000000000.0;
        visited[i] = 0;
    }
    distance[start] = 0;

    for (i = 0; i < count; i++)
    {
        current = -1;
        for (j = 0; j < count; j++)
        {
            if (visited[j] == 0 &&
                (current == -1 || distance[j] < distance[current]))
                current = j;
        }
        if (current == -1 || distance[current] == 1000000000.0) break;
        if (current == destination) return distance[destination];

        visited[current] = 1;
        for (j = 0; j < count; j++)
        {
            if (graph[current][j] > 0 && visited[j] == 0 &&
                distance[current] + graph[current][j] < distance[j])
                distance[j] = distance[current] + graph[current][j];
        }
    }
    return -1;
}

/* Small main function so the file can be compiled and run on its own. */
#ifndef ALGORITHMS_NO_MAIN
int main(void)
{
    char city[32] = "  ISBT  ";
    int numbers[6] = {10, 20, 30, 40, 50, 60};
    struct CMatchCandidate candidates[3];
    int i;

    printf("String trim example:\n");
    printf("Before: '%s'\n", city);
    c_str_trim(city);
    printf("After:  '%s'\n\n", city);

    printf("Case-insensitive comparison of ISBT and isbt: ");
    if (c_str_case_cmp("ISBT", "isbt") == 0)
    {
        printf("same\n\n");
    }
    else
    {
        printf("different\n\n");
    }

    printf("Linear search for 30: index %d\n",
           c_linear_search_int(numbers, 6, 30));
    printf("Binary search for 50: index %d\n\n",
           c_binary_search_int(numbers, 6, 50));

    strcpy(candidates[0].riderId, "Rider_A");
    candidates[0].score = 60;
    candidates[0].savings = 85.0;

    strcpy(candidates[1].riderId, "Rider_B");
    candidates[1].score = 80;
    candidates[1].savings = 65.0;

    strcpy(candidates[2].riderId, "Rider_C");
    candidates[2].score = 40;
    candidates[2].savings = 72.5;

    c_quicksort_candidates(candidates, 0, 2);
    printf("Candidates ranked by score:\n");
    for (i = 0; i < 3; i++)
    {
        printf("%s: score %d, savings Rs. %.2f\n",
               candidates[i].riderId,
               candidates[i].score,
               candidates[i].savings);
    }

    {
        double graph[C_MAX_LOCATIONS][C_MAX_LOCATIONS] = {{0}};
        graph[0][1] = 7;
        graph[0][2] = 2;
        graph[2][1] = 1;
        printf("Dijkstra shortest distance from 0 to 1: %.1f km\n",
               c_dijkstra(graph, 3, 0, 1));
    }

    return 0;
}
#endif
