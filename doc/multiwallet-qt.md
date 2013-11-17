Multiwallet Qt Development and Integration Strategy
===================================================

In order to support loading of multiple wallets in megacoin-qt, a few changes in the UI architecture will be needed.
Fortunately, only four of the files in the existing project are affected by this change.

Three new classes have been implemented in three new .h/.cpp file pairs, with much of the functionality that was previously
implemented in the MegacoinGUI class moved over to these new classes.

The two existing files most affected, by far, are megacoingui.h and megacoingui.cpp, as the MegacoinGUI class will require
some major retrofitting.

Only requiring some minor changes is megacoin.cpp.

Finally, three new headers and source files will have to be added to megacoin-qt.pro.

Changes to class MegacoinGUI
---------------------------
The principal change to the MegacoinGUI class concerns the QStackedWidget instance called centralWidget.
This widget owns five page views: overviewPage, transactionsPage, addressBookPage, receiveCoinsPage, and sendCoinsPage.

A new class called *WalletView* inheriting from QStackedWidget has been written to handle all renderings and updates of
these page views. In addition to owning these five page views, a WalletView also has a pointer to a WalletModel instance.
This allows the construction of multiple WalletView objects, each rendering a distinct wallet.

A second class called *WalletStack*, also inheriting from QStackedWidget, has been written to handle switching focus between
different loaded wallets. In its current implementation, as a QStackedWidget, only one wallet can be viewed at a time -
but this can be changed later.

A third class called *WalletFrame* inheriting from QFrame has been written as a container for embedding all wallet-related
controls into MegacoinGUI. At present it just contains a WalletStack instance and does little more than passing on messages
from MegacoinGUI to the WalletStack, which in turn passes them to the individual WalletViews. It is a WalletFrame instance
that takes the place of what used to be centralWidget in MegacoinGUI. The purpose of this class is to allow future
refinements of the wallet controls with minimal need for further modifications to MegacoinGUI, thus greatly simplifying
merges while reducing the risk of breaking top-level stuff.

Changes to megacoin.cpp
----------------------
megacoin.cpp is the entry point into megacoin-qt, and as such, will require some minor modifications to provide hooks for
multiple wallet support. Most importantly will be the way it instantiates WalletModels and passes them to the
singleton MegacoinGUI instance called window. Formerly, MegacoinGUI kept a pointer to a single instance of a WalletModel.
The initial change required is very simple: rather than calling `window.setWalletModel(&walletModel);` we perform the
following two steps:

	window.addWallet("~Default", &walletModel);
	window.setCurrentWallet("~Default");

The string parameter is just an arbitrary name given to the default wallet. It's been prepended with a tilde to avoid name collisions in the future with additional wallets.

The shutdown call `window.setWalletModel(0)` has also been removed. In its place is now:

window.removeAllWallets();
