// Kevin Armoogan
// CDA 3103
// Professor Torosdagli
// Bonus Assignment

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CACHE 32
#define EL_SIZE 4

// creating cache structure
typedef struct cache
{
    int size;
    int cache_hit;
    int cache_miss;
    int rep;
    int el_size;
    int sets;
    int associ;
    char ** data;
    int ** tab;
    int ** cert;
    int * lru;
};

// function to access the cache
void get_cache(Cache * cache, int add)
{
    int i, j, lru_in, rand_in;
    int tab = (add / cache->el_size) / cache->sets;
    int set_ind = (add / cache->el_size) % cache->sets;
    for (i = 0; i < cache->associ; i++)
    {
        if(cache->tab[set_ind][i] == tab && cache->cert[set_ind][i])
        {
            cache->cache_hit++;
            if(cache->rep == 0)
            {
                cache->lru[set_ind] = i;
            }
            return;
        }
        
    }
    cache->cache_miss++;
    if(cache->rep == 0)
    {
        lru_in = cache->lru[set_ind];
        for (i = 0; i < cache->associ; i++)
        {
            if(!cache->cert[set_ind][i])
            {
                lru_in = i;
                break;
            }
            if(cache->lru[set_ind] > cache->lru[set_ind + i])
            {
                lru_in = i;
            }
        }
        cache->lru[set_ind] = cache->lru[set_ind] + 1;
        if (lru_in == cache->associ) 
        {
            lru_in = 0;
            cache->lru[set_ind] = 0;
        }
cache->tab[set_ind][lru_in] = tab;
cache->valid[set_ind][lru_in] = 1;
memcpy(&cache->data[set_ind][lru_in * cache->el_size], &add, cache->el_size * sizeof(char));
    } 
        else 
        {
            rand_in = rand() % cache->associ;
            cache->tab[set_ind][rand_in] = tab;
            cache->valid[set_ind][rand_in] = 1;
            memcpy(&cache->data[set_ind][rand_in * cache->el_size], &add, cache->el_size * sizeof(char));
        }
}

// function to initialize cache
void start_cache(Cache * cache, int associ, int rep)
{
    int i, j;
    cache->rep = rep;
    cache->cache_hit = 0;
    cache->cache_miss = 0;
    cache->associ = associ;
    cache->sets = cache->size / (associ * cache->el_size);
    cache->data = (char **) malloc(cache->sets * sizeof(char *));
    cache->cert = (int **) malloc(cache->sets * sizeof(int *));
    cache->tab = (int **) malloc(cache->sets * sizeof(int *));
    if(rep == 0)
    {
        memset(cache->lru, 0, cache->sets * sizeof(int));
        cache->lru = (int *) malloc(cache->sets * sizeof(int));
    }
    for (i = 0; i < cache->sets; i++)
    {
        cache->data[i] = (char *) malloc(cache->el_size * associ * sizeof(char)));
        cache->cert[i] = (int *) malloc(associ * sizeof(int));
        cache->tab[i] = (int *) malloc(associ * sizeof(int));
        for (j = 0; j < associ; j++)
        {
            cache->cert[i][j] = 0;
            cache->tab[i][j] = -1;
            memset(&cache->data[i][j * cache->el_size], 0, cache->el_size * sizeof(char));
        }
    }
}

// convert string to long integer
int hex_int(char * hex)
{
    return (int) strtol(hex, NULL, 16)
}

int main() 
{
    Cache cache;
    char hex[9];
    int add;
    int i;

    // initializing the cache
    cache.size = CACHE;
    cache.el_size = EL_SIZE;
    start_cache(&cache, 1, 0);

    for (i = 0; i < 16; i++) 
    {
        // take user input for hexidecimal
        scanf("%s", hex);
        // convert hexidecimal
        add = hex_int(hex);
        // start the cache
        start_cache(&cache, add);
    }

        // print cache hits and misses
        printf("Cache Hits: %d\n", cache.cache_hit);
        printf("Cache Misses: %d\n", cache.cache_miss);

        return 0;
} 