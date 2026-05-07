#include "phys_logger.h"
#include <stdio.h>
#include <stdlib.h>

#define LOGGER_MAX_ITEMS 256

struct logger_entry {
    struct physItem *item;
    int id;       // -1 for floor
    FILE *file;   // NULL for floor
};

struct pending_record {
    int target_id;
    char guards[3];
};

static struct logger_entry entries[LOGGER_MAX_ITEMS];
static int entry_count = 0;
static int time_step = -1;

static struct pending_record pending[LOGGER_MAX_ITEMS];
static int pending_count = 0;
static struct logger_entry *current_origin = NULL;
static bool started = false;

static struct logger_entry *find_entry(struct physItem *item){
    for (int i = 0; i < entry_count; i++)
        if (entries[i].item == item) return &entries[i];
    return NULL;
}

static void phys_logger_close(void){
    for (int i = 0; i < entry_count; i++)
        if (entries[i].file){
            fclose(entries[i].file);
            entries[i].file = NULL;
        }
}

void phys_logger_start(struct physList *pList){
    if (started) return;
    started = true;

    int next_id = 0;
    for (struct physItem *cur = pList->head;
         cur != NULL && entry_count < LOGGER_MAX_ITEMS;
         cur = cur->next){
        struct logger_entry *e = &entries[entry_count++];
        e->item = cur;
        // Floor items are subscribed with process=false; non-floor with true.
        // This is the only stable way to tell them apart at init.
        if (cur->processFlag){
            e->id = next_id++;
            char path[64];
            snprintf(path, sizeof(path), "%d.detect", e->id);
            e->file = fopen(path, "w");
        } else {
            e->id = -1;
            e->file = NULL;
        }
    }
    atexit(phys_logger_close);
}

void phys_logger_tick(void){
    time_step++;
    for (int i = 0; i < entry_count; i++)
        if (entries[i].file)
            fprintf(entries[i].file, "t=%d\n", time_step);
}

void phys_logger_origin_begin(struct physItem *origin){
    current_origin = find_entry(origin);
    pending_count = 0;
}

void phys_logger_guard(struct physItem *target, int g, bool passed){
    if (!current_origin || !current_origin->file) return;
    if (g < 0 || g >= 3) return;
    struct logger_entry *te = find_entry(target);
    if (!te || te->id < 0) return;  // floor or unknown — ignore

    int idx = -1;
    for (int i = 0; i < pending_count; i++)
        if (pending[i].target_id == te->id){ idx = i; break; }
    if (idx == -1){
        if (pending_count >= LOGGER_MAX_ITEMS) return;
        idx = pending_count++;
        pending[idx].target_id = te->id;
        pending[idx].guards[0] = '-';
        pending[idx].guards[1] = '-';
        pending[idx].guards[2] = '-';
    }
    pending[idx].guards[g] = passed ? 'P' : 'F';
}

void phys_logger_origin_end(void){
    if (current_origin && current_origin->file){
        for (int i = 0; i < pending_count; i++){
            fprintf(current_origin->file,
                    "  target=%d g1=%c g2=%c g3=%c\n",
                    pending[i].target_id,
                    pending[i].guards[0],
                    pending[i].guards[1],
                    pending[i].guards[2]);
        }
        fflush(current_origin->file);
    }
    current_origin = NULL;
    pending_count = 0;
}
