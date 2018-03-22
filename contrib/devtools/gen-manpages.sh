#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

MEGACOIND=${MEGACOIND:-$SRCDIR/megacoind}
MEGACOINCLI=${MEGACOINCLI:-$SRCDIR/megacoin-cli}
MEGACOINTX=${MEGACOINTX:-$SRCDIR/megacoin-tx}
MEGACOINQT=${MEGACOINQT:-$SRCDIR/qt/megacoin-qt}

[ ! -x $MEGACOIND ] && echo "$MEGACOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
BTCVER=($($MEGACOINCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for megacoind if --version-string is not set,
# but has different outcomes for megacoin-qt and megacoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$MEGACOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $MEGACOIND $MEGACOINCLI $MEGACOINTX $MEGACOINQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${BTCVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${BTCVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
