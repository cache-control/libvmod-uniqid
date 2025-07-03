#include "config.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "cache/cache.h"

#include "vcc_uniqid_if.h"
#include "city.h"

uint128 seed;

int v_matchproto_(vmod_event_f)
vmod_event_function(VRT_CTX, struct vmod_priv *priv, enum vcl_event_e e)
{
    const char *event = NULL;

    (void) ctx;
    (void) priv;

    switch (e) {
        case VCL_EVENT_LOAD:
            int fd = open("/dev/urandom", O_RDONLY);
            if (fd < 0) {
                /*failed to open*/
            } else {
                size_t nbytes = sizeof seed;
                if (read(fd, &seed, nbytes) != nbytes) {
                    /*failed to read full*/
                }
                close(fd);
            }

            event = "loaded";
            break;

        case VCL_EVENT_WARM:
            event = "warmed";
            break;

        case VCL_EVENT_COLD:
            event = "cooled";
            break;

        case VCL_EVENT_DISCARD:
            return (0);
            break;

        default:
            return (0);
    }

    AN(event);

    return (0);
}

VCL_STRING
vmod_get(VRT_CTX, VCL_STRING s) {
    char *buf;
    unsigned maxLen, used;

    CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);

    /* use generated hash as seed for next run */
    seed = CityHash128WithSeed(s, strlen(s), seed);

    maxLen = WS_ReserveAll(ctx->ws);
    buf = ctx->ws->f;
    used = snprintf(buf, maxLen, "%lu%lu", Uint128Low64(seed), Uint128High64(seed));
    used++;

    if (used > maxLen) {
        WS_Release(ctx->ws, 0);
        return (NULL);
    }

    WS_Release(ctx->ws, used);

    return (buf);
}
