#!/bin/sh
set -e

# If first argument starts with a dash (-), prepend megacoind
if [ "${1#-}" != "$1" ]; then
    set -- megacoind "$@"
fi

# If executing a Megacoin binary, handle permissions and drop privileges to megacoin user
if [ "$1" = "megacoind" ] || [ "$1" = "megacoin-cli" ] || [ "$1" = "megacoin-tx" ]; then
    mkdir -p "$MEGACOIN_DATA"
    chmod 700 "$MEGACOIN_DATA"
    chown -R megacoin:megacoin "$MEGACOIN_DATA"

    if [ "$(id -u)" = "0" ]; then
        exec gosu megacoin "$@" -datadir="$MEGACOIN_DATA"
    fi
fi

exec "$@"
