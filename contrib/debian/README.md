
Debian
====================
This directory contains files used to package megacoind/megacoin-qt
for Debian-based Linux systems. If you compile megacoind/megacoin-qt yourself, there are some useful files here.

## megacoin: URI support ##


megacoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install megacoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your megacoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/megacoin128.png` to `/usr/share/pixmaps`

megacoin-qt.protocol (KDE)

