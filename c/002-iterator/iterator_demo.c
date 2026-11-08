#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    const char *const *items;
    size_t length;
    size_t index;
} MapIter;

typedef struct {
    MapIter *source;
    bool (*predicate)(int);
} FilterIter;

bool map_next(MapIter *iter, int *out)
{
    if (iter->index >= iter->length) {
        return false;
    }

    *out = (int)strtol(iter->items[iter->index], NULL, 10);
    iter->index++;
    return true;
}

bool below_1000(int value)
{
    return value < 1000;
}

bool filter_next(FilterIter *iter, int *out)
{
    while (map_next(iter->source, out)) {
        if (iter->predicate(*out)) {
            return true;
        }
    }
    return false;
}

int main(void)
{
    const char *const raw[] = {"1200", "850", "1530", "720"};
    MapIter sales = {raw, sizeof raw / sizeof raw[0], 0};
    FilterIter low = {&sales, below_1000};
    int value;

    printf("&sales=%p, low.source=%p\n", (void *)&sales, (void *)low.source);
    printf("index before: %zu\n", sales.index);

    if (filter_next(&low, &value)) {
        printf("first low: %d\n", value);
    }
    printf("index after filter: %zu\n", sales.index);

    printf("remaining sales:");
    while (map_next(&sales, &value)) {
        printf(" %d", value);
    }
    putchar('\n');

    printf("low has more: %s\n", filter_next(&low, &value) ? "true" : "false");
    printf("final index: %zu, raw[0]: %s\n", sales.index, raw[0]);
    return 0;
}
