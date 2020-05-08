Megacoin Core - Around the World!
=====================================

[![Build Status](https://travis-ci.org/megacoin-project/megacoin.svg?branch=master)](https://travis-ci.org/megacoin-project/megacoin)

https://megacoin-mec.cc

What is Megacoin?
----------------

Established on 01th of June 2013, Megacoin from the community for the community. Megacoin has one of the lowest transaction fees on the market while possibly providing the best on-chain scaling solution in the  entire crypto-sphere. Take a look at our [comparison chart](https://bitcore.cc/wp-content/uploads/2019/11/BitCoreVsAgosto2019.jpg).

Megacoin MEC preserves and implements Bitcoin features such as SegWit, which enables advanced scaling technologies like Lightning Network. Significant differences at launch time included:

- Unique wallet addresses (prefixes of M and 3 and mex (BIP 173 bench32)
- Command Fork System
- Masternode System Dash Core
- 4,200 MEC for one Masternode
- 220 Byte Datacarriersize (OP_RETURN)
- Hashalgorythm Mega-Mec
- Bitcore Diffshield
- Travis CI support - soon

For more information, as well as an immediately useable, binary version of
the Megacoin Core software, see [https://megacoin-mec.cc](https://megacoin-mec.cc).

License
-------

Megacoin Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.



Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Releases](https://github.com/LIMXTEC/Megacoin/release) are created
regularly to indicate new official, stable release versions of Megacoin Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md).

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test on short notice. Please be patient and help out by testing
other people's pull requests, and remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/test), written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/test) are installed) with: `test/functional/test_runner.py`

The Travis CI system makes sure that every pull request is built for Windows, Linux, and OS X, and that unit/sanity tests are run automatically.

License
-------

Megacoin is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.

**Important**: We do not accept translation changes as GitHub pull requests because the next
pull from Transifex would automatically overwrite them again.
