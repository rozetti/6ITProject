/* The 6IT Project. Copyright 2015 Conrad Rozetti, crz@6itproject.org. Distributed under the MIT License, see 6IT.h. */

#ifdef __cplusplus
extern "C"
{
#endif
#include "6IT.h"
#include "SubC_tester.h"
#include "subc_tester_bios.h"
int SubC_tester_main(void *context);

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>

#ifdef _6IT_QT
#include <QFile>
#endif

int source_seek(void *context, int offset)
{
#ifdef _6IT_ANDROID
    AAsset *asset = (AAsset *)context;
    AAsset_seek(asset, offset, SEEK_SET);
#elif defined(_6IT_QT)
    auto file = (QFile *)context;
    file->seek(offset);
#else
    FILE *p = (FILE *)context;

    return fseek(p, offset, SEEK_SET);
#endif

    return 1;
}

int source_get_char(void *context, char *c)
{
#ifdef _6IT_ANDROID
    AAsset *asset = (AAsset *)context;
    if (0 == AAsset_read(asset, c, 1))
    {
        *c = 0;
        return 0;
    }
#elif defined(_6IT_QT)
    auto file = (QFile *)context;
    if (0 == file->read(c, 1))
    {
        *c = 0;
        return 0;
    }
#else
    FILE *p = (FILE *)context;

    *c = getc(p);

    if (feof(p))
    {
        *c = 0;
        return 0;
    }
#endif

    return 1;
}

int load_file(struct machine_t *machine, char const *filename, char **buffer)
{
#ifdef _6IT_ANDROID
    struct android_app *app = (struct android_app*)_Bios.context;
    AAssetManager* mgr = app->activity->assetManager;
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
    if (NULL == asset) {
        machine->printf(machine, "could not open file\n");
        *buffer = 0;
        return 0;
    }

    long size = AAsset_getLength(asset);
    *buffer = (char *)malloc(sizeof(char)*size + 1);
    AAsset_read(asset, *buffer, size);
    (*buffer)[size] = 0;
    AAsset_close(asset);
#elif defined(_6IT_QT)
#else
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        machine->printf(machine, "could not open file\n");
        *buffer = 0;
        return 0;
    }

    struct stat st;
    fstat(_6IT_Fileno(fp), &st);
    if (!(*buffer = (char *)malloc(st.st_size * sizeof(char) + 1)))
    {
        machine->printf(machine, "malloc failed\n");
        *buffer = 0;
        return 0;
    }

    size_t bytes = fread(*buffer, 1, st.st_size, fp);

    fclose(fp);

#endif

    return 1;
}

int open_file(struct machine_t *machine, char const *filename)
{
#ifdef _6IT_ANDROID
    struct android_app *app = (struct android_app*)_Bios.context;
    AAssetManager* mgr = app->activity->assetManager;
    AAsset* asset = AAssetManager_open(mgr, filename, AASSET_MODE_UNKNOWN);
    if (NULL == asset) return 0;
    machine->environment->context = asset;
#elif defined(_6IT_QT)
    auto file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    machine->environment->context = file;
#else
    FILE *fp = 0;
    if ((fp = fopen(filename, "rb")) == NULL) return 0;
    machine->environment->context = fp;
#endif

    return 1;
}

void close_file(struct machine_t *machine)
{
#ifdef _6IT_ANDROID
    AAsset_close((AAsset *)machine->environment->context);
#elif defined(_6IT_QT)
    auto file = (QFile *)machine->environment->context;
    file->close();
#else
    fclose((FILE *)machine->environment->context);
#endif
}
